#include <iostream>
#include <unistd.h>
#include "utils.hpp"
#include "Tournament.hpp"

static void	mainMessage() {
	std::cout << "########################################\n";
	std::cout << "USJM Badminton: Programme pour tournois\n";
	std::cout << "Version: " << VERSION << '\n';
	std::cout << "########################################\n" << std::endl;
}

int	main() {
	Tournament	tournament;

	mainMessage();
	tournament.savePlayers();
	tournament.askCourtsNumber();

	printMessage("\nLancement du tournoi!");
	for (int i = 5; i > 0; i--) {
		std::cout << i << "...\n";
		sleep(1);
	}
	printMessage("Que les matchs commencent!\n");

	tournament.initFirstMatchs();
	tournament.showMatchs();
	tournament.managment();
	return 0;
}
