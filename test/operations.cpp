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

TEST( OperationsTest, binary_for_small )
{
  static_truth_table<2> tt1_s, tt2_s;

  create_nth_var( tt1_s, 0 );
  create_nth_var( tt2_s, 1 );

  EXPECT_EQ( binary_and( tt1_s, tt2_s )._bits, 0x8 );
  EXPECT_EQ( binary_or( tt1_s, tt2_s )._bits, 0xe );
  EXPECT_EQ( unary_not( binary_and( tt1_s, tt2_s ) )._bits, 0x7 );
  EXPECT_EQ( unary_not( binary_or( tt1_s, tt2_s ) )._bits, 0x1 );

  dynamic_truth_table tt1_d( 2 ), tt2_d( 2 );

  create_nth_var( tt1_d, 0 );
  create_nth_var( tt2_d, 1 );

  EXPECT_EQ( binary_and( tt1_d, tt2_d )._bits[0], 0x8 );
  EXPECT_EQ( binary_or( tt1_d, tt2_d )._bits[0], 0xe );
  EXPECT_EQ( unary_not( binary_and( tt1_d, tt2_d ) )._bits[0], 0x7 );
  EXPECT_EQ( unary_not( binary_or( tt1_d, tt2_d ) )._bits[0], 0x1 );
}

TEST( OperationsTest, ternary_for_small )
{
  {
    static_truth_table<3> tt1_s, tt2_s, tt3_s;

    create_nth_var( tt1_s, 0 );
    create_nth_var( tt2_s, 1 );
    create_nth_var( tt3_s, 2 );

    const auto maj_expr = binary_or( binary_or( binary_and( tt1_s, tt2_s ), binary_and( tt1_s, tt3_s ) ), binary_and( tt2_s, tt3_s ) );
    const auto maj_direct = ternary_majority( tt1_s, tt2_s, tt3_s );

    EXPECT_EQ( maj_expr._bits, 0xe8 );
    EXPECT_EQ( maj_expr._bits, maj_direct._bits );

    const auto ite_expr = binary_or( binary_and( tt1_s, tt2_s ), binary_and( unary_not( tt1_s ), tt3_s ) );
    const auto ite_direct = ternary_ite( tt1_s, tt2_s, tt3_s );

    EXPECT_EQ( ite_expr._bits, 0xd8 );
    EXPECT_EQ( ite_expr._bits, ite_direct._bits );
  }

  {
    dynamic_truth_table tt1_d( 3 ), tt2_d( 3 ), tt3_d( 3 );

    create_nth_var( tt1_d, 0 );
    create_nth_var( tt2_d, 1 );
    create_nth_var( tt3_d, 2 );

    const auto maj_expr = binary_or( binary_or( binary_and( tt1_d, tt2_d ), binary_and( tt1_d, tt3_d ) ), binary_and( tt2_d, tt3_d ) );
    const auto maj_direct = ternary_majority( tt1_d, tt2_d, tt3_d );

    EXPECT_EQ( maj_expr._bits[0], 0xe8 );
    EXPECT_EQ( maj_expr._bits[0], maj_direct._bits[0] );

    const auto ite_expr = binary_or( binary_and( tt1_d, tt2_d ), binary_and( unary_not( tt1_d ), tt3_d ) );
    const auto ite_direct = ternary_ite( tt1_d, tt2_d, tt3_d );

    EXPECT_EQ( ite_expr._bits[0], 0xd8 );
    EXPECT_EQ( ite_expr._bits[0], ite_direct._bits[0] );
  }
}

TEST( OperationsTest, binary_for_large )
{
  {
    static_truth_table<7> tt1_s, tt2_s;
    create_nth_var( tt1_s, 0 );
    create_nth_var( tt2_s, 1 );

    auto tt_s = binary_and( tt1_s, tt2_s );
    for ( auto i = 2; i < 7; ++i )
    {
      static_truth_table<7> ttv_s;
      create_nth_var( ttv_s, i );
      tt_s = binary_and( tt_s, ttv_s );
    }

    EXPECT_EQ( tt_s._bits[0], 0x0 );
    EXPECT_EQ( tt_s._bits[1], uint64_t( 1 ) << 63 );
  }

  {
    dynamic_truth_table tt1_d( 7 ), tt2_d( 7 );
    create_nth_var( tt1_d, 0 );
    create_nth_var( tt2_d, 1 );

    auto tt_d = binary_and( tt1_d, tt2_d );
    for ( auto i = 2; i < 7; ++i )
    {
      dynamic_truth_table ttv_d( 7 );
      create_nth_var( ttv_d, i );
      tt_d = binary_and( tt_d, ttv_d );
    }

    EXPECT_EQ( tt_d._bits[0], 0x0 );
    EXPECT_EQ( tt_d._bits[1], uint64_t( 1 ) << 63 );
  }
}

TEST( OperationsTest, ternary_for_large )
{
  {
    static_truth_table<7> tt1_s, tt2_s, tt3_s;

    create_nth_var( tt1_s, 0 );
    create_nth_var( tt2_s, 3 );
    create_nth_var( tt3_s, 6 );

    const auto maj_expr = binary_or( binary_or( binary_and( tt1_s, tt2_s ), binary_and( tt1_s, tt3_s ) ), binary_and( tt2_s, tt3_s ) );
    const auto maj_direct = ternary_majority( tt1_s, tt2_s, tt3_s );

    EXPECT_EQ( maj_expr._bits, maj_direct._bits );

    const auto ite_expr = binary_or( binary_and( tt1_s, tt2_s ), binary_and( unary_not( tt1_s ), tt3_s ) );
    const auto ite_direct = ternary_ite( tt1_s, tt2_s, tt3_s );

    EXPECT_EQ( ite_expr._bits, ite_direct._bits );
  }

  {
    dynamic_truth_table tt1_d( 7 ), tt2_d( 7 ), tt3_d( 7 );

    create_nth_var( tt1_d, 0 );
    create_nth_var( tt2_d, 3 );
    create_nth_var( tt3_d, 6 );

    const auto maj_expr = binary_or( binary_or( binary_and( tt1_d, tt2_d ), binary_and( tt1_d, tt3_d ) ), binary_and( tt2_d, tt3_d ) );
    const auto maj_direct = ternary_majority( tt1_d, tt2_d, tt3_d );

    EXPECT_EQ( maj_expr._bits, maj_direct._bits );

    const auto ite_expr = binary_or( binary_and( tt1_d, tt2_d ), binary_and( unary_not( tt1_d ), tt3_d ) );
    const auto ite_direct = ternary_ite( tt1_d, tt2_d, tt3_d );

    EXPECT_EQ( ite_expr._bits, ite_direct._bits );
  }
}
