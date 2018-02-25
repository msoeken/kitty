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

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include <kitty/constructors.hpp>
#include <kitty/esop.hpp>

#include "utility.hpp"

using namespace kitty;

class EsopTest : public kitty::testing::Test
{
};

TEST_F( EsopTest, esop_for_small )
{
  const auto tt = from_hex<3>( "e8" );

  const auto cubes = esop_from_optimum_pkrm( tt );

  auto tt_copy = tt.construct();
  create_from_cubes( tt_copy, cubes, true );
  EXPECT_EQ( tt, tt_copy );
}

TEST_F( EsopTest, random_esop )
{
  static_truth_table<10> tt;

  for ( auto i = 0; i < 100; ++i )
  {
    create_random( tt );
    const auto cubes = esop_from_optimum_pkrm( tt );
    auto tt_copy = tt.construct();
    create_from_cubes( tt_copy, cubes, true );
    EXPECT_EQ( tt, tt_copy );
  }
}

TEST_F( EsopTest, random_pprm )
{
  static_truth_table<10> tt;

  for ( auto i = 0; i < 100; ++i )
  {
    create_random( tt );
    const auto cubes = esop_from_pprm( tt );
    auto tt_copy = tt.construct();
    create_from_cubes( tt_copy, cubes, true );
    EXPECT_EQ( tt, tt_copy );
  }
}

TEST_F( EsopTest, random_pprm_dynamic_and_check )
{
  dynamic_truth_table tt( 8u );

  for ( auto i = 0; i < 50; ++i )
  {
    create_random( tt );
    const auto cubes = esop_from_pprm( tt );

    /* only positive literals */
    for ( const auto& c : cubes )
    {
      EXPECT_EQ( c._bits, c._mask );
    }

    auto tt_copy = tt.construct();
    create_from_cubes( tt_copy, cubes, true );
    EXPECT_EQ( tt, tt_copy );
  }
}
