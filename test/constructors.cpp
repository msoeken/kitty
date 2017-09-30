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

TEST( ConstructorsTest, create_from_bitstring )
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

  EXPECT_EQ( tt_s._bits[0u], 0xfee8e880e8808000 );
  EXPECT_EQ( tt_s._bits[1u], 0xfffefee8fee8e880 );

  dynamic_truth_table tt_d( 7 );
  create_majority( tt_d );

  EXPECT_TRUE( std::equal( std::begin( tt_d._bits ), std::end( tt_d._bits ), std::begin( tt_s._bits ), std::end( tt_s._bits ) ) );
}
