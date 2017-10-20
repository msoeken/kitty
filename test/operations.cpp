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

#include "utility.hpp"

using namespace kitty;

class OperationsTest : public kitty::testing::Test
{
};

TEST_F( OperationsTest, unary_for_small )
{
  EXPECT_EQ( unary_not( from_hex<2>( "8" ) ), from_hex<2>( "7" ) );
  EXPECT_EQ( unary_not( from_hex<2>( "6" ) ), from_hex<2>( "9" ) );

  EXPECT_EQ( unary_not_if( from_hex<2>( "8" ), true ), from_hex<2>( "7" ) );
  EXPECT_EQ( unary_not_if( from_hex<2>( "6" ), true ), from_hex<2>( "9" ) );

  EXPECT_EQ( unary_not_if( from_hex<2>( "8" ), false ), from_hex<2>( "8" ) );
  EXPECT_EQ( unary_not_if( from_hex<2>( "6" ), false ), from_hex<2>( "6" ) );
}

TEST_F( OperationsTest, binary_for_small )
{
  EXPECT_EQ( binary_and( nth<2>( 0 ), nth<2>( 1 ) ), from_hex<2>( "8" ) );
  EXPECT_EQ( binary_or( nth<2>( 0 ), nth<2>( 1 ) ), from_hex<2>( "e" ) );
  EXPECT_EQ( unary_not( binary_and( nth<2>( 0 ), nth<2>( 1 ) ) ), from_hex<2>( "7" ) );
  EXPECT_EQ( unary_not( binary_or( nth<2>( 0 ), nth<2>( 1 ) ) ), from_hex<2>( "1" ) );

  EXPECT_EQ( binary_and( nth( 2, 0 ), nth( 2, 1 ) ), from_hex( 2, "8" ) );
  EXPECT_EQ( binary_or( nth( 2, 0 ), nth( 2, 1 ) ), from_hex( 2, "e" ) );
  EXPECT_EQ( unary_not( binary_and( nth( 2, 0 ), nth( 2, 1 ) ) ), from_hex( 2, "7" ) );
  EXPECT_EQ( unary_not( binary_or( nth( 2, 0 ), nth( 2, 1 ) ) ), from_hex( 2, "1" ) );
}

TEST_F( OperationsTest, ternary_for_small )
{
  {
    const auto t1 = nth<3>( 0 ), t2 = nth<3>( 1 ), t3 = nth<3>( 2 );

    const auto maj_expr = binary_or( binary_or( binary_and( t1, t2 ), binary_and( t1, t3 ) ), binary_and( t2, t3 ) );
    const auto maj_direct = ternary_majority( t1, t2, t3 );

    EXPECT_EQ( maj_expr, from_hex<3>( "e8" ) );
    EXPECT_EQ( maj_expr, maj_direct );

    const auto ite_expr = binary_or( binary_and( t1, t2 ), binary_and( unary_not( t1 ), t3 ) );
    const auto ite_direct = ternary_ite( t1, t2, t3 );

    EXPECT_EQ( ite_expr, from_hex<3>( "d8" ) );
    EXPECT_EQ( ite_expr, ite_direct );
  }

  {
    const auto t1 = nth( 3, 0 ), t2 = nth( 3, 1 ), t3 = nth( 3, 2 );

    const auto maj_expr = binary_or( binary_or( binary_and( t1, t2 ), binary_and( t1, t3 ) ), binary_and( t2, t3 ) );
    const auto maj_direct = ternary_majority( t1, t2, t3 );

    EXPECT_EQ( maj_expr, from_hex( 3, "e8" ) );
    EXPECT_EQ( maj_expr, maj_direct );

    const auto ite_expr = binary_or( binary_and( t1, t2 ), binary_and( unary_not( t1 ), t3 ) );
    const auto ite_direct = ternary_ite( t1, t2, t3 );

    EXPECT_EQ( ite_expr, from_hex( 3, "d8" ) );
    EXPECT_EQ( ite_expr, ite_direct );
  }
}

TEST_F( OperationsTest, unary_for_large )
{
  EXPECT_EQ( unary_not( from_hex<7>( "80000000000000000000000000000000" ) ), from_hex<7>( "7fffffffffffffffffffffffffffffff" ) );
  EXPECT_EQ( unary_not( from_hex<7>( "66666666666666666666666666666666" ) ), from_hex<7>( "99999999999999999999999999999999" ) );

  EXPECT_EQ( unary_not_if( from_hex<7>( "80000000000000000000000000000000" ), true ), from_hex<7>( "7fffffffffffffffffffffffffffffff" ) );
  EXPECT_EQ( unary_not_if( from_hex<7>( "66666666666666666666666666666666" ), true ), from_hex<7>( "99999999999999999999999999999999" ) );

  EXPECT_EQ( unary_not_if( from_hex<7>( "80000000000000000000000000000000" ), false ), from_hex<7>( "80000000000000000000000000000000" ) );
  EXPECT_EQ( unary_not_if( from_hex<7>( "66666666666666666666666666666666" ), false ), from_hex<7>( "66666666666666666666666666666666" ) );
}

TEST_F( OperationsTest, binary_for_large )
{
  {
    auto tt = nth<7>( 0 );
    for ( auto i = 1; i < 7; ++i )
    {
      tt = binary_and( tt, nth<7>( i ) );
    }
    EXPECT_EQ( tt, from_hex<7>( "8000000000000000" ) );
  }

  {
    auto tt = nth( 7, 0 );
    for ( auto i = 1; i < 7; ++i )
    {
      tt = binary_and( tt, nth( 7, i ) );
    }
    EXPECT_EQ( tt, from_hex( 7, "8000000000000000" ) );
  }
}

TEST_F( OperationsTest, ternary_for_large )
{
  {
    const auto t1 = nth<7>( 0 ), t2 = nth<7>( 1 ), t3 = nth<7>( 2 );

    const auto maj_expr = binary_or( binary_or( binary_and( t1, t2 ), binary_and( t1, t3 ) ), binary_and( t2, t3 ) );
    const auto maj_direct = ternary_majority( t1, t2, t3 );

    EXPECT_EQ( maj_expr, maj_direct );

    const auto ite_expr = binary_or( binary_and( t1, t2 ), binary_and( unary_not( t1 ), t3 ) );
    const auto ite_direct = ternary_ite( t1, t2, t3 );

    EXPECT_EQ( ite_expr, ite_direct );
  }

  {
    const auto t1 = nth( 7, 0 ), t2 = nth( 7, 1 ), t3 = nth( 7, 2 );

    const auto maj_expr = binary_or( binary_or( binary_and( t1, t2 ), binary_and( t1, t3 ) ), binary_and( t2, t3 ) );
    const auto maj_direct = ternary_majority( t1, t2, t3 );

    EXPECT_EQ( maj_expr, maj_direct );

    const auto ite_expr = binary_or( binary_and( t1, t2 ), binary_and( unary_not( t1 ), t3 ) );
    const auto ite_direct = ternary_ite( t1, t2, t3 );

    EXPECT_EQ( ite_expr, ite_direct );
  }
}

TEST_F( OperationsTest, comparisons )
{
  EXPECT_TRUE( equal( from_hex<3>( "e8" ), from_hex<3>( "e8" ) ) );
  EXPECT_TRUE( equal( from_hex( 3, "e8" ), from_hex( 3, "e8" ) ) );

  EXPECT_TRUE( less_than( from_hex<3>( "e5" ), from_hex<3>( "f6" ) ) );
  EXPECT_TRUE( less_than( from_hex<3>( "e5" ), from_hex<3>( "f5" ) ) );
  EXPECT_TRUE( less_than( from_hex<3>( "e5" ), from_hex<3>( "f4" ) ) );
  EXPECT_TRUE( less_than( from_hex<3>( "e5" ), from_hex<3>( "e6" ) ) );
  EXPECT_FALSE( less_than( from_hex<3>( "e5" ), from_hex<3>( "e5" ) ) );
  EXPECT_FALSE( less_than( from_hex<3>( "e5" ), from_hex<3>( "e4" ) ) );
  EXPECT_FALSE( less_than( from_hex<3>( "e5" ), from_hex<3>( "d6" ) ) );
  EXPECT_FALSE( less_than( from_hex<3>( "e5" ), from_hex<3>( "d5" ) ) );
  EXPECT_FALSE( less_than( from_hex<3>( "e5" ), from_hex<3>( "d4" ) ) );

  EXPECT_TRUE( less_than( from_hex( 3, "e5" ), from_hex( 3, "f6" ) ) );
  EXPECT_TRUE( less_than( from_hex( 3, "e5" ), from_hex( 3, "f5" ) ) );
  EXPECT_TRUE( less_than( from_hex( 3, "e5" ), from_hex( 3, "f4" ) ) );
  EXPECT_TRUE( less_than( from_hex( 3, "e5" ), from_hex( 3, "e6" ) ) );
  EXPECT_FALSE( less_than( from_hex( 3, "e5" ), from_hex( 3, "e5" ) ) );
  EXPECT_FALSE( less_than( from_hex( 3, "e5" ), from_hex( 3, "e4" ) ) );
  EXPECT_FALSE( less_than( from_hex( 3, "e5" ), from_hex( 3, "d6" ) ) );
  EXPECT_FALSE( less_than( from_hex( 3, "e5" ), from_hex( 3, "d5" ) ) );
  EXPECT_FALSE( less_than( from_hex( 3, "e5" ), from_hex( 3, "d4" ) ) );

  EXPECT_TRUE( equal( from_hex<7>( "e92c774439c72c8955906ef92ecefec9" ), from_hex<7>( "e92c774439c72c8955906ef92ecefec9" ) ) );
  EXPECT_TRUE( equal( from_hex( 7, "e92c774439c72c8955906ef92ecefec9" ), from_hex( 7, "e92c774439c72c8955906ef92ecefec9" ) ) );

  EXPECT_TRUE( less_than( from_hex<7>( "e92c774439c72c8955906ef92ecefec9" ), from_hex<7>( "e92c774439c72c8955906ef92edefec9" ) ) );
  EXPECT_FALSE( less_than( from_hex<7>( "e92c774439c72c8955906ef92ecefec9" ), from_hex<7>( "e92c774439c72c8955906ef92ebefec9" ) ) );

  EXPECT_TRUE( less_than( from_hex( 7, "e92c774439c72c8955906ef92ecefec9" ), from_hex( 7, "e92c774439c72c8955906ef92edefec9" ) ) );
  EXPECT_FALSE( less_than( from_hex( 7, "e92c774439c72c8955906ef92ecefec9" ), from_hex( 7, "e92c774439c72c8955906ef92ebefec9" ) ) );
}

TEST_F( OperationsTest, support )
{
  EXPECT_TRUE( is_const0( from_hex<0>( "0" ) ) );
  EXPECT_TRUE( is_const0( from_hex<1>( "0" ) ) );
  EXPECT_TRUE( is_const0( from_hex<2>( "0" ) ) );
  EXPECT_TRUE( is_const0( from_hex<3>( "00" ) ) );
  EXPECT_TRUE( is_const0( from_hex<4>( "0000" ) ) );
  EXPECT_TRUE( is_const0( from_hex<5>( "00000000" ) ) );
  EXPECT_TRUE( is_const0( from_hex<6>( "0000000000000000" ) ) );
  EXPECT_TRUE( is_const0( from_hex<7>( "00000000000000000000000000000000" ) ) );
  EXPECT_TRUE( is_const0( from_hex<8>( "0000000000000000000000000000000000000000000000000000000000000000" ) ) );

  EXPECT_TRUE( has_var( from_hex<3>( "77" ), 0 ) );
  EXPECT_TRUE( has_var( from_hex<3>( "77" ), 1 ) );
  EXPECT_TRUE( !has_var( from_hex<3>( "77" ), 2 ) );

  EXPECT_TRUE( has_var( from_hex( 3, "77" ), 0 ) );
  EXPECT_TRUE( has_var( from_hex( 3, "77" ), 1 ) );
  EXPECT_TRUE( !has_var( from_hex( 3, "77" ), 2 ) );

  for ( auto i = 0; i < 8; ++i )
  {
    EXPECT_EQ( has_var( from_hex<8>( "3333333333cc33cc3333333333cc33cc33cc33cccccccccc33cc33cccccccccc" ), i ), i % 2 == 1 );
  }

  for ( auto i = 0; i < 8; ++i )
  {
    EXPECT_EQ( has_var( from_hex( 8, "3333333333cc33cc3333333333cc33cc33cc33cccccccccc33cc33cccccccccc" ), i ), i % 2 == 1 );
  }
}

TEST_F( OperationsTest, next )
{
  EXPECT_EQ( next( from_hex<3>( "00" ) ), from_hex<3>( "01" ) );
  EXPECT_EQ( next( from_hex<3>( "ab" ) ), from_hex<3>( "ac" ) );
  EXPECT_EQ( next( from_hex<3>( "ff" ) ), from_hex<3>( "00" ) );

  EXPECT_EQ( next( from_hex( 3, "00" ) ), from_hex( 3, "01" ) );
  EXPECT_EQ( next( from_hex( 3, "ab" ) ), from_hex( 3, "ac" ) );
  EXPECT_EQ( next( from_hex( 3, "ff" ) ), from_hex( 3, "00" ) );

  EXPECT_EQ( next( from_hex<7>( "00000000000000000000000000000000" ) ), from_hex<7>( "00000000000000000000000000000001" ) );
  EXPECT_EQ( next( from_hex<7>( "aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbb" ) ), from_hex<7>( "aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbc" ) );
  EXPECT_EQ( next( from_hex<7>( "ffffffffffffffffffffffffffffffff" ) ), from_hex<7>( "00000000000000000000000000000000" ) );

  EXPECT_EQ( next( from_hex( 7, "00000000000000000000000000000000" ) ), from_hex( 7, "00000000000000000000000000000001" ) );
  EXPECT_EQ( next( from_hex( 7, "aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbb" ) ), from_hex( 7, "aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbc" ) );
  EXPECT_EQ( next( from_hex( 7, "ffffffffffffffffffffffffffffffff" ) ), from_hex( 7, "00000000000000000000000000000000" ) );
}

TEST_F( OperationsTest, cofactors )
{
  EXPECT_EQ( cofactor0( from_hex<3>( "e8" ), 2 ), from_hex<3>( "88" ) );
  EXPECT_EQ( cofactor0( from_hex<3>( "e8" ), 1 ), from_hex<3>( "a0" ) );
  EXPECT_EQ( cofactor0( from_hex<3>( "e8" ), 0 ), from_hex<3>( "c0" ) );

  EXPECT_EQ( cofactor0( from_hex( 3, "e8" ), 2 ), from_hex( 3, "88" ) );
  EXPECT_EQ( cofactor0( from_hex( 3, "e8" ), 1 ), from_hex( 3, "a0" ) );
  EXPECT_EQ( cofactor0( from_hex( 3, "e8" ), 0 ), from_hex( 3, "c0" ) );

  // Co-factors of [(<abc>d){<efg>h}]
  EXPECT_EQ( cofactor0( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 0 ), from_hex<8>( "3fff3fff3fff3fff3fff3fff3fff3fff3fff3fff3fffc0003fffc000c000c000" ) );
  EXPECT_EQ( cofactor0( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 1 ), from_hex<8>( "5fff5fff5fff5fff5fff5fff5fff5fff5fff5fff5fffa0005fffa000a000a000" ) );
  EXPECT_EQ( cofactor0( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 2 ), from_hex<8>( "77ff77ff77ff77ff77ff77ff77ff77ff77ff77ff77ff880077ff880088008800" ) );
  EXPECT_EQ( cofactor0( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 3 ), from_hex<8>( "ffffffffffffffffffffffffffffffffffffffffffff0000ffff000000000000" ) );
  EXPECT_EQ( cofactor0( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 4 ), from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe800e800e800e800e800e800" ) );
  EXPECT_EQ( cofactor0( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 5 ), from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800e800e800" ) );
  EXPECT_EQ( cofactor0( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 6 ), from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ffe800e800e80017ffe800e800e800" ) );
  EXPECT_EQ( cofactor0( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 7 ), from_hex<8>( "17ff17ff17ffe80017ffe800e800e80017ff17ff17ffe80017ffe800e800e800" ) );

  EXPECT_EQ( cofactor0( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 0 ), from_hex( 8, "3fff3fff3fff3fff3fff3fff3fff3fff3fff3fff3fffc0003fffc000c000c000" ) );
  EXPECT_EQ( cofactor0( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 1 ), from_hex( 8, "5fff5fff5fff5fff5fff5fff5fff5fff5fff5fff5fffa0005fffa000a000a000" ) );
  EXPECT_EQ( cofactor0( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 2 ), from_hex( 8, "77ff77ff77ff77ff77ff77ff77ff77ff77ff77ff77ff880077ff880088008800" ) );
  EXPECT_EQ( cofactor0( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 3 ), from_hex( 8, "ffffffffffffffffffffffffffffffffffffffffffff0000ffff000000000000" ) );
  EXPECT_EQ( cofactor0( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 4 ), from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe800e800e800e800e800e800" ) );
  EXPECT_EQ( cofactor0( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 5 ), from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800e800e800" ) );
  EXPECT_EQ( cofactor0( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 6 ), from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ffe800e800e80017ffe800e800e800" ) );
  EXPECT_EQ( cofactor0( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 7 ), from_hex( 8, "17ff17ff17ffe80017ffe800e800e80017ff17ff17ffe80017ffe800e800e800" ) );

  EXPECT_EQ( cofactor1( from_hex<3>( "e8" ), 2 ), from_hex<3>( "ee" ) );
  EXPECT_EQ( cofactor1( from_hex<3>( "e8" ), 1 ), from_hex<3>( "fa" ) );
  EXPECT_EQ( cofactor1( from_hex<3>( "e8" ), 0 ), from_hex<3>( "fc" ) );

  EXPECT_EQ( cofactor1( from_hex( 3, "e8" ), 2 ), from_hex( 3, "ee" ) );
  EXPECT_EQ( cofactor1( from_hex( 3, "e8" ), 1 ), from_hex( 3, "fa" ) );
  EXPECT_EQ( cofactor1( from_hex( 3, "e8" ), 0 ), from_hex( 3, "fc" ) );

  // Co-factors of [(<abc>d){<efg>h}]
  EXPECT_EQ( cofactor1( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 0 ), from_hex<8>( "03ff03ff03ff03ff03ff03ff03ff03ff03ff03ff03fffc0003fffc00fc00fc00" ) );
  EXPECT_EQ( cofactor1( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 1 ), from_hex<8>( "05ff05ff05ff05ff05ff05ff05ff05ff05ff05ff05fffa0005fffa00fa00fa00" ) );
  EXPECT_EQ( cofactor1( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 2 ), from_hex<8>( "11ff11ff11ff11ff11ff11ff11ff11ff11ff11ff11ffee0011ffee00ee00ee00" ) );
  EXPECT_EQ( cofactor1( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 3 ), from_hex<8>( "17171717171717171717171717171717171717171717e8e81717e8e8e8e8e8e8" ) );
  EXPECT_EQ( cofactor1( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 4 ), from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe800e800" ) );
  EXPECT_EQ( cofactor1( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 5 ), from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800" ) );
  EXPECT_EQ( cofactor1( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 6 ), from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ff17ff17ffe800" ) );
  EXPECT_EQ( cofactor1( from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 7 ), from_hex<8>( "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff" ) );

  EXPECT_EQ( cofactor1( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 0 ), from_hex( 8, "03ff03ff03ff03ff03ff03ff03ff03ff03ff03ff03fffc0003fffc00fc00fc00" ) );
  EXPECT_EQ( cofactor1( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 1 ), from_hex( 8, "05ff05ff05ff05ff05ff05ff05ff05ff05ff05ff05fffa0005fffa00fa00fa00" ) );
  EXPECT_EQ( cofactor1( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 2 ), from_hex( 8, "11ff11ff11ff11ff11ff11ff11ff11ff11ff11ff11ffee0011ffee00ee00ee00" ) );
  EXPECT_EQ( cofactor1( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 3 ), from_hex( 8, "17171717171717171717171717171717171717171717e8e81717e8e8e8e8e8e8" ) );
  EXPECT_EQ( cofactor1( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 4 ), from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe800e800" ) );
  EXPECT_EQ( cofactor1( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 5 ), from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800" ) );
  EXPECT_EQ( cofactor1( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 6 ), from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ff17ff17ffe800" ) );
  EXPECT_EQ( cofactor1( from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ffe80017ffe800e800e800" ), 7 ), from_hex( 8, "17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff17ff" ) );
}

TEST_F( OperationsTest, swap_adjacent_inplace_small )
{
  for ( const auto& p : std::vector<std::pair<unsigned, std::string>>{{0u, "bce8"}, {1u, "e6e8"}, {2u, "dea8"}} )
  {
    auto tt_s = from_hex<4>( "dae8" );
    swap_adjacent_inplace( tt_s, p.first );
    EXPECT_EQ( tt_s, from_hex<4>( p.second ) );

    auto tt_d = from_hex( 4, "dae8" );
    swap_adjacent_inplace( tt_d, p.first );
    EXPECT_EQ( tt_d, from_hex( 4, p.second ) );
  }
}

TEST_F( OperationsTest, swap_adjacent_small )
{
  for ( const auto& p : std::vector<std::pair<unsigned, std::string>>{{0u, "bce8"}, {1u, "e6e8"}, {2u, "dea8"}} )
  {
    EXPECT_EQ( swap_adjacent( from_hex<4>( "dae8" ), p.first ), from_hex<4>( p.second ) );
    EXPECT_EQ( swap_adjacent( from_hex( 4, "dae8" ), p.first ), from_hex( 4, p.second ) );
  }
}

class OperationsTestSwap : public OperationsTest, public ::testing::WithParamInterface<std::pair<unsigned, std::string>>
{
};

TEST_P( OperationsTestSwap, swap_adjacent_inplace_large )
{
  auto tt_s = from_hex<9>( "28e3b8d62855c4b787eef391a93b33297856658b6743aa3cc7e11fde4e9cbf7c98b07f5febfff33bc7ad6f551bc4cbc440453e1bbe24f0cb4f268c6771b55eee" );
  swap_adjacent_inplace( tt_s, GetParam().first );
  EXPECT_EQ( tt_s, from_hex<9>( GetParam().second ) );

  auto tt_d = from_hex( 9, "28e3b8d62855c4b787eef391a93b33297856658b6743aa3cc7e11fde4e9cbf7c98b07f5febfff33bc7ad6f551bc4cbc440453e1bbe24f0cb4f268c6771b55eee" );
  swap_adjacent_inplace( tt_d, GetParam().first );
  EXPECT_EQ( tt_d, from_hex( 9, GetParam().second ) );
}

TEST_P( OperationsTestSwap, swap_adjacent_large )
{
  EXPECT_EQ( swap_adjacent( from_hex<9>( "28e3b8d62855c4b787eef391a93b33297856658b6743aa3cc7e11fde4e9cbf7c98b07f5febfff33bc7ad6f551bc4cbc440453e1bbe24f0cb4f268c6771b55eee" ),
                            GetParam().first ),
             from_hex<9>( GetParam().second ) );

  EXPECT_EQ( swap_adjacent( from_hex( 9, "28e3b8d62855c4b787eef391a93b33297856658b6743aa3cc7e11fde4e9cbf7c98b07f5febfff33bc7ad6f551bc4cbc440453e1bbe24f0cb4f268c6771b55eee" ),
                            GetParam().first ),
             from_hex( 9, GetParam().second ) );
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

TEST_F( OperationsTest, flip_inplace_small )
{
  for ( const auto& p : std::vector<std::pair<unsigned, std::string>>{{0u, "0b34"}, {1u, "0dc2"}, {2u, "7083"}, {3u, "3807"}} )
  {
    auto tt_s = from_hex<4>( "0738" );
    flip_inplace( tt_s, p.first );
    EXPECT_EQ( tt_s, from_hex<4>( p.second ) );

    auto tt_d = from_hex( 4, "0738" );
    flip_inplace( tt_d, p.first );
    EXPECT_EQ( tt_d, from_hex( 4, p.second ) );
  }
}

TEST_F( OperationsTest, flip_small )
{
  for ( const auto& p : std::vector<std::pair<unsigned, std::string>>{{0u, "0b34"}, {1u, "0dc2"}, {2u, "7083"}, {3u, "3807"}} )
  {
    EXPECT_EQ( flip( from_hex<4>( "0738" ), p.first ), from_hex<4>( p.second ) );
    EXPECT_EQ( flip( from_hex( 4, "0738" ), p.first ), from_hex( 4, p.second ) );
  }
}

class OperationsTestFlip : public OperationsTest, public ::testing::WithParamInterface<std::pair<unsigned, std::string>>
{
};

TEST_P( OperationsTestFlip, flip_inplace_large )
{
  auto tt_s = from_hex<9>( "8725ca41421c7bba0ca86e26347847526fc346d7f3e79e76566a9493fbef11e24f74a07643afd946195f6a372757e045f3bca58f110ef00ebf2d81e80ba5679f" );
  flip_inplace( tt_s, GetParam().first );
  EXPECT_EQ( tt_s, from_hex<9>( GetParam().second ) );

  auto tt_d = from_hex( 9, "8725ca41421c7bba0ca86e26347847526fc346d7f3e79e76566a9493fbef11e24f74a07643afd946195f6a372757e045f3bca58f110ef00ebf2d81e80ba5679f" );
  flip_inplace( tt_d, GetParam().first );
  EXPECT_EQ( tt_d, from_hex( 9, GetParam().second ) );
}

TEST_P( OperationsTestFlip, flip_large )
{
  EXPECT_EQ( flip( from_hex<9>( "8725ca41421c7bba0ca86e26347847526fc346d7f3e79e76566a9493fbef11e24f74a07643afd946195f6a372757e045f3bca58f110ef00ebf2d81e80ba5679f" ),
                   GetParam().first ),
             from_hex<9>( GetParam().second ) );
  EXPECT_EQ( flip( from_hex( 9, "8725ca41421c7bba0ca86e26347847526fc346d7f3e79e76566a9493fbef11e24f74a07643afd946195f6a372757e045f3bca58f110ef00ebf2d81e80ba5679f" ),
                   GetParam().first ),
             from_hex( 9, GetParam().second ) );
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
