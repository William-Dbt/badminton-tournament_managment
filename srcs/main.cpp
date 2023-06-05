#include <iostream>
#include "Tournament.hpp"

static void	initMessage() {
	std::cout << "########################################\n";
	std::cout << "USJM Badminton: Programme pour tournois\n";
	std::cout << "Version: " << VERSION << '\n';
	std::cout << "########################################\n" << std::endl;
}

int	main() {
	Tournament	tournament;

	initMessage();
	tournament.init();
}
