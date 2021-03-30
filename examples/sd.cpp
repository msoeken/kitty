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

#include <iostream>
#include <unordered_set>

#include <kitty/kitty.hpp>

int main()
{

  uint32_t num_vars = 2u;
  const auto classes_2u = kitty::calculate_sd_represtative_class( num_vars );

  /* Print the size of the class. */
  std::cout << "[i] enumerated "
            << ( 1 << ( 1 << num_vars ) ) << " functions into "
            << classes_2u.size() << " classes." << std::endl;
			
	num_vars = 3u;
  const auto classes_3u = kitty::calculate_sd_represtative_class( num_vars );

  /* Print the size of the class. */
  std::cout << "[i] enumerated "
            << ( 1 << ( 1 << num_vars ) ) << " functions into "
            << classes_3u.size() << " classes." << std::endl;

  num_vars = 4u;
  const auto classes_4u = kitty::calculate_sd_represtative_class( num_vars );

  /* Print the size of the class. */
  std::cout << "[i] enumerated "
            << ( 1 << ( 1 << num_vars ) ) << " functions into "
            << classes_4u.size() << " classes." << std::endl;

  num_vars = 5u;
  const auto classes_5u = kitty::calculate_sd_represtative_class( num_vars );

  /* Print the size of the class.*/
  std::cout << "[i] enumerated "
            << ( 1 << ( 1 << num_vars ) ) << " functions into "
            << classes_5u.size() << " classes." << std::endl;

  
  return 0;
}
