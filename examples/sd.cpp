#include <iostream>
#include <unordered_set>

#include <kitty/kitty.hpp>

int main () {
	uint32_t num_vars = 4u;
	const auto classes = kitty::calculate_sd_represtative_class(num_vars);
	std::cout << "[i] enumerated "
			<< ( 1 << ( 1 << num_vars ) ) << " functions into "
					<< classes.size() << " classes." << std::endl;

	
	return 0;
}

