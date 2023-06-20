#include <iostream>
#include "utils.hpp"
#include "Tournament.hpp"

void	HISTORY(Tournament* tournament) {
	std::string	buffer;
	Player*		player;

	std::pair<int, int>														score;
	std::vector< std::pair<std::string, std::pair<int, int> > >				scoreHistory;
	std::vector< std::pair<std::string, std::pair<int, int> > >::iterator	it;

	tournament->showPlayers(false);
	printMessage("Indiquez le nom du joueur dont vous voulez voir l'historique: ", -1, false);
	std::getline(std::cin, buffer);
	player = tournament->findPlayer(buffer);
	if (player == NULL)
		return printMessage("Le joueur " + buffer + " n'a pas été trouvé!", ERROR);

	scoreHistory = player->getScoreHistory();
	if (scoreHistory.size() == 0)
		return printMessage("Le joueur " + player->getName() + " n'a encore effectué aucun match.");

	player->showScoreHistory();
}
