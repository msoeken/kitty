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
  \file cube.hpp
  \brief A cube data structure for up to 32 variables

  \author Mathias Soeken
*/

#pragma once

#include <functional>
#include <iostream>

#include "hash.hpp"

namespace kitty
{
class cube
{
public:
  /*! \brief Constructs the empty cube

    Represents the one-cube
  */
  cube() : _value( 0 ) {}

  /*! \brief Constructs a cube from bits and mask

    For a valid cube and to be consistent in the ternary values, we
    assume that whenever a bit in the care bitmask is set to 0, also
    the polarity bitmask must be 0.

    \param bits Polarity bitmask of variables (0: negative, 1: positive)
    \param mask Care bitmask of variables (1: part of cube, 0: not part of cube)
  */
  cube( uint32_t bits, uint32_t mask ) : _bits( bits ), _mask( mask ) {}

  /*! \brief Returns number of literals */
  inline int num_literals() const
  {
    return __builtin_popcount( _mask );
  }

  /*! \brief Returns the distance to another cube */
  inline int distance( const cube& that ) const
  {
    return ( _bits ^ that._bits ) | ( _mask ^ that._mask );
  }

  /*! \brief Checks whether two cubes are equivalent */
  inline bool operator==( const cube& that ) const
  {
    return _value == that._value;
  }

  /*! \brief Checks whether two cubes are not equivalent */
  inline bool operator!=( const cube& that ) const
  {
    return _value != that._value;
  }

  /*! \brief Merges two cubes of distance-1 */
  inline cube merge( const cube& that ) const
  {
    const auto d = ( _bits ^ that._bits ) | ( _mask ^ that._mask );
    return cube( _bits ^ ( ~that._bits & d ), _mask ^ ( that._mask & d ) );
  }

  /*! \brief Adds literal to cube */
  inline void add_literal( uint8_t var_index, bool polarity = true )
  {
    _mask |= 1 << var_index;

    if ( polarity )
    {
      _bits |= 1 << var_index;
    }
    else
    {
      _bits &= ~( 1 << var_index );
    }
  }

  /*! \brief Removes literal from cube */
  inline void remove_literal( uint8_t var_index )
  {
    _mask &= ~( 1 << var_index );
    _bits &= ~( 1 << var_index );
  }

  /*! \brief Constructs the elementary cube representing a single variable */
  static cube nth_var_cube( uint8_t var_index )
  {
    const auto _bits = 1 << var_index;
    return cube( _bits, _bits );
  }

  /*! \brief Prints a cube */
  inline void print( unsigned length = 32u, std::ostream& os = std::cout ) const
  {
    for ( auto i = 0u; i < length; ++i )
    {
      os << ( ( ( _mask >> i ) & 1 ) ? ( ( ( _bits >> i ) & 1 ) ? '1' : '0' ) : '-' );
    }
  }

  /* cube data */
  union {
    struct
    {
      uint32_t _bits;
      uint32_t _mask;
    };
    uint64_t _value;
  };
};

template<>
struct hash<cube>
{
  std::size_t operator()( const cube& c ) const
  {
    return std::hash<uint64_t>{}( c._value );
  }
};
}
