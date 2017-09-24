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
#include <vector>

#include "detail/constants.hpp"

namespace kitty
{

/*! Truth table in which number of variables is known at runtime.
*/
struct dynamic_truth_table
{
  /*! Standard constructor.

    The number of variables provided to the truth table can be
    computed at runtime.  However, once the truth table is constructed
    its number of variables cannot change anymore.

    The constructor computes the number of blocks and resizes the
    vector accordingly.

    \param num_vars Number of variables
  */
  explicit dynamic_truth_table( int num_vars )
      : _bits( ( num_vars <= 6 ) ? 1 : ( 1 << ( num_vars - 6 ) ) ),
        _num_vars( num_vars )
  {
  }

  /*! Returns number of variables.
   */
  inline auto num_vars() const noexcept { return _num_vars; }

  /*! Returns number of blocks.
   */
  inline auto num_blocks() const noexcept { return _bits.size(); }

  /*! Returns number of bits.
   */
  inline auto num_bits() const noexcept { return uint64_t( 1 ) << _num_vars; }

  /*! \cond PRIVATE */
public: /* fields */
  std::vector<uint64_t> _bits;
  int _num_vars;
  /*! \endcond */
};
} // namespace kitty
