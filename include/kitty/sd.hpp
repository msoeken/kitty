/* kitty: C++ truth table library
 * Copyright (C) 2017-2021  EPFL
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

/*!
	\file sd.hpp
	\brief Implements SD  canonization algorithms

	\author Pulkit Agrawal and Shubham Rai
 */

#pragma once

#include <iostream>
#include <unordered_set>

#include <kitty/kitty.hpp>

namespace kitty {



template<typename TT, typename = std::enable_if_t<kitty::is_complete_truth_table<TT>::value>>
kitty::dynamic_truth_table dual_of( const TT& tt ){
	auto numvars = tt.num_vars();
	auto tt1 = tt;
	auto tt2 = ~tt1;
	for ( auto i = 0u; i < numvars; i++ ){
		tt1 = flip( tt1, i );
	}
	return ~tt1;
}

template<typename TT, typename = std::enable_if_t<kitty::is_complete_truth_table<TT>::value>>
kitty::dynamic_truth_table extend_tt (const TT& tt){
	int num_vars = (int)tt.num_vars();
	kitty::dynamic_truth_table extended_tt(num_vars+1);
	for (int i=0; i<(int)extended_tt.num_bits(); i++){
		if (kitty::get_bit(tt, i%(tt.num_bits())) == 1){
			kitty::set_bit(extended_tt, i);
		}
	}
	return extended_tt;
}

std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> calculate_sd_class(uint8_t num_vars){
	/* compute SD classes */
	auto npn_class = calculate_npn_represtative_class(num_vars);

	/* classes is the class which is returned containing all the tt in SD classification. */
	std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> classes;
	std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> :: iterator itr;

	/* initializing  the iterator */ 
	itr = npn_class.begin();

	kitty::dynamic_truth_table sd_tt(num_vars+1), a(num_vars+1);
	create_nth_var(a, num_vars);

	/* extending the tt from 'num_vars' to 'num_vars+1' */ 
	kitty::dynamic_truth_table extended_tt(num_vars+1);

	do {

		extended_tt = extend_tt((*itr));


		sd_tt = kitty::binary_or(kitty::binary_and(extended_tt, a), kitty::binary_and(~a, dual_of(extended_tt)));

		/* apply NPN canonization and add resulting representative to set */
		classes.insert(exact_npn_representative(sd_tt));

		/* increment to next tt in the class. */
		itr++;
	} while (itr != npn_class.end());

	/* Print the size of the class. */
	std::cout << "[i] enumerated "
		<< ( 1 << ( 1 << num_vars ) ) << " functions into "
		<< classes.size() << " classes." << std::endl;
	return classes;
}

template<typename TT, typename = std::enable_if_t<kitty::is_complete_truth_table<TT>::value>>
kitty::dynamic_truth_table exact_sd_canonization (const TT& tt){

	int num_vars = tt.num_vars();
	kitty::dynamic_truth_table sd_tt(num_vars+1), a(num_vars+1);
	create_nth_var(a, num_vars);

	/* extending the tt from 'num_vars' to 'num_vars+1' */ 
	kitty::dynamic_truth_table extended_tt(num_vars+1);

	for (int i=0; i<(int)extended_tt.num_bits(); i++){
		if (kitty::get_bit(tt, i%(tt.num_bits())) == 1){
			kitty::set_bit(extended_tt, i);
		}
	}

	/* Creating the tt with dual of extended_tt*/
	auto numvars = extended_tt.num_vars();
	auto tt1 = extended_tt;
	auto tt2 = ~tt1;

	for ( auto i = 0u; i < numvars; i++ ){
		tt1 = flip( tt1, i );
	}

	auto dual_of_extended_tt = ~tt1;

	sd_tt = kitty::binary_or(kitty::binary_and(extended_tt, a), kitty::binary_and(~a, dual_of_extended_tt));

	/* apply NPN canonization and add resulting representative to set */
	return exact_npn_representative(sd_tt);
	}

}

