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

  auto permutation( unsigned i, unsigned j )
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

  auto input_negation( unsigned i )
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

  auto output_negation()
  {
    for ( auto k = 0u; k < _s.size(); ++k )
    {
      _s[k] = -_s[k];
    }
    return spectral_operation( spectral_operation::kind::output_negation );
  }

  auto spectral_translation( int i, int j )
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

  auto disjoint_translation( int i )
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

  void apply( const spectral_operation& op )
  {
    switch ( op._kind )
    {
    case spectral_operation::kind::permutation:
      permutation( op._var1, op._var2 );
      break;
    case spectral_operation::kind::input_negation:
      input_negation( op._var1 );
      break;
    case spectral_operation::kind::output_negation:
      output_negation();
      break;
    case spectral_operation::kind::spectral_translation:
      spectral_translation( op._var1, op._var2 );
      break;
    case spectral_operation::kind::disjoint_translation:
      disjoint_translation( op._var1 );
      break;
    }
  }

  inline auto operator[]( std::vector<int32_t>::size_type pos )
  {
    return _s[pos];
  }

  inline auto operator[]( std::vector<int32_t>::size_type pos ) const
  {
    return _s[pos];
  }

  inline auto size() const
  {
    return _s.size();
  }

private:
  std::vector<int32_t> _s;
};

template<typename TT>
class miller_spectral_canonization_impl
{
public:
  explicit miller_spectral_canonization_impl( const TT& func )
      : func( func ),
        num_vars( func.num_vars() ),
        spec( spectrum::from_truth_table( func ) ),
        best_spec( spec ),
        transforms( 100u ),
        transform_index( 0u )
  {
  }

  TT run()
  {
    order = get_initial_coeffecient_order();
    normalize();
    return spec.to_truth_table<TT>();
  }

private:
  std::vector<unsigned> get_initial_coeffecient_order()
  {
    std::vector<unsigned> map( spec.size(), 0u );
    auto p = std::begin( map ) + 1;

    for ( auto i = 1u; i <= num_vars; ++i )
    {
      for ( auto j = 1u; j < spec.size(); ++j )
      {
        if ( __builtin_popcount( j ) == static_cast<int>( i ) )
        {
          *p++ = j;
        }
      }
    }

    return map;
  }

  unsigned transformation_costs( const std::vector<spectral_operation>& transforms )
  {
    auto costs = 0u;
    for ( const auto& t : transforms )
    {
      costs += ( t._kind == spectral_operation::kind::permutation ) ? 3u : 1u;
    }
    return costs;
  }

  void closer( spectrum& lspec )
  {
    for ( auto i = 0u; i < lspec.size(); ++i )
    {
      const auto j = order[i];
      if ( lspec[j] == best_spec[j] )
        continue;
      if ( abs( lspec[j] ) > abs( best_spec[j] ) ||
           ( abs( lspec[j] ) == abs( best_spec[j] ) && lspec[j] > best_spec[j] ) )
      {
        update_best( lspec );
        return;
      }

      if ( abs( lspec[j] ) < abs( best_spec[j] ) ||
           ( abs( lspec[j] ) == abs( best_spec[j] ) && lspec[j] < best_spec[j] ) )
      {
        return;
      }
    }

    if ( transformation_costs( transforms ) < transformation_costs( best_transforms ) )
    {
      update_best( lspec );
    }
  }

  void normalize_rec( spectrum& lspec, unsigned v )
  {
    if ( v == num_vars ) /* leaf case */
    {
      /* invert function if necessary */
      if ( lspec[0u] < 0 )
      {
        insert( lspec.output_negation() );
      }
      /* invert any variable as necessary */
      for ( auto i = 0u; i < num_vars; ++i )
      {
        if ( lspec[1 << i] < 0 )
        {
          insert( lspec.input_negation( i ) );
        }
      }

      closer( lspec );
      return;
    }
    const auto locked = ( 1 << v ) - 1;
    auto max = -1;

    for ( auto i = 1u; i < lspec.size(); ++i )
    {
      if ( ( locked & i ) == i )
        continue;
      max = std::max( max, abs( lspec[i] ) );
    }

    if ( max == 0 )
    {
      auto spec2 = lspec;
      normalize_rec( spec2, num_vars );
    }
    else
    {
      for ( auto i = 1u; i < lspec.size(); ++i )
      {
        auto j = order[i];
        if ( abs( lspec[j] ) != max )
          continue;

        const auto save = transform_index;
        auto spec2 = lspec;
        auto k = 0u;
        for ( ; k < num_vars; ++k )
        {
          if ( ( 1 << k ) & locked )
            continue;
          if ( ( 1 << k ) & j )
            break;
        }
        if ( k == num_vars )
          continue;
        j -= 1 << k;
        while ( j > 0 )
        {
          auto p = 0u;
          for ( ; ( ( 1 << p ) & j ) == 0; ++p )
            ;
          j -= 1 << p;
          insert( spec2.spectral_translation( k, p ) );
        }
        if ( k != v )
        {
          insert( spec2.permutation( k, v ) );
        }
        normalize_rec( spec2, v + 1 );
        transform_index = save;
      }
    }
  }

  void normalize()
  {
    /* find maximum absolute element in spectrum (by order) */
    auto max = abs( spec[0u] );
    auto j = 0u;
    for ( auto i = 1u; i < spec.size(); ++i )
    {
      auto p = order[i];
      if ( abs( spec[p] ) > max )
      {
        max = abs( spec[p] );
        j = p;
      }
    }

    /* if max element is not the first element */
    if ( j > 0 )
    {
      auto k = 0u;
      for ( ; ( ( 1 << k ) & j ) == 0; ++k )
        ;
      j -= 1 << k;
      while ( j > 0 )
      {
        auto p = k + 1;
        for ( ; ( ( 1 << p ) & j ) == 0; ++p )
          ;
        j -= 1 << p;
        insert( spec.spectral_translation( k, p ) );
      }
      insert( spec.disjoint_translation( k ) );
    }

    update_best( spec );
    normalize_rec( spec, 0u );
    spec = best_spec;
  }

  void insert( const spectral_operation& trans )
  {
    assert( transform_index < 100u );
    transforms[transform_index++] = trans;
  }

  void update_best( const spectrum& lbest )
  {
    best_spec = lbest;
    best_transforms.resize( transform_index );
    std::copy( transforms.begin(), transforms.begin() + transform_index, best_transforms.begin() );
  }

private:
  const TT& func;

  unsigned num_vars;
  spectrum spec;
  spectrum best_spec;

  std::vector<unsigned> order;
  std::vector<spectral_operation> transforms;
  std::vector<spectral_operation> best_transforms;
  unsigned transform_index = 0u;
};
} // namespace detail

template<typename TT>
inline TT exact_spectral_canonization( const TT& tt )
{
  detail::miller_spectral_canonization_impl<TT> impl( tt );
  return impl.run();
}

} // namespace kitty
