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

#include "static_truth_table.hpp"
#include "detail/constants.hpp"

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
    if ( tt.num_vars() < 6 )
    {
      tt._bits[0] &= ( uint64_t( 1 ) << ( uint64_t( 1 ) << tt.num_vars() ) ) - 1;
    }
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
  if ( tt.num_vars() < 6 )
  {
    tt._bits &= ( uint64_t( 1 ) << ( uint64_t( 1 ) << tt.num_vars() ) ) - 1;
  }
}
/*! \endcond */

/*! Constructs majority-n function.

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
