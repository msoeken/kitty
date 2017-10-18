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

#include <cctype>

#include "detail/constants.hpp"
#include "static_truth_table.hpp"

namespace kitty
{

/*! Constructs projections (single-variable functions).

  \param tt Truth table
  \param var_index Index of the variable, must be smaller than the truth table's number of variables
  \param complement If true, realize inverse projection
*/
template<typename TT>
void create_nth_var( TT& tt, uint64_t var_index, bool complement = false )
{
  if ( tt.num_vars() <= 6 )
  {
    /* assign from precomputed table */
    tt._bits[0] = complement ? !detail::projections[var_index] : detail::projections[var_index];

    /* mask if truth table does not require all bits */
    tt.mask_bits();
  }
  else if ( var_index < 6 )
  {
    std::fill( std::begin( tt._bits ), std::end( tt._bits ), complement ? ~detail::projections[var_index] : detail::projections[var_index] );
  }
  else
  {
    const auto c = 1 << ( var_index - 6 );
    const auto zero = uint64_t( 0 );
    const auto one = ~zero;
    auto block = 0;

    while ( block < tt.num_blocks() )
    {
      for ( auto i = 0; i < c; ++i )
      {
        tt._bits[block++] = complement ? one : zero;
      }
      for ( auto i = 0; i < c; ++i )
      {
        tt._bits[block++] = complement ? zero : one;
      }
    }
  }
};

/*! \cond PRIVATE */
template<int NumVars>
void create_nth_var( static_truth_table<NumVars, true>& tt, uint64_t var_index, bool complement = false )
{
  /* assign from precomputed table */
  tt._bits = complement ? ~detail::projections[var_index] : detail::projections[var_index];

  /* mask if truth table does not require all bits */
  tt.mask_bits();
}
/*! \endcond */

/*! Constructs truth table from binary string

  Note that the first character in the string represents the most
  significant bit in the truth table.  For example, the 2-input AND
  function is represented by the binary string "1000".  The number of
  characters in `binary` must match the number of bits in `tt`.

  \param tt Truth table
  \param binary Binary string with as many characters as bits in the truth table
*/
template<typename TT>
void create_from_binary_string( TT& tt, const std::string& binary )
{
  assert( binary.size() == tt.num_bits() );

  size_t i = 0u, j = binary.size();
  do
  {
    --j;
    if ( binary[i++] == '1' )
    {
      set_bit( tt, j );
    }
  } while ( j );
}

/*! Constructs truth table from hexadecimal string

  Note that the first character in the string represents the four most
  significant bit in the truth table.  For example, the 3-input
  majority function is represented by the binary string "E8" or "e8".
  The number of characters in `hex` must be one fourth the number of
  bits in `tt`.

  \param tt Truth table
  \param hex Hexadecimal string
*/
template<typename TT>
void create_from_hex_string( TT& tt, const std::string& hex )
{
  assert( ( hex.size() << 2 ) == tt.num_bits() );

  auto j = tt.num_bits() - 1;

  for ( unsigned char c : hex )
  {
    const auto i = detail::hex_to_int[c];
    if ( i & 8 ) set_bit( tt, j );
    if ( i & 4 ) set_bit( tt, j - 1 );
    if ( i & 2 ) set_bit( tt, j - 2 );
    if ( i & 1 ) set_bit( tt, j - 3 );
    j -= 4;
  }
}

/*! Constructs majority-\f$n\f$ function.

  The number of variables is determined from the truth table.

  \param tt Truth table
*/
template<typename TT>
void create_majority( TT& tt )
{
  const auto t = tt.num_vars() >> 1;

  for ( uint64_t x = 0; x < tt.num_bits(); ++x )
  {
    if ( __builtin_popcount( x ) > t )
    {
      set_bit( tt, x );
    }
  }
}
} // namespace kitty
