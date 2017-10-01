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

class OperationsTestSwap : public ::testing::TestWithParam<std::pair<unsigned, std::string>>
{
};

TEST_P( OperationsTestSwap, swap_adjacent_inplace_large )
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

TEST_P( OperationsTestSwap, swap_adjacent_large )
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

INSTANTIATE_TEST_CASE_P( OperationsTestSwapInst,
                         OperationsTestSwap,
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

TEST( OperationsTest, flip_inplace_small )
{
  for ( const auto& p : std::vector<std::pair<unsigned, std::string>>{{0u, "0b34"}, {1u, "0dc2"}, {2u, "7083"}, {3u, "3807"}} )
  {
    static_truth_table<4> tt_s, tt_s_res;
    create_from_hex_string( tt_s, "0738" );

    flip_inplace( tt_s, p.first );
    create_from_hex_string( tt_s_res, p.second );

    EXPECT_TRUE( equal( tt_s, tt_s_res ) );

    dynamic_truth_table tt_d( 4 ), tt_d_res( 4 );
    create_from_hex_string( tt_d, "0738" );

    flip_inplace( tt_d, p.first );
    create_from_hex_string( tt_d_res, p.second );

    EXPECT_TRUE( equal( tt_d, tt_d_res ) );
  }
}

TEST( OperationsTest, flip_small )
{
  for ( const auto& p : std::vector<std::pair<unsigned, std::string>>{{0u, "0b34"}, {1u, "0dc2"}, {2u, "7083"}, {3u, "3807"}} )
  {
    static_truth_table<4> tt_s, tt_s_res;
    create_from_hex_string( tt_s, "0738" );
    create_from_hex_string( tt_s_res, p.second );

    EXPECT_TRUE( equal( flip( tt_s, p.first ), tt_s_res ) );

    dynamic_truth_table tt_d( 4 ), tt_d_res( 4 );
    create_from_hex_string( tt_d, "0738" );
    create_from_hex_string( tt_d_res, p.second );

    EXPECT_TRUE( equal( flip( tt_d, p.first ), tt_d_res ) );
  }
}

class OperationsTestFlip : public ::testing::TestWithParam<std::pair<unsigned, std::string>>
{
};

TEST_P( OperationsTestFlip, flip_inplace_large )
{
  static_truth_table<9> tt_s, tt_s_res;
  create_from_hex_string( tt_s, "8725ca41421c7bba0ca86e26347847526fc346d7f3e79e76566a9493fbef11e24f74a07643afd946195f6a372757e045f3bca58f110ef00ebf2d81e80ba5679f" );

  flip_inplace( tt_s, GetParam().first );
  create_from_hex_string( tt_s_res, GetParam().second );

  EXPECT_TRUE( equal( tt_s, tt_s_res ) );

  dynamic_truth_table tt_d( 9 ), tt_d_res( 9 );
  create_from_hex_string( tt_d, "8725ca41421c7bba0ca86e26347847526fc346d7f3e79e76566a9493fbef11e24f74a07643afd946195f6a372757e045f3bca58f110ef00ebf2d81e80ba5679f" );

  flip_inplace( tt_d, GetParam().first );
  create_from_hex_string( tt_d_res, GetParam().second );

  EXPECT_TRUE( equal( tt_d, tt_d_res ) );
}

TEST_P( OperationsTestFlip, flip_large )
{
  static_truth_table<9> tt_s, tt_s_res;
  create_from_hex_string( tt_s, "8725ca41421c7bba0ca86e26347847526fc346d7f3e79e76566a9493fbef11e24f74a07643afd946195f6a372757e045f3bca58f110ef00ebf2d81e80ba5679f" );
  create_from_hex_string( tt_s_res, GetParam().second );

  EXPECT_TRUE( equal( flip( tt_s, GetParam().first ), tt_s_res ) );

  dynamic_truth_table tt_d( 9 ), tt_d_res( 9 );
  create_from_hex_string( tt_d, "8725ca41421c7bba0ca86e26347847526fc346d7f3e79e76566a9493fbef11e24f74a07643afd946195f6a372757e045f3bca58f110ef00ebf2d81e80ba5679f" );
  create_from_hex_string( tt_d_res, GetParam().second );

  EXPECT_TRUE( equal( flip( tt_d, GetParam().first ), tt_d_res ) );
}

INSTANTIATE_TEST_CASE_P( OperationsTestFlipInst,
                         OperationsTestFlip,
                         ::testing::Values( std::make_pair( 0u, "4b1ac582812cb7750c549d1938b48ba19fc389ebf3db6db9a9956863f7df22d1"
                                                                "8fb850b9835fe68926af953b1babd08af37c5a4f220df00d7f1e42d4075a9b6f" ),
                                            std::make_pair( 1u, "2d853a141843deea03a29b89c1d21d589f3c197dfcbd6bd9599a616cfebf44b8"
                                                                "1fd1a0d91caf7619465f9acd8d5db015fce3a52f440bf00bef8724b20ea59d6f" ),
                                            std::make_pair( 2u, "7852ac1424c1b7abc08ae66243877425f63c647d3f7ee96765a64939bffe112e"
                                                                "f4470a6734fa9d6491f5a67372750e543fcb5af811e00fe0fbd2188eb05a76f9" ),
                                            std::make_pair( 3u, "258741ca1c42ba7ba80c266e78345247c36fd746e7f3769e6a569394effbe211"
                                                                "744f76a0af4346d95f19376a572745e0bcf38fa50e110ef02dbfe881a50b9f67" ),
                                            std::make_pair( 4u, "ca4187257bba421c6e260ca84752347846d76fc39e76f3e79493566a11e2fbef"
                                                                "a0764f74d94643af6a37195fe0452757a58ff3bcf00e110e81e8bf2d679f0ba5" ),
                                            std::make_pair( 5u, "421c7bba8725ca41347847520ca86e26f3e79e766fc346d7fbef11e2566a9493"
                                                                "43afd9464f74a0762757e045195f6a37110ef00ef3bca58f0ba5679fbf2d81e8" ),
                                            std::make_pair( 6u, "0ca86e26347847528725ca41421c7bba566a9493fbef11e26fc346d7f3e79e76"
                                                                "195f6a372757e0454f74a07643afd946bf2d81e80ba5679ff3bca58f110ef00e" ),
                                            std::make_pair( 7u, "6fc346d7f3e79e76566a9493fbef11e28725ca41421c7bba0ca86e2634784752"
                                                                "f3bca58f110ef00ebf2d81e80ba5679f4f74a07643afd946195f6a372757e045" ),
                                            std::make_pair( 8u, "4f74a07643afd946195f6a372757e045f3bca58f110ef00ebf2d81e80ba5679f"
                                                                "8725ca41421c7bba0ca86e26347847526fc346d7f3e79e76566a9493fbef11e2" ) ) );
