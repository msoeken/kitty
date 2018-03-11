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

namespace kitty
{

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

inline std::vector<std::pair<uint32_t, uint32_t>> get_jbuddies( const std::vector<uint32_t>& minterms, uint32_t j )
{
  std::vector<std::pair<uint32_t, uint32_t>> buddies;

  auto mask = uint32_t( 1 ) << j;
  auto k = minterms.cbegin();
  auto kk = minterms.cbegin();

  while ( true )
  {
    k = std::find_if( k, minterms.end(), [mask]( auto m ) { return ( m & mask ) == 0; } );
    if ( k == minterms.end() )
      break;

    if ( kk <= k )
    {
      kk = k + 1;
    }

    kk = std::find_if( kk, minterms.end(), [mask, &k]( auto m ) { return m >= ( *k | mask ); } );
    if ( kk == minterms.end() )
      break;

    if ( ( *k ^ *kk ) >= ( mask << 1 ) )
    {
      k = kk;
      continue;
    }

    if ( *kk == ( *k | mask ) )
    {
      buddies.emplace_back( std::distance( minterms.begin(), k ),
                            std::distance( minterms.begin(), kk ) );
    }

    ++k;
  }

  return buddies;
}

template<typename TT>
void quine_mccluskey( const TT& tt )
{
  const auto minterms = get_minterms( tt );
  const auto n = tt.num_vars();
  const auto m = minterms.size();

  std::vector<uint32_t> tags( m, 0 );
  std::vector<uint32_t> stack( 2 * m + n , 0 );

  uint64_t a{};

  /* Update tags using j-buddy algorithm */
  for ( auto j = 0; j < n; ++j )
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

  //std::cout << "Minterms:\n";
  //for ( auto i = 0; i < m; ++i )
  //{
  //  std::cout << std::setw( 3 ) << minterms[i] << ": " << std::setw( 8 ) << std::hex << tags[i] << "\n";
  //}

  auto t = 0;
  for ( auto s = 0; s < m; ++s )
  {
    if ( tags[s] == 0 )
    {
      // TODO output cube
    }
    else
    {
      stack[t] = minterms[s];
      tags[t] = tags[s];
      t++;
    }
  }

  std::cout << "After step P1\n";
  std::cout << "T =";
  for ( auto j = 0; j < t; ++j )
  {
    std::cout << " " << tags[j];
  }

  std::cout << "\nS =";
  for ( auto j = 0; j < t; ++j )
  {
    std::cout << " " << stack[j];
  }

  std::cout << "\nt = " << t << "\n";

  stack.push_back( 0 );
  auto A = 0;

  while ( true )
  {
    /* P2 */
    auto j = 0;
    if ( stack[t] == t )
    {
      while ( j < n && ( ( A >> j ) & 1 ) == 0 )
      {
        ++j;
      }
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

    std::cout << "After step P2\n";
    std::cout << "j = " << j << " t = " << t << " A = " << A << "\n";

    // P3
    auto r = t;
    auto s = stack[t];

    std::vector<uint32_t> subterms( r - s );
    for ( auto i = 0; i < r - s; ++i )
    {
      subterms[i] = stack[s + i];
    }

    assert( !subterms.empty() );
    assert( j < n );

    if ( subterms.size() > 1 )
    {
      assert( subterms.size() > 1 );
      for ( const auto& p : get_jbuddies( subterms, j ) )
      {
        std::cout << "loop\n";
        auto x = tags[p.first] & tags[p.second];
        //assert( ( x >> j ) & 1 );
        x -= 1 << j;

        if ( x == 0 )
        {
          std::cout << "CUBE(" << A << ", " << stack[p.first] << ")\n";
          // TODO output cube
        }
        else
        {
          ++t;
          stack[t] = stack[p.first];
          tags[t] = x;
        }
      }
    }

    ++t;
    stack[t] = r + 1;

    std::cout << "After step P3\n";
    std::cout << "\nS =";
    for ( auto j = 0; j <= t; ++j )
    {
      std::cout << " " << stack[j];
    }
    std::cout << "\nt = " << t << "\n";

    //return;
    //break;
  }
}
} // namespace kitty
