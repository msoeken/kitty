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
#include <iostream>

#include <kitty/kitty.hpp>

int main( int argc, char** argv )
{
  using TTg = kitty::static_truth_table<3>;
  using TTh = kitty::static_truth_table<3>;
  using TTf = kitty::static_truth_table<5>;

  TTf tt, x1, x2, x3, x4, x5;
  kitty::create_nth_var( x1, 0 );
  kitty::create_nth_var( x2, 1 );
  kitty::create_nth_var( x3, 2 );
  kitty::create_nth_var( x4, 3 );
  kitty::create_nth_var( x5, 4 );

  tt = x1 | x2 | ( x3 & x4 & x5 );

  std::cout << "tt = \t\t" << kitty::to_binary( tt ) << std::endl;

  TTg g;
  TTh h;
  kitty::create_from_binary_string( g, "11111110" );
  kitty::create_from_binary_string( h, "10000000" );

  std::vector<uint32_t> ys_idx{2, 3, 4};

  std::vector<std::pair<TTg, TTg>> decomposition;

  if ( auto count = kitty::ashenhurst_decomposition( tt, ys_idx, decomposition ) )
    std::cout << "Found " << count << " decompositions" << std::endl;

  for ( const auto& pair : decomposition )
  {
    auto g = pair.first;
    auto h = pair.second;
    std::cout << "G: " << kitty::to_binary( g ) << " H: " << kitty::to_binary( h ) << std::endl;
  }

  return 0;
}
