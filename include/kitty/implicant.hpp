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

inline std::vector<std::pair<uint32_t, uint32_t>> get_jbuddies( const std::vector<uint32_t>& minterms, uint8_t j )
{
  std::vector<std::pair<uint32_t, uint32_t>> buddies;

  auto mask = uint32_t( 1 ) << j;
  auto k = minterms.cbegin();
  auto kk = minterms.cbegin();
  
  while ( true )
  {
    k = std::find_if( k, minterms.end(), [mask]( auto m ) { return ( m & mask ) == 0; } );
    if ( k == minterms.end() ) break;

    if ( kk <= k )
    {
      kk = k + 1;
    }

    kk = std::find_if( kk, minterms.end(), [mask, &k]( auto m ) { return m >= ( *k | mask ); } );
    if ( kk == minterms.end() ) break;

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
  const auto m = minterms.size();

  std::vector<uint32_t> tags( m, 0 );
  std::vector<uint32_t> stack;

  for ( auto j = 0; j < m; ++j )
  {
    for ( const auto& p : get_jbuddies( minterms, j ) )
    {
      tags[p.first] |= ( 1 << j );
      tags[p.second] |= ( 1 << j );
    }

    auto t = 0;
    for ( auto s = 0; s < m; ++s )
    {
      if ( t[s] == 0 )
      {
        // TODO output cube
      }
      else
      {
        stack.push_back( minterms[s] );
        tags[t++] = tags[s];
      }
    }

    // TODO A = 0;
    stack.push_back( 0 );
  }
}
} // namespace kitty
