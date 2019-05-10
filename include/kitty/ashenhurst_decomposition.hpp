/* kitty: C++ truth table library
 * Copyright (C) 2017-2019  EPFL
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
  \file ashenhurst_decomposition.hpp
  \brief Given a partition of function arguments, finds all of the ashenhurst decompositions.

  \author Mahyar Emami (mahyar.emami@epfl.ch)
*/

#pragma once

#include <cstdint>

#include "constructors.hpp"
#include "operations.hpp"
#include <bitset>

namespace kitty
{

namespace detail
{

/*! Composes a truth table
  Given a function f represented as f(.), and a set of truth tables as arguments,
  computes the composed truth table.
  For example, if f(x1, x2) = 1001 and vars = {x1 = 1001, x2= 1010} the function
  returns 1000 This function could be viewed as a general operator with arity 
  vars.size() where the behavior of the operator is given by f.
  \param f The outer function
  \param vars The ordered set of input variables
  \return The composed truth table with vars.size() variables
*/
template<class TTf, class TTv>
auto compose_truth_table( const TTf& f, const std::vector<TTv> &vars )
{
  assert( vars.size() == f.num_vars() );
  auto composed = vars[0].construct();
  for ( uint64_t i = 0; i < composed.num_bits(); i++ )
  {

    uint64_t index = 0;
    for ( uint64_t j = 0; j < vars.size(); j++ )
    {
      index += get_bit( vars[j], i ) << j;
    }
    auto bit = get_bit( f, index );
    if ( bit == 1 )
    {
      set_bit( composed, i );
    }
    else
    {
      clear_bit( composed, i );
    }
  }
  return composed;
}
/*! A helper function to enumerate missing indices
  \param ys_index A list of already selected indices
  \param max_index The maximum value for an index
  \return Remaining indices
*/
std::vector<uint32_t>
enumerate_zs_index( std::vector<uint32_t> ys_index, uint32_t max_index )
{

  std::vector<uint32_t> zs_index;
  for ( uint32_t i = 0; i <= max_index; i++ )
    if ( std::find( ys_index.begin(), ys_index.end(), i ) == ys_index.end() )
      zs_index.push_back( i );

  return zs_index;
}
std::string make_binary_string( uint64_t val, int length )
{
  std::string str = "";
  for ( int i = length - 1; i >= 0; i-- )
    str += std::to_string( ( val >> i ) % 2 );
  return str;
}

} // namespace detail

/*! Is a function ashenhurst decomposable
  Given functions f(.), g(.), and h(.) and a partition
  on arguments into z and y. This function determines whether 
  f(x) is decomposable into g(z, h(y)) where x = union(z,y) and 
  intersect(z, y) = null.
  This function does not check for permutation of variables given by
  zs_index and ys_index. The elements in these vectors are treated as ordered
  values.
  \param tt The function to the check the decomposition on (function f)
  \param zs_index The ordered set of indices of vector x (input to f) that
                  are the inputs to outer_func (g).
  \param ys_index The ordered set of indices of vector x (input to f) that are
                  input to the inner_func (h).
  \param outer_func The outer decomposition function (function g).
  \param inner_func The inner decomposition function (function h).
  \return true if the given decomposition is a valid one, false otherwise.
*/
template<class TTf, class TTg, class TTh>
bool ashenhurst_decomposable( const TTf& tt,
                              const std::vector<uint32_t> &zs_index,
                              const std::vector<uint32_t> &ys_index,
                              const TTg& outer_func,
                              const TTh& inner_func )
{
  std::vector<TTf> y_vars;
  std::vector<TTf> z_vars;

  for ( const auto idx : ys_index )
  {
    auto var = tt.construct();
    create_nth_var( var, idx );
    y_vars.push_back( var );
  }
  for ( const auto idx : zs_index )
  {
    auto var = tt.construct();
    create_nth_var( var, idx );
    z_vars.push_back( var );
  }
  auto h = detail::compose_truth_table( inner_func, y_vars );
  z_vars.push_back( h );
  auto f = detail::compose_truth_table( outer_func, z_vars );
  return equal( f, tt );
}

/*! Finds all of the possible decompositions of a function given an input 
  partitioning.

  \param tt The function to find all of its decompositions 
  \param ys_index Indices indicating the partitioning of inputs
  \param decomposition A vector of decomposition pairs. This serves as a return
                       return container.
  \return Returns the number of possible decompositions.
*/
template<class TTf, class TTg, class TTh>
int ashenhurst_decomposition( const TTf& tt, const std::vector<uint32_t> &ys_index, std::vector<std::pair<TTg, TTh>>& decomposition )
{
  std::vector<uint32_t> zs_index = detail::enumerate_zs_index( ys_index, tt.num_vars() - 1 );
  decomposition.clear();
  //assert( decomposition.empty() );
  int num_vars_h = ys_index.size();
  int num_vars_g = tt.num_vars() - num_vars_h + 1;

  uint64_t num_bits_h = ( uint64_t( 1 ) << num_vars_h );
  uint64_t num_bits_g = ( uint64_t( 1 ) << num_vars_g );


  // Note that this implementation does not support decompositions in which
  // either f or h have more than 6 variables.
  assert( num_bits_g <= 64 );
  assert( num_bits_h <= 64 );

  uint64_t num_funcs_h = ( uint64_t( 1 ) << num_bits_h );
  uint64_t num_funcs_g = ( uint64_t( 1 ) << num_bits_g );

  for ( uint64_t i = 0; i < num_funcs_g; i++ )
  {
    auto g_string = detail::make_binary_string( i, num_bits_g );
    TTg g;
    create_from_binary_string( g, g_string );
    for ( uint64_t j = 0; j < num_funcs_h; j++ )
    {
      auto h_string = detail::make_binary_string( j, num_bits_h );
      TTh h;
      create_from_binary_string( h, h_string );
      if ( ashenhurst_decomposable( tt, zs_index, ys_index, g, h ) )
        decomposition.push_back( std::make_pair( g, h ) );
    }
  }

  return decomposition.size();
}

} // namespace kitty
