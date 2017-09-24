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

#include <array>
#include <cstdint>

#include "detail/constants.hpp"

namespace kitty
{

/*! Truth table in which number of variables is known at compile time.

  We dispatch on the Boolean template parameter to distinguish between
  a small truth table (up to 6 variables) and a large truth table
  (more than 6 variables).  A small truth table fits into a single
  block and therefore dedicated optimizations are possible.
 */
template<int NumVars, bool = ( NumVars <= 6 )>
struct static_truth_table;

/*! Truth table (for up to 6 variables) in which number of variables is known at compile time.
 */
template<int NumVars>
struct static_truth_table<NumVars, true>
{
  /*! \cond PRIVATE */
  enum
  {
    NumBits = 1 << NumVars
  };
  /*! \endcond */

  /*! Constructs a new static truth table instance with the same number of variables. */
  inline static_truth_table<NumVars> construct() const
  {
    return static_truth_table<NumVars>();
  }

  /*! Returns number of variables.
   */
  inline auto num_vars() const noexcept { return NumVars; }

  /*! Returns number of blocks.
   */
  inline auto num_blocks() const noexcept { return 1; }

  /*! Returns number of bits.
   */
  inline auto num_bits() const noexcept { return NumBits; }

  /*! \cond PRIVATE */
public: /* fields */
  uint64_t _bits = 0;
  /*! \endcond */
};

/*! Truth table (more than 6 variables) in which number of variables is known at compile time.
 */
template<int NumVars>
struct static_truth_table<NumVars, false>
{
  /*! \cond PRIVATE */
  enum
  {
    NumBlocks = ( NumVars <= 6 ) ? 1 : ( 1 << ( NumVars - 6 ) )
  };

  enum
  {
    NumBits = 1 << NumVars
  };
  /*! \endcond */

  /*! Standard constructor.

    The number of variables provided to the truth table must be known
    at runtime.  The number of blocks will be computed as a compile
    time constant.
  */
  static_truth_table()
  {
    _bits.fill( 0 );
  }

  /*! Constructs a new static truth table instance with the same number of variables. */
  inline static_truth_table<NumVars> construct() const
  {
    return static_truth_table<NumVars>();
  }

  /*! Returns number of variables.
   */
  inline auto num_vars() const noexcept { return NumVars; }

  /*! Returns number of blocks.
   */
  inline auto num_blocks() const noexcept { return NumBlocks; }

  /*! Returns number of bits.
   */
  inline auto num_bits() const noexcept { return NumBits; }

  /*! \cond PRIVATE */
public: /* fields */
  std::array<uint64_t, NumBlocks> _bits;
  /*! \endcond */
};
} // namespace kitty
