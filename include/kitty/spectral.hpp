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

inline void fast_hadamard_transform( std::vector<int32_t>& s, bool reverse = false )
{
  unsigned k{};
  int t{};

  for ( auto m = 1u; m < s.size(); m <<= 1u )
  {
    for ( auto i = 0u; i < s.size(); i += ( m << 1u ) )
    {
      for ( auto j = i, p = k = i + m; j < p; ++j, ++k )
      {
        t = s[j];
        s[j] += s[k];
        s[k] = t - s[k];
      }
    }
  }

  if ( reverse )
  {
    for ( auto i = 0u; i < s.size(); ++i )
    {
      s[i] /= static_cast<int>( s.size() );
    }
  }
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
    for_each_one_bit( tt, [&_s]( auto bit ) { _s[bit] = -1; } );
    fast_hadamard_transform( _s );
    return spectrum( _s );
  }

  template<typename TT>
  TT to_truth_table() const
  {
    auto copy = _s;
    fast_hadamard_transform( copy, true );

    TT tt;
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
  spectral_operation permutation( unsigned i, unsigned j )
  {
    spectral_operation op( spectral_operation::kind::permutation, i, j );

    i = 1 << i;
    j = 1 << j;
    for ( auto k = 0u; k < _s.size(); ++k )
    {
      if ( ( k & i ) > 0 && ( k & j ) == 0 )
      {
        std::swap( _s[k], _s[k - i + j] );
      }
    }

    return op;
  }

  spectral_operation input_negation( unsigned i )
  {
    spectral_operation op( spectral_operation::kind::input_negation, i );
    i = 1 << i;
    for ( auto k = 0u; k < _s.size(); ++k )
    {
      if ( ( k & i ) > 0 )
      {
        _s[k] = -_s[k];
      }
    }

    return op;
  }

  spectral_operation output_negation()
  {
    for ( auto k = 0u; k < _s.size(); ++k )
    {
      _s[k] = -_s[k];
    }
    return spectral_operation( spectral_operation::kind::output_negation );
  }

  spectral_operation spectral_translation( int i, int j )
  {
    spectral_operation op( spectral_operation::kind::spectral_translation, i, j );

    i = 1 << i;
    j = 1 << j;

    for ( auto k = 0u; k < _s.size(); ++k )
    {
      if ( ( k & i ) > 0 && ( k & j ) == 0 )
      {
        std::swap( _s[k], _s[k + j] );
      }
    }

    return op;
  }

  spectral_operation disjoint_translation( int i )
  {
    spectral_operation op( spectral_operation::kind::disjoint_translation, i );

    i = 1 << i;

    for ( auto k = 0u; k < _s.size(); ++k )
    {
      if ( ( k & i ) > 0 )
      {
        std::swap( _s[k], _s[k - i] );
      }
    }

    return op;
  }

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
