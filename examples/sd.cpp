#include <iostream>
#include <unordered_set>

#include <kitty/kitty.hpp>

int main () {
	const auto classes_3u_sd = kitty::calculate_sd_represtative_class(4u);
	std::cout<< classes_3u_sd.size()<<std::endl;
	return 0;
}

