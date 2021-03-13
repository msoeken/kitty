#include <iostream>
#include <unordered_set>

#include <kitty/kitty.hpp>

template<typename TT, typename Callback = decltype( kitty::detail::exact_npn_canonization_null_callback<TT> )>
TT npn_canonization( const TT& tt, Callback&& fn = kitty::detail::exact_npn_canonization_null_callback<TT> )
{
				static_assert( kitty::is_complete_truth_table<TT>::value, "Can only be applied on complete truth tables." );

				const auto num_vars = tt.num_vars();

				/* Special case for n = 0 */
				if ( num_vars == 0 )
				{
								const auto bit = get_bit( tt, 0 );
								return kitty::unary_not_if( tt, bit );
				}

				/* Special case for n = 1 */
				if ( num_vars == 1 )
				{
								const auto bit1 = get_bit( tt, 1 );
								return kitty::unary_not_if( tt, bit1 );
				}

				assert( num_vars >= 2 && num_vars <= 6 );

				auto t1 = tt, t2 = ~tt;
				auto tmin = std::min( t1, t2 );
				auto invo = tmin == t2;

				fn( t1 );
				fn( t2 );

				const auto& swaps = kitty::detail::swaps[num_vars - 2u];
				const auto& flips = kitty::detail::flips[num_vars - 2u];

				int best_swap = -1;
				int best_flip = -1;

				for ( std::size_t i = 0; i < swaps.size(); ++i )
				{
								const auto pos = swaps[i];
								kitty::swap_adjacent_inplace( t1, pos );
								kitty::swap_adjacent_inplace( t2, pos );

								fn( t1 );
								fn( t2 );

								if ( t1 < tmin || t2 < tmin )
								{
												best_swap = static_cast<int>( i );
												tmin = std::min( t1, t2 );
												invo = tmin == t2;
								}
				}

				for ( std::size_t j = 0; j < flips.size(); ++j )
				{
								const auto pos = flips[j];
							  kitty::swap_adjacent_inplace( t1, 0 );
							  kitty::flip_inplace( t1, pos );
							  kitty::swap_adjacent_inplace( t2, 0 );
							  kitty::flip_inplace( t2, pos );

							  fn( t1 );
							  fn( t2 );

								if ( t1 < tmin || t2 < tmin )
								{
												best_swap = -1;
												best_flip = static_cast<int>( j );
												tmin = std::min( t1, t2 );
												invo = tmin == t2;
								}

								for ( std::size_t i = 0; i < swaps.size(); ++i )
								{
												const auto pos = swaps[i];
												kitty::swap_adjacent_inplace( t1, pos );
												kitty::swap_adjacent_inplace( t2, pos );

												fn( t1 );
												fn( t2 );

												if ( t1 < tmin || t2 < tmin )
												{
																best_swap = static_cast<int>( i );
																best_flip = static_cast<int>( j );
																tmin = std::min( t1, t2 );
																invo = tmin == t2;
												}
								}
				}

				return tmin;
}

std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> calculate_npn_class(uint8_t num_vars){
				/* compute NPN classe */
				std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> classes;
				kitty::dynamic_truth_table tt( num_vars );
				do{
								/* apply NPN canonization and add resulting representative to set */
								classes.insert(npn_canonization(tt));

								/* increment truth table */
								kitty::next_inplace( tt );
				} while ( !kitty::is_const0( tt ) );

				std::cout << "[i] enumerated "
								<< ( 1 << ( 1 << tt.num_vars() ) ) << " functions into "
								<< classes.size() << " classes." << std::endl;
				return classes;
}

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

std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> calculate_sd_class_fast(uint8_t num_vars){
				/* compute SD classes */
				auto npn_class = calculate_npn_class(num_vars);

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
								classes.insert(npn_canonization(sd_tt));

								/* increment to next tt in the class. */
								itr++;
				} while (itr != npn_class.end());

				/* Print the size of the class. */
				std::cout << "[i] enumerated "
								<< ( 1 << ( 1 << num_vars ) ) << " functions into "
								<< classes.size() << " classes." << std::endl;
				return classes;
}
/*
	 har sd class mai kitne npn hai dekhna hai
	 timing dekhna hai ki kitna time lagra hai. npn timing dekhna hai. 


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
				return npn_canonization(sd_tt);
}


std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> calculate_sd_class(uint8_t num_vars){
				kitty::dynamic_truth_table tt(num_vars);
				std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> classes;
				kitty::dynamic_truth_table sd_tt(num_vars+1), a(num_vars+1), extended_tt(num_vars+1);
				create_nth_var( a, num_vars);
				bool res;
				do {
								res = kitty::is_selfdual(tt);
								if (res){
												classes.insert(npn_canonization(tt));
								} else if (!res){
												extended_tt = extend_tt(tt);
												sd_tt = kitty::binary_or(kitty::binary_and(extended_tt, a), kitty::binary_and(~a, dual_of(extended_tt)));
												classes.insert(npn_canonization(sd_tt));
								}
								kitty::next_inplace( tt );
				} while (!kitty::is_const0( tt ));
				std::cout << "[i] enumerated "
								<< ( 1 << ( 1 << tt.num_vars() ) ) << " functions into "
								<< classes.size() << " classes." << std::endl;

				return classes;
}

int main () {
				const auto classes_3u_sd = calculate_sd_class_fast(3u);
				return 0;
}

