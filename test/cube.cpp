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
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <kitty/kitty.hpp>

#include "utility.hpp"

using namespace kitty;

class CubeTest : public kitty::testing::Test
{
protected:
  std::string to_string( const cube& c, uint8_t num_vars )
  {
    std::stringstream ss;
    c.print( num_vars, ss );
    return ss.str();
  }
};

TEST_F( CubeTest, constructing_and_printing )
{
  EXPECT_EQ( to_string( cube(), 2 ), "--" );
  EXPECT_EQ( to_string( cube( 0, 2 ), 2 ), "-0" );
}

TEST_F( CubeTest, distance )
{
  EXPECT_EQ( cube().distance( cube( 0, 2 ) ), 1 );
}

