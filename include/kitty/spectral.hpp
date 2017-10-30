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
  \file spectral.hpp
  \brief Implements methods for spectral classification

  \author Mathias Soeken
*/

#pragma once

#include "bit_operations.hpp"

namespace kitty
{

namespace detail
{
struct spectral_operation
{
  enum class kind : uint16_t
  {
    permutation,
    input_negation,
    output_negation,
    spectral_translation,
    disjoint_translation
  };

  spectral_operation() : _data( 0 ) {}
  spectral_operation( kind _kind, uint16_t _var1 = 0, uint16_t _var2 = 0 ) : _kind( _kind ), _var1( _var1 ), _var2( _var2 ) {}

  union {
    struct
    {
      kind _kind : 4;
      uint16_t _var1 : 6;
      uint16_t _var2 : 6;
    };
    uint16_t _data;
  };
};

void fast_hadamard_transform( std::vector<int32_t>& s, bool reverse = false )
{
}

class spectrum
{
public:
  spectrum() = delete;

  spectrum( const spectrum& other ) : _s( std::begin( other._s ), std::end( other._s ) ) {}

  spectrum& operator=( const spectrum& other )
  {
    if ( this != &other )
    {
      _s = this->_s;
    }
    return *this;
  }

private:
  spectrum( const std::vector<int32_t>& _s ) : _s( _s ) {}

public:
  template<typename TT>
  static spectrum from_truth_table( const TT& tt )
  {
    std::vector<int32_t> _s( tt.num_bits(), 1 );
    fast_hadamard_transform( _s );
    return spectrum( _s );
  }

  template<typename TT>
  TT to_truth_table() const
  {
    auto copy = _s;
    fast_hadamard_transform( copy, true );

    auto tt = TT::construct();
    for ( auto i = 0u; i < copy.size(); ++i )
    {
      if ( copy[i] == -1 )
      {
        set_bit( tt, i );
      }
    }
    return tt;
  }

  // void apply( const trans_t& trans );
  // trans_t trans1( unsigned i, unsigned j ); /* xi <-> xj */
  // trans_t trans2( unsigned i ); /* xi <- !xi */
  // trans_t trans3(); /* f <- !f */
  // trans_t trans4( unsigned i, unsigned j ); /* xi <- xi XOR xj */
  // trans_t trans5( unsigned i ); /* f <- f XOR xi */

  // inline std::vector<int>::reference operator[]( std::vector<int>::size_type pos )
  // {
  //   return _s[pos];
  // }

  // inline std::vector<int>::const_reference operator[]( std::vector<int>::size_type pos ) const
  // {
  //   return _s[pos];
  // }

  // inline std::vector<int>::size_type size() const
  // {
  //   return _s.size();
  // }

private:
  std::vector<int32_t> _s;
};
}

} // namespace kitty
