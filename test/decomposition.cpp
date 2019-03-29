/* kitty: C++ truth table library
 * Copyright (C) 2017-2018  EPFL
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

#include <kitty/constructors.hpp>
#include <kitty/dynamic_truth_table.hpp>
#include <kitty/decomposition.hpp>
#include <kitty/static_truth_table.hpp>

#include "utility.hpp"

using namespace kitty;

class DecompositionTest : public kitty::testing::Test
{
};

TEST_F( DecompositionTest, top_down )
{
  EXPECT_EQ( is_top_decomposable( from_hex<3>( "e8" ), 0u ), top_decomposition::none );
  EXPECT_EQ( is_top_decomposable( from_hex<3>( "e8" ), 1u ), top_decomposition::none );
  EXPECT_EQ( is_top_decomposable( from_hex<3>( "e8" ), 2u ), top_decomposition::none );

  kitty::static_truth_table<3> expr1;
  kitty::static_truth_table<3> expr2, expr2_der;
  kitty::static_truth_table<3> expr3, expr3_der;

  create_from_expression( expr1, "(a{bc})" );
  create_from_expression( expr2, "{bc}" );
  create_from_expression( expr3, "c" );

  EXPECT_EQ( is_top_decomposable( expr1, 0u, &expr2_der ), top_decomposition::and_ );
  EXPECT_EQ( expr2, expr2_der );
  EXPECT_EQ( is_top_decomposable( expr2, 1u, &expr3_der ), top_decomposition::or_ );
  EXPECT_EQ( expr3, expr3_der );

  create_from_expression( expr1, "(!a{!bc})" );
  create_from_expression( expr2, "{!bc}" );
  create_from_expression( expr3, "c" );

  EXPECT_EQ( is_top_decomposable( expr1, 0u, &expr2_der ), top_decomposition::lt_ );
  EXPECT_EQ( expr2, expr2_der );
  EXPECT_EQ( is_top_decomposable( expr2, 1u, &expr3_der ), top_decomposition::le_ );
  EXPECT_EQ( expr3, expr3_der );

  create_from_expression( expr1, "[a{bc}]" );
  create_from_expression( expr2, "{bc}" );
  create_from_expression( expr3, "c" );

  EXPECT_EQ( is_top_decomposable( expr1, 0u, &expr2_der ), top_decomposition::xor_ );
  EXPECT_EQ( expr2, expr2_der );
  EXPECT_EQ( is_top_decomposable( expr2, 1u, &expr3_der ), top_decomposition::or_ );
  EXPECT_EQ( expr3, expr3_der );
}

TEST_F( DecompositionTest, bottom_up )
{
  kitty::static_truth_table<6> expr1;
  kitty::static_truth_table<6> expr2, expr2_der;
  kitty::static_truth_table<6> expr3, expr3_der;
  kitty::static_truth_table<6> expr4, expr4_der;
  kitty::static_truth_table<6> expr5, expr5_der;
  kitty::static_truth_table<6> expr6, expr6_der;

  create_from_expression( expr1, "[a(b{!c(!d{ef})})]" );
  create_from_expression( expr2, "[a(b{!c(!de)})]" );
  create_from_expression( expr3, "[a(b{!cd})]" );
  create_from_expression( expr4, "[a(bc)]" );
  create_from_expression( expr5, "[ab]" );
  create_from_expression( expr6, "a" );

  EXPECT_EQ( is_bottom_decomposable( expr1, 4u, 5u, &expr2_der ), bottom_decomposition::or_ );
  EXPECT_EQ( expr2, expr2_der );
  EXPECT_EQ( is_bottom_decomposable( expr2, 3u, 4u, &expr3_der ), bottom_decomposition::lt_ );
  EXPECT_EQ( expr3, expr3_der );
  EXPECT_EQ( is_bottom_decomposable( expr3, 2u, 3u, &expr4_der ), bottom_decomposition::le_ );
  EXPECT_EQ( expr4, expr4_der );
  EXPECT_EQ( is_bottom_decomposable( expr4, 1u, 2u, &expr5_der ), bottom_decomposition::and_ );
  EXPECT_EQ( expr5, expr5_der );
  EXPECT_EQ( is_bottom_decomposable( expr5, 0u, 1u, &expr6_der ), bottom_decomposition::xor_ );
  EXPECT_EQ( expr6, expr6_der );
}

TEST_F( DecompositionTest, combined )
{
  kitty::static_truth_table<6> expr1;
  kitty::static_truth_table<6> expr2, expr2_der;
  kitty::static_truth_table<6> expr3, expr3_der;
  kitty::static_truth_table<6> expr4, expr4_der;
  kitty::static_truth_table<6> expr5, expr5_der;
  kitty::static_truth_table<6> expr6, expr6_der;

  create_from_expression( expr1, "{[a{bc}]{d(ef)}}" );
  create_from_expression( expr2, "{[a{bc}]{de}}" );
  create_from_expression( expr3, "{[ab]{de}}" );
  create_from_expression( expr4, "{[ab]d}" );
  create_from_expression( expr5, "[ab]" );
  create_from_expression( expr6, "a" );

  EXPECT_EQ( is_bottom_decomposable( expr1, 4u, 5u, &expr2_der ), bottom_decomposition::and_ );
  EXPECT_EQ( expr2, expr2_der );
  EXPECT_EQ( is_bottom_decomposable( expr2, 1u, 2u, &expr3_der ), bottom_decomposition::or_ );
  EXPECT_EQ( expr3, expr3_der );
  EXPECT_EQ( is_bottom_decomposable( expr3, 3u, 4u, &expr4_der ), bottom_decomposition::or_ );
  EXPECT_EQ( expr4, expr4_der );
  EXPECT_EQ( is_top_decomposable( expr4, 3u, &expr5_der ), top_decomposition::or_ );
  EXPECT_EQ( expr5, expr5_der );
  EXPECT_EQ( is_top_decomposable( expr5, 1u, &expr6_der ), top_decomposition::xor_ );
  EXPECT_EQ( expr6, expr6_der );
}
