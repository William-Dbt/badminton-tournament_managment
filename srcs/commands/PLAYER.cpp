#include <iostream>
#include "utils.hpp"
#include "Tournament.hpp"

static void	addPlayer(Tournament* tournament) {
	(void)tournament;
	std::cout << "addPlayer" << std::endl;
}

static void	removePlayer(Tournament* tournament) {
	Player*		player;
	std::string	buffer;

	tournament->showPlayers(false);
	printMessage("Entrez le nom du joueur que vous voulez supprimer: ", -1, false);
	std::getline(std::cin, buffer);
	player = tournament->findPlayer(buffer);
	if (!player)
		return (printMessage("Le joueur \'" + buffer + "\' n'existe pas.", WARNING));

	std::cout << std::endl;
	tournament->removePlayer(player->getName());
}

void	PLAYER(Tournament* tournament) {
	std::string	buffer;

	printMessage("Voulez-vous supprimer ou ajouter un joueur? (AJOUTER/SUPPRIMER) ", -1, false);
	std::getline(std::cin, buffer);
	if (buffer.compare("AJOUTER") == 0)
		addPlayer(tournament);
	else if (buffer.compare("SUPPRIMER") == 0)
		removePlayer(tournament);
	else
		printMessage("La commande \'" + buffer + "\' n'existe pas.", WARNING);
}
