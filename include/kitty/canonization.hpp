/* kitty: C++ truth table library
 * Copyright (C) 2017  EPFL
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

#pragma once

#include <numeric>

#include "detail/constants.hpp"
#include "operators.hpp"

namespace kitty
{

/*! Exact NPN canonization

  Given a truth table, this function finds the lexicographically
  smallest truth table in its NPN class, called NPN representative.
  Two functions are in the same NPN class, if one can obtain one from
  the other by input negation, input permutation, and output negation.

  The function returns a NPN configuration which contains the
  necessary transformations to obtain the representative.  It is a
  tuple of

  - the NPN representative
  - input negations and output negation, output negation is stored as
    bit *n*, where *n* is the number of variables in `tt`
  - input permutation to apply

  \param tt The truth table
  \return NPN configuration
*/
template<typename TT>
std::tuple<TT, uint32_t, std::vector<uint8_t>> exact_npn_canonization( const TT& tt )
{
  const auto num_vars = tt.num_vars();

  /* Special case for n = 0 */
  if ( num_vars == 0 )
  {
    const auto bit = get_bit( tt, 0 );
    return std::make_tuple( unary_not_if( tt, bit ), bit, std::vector<uint8_t>{} );
  }

  /* Special case for n = 1 */
  if ( num_vars == 1 )
  {
    const auto bit1 = get_bit( tt, 1 );
    return std::make_tuple( unary_not_if( tt, bit1 ), bit1 << 1, std::vector<uint8_t>{ 0 } );
  }

  assert( num_vars >= 2 && num_vars <= 6u );

  auto t1 = tt, t2 = ~tt;
  auto tmin = std::min( t1, t2 );
  auto invo = tmin == t2;

  const auto& swaps = detail::swaps[num_vars - 2u];
  const auto& flips = detail::flips[num_vars - 2u];

  int best_swap = -1;
  int best_flip = -1;

  for ( int i = 0; i < swaps.size(); ++i )
  {
    const auto pos = swaps[i];
    swap_adjacent_inplace( t1, pos );
    swap_adjacent_inplace( t2, pos );
    if ( t1 < tmin || t2 < tmin )
    {
      best_swap = i;
      tmin = std::min( t1, t2 );
      invo = tmin == t2;
    }
  }

  for ( int j = 0; j < flips.size(); ++j )
  {
    const auto pos = flips[j];
    swap_adjacent_inplace( t1, 0 );
    flip_inplace( t1, pos );
    swap_adjacent_inplace( t2, 0 );
    flip_inplace( t2, pos );
    if ( t1 < tmin || t2 < tmin )
    {
      best_swap = -1;
      best_flip = j;
      tmin = std::min( t1, t2 );
      invo = tmin == t2;
    }

    for ( int i = 0; i < swaps.size(); ++i )
    {
      const auto pos = swaps[i];
      swap_adjacent_inplace( t1, pos );
      swap_adjacent_inplace( t2, pos );
      if ( t1 < tmin || t2 < tmin )
      {
        best_swap = i;
        best_flip = j;
        tmin = std::min( t1, t2 );
        invo = tmin == t2;
      }
    }
  }

  std::vector<uint8_t> perm( num_vars );
  std::iota( perm.begin(), perm.end(), 0u );

  for ( auto i = 0; i <= best_swap; ++i )
  {
    const auto pos = swaps[i];
    std::swap( perm[pos], perm[pos + 1] );
  }

  uint32_t phase = uint32_t( invo ) << num_vars;
  for ( auto i = 0; i <= best_flip; ++i )
  {
    phase ^= 1 << flips[i];
  }

  return std::make_tuple( tmin, phase, perm );
}

/*! Obtain truth table from NPN configuration

  Given an NPN configuration, which contains a representative
  function, input/output negations, and input permutations this
  function computes the original truth table.

  \param config NPN configuration
*/
template<typename TT>
TT create_from_npn_config( const std::tuple<TT, uint32_t, std::vector<uint8_t>>& config )
{
  const auto& from = std::get<0>( config );
  const auto& phase = std::get<1>( config );
  auto perm = std::get<2>( config );
  const auto num_vars = from.num_vars();

  /* is output complemented? */
  auto res = ( ( phase >> num_vars ) & 1 ) ? ~from : from;

  /* input complementations */
  for ( auto i = 0; i < num_vars; ++i )
  {
    if ( ( phase >> i ) & 1 )
    {
      flip_inplace( res, i );
    }
  }

  /* input permutations */
  for ( auto i = 0; i < num_vars; ++i )
  {
    int k = i;
    while ( perm[k] != i )
    {
      ++k;
    }

    swap_inplace( res, i, k );
    std::swap( perm[i], perm[k] );
  }

  return res;
}

} // namespace kitty
