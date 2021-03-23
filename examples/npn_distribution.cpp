#include <iostream>
#include <unordered_set>

#include <kitty/kitty.hpp>

int main () {
				//std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> classes;
				std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> :: iterator itr;	
				kitty::dynamic_truth_table sd_4u[83];

				auto classes = kitty::calculate_sd_represtative_class(4u);
				itr = classes.begin();
				int i = 0;
				std::cout<<classes.size();
				for (itr=classes.begin(); itr!=classes.end(); itr++){
								sd_4u[i] = *itr;
								i++;
								std::cout<<"32 ";
				}

				std::cout<<"initialization done...";
				auto npn_class = kitty::calculate_npn_represtative_class(4u);
				kitty::dynamic_truth_table npn_4u[222];

				itr = npn_class.begin();
				i = 0;
				for (itr=npn_class.begin(); itr!=npn_class.end(); itr++){
								npn_4u[i] = *itr;
								i++;
				}
				std::cout<<"initialization done...";
				int distribution[83];
				int j=0;
				i=0;
				for (i=0; i<222; i++){
								auto temp = npn_4u[i];
								temp = kitty::exact_sd_canonization(npn_4u[i]);
								for (j=0; j<83; j++){
												distribution[j] = 0;
												if (classes.find(temp)!=classes.end()){
																distribution[j] += 1;
												}
												j=0;
								}
				}
				j=0;
				for (j=0; j<83; j++){
								std::cout<<distribution[j]<<std::endl;
				}
				return 0;
}
