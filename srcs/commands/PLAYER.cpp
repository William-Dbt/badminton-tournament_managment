#include <iostream>
#include "utils.hpp"
#include "Tournament.hpp"

bool	isValidName(std::string name);
void	startMatch(Tournament* tournament);

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
	if (!player && !isValidName(buffer))
		return ;

	if (buffer[0] == '-')
		return (printMessage("Le nom du joueur est incorrect! Il ne doit contenir que des lettres et le caractère \'.\'", ERROR));

	if (!player)
		tournament->addPlayer(buffer);

	if (!player && tournament->findPlayer(buffer) != NULL)
		printMessage("Le joueur " + buffer + "a bien été ajouté au tournoi!");

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
