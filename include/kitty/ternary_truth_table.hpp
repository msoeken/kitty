/* kitty: C++ truth table library
 * Copyright (C) 2017-2022  EPFL
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
  \file ternary_truth_table.hpp
  \brief Implements ternary_truth_table

  \author Siang-Yun Lee
*/

#pragma once

#include <cstdint>
#include <type_traits>
#include <vector>

#include "detail/constants.hpp"
#include "traits.hpp"

namespace kitty
{

template<class TT>
struct ternary_truth_table
{
  /*! Standard constructor.

    Initialize the truth table using the constructor of the inner
    truth table type.

    \param n Number of variables or number of bits (when `TT = partial_truth_table`)
  */
  explicit ternary_truth_table( uint32_t n )
      : _care( n ), _bits( n )
  {
  }

  /*! Empty constructor.

    Creates an empty truth table by calling the empty constructor
    of the inner truth table type.
   */
  ternary_truth_table() : _care(), _bits() {}

  /*! Constructs a new ternary_truth_table instance of the same size. */
  inline ternary_truth_table<TT> construct() const
  {
    if constexpr ( std::is_same_v<TT, partial_truth_table> )
      return ternary_truth_table<TT>( _bits.num_bits() );
    else
      return ternary_truth_table<TT>( _bits.num_vars() );
  }

  /*! Returns number of variables.
   */
  template<typename = std::enable_if_t<is_complete_truth_table<TT>::value>>
  auto num_vars() const noexcept { return _bits.num_vars(); }

  /*! Returns number of blocks.
   */
  inline auto num_blocks() const noexcept { return _bits.num_blocks(); }

  /*! Returns number of bits.
   */
  inline auto num_bits() const noexcept { return _bits.num_bits(); }

#if 0
  /*! \brief Begin iterator to bits.
   */
  inline auto begin() noexcept { return _bits.begin(); }

  /*! \brief End iterator to bits.
   */
  inline auto end() noexcept { return _bits.end(); }

  /*! \brief Begin iterator to bits.
   */
  inline auto begin() const noexcept { return _bits.begin(); }

  /*! \brief End iterator to bits.
   */
  inline auto end() const noexcept { return _bits.end(); }

  /*! \brief Reverse begin iterator to bits.
   */
  inline auto rbegin() noexcept { return _bits.rbegin(); }

  /*! \brief Reverse end iterator to bits.
   */
  inline auto rend() noexcept { return _bits.rend(); }

  /*! \brief Constant begin iterator to bits.
   */
  inline auto cbegin() const noexcept { return _bits.cbegin(); }

  /*! \brief Constant end iterator to bits.
   */
  inline auto cend() const noexcept { return _bits.cend(); }

  /*! \brief Constant reverse begin iterator to bits.
   */
  inline auto crbegin() const noexcept { return _bits.crbegin(); }

  /*! \brief Constant teverse end iterator to bits.
   */
  inline auto crend() const noexcept { return _bits.crend(); }
#endif

  /*! \brief Assign other truth table.

    This replaces the current truth table with another truth table.
    The other truth table must also be ternary, and the inner type of
    the other truth table must be assignable to the inner type of this
    truth table.

    \param other Other truth table
  */
  template<class TT2>
  ternary_truth_table<TT>& operator=( const ternary_truth_table<TT2>& other )
  {
    _bits = other._bits;
    _care = other._care;

    return *this;
  }

  /*! Masks valid truth table bits.

    This operation makes sure to zero out all unused bits.
  */
  inline void mask_bits() noexcept
  {
    _care.mask_bits();
    _bits.mask_bits();
  }

  /*! \cond PRIVATE */
public: /* fields */
  TT _care;
  TT _bits;
  /*! \endcond */
};


template<class TT>
struct is_truth_table<kitty::ternary_truth_table<TT>> : is_truth_table<TT> {};

template<class TT>
struct is_complete_truth_table<kitty::ternary_truth_table<TT>> : is_complete_truth_table<TT> {};

} // namespace kitty
