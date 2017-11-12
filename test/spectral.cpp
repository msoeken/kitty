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

#include "utility.hpp"

using namespace kitty;

class SpectralTest : public kitty::testing::Test
{
};

TEST_F( SpectralTest, datatypes )
{
  detail::spectral_operation op;
  EXPECT_EQ( sizeof( op ), 2u );
}

TEST_F( SpectralTest, to_and_from_truth_table )
{
  static_truth_table<5> tt;
  for ( auto i = 0u; i < 1000u; ++i )
  {
    create_random( tt );
    const auto s = detail::spectrum::from_truth_table( tt );
    auto tt2 = tt.construct();
    s.to_truth_table( tt2 );
    EXPECT_EQ( tt, tt2 );
  }
}

TEST_F( SpectralTest, apply_permutation )
{
  static_truth_table<5> tt;
  for ( auto i = 0u; i < 5u; ++i )
  {
    create_random( tt );
    auto s = detail::spectrum::from_truth_table( tt );

    s.permutation( 2, 3 );
    swap_inplace( tt, 2, 3 );

    auto tt2 = tt.construct();
    s.to_truth_table( tt2 );
    EXPECT_EQ( tt, tt2 );
  }
}

TEST_F( SpectralTest, apply_input_negation )
{
  static_truth_table<5> tt;
  for ( auto i = 0u; i < 5u; ++i )
  {
    create_random( tt );
    auto s = detail::spectrum::from_truth_table( tt );

    s.input_negation( 2 );
    flip_inplace( tt, 2 );

    auto tt2 = tt.construct();
    s.to_truth_table( tt2 );
    EXPECT_EQ( tt, tt2 );
  }
}

TEST_F( SpectralTest, apply_output_negation )
{
  static_truth_table<5> tt;
  for ( auto i = 0u; i < 5u; ++i )
  {
    create_random( tt );
    auto s = detail::spectrum::from_truth_table( tt );

    s.output_negation();
    tt = ~tt;

    auto tt2 = tt.construct();
    s.to_truth_table( tt2 );
    EXPECT_EQ( tt, tt2 );
  }
}

TEST_F( SpectralTest, apply_disjoint_translation )
{
  static_truth_table<5> tt;
  for ( auto i = 0u; i < 5u; ++i )
  {
    create_random( tt );
    auto s = detail::spectrum::from_truth_table( tt );

    s.disjoint_translation( 3 );

    tt ^= nth<5>( 3 );

    auto tt2 = tt.construct();
    s.to_truth_table( tt2 );
    EXPECT_EQ( tt, tt2 );
  }
}

TEST_F( SpectralTest, canonization )
{
  EXPECT_EQ( exact_spectral_canonization( from_hex<3>( "57" ) ), from_hex<3>( "80" ) );
}

