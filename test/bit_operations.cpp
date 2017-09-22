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

#include <kitty/kitty.hpp>

using namespace kitty;

template<typename NumVars>
class BitOperationsTest : public ::testing::Test
{
public:
};

typedef ::testing::Types<static_truth_table<5>, static_truth_table<7>, static_truth_table<9>> truth_table_types;
TYPED_TEST_CASE( BitOperationsTest, truth_table_types );

TYPED_TEST( BitOperationsTest, all_initially_zero )
{
  TypeParam tt_s;

  for ( auto i = 0; i < tt_s.num_bits(); ++i )
  {
    EXPECT_EQ( get_bit( tt_s, i ), 0 );
  }

  dynamic_truth_table tt_d( tt_s.num_vars() );

  for ( auto i = 0; i < tt_d.num_bits(); ++i )
  {
    EXPECT_EQ( get_bit( tt_d, i ), 0 );
  }
}

TYPED_TEST( BitOperationsTest, set_get_clear )
{
  TypeParam tt_s;

  for ( auto i = 0; i < tt_s.num_bits(); ++i )
  {
    set_bit( tt_s, i );
    EXPECT_EQ( get_bit( tt_s, i ), 1 );
    clear_bit( tt_s, i );
    EXPECT_EQ( get_bit( tt_s, i ), 0 );
  }

  dynamic_truth_table tt_d( tt_s.num_vars() );

  for ( auto i = 0; i < tt_d.num_bits(); ++i )
  {
    set_bit( tt_d, i );
    EXPECT_EQ( get_bit( tt_d, i ), 1 );
    clear_bit( tt_d, i );
    EXPECT_EQ( get_bit( tt_d, i ), 0 );
  }
}
