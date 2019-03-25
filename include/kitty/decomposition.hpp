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
  \file decomposition.hpp
  \brief Check decomposition properties of a function

  \author Mathias Soeken
*/

#pragma once

#include <cstdint>

#include "constructors.hpp"
#include "operations.hpp"

namespace kitty
{

enum class top_decomposition
{
  none,
  and_,
  or_,
  lt_,
  le_,
  xor_
};

enum class bottom_decomposition
{
  none,
  and_,
  or_
};

template<class TT>
top_decomposition is_top_decomposable( const TT& tt, uint32_t var_index, TT* func = nullptr )
{
  auto var = tt.construct();
  kitty::create_nth_var( var, var_index );

  if ( implies( tt, var ) )
  {
    if ( func )
    {
      *func = cofactor1( tt, var_index );
    }
    return top_decomposition::and_;
  }
  else if ( implies( var, tt ) )
  {
    if ( func )
    {
      *func = cofactor0( tt, var_index );
    }
    return top_decomposition::or_;
  }
  else if ( implies( tt, ~var ) )
  {
    if ( func )
    {
      *func = cofactor0( tt, var_index );
    }
    return top_decomposition::lt_;
  }
  else if ( implies( ~var, tt ) )
  {
    if ( func )
    {
      *func = cofactor1( tt, var_index );
    }
    return top_decomposition::le_;
  }

  /* try XOR */
  const auto co0 = cofactor0( tt, var_index );
  const auto co1 = cofactor1( tt, var_index );

  if ( equal( co0, ~co1 ) )
  {
    if ( func )
    {
      *func = co0;
    }
    return top_decomposition::xor_;
  }

  return top_decomposition::none;
}

template<class TT>
bool _is_and_bottom_decomposable( const TT& tt, uint32_t var_index1, uint32_t var_index2, TT* func = nullptr )
{
  if ( var_index2 < var_index1 )
  {
    std::swap( var_index1, var_index2 );
  }

  auto var1 = tt.construct();
  auto var2 = tt.construct();
  kitty::create_nth_var( var1, var_index1 );
  kitty::create_nth_var( var2, var_index2 );

  /* copy out bit at 00 and 11 */
  auto mask00 = tt.construct();
  auto mask11 = tt.construct();

  auto it = tt.begin();
  auto it1 = var1.begin();
  auto it2 = var2.begin();
  auto it00 = mask00.begin();
  auto it11 = mask11.begin();

  for ( ; it != tt.end(); ++it, ++it1, ++it2 )
  {
    *it00++ = *it & ~( *it1 ) & ~( *it2 );
    *it11++ = *it & *it1 & *it2;
  }

  auto sh1 = shift_left( mask00, 1 << var_index1 );
  auto sh2 = shift_left( mask00, 1 << var_index2 );

  /* shift 00 to 01 and 10 positions */
  it = tt.begin();
  it00 = mask00.begin();
  it11 = mask11.begin();
  auto it01 = sh1.begin();
  auto it10 = sh2.begin();

  for ( ; it != tt.end(); ++it, ++it00, ++it01, ++it10, ++it11 )
  {
    if ( ( *it00 | *it01 | *it10 | *it11 ) != *it )
    {
      return false;
    }
  }

  if ( func )
  {
    *func = binary_or( cofactor0( tt, var_index2 ), cofactor1( tt, var_index2 ) );
  }

  return true;
}

template<class TT>
bool _is_or_bottom_decomposable( const TT& tt, uint32_t var_index1, uint32_t var_index2, TT* func = nullptr )
{
  if ( var_index2 < var_index1 )
  {
    std::swap( var_index1, var_index2 );
  }

  auto var1 = tt.construct();
  auto var2 = tt.construct();
  kitty::create_nth_var( var1, var_index1 );
  kitty::create_nth_var( var2, var_index2 );

  /* copy out bit at 00 and 11 */
  auto mask00 = tt.construct();
  auto mask01 = tt.construct();
  auto mask11 = tt.construct();

  auto it = tt.begin();
  auto it1 = var1.begin();
  auto it2 = var2.begin();
  auto it00 = mask00.begin();
  auto it11 = mask11.begin();
  auto it01 = mask01.begin();

  for ( ; it != tt.end(); ++it, ++it1, ++it2 )
  {
    *it00++ = *it & ~( *it1 ) & ~( *it2 );
    *it11++ = *it & *it1 & *it2;
    *it01++ = *it & ~( *it1 ) & *it2;
  }

  auto sh1 = shift_right( mask11, 1 << var_index1 );
  auto sh2 = shift_right( mask11, 1 << var_index2 );

  /* shift 00 to 01 and 10 positions */
  it = tt.begin();
  it00 = mask00.begin();
  it11 = mask11.begin();
  auto its01 = sh1.begin();
  auto its10 = sh2.begin();

  for ( ; it != tt.end(); ++it, ++it00, ++its01, ++its10, ++it11 )
  {
    if ( ( *it00 | *its01 | *its10 | *it11 ) != *it )
    {
      return false;
    }
  }

  // 11 -> a => a
  // 10 -> a => a
  // 01 -> a => b
  // 00 -> b => b
  if ( func )
  {

    *func = binary_or( binary_and( tt, unary_not( mask01 ) ), shift_left( mask00, 1 << var_index2 ) );
  }

  return true;
}

template<class TT>
bottom_decomposition is_bottom_decomposable( const TT& tt, uint32_t var_index1, uint32_t var_index2, TT* func = nullptr )
{
  if ( _is_and_bottom_decomposable( tt, var_index1, var_index2, func ) )
  {
    return bottom_decomposition::and_;
  }
  else if ( _is_or_bottom_decomposable( tt, var_index1, var_index2, func ) )
  {
    return bottom_decomposition::or_;
  }
  return bottom_decomposition::none;
}

} // namespace kitty
