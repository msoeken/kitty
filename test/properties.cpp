/* kitty: C++ truth table library
 * Copyright (C) 2017-2019  EPFL
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

#include <kitty/properties.hpp>
#include "utility.hpp"

using namespace kitty;

class PropertiesTest : public kitty::testing::Test
{
};

TEST_F( PropertiesTest, chow_small_example )
{
  auto r = chow_parameters( from_hex<2>( "e" ) );
  EXPECT_EQ( r.first, 3u );
  EXPECT_EQ( r.second.size(), 2u );
  EXPECT_EQ( r.second[0u], 2u );
  EXPECT_EQ( r.second[1u], 2u );
}

TEST_F( PropertiesTest, is_canalizing )
{
  static_truth_table<4> tt;
  uint32_t counter{};

  do
  {
    if ( is_canalizing( tt ) )
    {
      ++counter;
    }

    next_inplace( tt );
  } while ( !is_const0( tt ) );

  EXPECT_EQ( counter, 3514u );
}

TEST_F( PropertiesTest, is_selfdual )
{
  static_truth_table<4> tt;
  uint32_t counter{};

  do
  {
    if ( is_selfdual( tt ) )
    {
      ++counter;
    }

    next_inplace( tt );
  } while ( !is_const0( tt ) );

  EXPECT_EQ( counter, 256u );
}

TEST_F( PropertiesTest, is_monotone )
{
  static_truth_table<4> tt;
  uint32_t counter{};

  do
  {
    if ( is_monotone( tt ) )
    {
      ++counter;
    }

    next_inplace( tt );
  } while ( !is_const0( tt ) );

  EXPECT_EQ( counter, 168u );
}

TEST_F( PropertiesTest, is_horn )
{
  static_truth_table<4> tt;
  uint32_t counter{};

  do
  {
    if ( is_horn( tt ) )
    {
      ++counter;
    }

    next_inplace( tt );
  } while ( !is_const0( tt ) );

  EXPECT_EQ( counter, 4960u );
}

TEST_F( PropertiesTest, is_krom )
{
  static_truth_table<4> tt;
  uint32_t counter{};

  do
  {
    if ( is_krom( tt ) )
    {
      ++counter;
    }

    next_inplace( tt );
  } while ( !is_const0( tt ) );

  EXPECT_EQ( counter, 4170u );
}

TEST_F( PropertiesTest, runlength_pattern )
{
  auto xor2 = from_hex<2>( "6" );
  auto xor3 = from_hex<3>( "96" );
  auto xor4 = from_hex<4>( "6996" );

  EXPECT_EQ( runlength_pattern( xor2 ), ( std::vector<uint32_t>{{1, 2, 1}} ) );
  EXPECT_EQ( runlength_pattern( xor3 ), ( std::vector<uint32_t>{{1, 2, 1, 1, 2, 1}} ) );
  EXPECT_EQ( runlength_pattern( xor4 ), ( std::vector<uint32_t>{{1, 2, 1, 1, 2, 2, 2, 1, 1, 2, 1}} ) );
}

TEST_F( PropertiesTest, maj7_complemented )
{
  static_truth_table<7> maj7; 
  create_majority(maj7); 
  auto inv_maj7 = ~maj7; 

  auto one_compl = flip( maj7, 5 );
  
  auto two_compl = flip( maj7, 5 );
  two_compl = flip( two_compl, 3 );

  auto three_compl = flip( maj7, 0 );
  three_compl = flip( three_compl, 2 );
  three_compl = flip( three_compl, 6 );

  auto four_compl = flip( maj7, 0 );
  four_compl = flip( four_compl, 1 );
  four_compl = flip( four_compl, 4 );
  four_compl = flip( four_compl, 5 );

  auto four_compl_bis = flip( maj7, 0 );
  four_compl_bis = flip( four_compl_bis, 1 );
  four_compl_bis = flip( four_compl_bis, 3 );
  four_compl_bis = flip( four_compl_bis, 6 );

  auto five_compl = flip( maj7, 0 );
  five_compl = flip( five_compl, 1 );
  five_compl = flip( five_compl, 2 );
  five_compl = flip( five_compl, 5 );
  five_compl = flip( five_compl, 6 );

  auto six_compl = flip( maj7, 0 );
  six_compl = flip( six_compl, 1 );
  six_compl = flip( six_compl, 3 );
  six_compl = flip( six_compl, 4 );
  six_compl = flip( six_compl, 5 );
  six_compl = flip( six_compl, 6 );

  static_truth_table<7> no_maj7;
  create_from_hex_string(no_maj7, "feeaeaa8eaa8e8a8eae8eaa8eaa8a880"); 

  EXPECT_EQ( maj7_complemented( maj7 ), ( std::vector<uint32_t>{{0, 0, 0, 0, 0, 0, 0}} ) );
  EXPECT_EQ( maj7_complemented( inv_maj7 ), ( std::vector<uint32_t>{{1, 1, 1, 1, 1, 1, 1}} ) );
  EXPECT_EQ( maj7_complemented( one_compl ), ( std::vector<uint32_t>{{0, 0, 0, 0, 0, 1, 0}} ) );
  EXPECT_EQ( maj7_complemented( two_compl ), ( std::vector<uint32_t>{{0, 0, 0, 1, 0, 1, 0}} ) );
  EXPECT_EQ( maj7_complemented( three_compl ), ( std::vector<uint32_t>{{1, 0, 1, 0, 0, 0, 1}} ) );
  EXPECT_EQ( maj7_complemented( four_compl ), ( std::vector<uint32_t>{{1, 1, 0, 0, 1, 1, 0}} ) );
  EXPECT_EQ( maj7_complemented( four_compl_bis ), ( std::vector<uint32_t>{{1, 1, 0, 1, 0, 0, 1}} ) );
  EXPECT_EQ( maj7_complemented( five_compl ), ( std::vector<uint32_t>{{1, 1, 1, 0, 0, 1, 1}} ) );
  EXPECT_EQ( maj7_complemented( six_compl ), ( std::vector<uint32_t>{{1, 1, 0, 1, 1, 1, 1}} ) );
  EXPECT_EQ( maj7_complemented( no_maj7 ), ( std::vector<uint32_t>{{2, 2, 2, 2, 2, 2, 2}} ) );
}
