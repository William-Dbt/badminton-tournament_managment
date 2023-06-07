#include <iostream>
#include <unistd.h>
#include "utils.hpp"
#include "Tournament.hpp"

// bool	g_bTournamentRunning = true;

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

	printMessage("\nLancement du tournoi dans 5 seconds!");
	for (int i = 5; i > 0; i--) {
		std::cout << i << "...\n";
		sleep(1);
	}
	std::cout << "Que les matchs commencent!\n" << std::endl;

	tournament.initFirstMatchs();
	tournament.showMatchs();

	return 0;
}
