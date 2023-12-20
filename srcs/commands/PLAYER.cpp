#include <iostream>
#include "utils.hpp"
#include "Tournament.hpp"

void	startMatch(Tournament* tournament);
bool	isValidName(std::string name, bool ignoreMinus);

static void	checkPlayerStatus(Tournament* tournament) {
	std::map<const std::string, Player*>::iterator	it;

	for (it = tournament->getPlayersList().begin(); it != tournament->getPlayersList().end(); it++) {
		if ((*it).second->getStatus() == FINISHED 
			&&(*it).second->getNbOfMatches() < (int)(tournament->getNumberOfPlayers() - 1))
		{
			std::cout << CBOLD << "Le joueur " << CYELLOW << (*it).second->getName() << CRESETB;
			std::cout << " retourne dans le tournoi à la suite de l'ajout d'un joueur!" << std::endl;
			tournament->addPlayerToWaitingQueue((*it).second);
		}
	}
}

static void	addPlayer(Tournament* tournament) {
	Player*		player;
	std::string	buffer;

	printMessage("Entrez le nom du joueur que vous souhaitez ajouter: ", -1, false);
	std::getline(std::cin, buffer);
	std::cout << std::endl;
	player = tournament->findPlayer(buffer, true);
	if (player) {
		if (player->getStatus() == STOPPED) {
			printMessage("Le joueur " + buffer + " existe déjà mais ne participe plus au tournoi, souhaitez-vous le réintégrer à celui-ci? (O:oui/N:non) ", -1, false);
			std::getline(std::cin, buffer);
			if (isOui(buffer)) {
				tournament->addPlayerToWaitingQueue(player);
				printMessage(player->getName() + " a été ajouté à la file d'attente!");
			}
			else
				return (printMessage("Le joueur " + player->getName() + " restera donc sur le banc de touche."));
		}
		else
			return (printMessage("Le joueur " + player->getName() + " existe déjà!", WARNING));
	}
	if (!player && !isValidName(buffer, true))
		return ;

	if (!player)
		tournament->addPlayer(buffer);

	if (!player && tournament->findPlayer(buffer) != NULL) {
		printMessage("Le joueur " + buffer + " a bien été ajouté au tournoi!");
		checkPlayerStatus(tournament);
	}
	else
		return (printMessage("Le joueur n'a pas pu être ajouté au tournoi.", ERROR));

	if (tournament->getNumberOfWaitingPlayers() >= 2
		&& tournament->getNumberOfPlayingMatches() < tournament->getNumberOfCourts()) {
		printMessage("\nIl semblerait qu'un/des match(s) puisse(nt) être lancé!");
		startMatch(tournament);
	}
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
