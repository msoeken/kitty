/* kitty: C++ truth table library
 * Copyright (C) 2017-2018  EPFL
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*!
  \file implicant.hpp
  \brief Find implicants and prime implicants

  \author Mathias Soeken
*/

#pragma once

#include <cstdint>
#include <vector>

#include "algorithm.hpp"
#include "cube.hpp"

namespace kitty
{

/*! \brief Computes all minterms 

  \param tt Truth table
*/
template<typename TT>
std::vector<uint32_t> get_minterms( const TT& tt )
{
  std::vector<uint32_t> m;
  m.reserve( count_ones( tt ) );
  for_each_one_bit( tt, [&m]( auto index ) {
    m.emplace_back( index );
  } );
  return m;
}

/*! \cond PRIVATE */
template<typename Iterator>
inline std::vector<std::pair<uint32_t, uint32_t>> get_jbuddies( Iterator begin, Iterator end, uint32_t j )
{
  std::vector<std::pair<uint32_t, uint32_t>> buddies;

  auto mask = uint32_t( 1 ) << j;
  auto k = begin;
  auto kk = begin;

  while ( true )
  {
    k = std::find_if( k, end, [mask]( auto m ) { return ( m & mask ) == 0; } );
    if ( k == end )
      break;

    if ( kk <= k )
    {
      kk = k + 1;
    }

    kk = std::find_if( kk, end, [mask, &k]( auto m ) { return m >= ( *k | mask ); } );
    if ( kk == end )
      break;

    if ( ( *k ^ *kk ) >= ( mask << 1 ) )
    {
      k = kk;
      continue;
    }

    if ( *kk == ( *k | mask ) )
    {
      buddies.emplace_back( std::distance( begin, k ), std::distance( begin, kk ) );
    }

    ++k;
  }

  return buddies;
}
/*! \endcond */

/*! \brief Computes all j-buddies in a list of minterms

  Computes all pairs \f$(k, k')\f$ such that \f$k < k'\f$ and the two minterms
  at indexes \f$k\f$ and \f$k'\f$ only differ in bit \f$j\f$.

  This algorithm is described by Knuth in Exercise TAOCP 7.1.1-29.

  \param minterms Vector of minterms
  \param j Bit position
*/
inline std::vector<std::pair<uint32_t, uint32_t>> get_jbuddies( const std::vector<uint32_t>& minterms, uint32_t j )
{
  return get_jbuddies( minterms.begin(), minterms.end(), j );
}

/*! \brief Computes all prime implicants (from minterms)

  This algorithm computes all prime implicants for a list of minterms.  The
  running time is at most proportional to \f$mn\f$, where \f$m\f$ is the number
  of minterms and \f$n\f$ is the number of variables.

  The algorithm is described in Exercise TAOCP 7.1.1-30 by Knuth and is inspired
  by the algorithm described in [E. Morreale, IEEE Trans. EC 16(5), 1967,
  611–620].

  \param minterms Vector of minterms (as integer values)
  \param num_vars Number of variables
*/
inline std::vector<cube> get_prime_implicants_morreale( const std::vector<uint32_t>& minterms, unsigned num_vars )
{
  std::vector<cube> cubes;

  const auto n = num_vars;
  const auto m = minterms.size();

  std::vector<uint32_t> tags( 2 * m + n, 0 );
  std::vector<uint32_t> stack( 2 * m + n , 0 );

  uint32_t mask = ( 1 << n ) - 1;
  uint32_t A{};

  /* P1 */

  /* Update tags using j-buddy algorithm */
  for ( auto j = 0u; j < n; ++j )
  {
    for ( const auto& p : get_jbuddies( minterms, j ) )
    {
      assert( p.first < m );
      assert( p.second < m );
      assert( j <= n );

      assert( !( ( tags[p.first] >> j ) & 1 ) );
      tags[p.first] |= ( 1 << j );
      assert( !( ( tags[p.second] >> j ) & 1 ) );
      tags[p.second] |= ( 1 << j );
    }
  }

  auto t = 0u;
  for ( auto s = 0u; s < m; ++s )
  {
    if ( tags[s] == 0u )
    {
      cubes.emplace_back( minterms[s], mask );
    }
    else
    {
      stack[t] = minterms[s];
      tags[t] = tags[s];
      t++;
    }
  }

  stack.push_back( 0 );

  while ( true )
  {
    /* P2 */
    auto j = 0u;
    if ( stack[t] == t )
    {
      while ( j < n && ( ( A >> j ) & 1 ) == 0 )
      {
        ++j;
      }
      assert( j == n || ( A >> j ) & 1 );
    }

    while ( j < n && ( ( A >> j ) & 1 ) != 0 )
    {
      t = stack[t] - 1;

      assert( ( A >> j ) & 1 );
      A &= ~( 1 << j );
      ++j;
    }

    if ( j >= n )
    {
      break;
    }

    assert( !( ( A >> j ) & 1 ) );
    A |= ( 1 << j );

    /* P3 */
    auto r = t;
    auto s = stack[t];

    assert( j < n );

    for ( const auto& p : get_jbuddies( stack.begin() + s, stack.begin() + r, j ) )
    {
      auto x = tags[s + p.first] & tags[s + p.second];
      assert( ( x >> j ) & 1 );
      x &= ~( 1 << j );

      if ( x == 0 )
      {
        cubes.emplace_back( stack[s + p.first], ~A & mask );
      }
      else
      {
        ++t;
        stack[t] = stack[s + p.first];
        tags[t] = x;
      }
    }

    ++t;
    stack[t] = r + 1;
  }

  return cubes;
}

/*! \brief Computes all prime implicants (from truth table)

  Computes minterms from truth table and calls overloaded function.

  \param tt Truth table
*/
template<typename TT>
std::vector<cube> get_prime_implicants_morreale( const TT& tt )
{
  return get_prime_implicants_morreale( get_minterms( tt ), tt.num_vars() );
}
} // namespace kitty
