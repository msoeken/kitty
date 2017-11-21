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

#include <cstdint>
#include <iostream>
#include <unordered_set>

#include <kitty/kitty.hpp>

/* compile time constant for the number of variables */
auto constexpr num_vars = 5;

/* truth table type in this example */
using truth_table = kitty::static_truth_table<num_vars>;

/* spectral class type */
using class_entry = std::tuple<truth_table, uint64_t, uint64_t>;

int main( int argc, char** argv )
{
  static_assert( num_vars <= 5, "number of variables is limited to 5" );

  /* set to store all visited functions during NPN canonization (dynamic to store bits on heap) */
  kitty::dynamic_truth_table map( truth_table::NumBits );

  /* invert bits: 1 means not classified yet */
  std::transform( map.cbegin(), map.cend(), map.begin(), []( auto word ) { return ~word; } );

  /* set to store all NPN and spectral representatives */
  using truth_table_vec = std::vector<class_entry>;
  truth_table_vec classes;
  classes.reserve( 48 );

  /* start from 0 */
  int64_t index = 0;
  truth_table tt;

  /* for counting the classes and functions */
  uint64_t ones = map.num_bits();

  auto ctr = 0, bctr = 0;

  while ( index != -1 )
  {
    /* create truth table from index value */
    kitty::create_from_words( tt, &index, &index + 1 );

    /* apply NPN canonization and add resulting representative to set;
       also cross out all the visited functions */
    const auto f_npn = std::get<0>( kitty::exact_npn_canonization( tt, [&map]( const auto& tt ) { kitty::clear_bit( map, *tt.cbegin() ); } ) );

    auto new_ones = count_ones( map );
    auto func_count = ones - new_ones;
    ones = new_ones;

    const auto spectral = kitty::exact_spectral_canonization( f_npn );

    auto it = std::find_if( classes.begin(), classes.end(),
                            [&spectral]( const auto& entry ) { return std::get<0>( entry ) == spectral; } );

    if ( it == classes.end() )
    {
      classes.emplace_back( spectral, 1u, func_count );
    }
    else
    {
      std::get<1>( *it )++;
      std::get<2>( *it ) += func_count;
    }

    std::cout << ".";

    if ( ++ctr == 100 )
    {
      std::cout << " " << ++bctr << std::endl;
      ctr = 0;
    }

    /* find next non-classified truth table */
    index = find_first_one_bit( map, index );
  }

  std::cout << "[i] enumerated "
            << map.num_bits() << " functions into "
            << classes.size() << " classes." << std::endl;

  std::cout << "[i] detailed classes:" << std::endl;

  for ( const auto& p : classes )
  {
    print_hex( std::get<0>( p ) );
    std::cout << "   ";
    print_spectrum( std::get<0>( p ) );
    std::cout << " " << std::setw( 8 ) << std::get<1>( p ) << " " << std::setw( 8 ) << std::get<2>( p ) << std::endl;
  }

  return 0;
}
