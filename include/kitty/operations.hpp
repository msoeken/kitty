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

/*! Inverts all bits in a truth table. */
template<typename TT>
inline TT unary_not( const TT& tt )
{
  return unary_operation( tt, []( auto a ) { return ~a; } );
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

} // namespace kitty
