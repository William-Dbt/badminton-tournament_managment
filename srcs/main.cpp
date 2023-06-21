#include <iostream>
#include <unistd.h>
#include "utils.hpp"
#include "Tournament.hpp"

void	saveHistory(Tournament* tournament);
void	getHistory(Tournament* tournament);
void	showEndHistory(Tournament* tournament);

static void	mainMessage() {
	std::cout << "########################################\n";
	std::cout << "USJM Badminton: Programme pour tournois\n";
	std::cout << "Version: " << VERSION << '\n';
	std::cout << "########################################\n" << std::endl;
}

int	main() {
	Tournament	tournament;

	mainMessage();
	if (tournament.startWithHistory())
		getHistory(&tournament);
	else {
		tournament.savePlayers();
		tournament.askCourtsNumber();

		printMessage("\nLancement du tournoi!");
		for (int i = 5; i > 0; i--) {
			std::cout << i << "...\n";
			sleep(1);
		}
		printMessage("Que les matchs commencent!\n");

		tournament.initFirstMatchs();
	}
	tournament.showMatchs();
	tournament.managment();

	saveHistory(&tournament);
	showEndHistory(&tournament);
	return 0;
}
