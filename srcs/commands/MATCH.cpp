#include <iostream>
#include "utils.hpp"
#include "Tournament.hpp"

static void	startMatch(Tournament* tournament) {
	unsigned int					nbWaitingPlayers = tournament->getNumberOfWaitingPlayers();
	std::vector<Player*>&			waitingQueue = tournament->getWaitingQueue();
	std::vector<Player*>::iterator	it;

	if (nbWaitingPlayers <= 1)
		return (printMessage("Il n'y a pas assez de joueur en fil d'attente pour lancer un match!", ERROR));

	if (tournament->isCourtsFull())
		return (printMessage("Tous les terrains sont occupés pour le moment.", ERROR));

	for (it = waitingQueue.begin(); it != waitingQueue.end(); it++) {
		if (tournament->getNumberOfWaitingPlayers() <= 1)
			break ;

		if (tournament->isCourtsFull())
			break ;

		(*it)->findMatch(tournament);
	}
	if (nbWaitingPlayers == tournament->getNumberOfWaitingPlayers())
		return (printMessage("Aucun match n'a été trouvé, il se peut que tous les joueurs en fil d'attente aient déjà joué entre eux."));

	tournament->showMatchs(false);
}

static bool	isScoreCorrect(std::string score) {
	std::string::iterator	it;

	if (score.find('-') == std::string::npos)
		return false;

	it = score.begin();
	while (it != score.end() && (*it) != '-')
		if (!isdigit((*(it++))))
			return false;

	if (it == score.end())
		return false;

	it++;
	if (it == score.end())
		return false;

	while (it != score.end())
		if (!isdigit((*(it++))))
			return false;

	return true;
}

static void	finishMatch(Tournament* tournament) {
	std::string					buffer;
	std::pair<Player*, Player*>	match;
	std::pair<int, int>			score;

	if (tournament->getNumberOfPlayingMatches() == 0)
		return (printMessage("Il n'y a aucun match de lancé pour le moment, esseyons d'en lancer."), startMatch(tournament));

	tournament->showMatchs(true, false);
	printMessage("Entrez le nom de l'un des deux joueurs d'un match: ", -1, false);
	std::getline(std::cin, buffer);
	match = tournament->findMatchByPlayer(tournament->findPlayer(buffer));
	if (match.first == NULL)
		return (printMessage("Le participant " + buffer + " n'a été trouvé dans aucun match!", WARNING));

	printMessage("Indiquez le score des joueurs au format suivant:");
	printMessage("\t\'score de " + match.first->getName() + "\'-\'score de " + match.second->getName() + "\' sans les guillemets.");
	std::getline(std::cin, buffer);
	if (!isScoreCorrect(buffer))
		return (printMessage("Le format des scores est incorrect!", WARNING));

	score.first = atoi(buffer.substr(0, buffer.find('-')).c_str());
	score.second = atoi(buffer.substr(buffer.find('-') + 1).c_str());
	match.first->addScoreMatch(match.second->getName(), score);
	match.second->addScoreMatch(match.first->getName(), std::pair<int, int>(score.second, score.first));
	match.first->addToListAlreadyPlayed(match.second);
	match.second->addToListAlreadyPlayed(match.first);
	tournament->addPlayerToWaitingQueue(match.first);
	tournament->addPlayerToWaitingQueue(match.second);
	tournament->removeMatch(match);
	printMessage("Les deux joueurs ont été ajoutés dans la fil d'attente, le score est enregistré.\n");
	printMessage("Lancement d'un nouveau match!");
	startMatch(tournament);
}

void	MATCH(Tournament* tournament) {
	// std::string	buffer;

	// printMessage("Pour commencer un nouveau match tapez \'DEBUT\'");
	// printMessage("Pour finir un match tapez \'FIN\'.");
	// std::getline(std::cin, buffer);
	// std::cout << std::endl;
	// if (buffer.compare("DEBUT") == 0)
	// 	startMatch(tournament);
	// else if (buffer.compare("FIN") == 0)
		finishMatch(tournament);
	// else
	// 	printMessage(buffer + " n'est pas un argument valide.", WARNING);
}
