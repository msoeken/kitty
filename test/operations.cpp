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

class OperationsTest : public ::testing::TestWithParam<std::pair<unsigned, std::string>>
{
};

TEST( OperationsTest, swap_adjacent_inplace_small )
{
  for ( const auto& p : std::vector<std::pair<unsigned, std::string>>{{0u, "bce8"}, {1u, "e6e8"}, {2u, "dea8"}} )
  {
    static_truth_table<4> tt_s, tt_s_res;
    create_from_hex_string( tt_s, "dae8" );

    swap_adjacent_inplace( tt_s, p.first );
    create_from_hex_string( tt_s_res, p.second );

    EXPECT_TRUE( equal( tt_s, tt_s_res ) );


    dynamic_truth_table tt_d( 4 ), tt_d_res( 4 );
    create_from_hex_string( tt_d, "dae8" );

    swap_adjacent_inplace( tt_d, p.first );
    create_from_hex_string( tt_d_res, p.second );

    EXPECT_TRUE( equal( tt_d, tt_d_res ) );
  }
}

TEST( OperationsTest, swap_adjacent_small )
{
  for ( const auto& p : std::vector<std::pair<unsigned, std::string>>{{0u, "bce8"}, {1u, "e6e8"}, {2u, "dea8"}} )
  {
    static_truth_table<4> tt_s, tt_s_res;
    create_from_hex_string( tt_s, "dae8" );
    create_from_hex_string( tt_s_res, p.second );

    EXPECT_TRUE( equal( swap_adjacent( tt_s, p.first ), tt_s_res ) );


    dynamic_truth_table tt_d( 4 ), tt_d_res( 4 );
    create_from_hex_string( tt_d, "dae8" );
    create_from_hex_string( tt_d_res, p.second );

    EXPECT_TRUE( equal( swap_adjacent( tt_d, p.first ), tt_d_res ) );
  }
}

TEST_P( OperationsTest, swap_adjacent_inplace_large )
{
  static_truth_table<9> tt_s, tt_s_res;
  create_from_hex_string( tt_s, "28e3b8d62855c4b787eef391a93b33297856658b6743aa3cc7e11fde4e9cbf7c98b07f5febfff33bc7ad6f551bc4cbc440453e1bbe24f0cb4f268c6771b55eee" );

  swap_adjacent_inplace( tt_s, GetParam().first );
  create_from_hex_string( tt_s_res, GetParam().second );

  EXPECT_TRUE( equal( tt_s, tt_s_res ) );

  dynamic_truth_table tt_d( 9 ), tt_d_res( 9 );
  create_from_hex_string( tt_d, "28e3b8d62855c4b787eef391a93b33297856658b6743aa3cc7e11fde4e9cbf7c98b07f5febfff33bc7ad6f551bc4cbc440453e1bbe24f0cb4f268c6771b55eee" );

  swap_adjacent_inplace( tt_d, GetParam().first );
  create_from_hex_string( tt_d_res, GetParam().second );

  EXPECT_TRUE( equal( tt_d, tt_d_res ) );
}

TEST_P( OperationsTest, swap_adjacent_large )
{
  static_truth_table<9> tt_s, tt_s_res;
  create_from_hex_string( tt_s, "28e3b8d62855c4b787eef391a93b33297856658b6743aa3cc7e11fde4e9cbf7c98b07f5febfff33bc7ad6f551bc4cbc440453e1bbe24f0cb4f268c6771b55eee" );
  create_from_hex_string( tt_s_res, GetParam().second );

  EXPECT_TRUE( equal( swap_adjacent( tt_s, GetParam().first ), tt_s_res ) );

  dynamic_truth_table tt_d( 9 ), tt_d_res( 9 );
  create_from_hex_string( tt_d, "28e3b8d62855c4b787eef391a93b33297856658b6743aa3cc7e11fde4e9cbf7c98b07f5febfff33bc7ad6f551bc4cbc440453e1bbe24f0cb4f268c6771b55eee" );
  create_from_hex_string( tt_d_res, GetParam().second );

  EXPECT_TRUE( equal( swap_adjacent( tt_d, GetParam().first ), tt_d_res ) );
}

INSTANTIATE_TEST_CASE_P( OperationsTestInst,
                         OperationsTest,
                         ::testing::Values( std::make_pair( 0u, "48e5d8b64833a2d787eef591c95d55497836638d6725cc5aa7e11fbe2e9adf7a"
                                                                "98d07f3fedfff55da7cb6f331da2ada220235e1dde42f0ad2f468a6771d33eee" ),
                                            std::make_pair( 1u, "28cbacd62855d09f93facf85a92f0f296c5659a35b43aa3cd3c937f672b4bf7c"
                                                                "a48c7f77ebffcf2fd3b97b5527d0e3d040513e27be18cce3731ab05b4d9d76fa" ),
                                            std::make_pair( 2u, "2e83bd862585cb478e7ef931a39b32397586685b6473a3acce711dfe49ecb7fc"
                                                                "9b8075ffefbff33bca7d65f51cb4ccb4440531ebb2e4fc0b42f686c77b155eee" ),
                                            std::make_pair( 3u, "28b8e3d628c455b787f3ee91a9333b297865568b67aa433cc71fe1de4ebf9c7c"
                                                                "987fb05febf3ff3bc76fad551bcbc4c4403e451bbef024cb4f8c2667715eb5ee" ),
                                            std::make_pair( 4u, "28e32855b8d6c4b787eea93bf391332978566743658baa3cc7e14e9c1fdebf7c"
                                                                "98b0ebff7f5ff33bc7ad1bc46f55cbc44045be243e1bf0cb4f2671b58c675eee" ),
                                            std::make_pair( 5u, "28e3b8d687eef3912855c4b7a93b33297856658bc7e11fde6743aa3c4e9cbf7c"
                                                                "98b07f5fc7ad6f55ebfff33b1bc4cbc440453e1b4f268c67be24f0cb71b55eee" ),
                                            std::make_pair( 6u, "28e3b8d62855c4b77856658b6743aa3c87eef391a93b3329c7e11fde4e9cbf7c"
                                                                "98b07f5febfff33b40453e1bbe24f0cbc7ad6f551bc4cbc44f268c6771b55eee" ),
                                            std::make_pair( 7u, "28e3b8d62855c4b787eef391a93b332998b07f5febfff33bc7ad6f551bc4cbc4"
                                                                "7856658b6743aa3cc7e11fde4e9cbf7c40453e1bbe24f0cb4f268c6771b55eee" ) ) );
