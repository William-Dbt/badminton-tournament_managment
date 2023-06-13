#include <iostream>
#include <unistd.h>
#include "utils.hpp"
#include "Tournament.hpp"

void	saveHistory(Tournament* tournament);
void	getHistory(Tournament* tournament);

static void	showScoreHistoryOfAllPlayers(std::map<const std::string, Player*>& playerList) {
	std::map<const std::string, Player*>::iterator	it;

	std::cout << std::endl;
	for (it = playerList.begin(); it != playerList.end(); it++) {
		(*it).second->showScoreHistory();
		std::cout << std::endl;
	}
}

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

	showScoreHistoryOfAllPlayers(tournament.getPlayersList());
	saveHistory(&tournament);
	return 0;
}
