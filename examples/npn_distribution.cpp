#include <iostream>
#include <unordered_set>

#include <kitty/kitty.hpp>

int main () {
	std::unordered_set<kitty::dynamic_truth_table, kitty::hash<kitty::dynamic_truth_table>> :: iterator itr;

	auto class_sd = kitty::calculate_sd_represtative_class(4u);
	auto class_npn = kitty::calculate_npn_represtative_class(4u);
	kitty::dynamic_truth_table array_npn[222];

	itr = class_npn.begin();
	int i = 0;
	for (itr=class_npn.begin(); itr!=class_npn.end(); itr++){
		array_npn[i] = *itr;
		i++;
	}
	
	int distribution[83];
	int j=0;
	i=0;
	for (i=0; i<222; i++){
		auto temp = kitty::exact_sd_canonization(array_npn[i]);
		for (j=0; j<83; j++){
			distribution[j] = 0;
		}
		if (class_sd.find(temp)!=class_sd.end()){
			std::cout<<"Oh Yeah";
	}
	return 0;
}
