/* kitty: C++ truth table library
 * Copyright (C) 2017-2020  EPFL
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

#include <vector>
#include <gtest/gtest.h>

#include <kitty/constructors.hpp>
#include <kitty/dynamic_truth_table.hpp>
#include <kitty/static_truth_table.hpp>
#include <kitty/threshold_identification.hpp>

using namespace kitty;

TEST( TFI_test, TF_OR )
{
  static_truth_table<2> tt, tt_lf;
  create_from_hex_string( tt, "e" ); // OR

  std::vector<int64_t> lf;
  EXPECT_TRUE( is_threshold( tt, &lf ) );
  EXPECT_EQ( lf.size(), 3 );
  if ( lf.size() == 3 )
  {
    create_threshold( tt_lf, lf );
    EXPECT_EQ( tt_lf, tt );
  }
}

TEST( TFI_test, nonTF_binate )
{
  static_truth_table<2> tt;
  create_from_hex_string( tt, "6" ); // XOR
  EXPECT_FALSE( is_threshold( tt ) );
}

TEST( TFI_test, TF_negative_unate )
{
  static_truth_table<3> tt, tt_lf;
  create_from_hex_string( tt, "8f" ); // ~x2 + x1x0
  
  std::vector<int64_t> lf;
  EXPECT_TRUE( is_threshold( tt, &lf ) );
  EXPECT_EQ( lf.size(), 4 );
  if ( lf.size() == 4 )
  {
    create_threshold( tt_lf, lf );
    EXPECT_EQ( tt_lf, tt );
  }
}

TEST( TFI_test, nonTF_unate )
{
  static_truth_table<4> tt;
  create_from_hex_string( tt, "f888" ); // x0x1 + x2x3
  EXPECT_FALSE( is_threshold( tt ) );
}

TEST( TFI_test, TF_majority9 )
{
  dynamic_truth_table tt( 9 );
  dynamic_truth_table tt_lf( 9 );
  create_majority( tt );
  
  std::vector<int64_t> lf;
  EXPECT_TRUE( is_threshold( tt, &lf ) );
  EXPECT_EQ( lf.size(), 10 );
  if ( lf.size() == 10 )
  {
    create_threshold( tt_lf, lf );
    EXPECT_EQ( tt_lf, tt );
  }
}

