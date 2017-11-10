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

/*!
  \file print.hpp
  \brief Implements functions to print truth tables

  \author Mathias Soeken
*/

#pragma once

#include <iostream>
#include <string>

#include "algorithm.hpp"

namespace kitty
{

/*! \brief Prints truth table in binary representation

  The most-significant bit will be the first character of the string.

  \param tt Truth table
  \param os Output stream
*/
template<typename TT>
void print_binary( const TT& tt, std::ostream& os = std::cout )
{
  for_each_block_reversed( tt, [&tt, &os]( auto word ) {
    std::string chunk( std::min<uint64_t>( tt.num_bits(), 64 ), '0' );
    auto it = chunk.rbegin();
    while ( word && it < chunk.rend() )
    {
      if ( word & 1 )
      {
        *it = '1';
      }
      ++it;
      word >>= 1;
    }
    os << chunk;
  } );
}
}
