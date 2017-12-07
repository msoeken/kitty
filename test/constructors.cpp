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

#include <gtest/gtest.h>

#include <algorithm>

#include <kitty/kitty.hpp>

using namespace kitty;

TEST( ConstructorsTest, create_nth_var5 )
{
  static_truth_table<5> tt_s;
  dynamic_truth_table tt_d( 5 );

  const auto mask = 0xffffffff;

  for ( auto i = 0; i <= 4; ++i )
  {
    create_nth_var( tt_s, i );
    EXPECT_EQ( tt_s._bits, detail::projections[i] & mask );
    create_nth_var( tt_d, i );
    EXPECT_EQ( tt_s._bits, tt_d._bits[0] );
  }
}

TEST( ConstructorsTest, create_from_binary_string )
{
  static_truth_table<3> tt_s, tt_s_str;
  create_majority( tt_s );
  create_from_binary_string( tt_s_str, "11101000" );

  dynamic_truth_table tt_d( 3 ), tt_d_str( 3 );
  create_majority( tt_d );
  create_from_binary_string( tt_d_str, "11101000" );

  EXPECT_TRUE( equal( tt_s, tt_s_str ) );
  EXPECT_TRUE( equal( tt_d, tt_d_str ) );
}

TEST( ConstructorsTest, create_from_hex_string )
{
  static_truth_table<9> tt_s, tt_s_str;
  create_majority( tt_s );
  create_from_hex_string( tt_s_str, "fffffffefffefee8fffefee8fee8e880fffefee8fee8e880fee8e880e8808000fffefee8fee8e880fee8e880e8808000fee8e880e8808000e880800080000000" );

  dynamic_truth_table tt_d( 9 ), tt_d_str( 9 );
  create_majority( tt_d );
  create_from_hex_string( tt_d_str, "FFFFFFFEFFFEFEE8FFFEFEE8FEE8E880FFFEFEE8FEE8E880FEE8E880E8808000FFFEFEE8FEE8E880FEE8E880E8808000FEE8E880E8808000E880800080000000" );

  EXPECT_TRUE( equal( tt_s, tt_s_str ) );
  EXPECT_TRUE( equal( tt_d, tt_d_str ) );
}

TEST( ConstructorsTest, create_constants )
{
  static_truth_table<0> tt_s;
  EXPECT_EQ( tt_s.num_vars(), 0 );
  EXPECT_EQ( tt_s.num_bits(), 1 );

  create_from_hex_string( tt_s, "0" );
  EXPECT_EQ( tt_s._bits, 0x0u );

  create_from_hex_string( tt_s, "1" );
  EXPECT_EQ( tt_s._bits, 0x1u );
}

TEST( ConstructorsTest, create_one_variable_functions )
{
  static_truth_table<1> tt_s;
  EXPECT_EQ( tt_s.num_vars(), 1 );
  EXPECT_EQ( tt_s.num_bits(), 2 );

  create_from_hex_string( tt_s, "0" );
  EXPECT_EQ( tt_s._bits, 0x0u );

  create_from_hex_string( tt_s, "1" );
  EXPECT_EQ( tt_s._bits, 0x1u );

  create_from_hex_string( tt_s, "2" );
  EXPECT_EQ( tt_s._bits, 0x2u );

  create_from_hex_string( tt_s, "3" );
  EXPECT_EQ( tt_s._bits, 0x3u );
}

TEST( ConstructorsTest, create_random )
{
  static_truth_table<5> tt_s5;
  static_truth_table<7> tt_s7;
  dynamic_truth_table tt_d5( 5 );
  dynamic_truth_table tt_d7( 7 );

  create_random( tt_s5 );
  create_random( tt_s7 );
  create_random( tt_d5 );
  create_random( tt_d7 );

  // std::cout << tt_s5._bits[0] << std::endl;

  EXPECT_TRUE( true ); /* a dummy test to enable the print out */
}

TEST( ConstructorsTest, create_from_words )
{
  std::vector<uint64_t> words_vec{UINT64_C( 0xfee8e880e8808000 ), UINT64_C( 0xfffefee8fee8e880 )};
  constexpr uint64_t words_arr[]{UINT64_C( 0xfee8e880e8808000 ), UINT64_C( 0xfffefee8fee8e880 )};

  static_truth_table<7> tt_v, tt_a, tt_s;

  create_from_words( tt_v, words_vec.begin(), words_vec.end() );
  create_from_words( tt_a, words_arr, words_arr + 2 );
  create_majority( tt_s );

  EXPECT_EQ( tt_v, tt_s );
  EXPECT_EQ( tt_a, tt_s );
}

TEST( ConstructorsTest, create_majority5 )
{
  static_truth_table<5> tt_s;
  create_majority( tt_s );

  EXPECT_EQ( tt_s._bits, 0xfee8e880 );

  dynamic_truth_table tt_d( 5 );
  create_majority( tt_d );

  EXPECT_EQ( tt_s._bits, tt_d._bits[0u] );
}

TEST( ConstructorsTest, create_majority7 )
{
  static_truth_table<7> tt_s;
  create_majority( tt_s );

  EXPECT_EQ( tt_s._bits[0u], UINT64_C( 0xfee8e880e8808000 ) );
  EXPECT_EQ( tt_s._bits[1u], UINT64_C( 0xfffefee8fee8e880 ) );

  dynamic_truth_table tt_d( 7 );
  create_majority( tt_d );

  EXPECT_TRUE( std::equal( std::begin( tt_d._bits ), std::end( tt_d._bits ), std::begin( tt_s._bits ), std::end( tt_s._bits ) ) );
}

TEST( ConstructorsTest, create_threshold7 )
{
  static_truth_table<9> t1, t2;

  create_threshold( t1, 4 );
  create_threshold( t2, 5 );

  const auto t3 = t1 & ~t2;

  EXPECT_EQ( count_ones( t3 ), uint64_t( 126 ) );

  for_each_one_bit( t3, []( auto index ) {
    EXPECT_EQ( __builtin_popcount( index ), 5 );
  } );
}
