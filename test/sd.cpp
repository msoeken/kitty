/* kitty: C++ truth table library
 * Copyright (C) 2017-2021 EPFL
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

#include <iostream>
#include <string>
#include <vector>

#include <kitty/sd.hpp>

#include "utility.hpp"

using namespace kitty;

class SDTest : public kitty::testing::Test
{
protected:
  template<uint32_t NumVars>
  void check_sd( const uint32_t num_vars, const int number_of_classes ) const
  {
    const auto res = calculate_sd_represtative_class( num_vars );
    ASSERT_EQ( res.size(), number_of_classes );
    }
};


TEST_F( SDTest, class_4u_size )
{
  check_sd<4>( 4u, 83 );
}

TEST_F( SDTest, class_3u_size )
{
  check_sd<3>( 3u, 7 );
}
