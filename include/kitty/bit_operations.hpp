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

#include <cstdint>

#include "static_truth_table.hpp"

namespace kitty
{

/*! \brief Sets bit at index to true

  \param tt Truth table
  \param index Bit index
*/
template<typename TT>
void set_bit( TT& tt, uint64_t index )
{
  tt._bits[index >> 6] |= uint64_t( 1 ) << ( index & 0x3f );
}

/*! \cond PRIVATE */
template<int NumVars>
void set_bit( static_truth_table<NumVars, true>& tt, uint64_t index )
{
  tt._bits |= uint64_t( 1 ) << index;
}
/*! \endcond */

/*! \brief Gets bit at index

  \param tt Truth table
  \param index Bit index

  \return 1 if bit is set, otherwise 0
*/
template<typename TT>
auto get_bit( const TT& tt, uint64_t index )
{
  return ( tt._bits[index >> 6] >> ( index & 0x3f ) ) & 0x1;
}

/*! \cond PRIVATE */
template<int NumVars>
auto get_bit( const static_truth_table<NumVars, true>& tt, uint64_t index )
{
  return ( tt._bits >> index ) & 0x1;
}
/*! \endcond */

/*! \brief Clears bit at index

  \param tt Truth table
  \param index Bit index

  \return 1 if bit is set, otherwise 0
*/
template<typename TT>
void clear_bit( TT& tt, uint64_t index )
{
  tt._bits[index >> 6] &= ~( uint64_t( 1 ) << ( index & 0x3f ) );
}

/*! \cond PRIVATE */
template<int NumVars>
void clear_bit( static_truth_table<NumVars, true>& tt, uint64_t index )
{
  tt._bits &= ~( uint64_t( 1 ) << index );
}
/*! \endcond */

/*! \brief Clears all bits

  \param tt Truth table
*/
template<typename TT>
void clear( TT& tt )
{
  std::fill( std::begin( tt._bits ), std::end( tt._bits ), 0 );
}

/*! \cond PRIVATE */
template<int NumVars>
void clear( static_truth_table<NumVars, true>& tt )
{
  tt._bits = 0;
}
/*! \endcond */
} // namespace kitty
