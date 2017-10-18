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

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>

#include "static_truth_table.hpp"

namespace kitty
{

/*! Perform bitwise unary operation on truth table.

  \param tt Truth table
  \param op Unary operation that takes as input a word (`uint64_t`) and returns a word

  \return new constructed truth table of same type and dimensions
 */
template<typename TT, typename Fn>
TT unary_operation( const TT& tt, Fn&& op )
{
  auto result = tt.construct();
  std::transform( std::begin( tt._bits ), std::end( tt._bits ), std::begin( result._bits ), op );
  result.mask_bits();
  return result;
}

/*! \cond PRIVATE */
template<int NumVars, typename Fn>
static_truth_table<NumVars, true> unary_operation( const static_truth_table<NumVars, true>& tt, Fn&& op )
{
  auto result = tt.construct();
  result._bits = op( tt._bits );
  result.mask_bits();
  return result;
}
/* \endcond */

/*! Perform bitwise binary operation on two truth tables.

  The dimensions of `first` and `second` must match.  This is ensured
  at compile-time for static truth tables, but at run-time for dynamic
  truth tables.

  \param first First truth table
  \param second Second truth table
  \param op Binary operation that takes as input two words (`uint64_t`) and returns a word

  \return new constructed truth table of same type and dimensions
 */
template<typename TT, typename Fn>
TT binary_operation( const TT& first, const TT& second, Fn&& op )
{
  assert( first.num_vars() == second.num_vars() );

  auto result = first.construct();
  std::transform( std::begin( first._bits ), std::end( first._bits ), std::begin( second._bits ), std::begin( result._bits ), op );
  result.mask_bits();
  return result;
}

/*! \cond PRIVATE */
template<int NumVars, typename Fn>
static_truth_table<NumVars, true> binary_operation( const static_truth_table<NumVars, true>& first, const static_truth_table<NumVars, true>& second, Fn&& op )
{
  auto result = first.construct();
  result._bits = op( first._bits, second._bits );
  result.mask_bits();
  return result;
}
/*! \endcond */

/*! Perform bitwise ternary operation on three truth tables.

  The dimensions of `first`, `second`, and `third` must match.  This
  is ensured at compile-time for static truth tables, but at run-time
  for dynamic truth tables.

  \param first First truth table
  \param second Second truth table
  \param third Third truth table
  \param op Ternary operation that takes as input two words (`uint64_t`) and returns a word

  \return new constructed truth table of same type and dimensions
 */
template<typename TT, typename Fn>
TT ternary_operation( const TT& first, const TT& second, const TT& third, Fn&& op )
{
  assert( first.num_vars() == second.num_vars() && second.num_vars() == third.num_vars() );

  auto result = first.construct();
  auto it1 = std::begin( first._bits );
  const auto it1_e = std::end( first._bits );
  auto it2 = std::begin( second._bits );
  auto it3 = std::begin( third._bits );
  auto it = std::begin( result._bits );

  while ( it1 != it1_e )
  {
    *it++ = op( *it1++, *it2++, *it3++ );
  }

  result.mask_bits();
  return result;
}

/*! \cond PRIVATE */
template<int NumVars, typename Fn>
static_truth_table<NumVars, true> ternary_operation( const static_truth_table<NumVars, true>& first, const static_truth_table<NumVars, true>& second, const static_truth_table<NumVars, true>& third, Fn&& op )
{
  auto result = first.construct();
  result._bits = op( first._bits, second._bits, third._bits );
  result.mask_bits();
  return result;
}
/*! \endcond */

/*! Computes a predicate based on two truth tables.

  The dimensions of `first` and `second` must match.  This is ensured
  at compile-time for static truth tables, but at run-time for dynamic
  truth tables.

  \param first First truth table
  \param second Second truth table
  \param op Binary operation that takes as input two words (`uint64_t`) and returns a Boolean

  \return true or false based on the predicate
 */
template<typename TT, typename Fn>
bool binary_predicate( const TT& first, const TT& second, Fn&& op )
{
  assert( first.num_vars() == second.num_vars() );

  return std::equal( std::begin( first._bits ), std::end( first._bits ), std::begin( second._bits ), op );
}

/*! \cond PRIVATE */
template<int NumVars, typename Fn>
bool binary_predicate( const static_truth_table<NumVars, true>& first, const static_truth_table<NumVars, true>& second, Fn&& op )
{
  return op( first._bits, second._bits );
}
/*! \endcond */

/*! Inverts all bits in a truth table. */
template<typename TT>
inline TT unary_not( const TT& tt )
{
  return unary_operation( tt, []( auto a ) { return ~a; } );
}

/*! Inverts all bits in a truth table, based on a condition */
template<typename TT>
inline TT unary_not_if( const TT& tt, bool cond )
{
  const auto mask = -static_cast<uint64_t>( cond );
  return unary_operation( tt, [mask]( auto a ) { return a ^ mask; } );
}

/*! Performs bitwise AND of two truth tables. */
template<typename TT>
inline TT binary_and( const TT& first, const TT& second )
{
  return binary_operation( first, second, std::bit_and<>() );
}

/*! Performs bitwise OR of two truth tables. */
template<typename TT>
inline TT binary_or( const TT& first, const TT& second )
{
  return binary_operation( first, second, std::bit_or<>() );
}

/*! Performs bitwise XOR of two truth tables. */
template<typename TT>
inline TT binary_xor( const TT& first, const TT& second )
{
  return binary_operation( first, second, std::bit_xor<>() );
}

/*! Performs ternary majority of three truth tables. */
template<typename TT>
inline TT ternary_majority( const TT& first, const TT& second, const TT& third )
{
  return ternary_operation( first, second, third, []( auto a, auto b, auto c ) { return ( a & ( b ^ c ) ) ^ ( b & c ); } );
}

/*! Performs ternary if-then-else of three truth tables.

  \param first Truth table for condition
  \param second Truth table for then-case
  \param third Truth table for else-case
 */
template<typename TT>
inline TT ternary_ite( const TT& first, const TT& second, const TT& third )
{
  return ternary_operation( first, second, third, []( auto a, auto b, auto c ) { return ( a & b ) ^ ( ~a & c ); } );
}

/*! Checks whether two truth tables are equal

  \param first First truth table
  \param second Second truth table
*/
template<typename TT>
inline bool equal( const TT& first, const TT& second )
{
  return binary_predicate( first, second, std::equal_to<>() );
}

/*! Checks whether a truth table is lexicographically smaller than another

  Comparison is initiated from most-significant bit.

  \param first First truth table
  \param second Second truth table
*/
template<typename TT>
inline bool less_than( const TT& first, const TT& second )
{
  return std::lexicographical_compare( first._bits.rbegin(), first._bits.rend(),
                                       second._bits.rbegin(), second._bits.rend() );
}

/*! \cond PRIVATE */
template<int NumVars>
inline bool less_than( const static_truth_table<NumVars, true>& first, const static_truth_table<NumVars, true>& second )
{
  return first._bits < second._bits;
}
/*! \endcond PRIVATE */

/*! Swaps two adjacent variables in a truth table

  The function swaps variable `var_index` with `var_index + 1`.  The
  function will change `tt` in-place.  If `tt` should not be changed,
  one can use `swap_adjacent` instead.

  \param tt Truth table
  \param var_index A variable
*/
template<typename TT>
void swap_adjacent_inplace( TT& tt, uint8_t var_index )
{
  assert( var_index < tt.num_vars() - 1 );

  /* permute within each word */
  if ( var_index < 5 )
  {
    const auto shift = uint64_t( 1 ) << var_index;
    std::transform( std::begin( tt._bits ), std::end( tt._bits ), std::begin( tt._bits ),
                    [shift, var_index]( uint64_t word ) {
                      return ( word & detail::permutation_masks[var_index][0] ) |
                             ( ( word & detail::permutation_masks[var_index][1] ) << shift ) |
                             ( ( word & detail::permutation_masks[var_index][2] ) >> shift );
                    } );
  }
  /* permute (half) parts of words */
  else if ( var_index == 5 )
  {
    auto it = std::begin( tt._bits );
    while ( it != std::end( tt._bits ) )
    {
      const auto tmp = *it;
      auto it2 = it + 1;
      *it = ( tmp & 0xffffffff ) | ( *it2 << 0x20 );
      *it2 = ( *it2 & 0xffffffff00000000 ) | ( tmp >> 0x20 );
      it += 2;
    }
  }
  /* permute comlete words */
  else
  {
    const auto step = 1 << ( var_index - 6 );
    auto it = std::begin( tt._bits );
    while ( it != std::end( tt._bits ) )
    {
      for ( auto i = decltype( step ){0}; i < step; ++i )
      {
        std::swap( *( it + i + step ), *( it + i + 2 * step ) );
      }
      it += 4 * step;
    }
  }
}

/*! \cond PRIVATE */
template<int NumVars>
void swap_adjacent_inplace( static_truth_table<NumVars, true>& tt, uint8_t var_index )
{
  assert( var_index < tt.num_vars() );

  const auto shift = uint64_t( 1 ) << var_index;

  tt._bits = ( tt._bits & detail::permutation_masks[var_index][0] ) |
             ( ( tt._bits & detail::permutation_masks[var_index][1] ) << shift ) |
             ( ( tt._bits & detail::permutation_masks[var_index][2] ) >> shift );
}
/*! \endcond */

/*! Swaps two adjacent variables in a truth table

  The function swaps variable `var_index` with `var_index + 1`.  The
  function will return a new truth table with the result.

  \param tt Truth table
  \param var_index A variable
*/
template<typename TT>
inline TT swap_adjacent( const TT& tt, uint8_t var_index )
{
  auto copy = tt;
  swap_adjacent_inplace( copy, var_index );
  return copy;
}

/*! Swaps two variables in a truth table

  The function swaps variable `var_index1` with `var_index2`.  The
  function will change `tt` in-place.  If `tt` should not be changed,
  one can use `swap` instead.

  \param tt Truth table
  \param var_index1 First variable
  \param var_index2 Second variable
*/
template<typename TT>
void swap_inplace( TT& tt, uint8_t var_index1, uint8_t var_index2 )
{
  if ( var_index1 == var_index2 )
  {
    return;
  }

  if ( var_index1 > var_index2 )
  {
    std::swap( var_index1, var_index2 );
  }

  if ( tt.num_vars <= 6 )
  {
    const auto& pmask = detail::ppermutation_masks[var_index1][var_index2];
    const auto shift = ( 1 << var_index2 ) - ( 1 << var_index1 );
    tt._bits[0] = ( tt._bits[0] & pmask[0] ) | ( ( tt._bits[0] & pmask[1] ) << shift ) | ( ( tt._bits[0] & pmask[2] ) >> shift );
  }
  else if ( var_index2 <= 5 )
  {
    const auto& pmask = detail::ppermutation_masks[var_index1][var_index2];
    const auto shift = ( 1 << var_index2 ) - ( 1 << var_index1 );
    std::transform( std::begin( tt._bits ), std::end( tt._bits ), std::begin( tt._bits ),
                    [var_index1, var_index2, shift, &pmask]( uint64_t word ) {
                      return ( word & pmask[0] ) | ( ( word & pmask[1] ) << shift ) | ( ( word & pmask[2] ) >> shift );
                    } );
  }
  else if ( var_index1 <= 5 ) /* in this case, var_index2 > 5 */
  {
    const auto step = 1 << ( var_index2 - 6 );
    const auto shift = 1 << var_index1;
    auto it = std::begin( tt._bits );
    while ( it != std::end( tt._bits ) )
    {
      for ( auto i = decltype( step ){0}; i < step; ++i )
      {
        const auto low_to_high = ( *( it + i ) & detail::projections[var_index1] ) >> shift;
        const auto high_to_low = ( *( it + i + step ) << shift ) & detail::projections[var_index1];
        *( it + i ) = ( *( it + i ) & ~detail::projections[var_index1] ) | high_to_low;
        *( it + i + step ) = ( *( it + i + step ) & detail::projections[var_index1] ) | low_to_high;
      }
      it += 2 * step;
    }
  }
  else
  {
    const auto step1 = 1 << ( var_index1 - 6 );
    const auto step2 = 1 << ( var_index2 - 6 );
    auto it = std::begin( tt._bits );
    while ( it != std::end( tt._bits ) )
    {
      for ( auto i = 0; i < step2; i += 2 * step1 )
      {
        for ( auto j = 0; j < step1; ++j )
        {
          std::swap( *( it + i + j + step1 ), *( it + i + j + step2 ) );
        }
      }
      it += 2 * step2;
    }
  }
}

/*! \cond PRIVATE */
template<int NumVars>
inline void swap_inplace( static_truth_table<NumVars, true>& tt, uint8_t var_index1, uint8_t var_index2 )
{
  if ( var_index1 == var_index2 )
  {
    return;
  }

  if ( var_index1 > var_index2 )
  {
    std::swap( var_index1, var_index2 );
  }

  const auto& pmask = detail::ppermutation_masks[var_index1][var_index2];
  const auto shift = ( 1 << var_index2 ) - ( 1 << var_index1 );
  tt._bits = ( tt._bits & pmask[0] ) | ( ( tt._bits & pmask[1] ) << shift ) | ( ( tt._bits & pmask[2] ) >> shift );
}
/* \endcond */

/*! Swaps two adjacent variables in a truth table

  The function swaps variable `var_index1` with `var_index2`.  The
  function will return a new truth table with the result.

  \param tt Truth table
  \param var_index1 First variable
  \param var_index2 Second variable
*/
template<typename TT>
inline TT swap( const TT& tt, uint8_t var_index1, uint8_t var_index2 )
{
  auto copy = tt;
  swap_inplace( copy, var_index1, var_index2 );
  return copy;
}

/*! Flips a variable in a truth table

  The function flips variable `var_index` in `tt`.  The function will
  change `tt` in-place.  If `tt` should not be changed, one can use
  `flip` instead.

  \param tt Truth table
  \param var_index A variable
*/
template<typename TT>
void flip_inplace( TT& tt, uint8_t var_index )
{
  assert( var_index < tt.num_vars() );

  if ( tt.num_blocks() == 1 )
  {
    const auto shift = 1 << var_index;
    tt._bits[0] = ( ( tt._bits[0] << shift ) & detail::projections[var_index] ) | ( ( tt._bits[0] & detail::projections[var_index] ) >> shift );
  }
  else if ( var_index < 6 )
  {
    const auto shift = 1 << var_index;
    std::transform( std::begin( tt._bits ), std::end( tt._bits ), std::begin( tt._bits ),
                    [var_index, shift]( uint64_t word ) {
                      return ( ( word << shift ) & detail::projections[var_index] ) | ( ( word & detail::projections[var_index] ) >> shift );
                    } );
  }
  else
  {
    const auto step = 1 << ( var_index - 6 );
    auto it = std::begin( tt._bits );
    while ( it != std::end( tt._bits ) )
    {
      for ( auto i = decltype( step ){0}; i < step; ++i )
      {
        std::swap( *( it + i ), *( it + i + step ) );
      }
      it += 2 * step;
    }
  }
}

/*! \cond PRIVATE */
template<int NumVars>
inline void flip_inplace( static_truth_table<NumVars, true>& tt, uint8_t var_index )
{
  assert( var_index < tt.num_vars() );

  const auto shift = 1 << var_index;
  tt._bits = ( ( tt._bits << shift ) & detail::projections[var_index] ) | ( ( tt._bits & detail::projections[var_index] ) >> shift );
}
/* \endcond */

/*! Flips a variable in a truth table

  The function flips variable `var_index` in `tt`.  The function will
  not change `tt` and return the result as a copy.

  \param tt Truth table
  \param var_index A variable
*/
template<typename TT>
inline TT flip( const TT& tt, uint8_t var_index )
{
  auto copy = tt;
  flip_inplace( copy, var_index );
  return copy;
}

} // namespace kitty
