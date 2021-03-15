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

#pragma once

#include <unordered_set>

#include "npn.hpp"
#include "operations.hpp"
#include "constructors.hpp"

namespace kitty {

/*! \brief SD Represtatives Class

  This function returns an unordered set of all the SD represtatives for 
  a given number of variables. 

  \param num_vars Number of variables in the truth tables of the SD reprentative class. 
  \return unordered set of SD reprentative
*/
std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> calculate_sd_represtative_class(uint8_t num_vars){
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
	/*std::cout << "[i] enumerated "
		<< ( 1 << ( 1 << num_vars ) ) << " functions into "
		<< classes.size() << " classes." << std::endl;*/
	return classes;
}

/*! \brief Exact SD Represtative 

  Given a truth table, this function finds the lexicographically smallest truth
  table in its SD class, called SD representative. Two functions are in the
  same SD class, if the dual of the given function is equal to the given function, 
  also known as Self-Dual Functions.

  The function returns a SD representative (truth table).

  \param tt The truth table (with at most 6 variables)
  \return SD representative 
*/
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
	
/*! \brief NPN Represtatives Class

  This function returns an unordered set of all the NPN represtatives for 
  a given number of variables. 

  \param num_vars Number of variables in the truth tables of the NPN reprentative class. 
  \return NPN reprentative class
*/
std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> calculate_npn_represtative_class(uint8_t num_vars){
	/* compute NPN classe */
	std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> classes;
	kitty::dynamic_truth_table tt( num_vars );
	do{
		/* apply NPN canonization and add resulting representative to set */
		classes.insert(exact_npn_representative(tt));

		/* increment truth table */
		kitty::next_inplace( tt );
	} while ( !kitty::is_const0( tt ) );

	/*std::cout << "[i] enumerated "
		<< ( 1 << ( 1 << tt.num_vars() ) ) << " functions into "
		<< classes.size() << " classes." << std::endl;*/
	return classes;
}


}

