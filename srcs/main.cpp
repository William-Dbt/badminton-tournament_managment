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
	std::string	buffer;
	Tournament	tournament;

	mainMessage();
	printMessage("Quel mode voulez-vous lancer?");
	std::cout << " 0 - Simple All vs All\n";
	std::cout << " 1 - Double All vs All" << std::endl;
	std::getline(std::cin, buffer);
	if (!isStringNumeric(buffer))
		return (printMessage("Mode non reconnu.", ERROR), 1);

	switch (atoi(buffer.c_str())) {
		case 0: { 
			tournament.setMode(ALL_SIMPLE);
			break ;
		}
		case 1: {
			tournament.setMode(ALL_DOUBLE);
			break ;
		}
		default: return (printMessage("Mode non reconnu.", ERROR), 1);
	}

	if (tournament.getMode() != ALL_DOUBLE && tournament.startWithHistory())
		getHistory(&tournament);
	else {
		tournament.savePlayers();
		// tournament.askCourtsNumber();
		tournament.setCourts(5);

		// printMessage("\nLancement du tournoi!");
		// for (int i = 5; i > 0; i--) {
		// 	std::cout << i << "...\n";
		// 	sleep(1);
		// }
		// printMessage("Que les matchs commencent!\n");

		tournament.initFirstMatchs();
	}
	tournament.showMatchs();
	tournament.managment();

	saveHistory(&tournament);
	showEndHistory(&tournament);
	return 0;
}
