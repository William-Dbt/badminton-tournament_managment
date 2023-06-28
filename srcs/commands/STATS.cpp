#include <iostream>
#include <sstream>
#include "utils.hpp"
#include "Tournament.hpp"

static void	printName(Player* player) {
	int	i = 0;

	if (player->getName().size() > 11)
		std::cout << player->getName().substr(0, 10) << '.';
	else
		std::cout << player->getName();

	i = player->getName().size();
	while (i++ <= 11)
		std::cout << ' ';
}

static void	printScore(Player* player) {
	int					i;
	int					totalScore = player->getTotalScore();
	std::string			buffer;
	std::stringstream	ss;

	ss << totalScore;
	ss >> buffer;
	std::cout << buffer;
	i = buffer.size();
	while (i++ <= 4)
		std::cout << ' ';
}

static void	printMatchesStats(Tournament* tournament, Player* player) {
	int					i;
	std::string			buffer;
	std::stringstream	ss;

	ss << player->getNbOfMatches(tournament, false) << '/' << tournament->getNumberOfPlayers(false) - 1;
	ss >> buffer;
	std::cout << buffer;
	i = buffer.size();
	while (i++ <= 6)
		std::cout << ' ';
}

static void	printStatOfPlayer(Tournament* tournament, Player* player) {
	std::cout << "| ";
	printName(player);

	std::cout << "| ";
	printScore(player);

	std::cout << "| ";
	printMatchesStats(tournament, player);

	std::cout << "|\n";
}

void	STATS(Tournament* tournament) {
	std::map<const std::string, Player*>::iterator	it;

	printMessage("\nVoici les statistiques des joueurs:");
	std::cout << "|‾‾‾‾‾‾‾‾‾‾‾‾‾|‾‾‾‾‾‾|‾‾‾‾‾‾‾‾|\n";
	std::cout << "|   Joueurs   | Pts. | Nb. de |\n";
	std::cout << "|             |      | Matchs |\n";
	std::cout << "|             |      |        |\n";
	std::cout << "|‾‾‾‾‾‾‾‾‾‾‾‾‾|‾‾‾‾‾‾|‾‾‾‾‾‾‾‾|\n";
	for (it = tournament->getPlayersList().begin(); it != tournament->getPlayersList().end(); it++)
		printStatOfPlayer(tournament, (*it).second);

	std::cout << "|_____________|______|________|" << std::endl;
	if ((tournament->getNumberOfPlayers() - tournament->getNumberOfPlayers(false)) != 0)
		std::cout << "Rappel: " << (tournament->getNumberOfPlayers() - tournament->getNumberOfPlayers(false)) << " ne sont pas pris en compte dans le tournoi." << std::endl;
}
