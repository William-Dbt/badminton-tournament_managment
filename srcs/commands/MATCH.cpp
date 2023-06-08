#include <iostream>
#include "utils.hpp"
#include "Tournament.hpp"

static void	startMatch(Tournament* tournament) {
	std::vector<Player*>&			_waitingQueue = tournament->getWaitingQueue();
	std::vector<Player*>::iterator	mainIt;
	std::vector<Player*>::iterator	it;

	if (tournament->getNumberOfWaitingPlayers() <= 1)
		printMessage("Impossible de trouver un match pour le moment!", ERROR);

	for (mainIt = _waitingQueue.begin(); mainIt != _waitingQueue.end(); mainIt++) {
		
	}
	// Check Waiting Queue in order to give match for thoses which aren't fight
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

	printMessage("Entrez le nom d'un des deux joueurs du match: ");
	std::getline(std::cin, buffer);
	match = tournament->findMatchByPlayer(tournament->findPlayer(buffer));
	if (match.first == NULL) {
		printMessage("Le participant " + buffer + " n'a été trouvé dans aucun match!", WARNING);
		return ;
	}

	printMessage("Indiquez le score des joueurs au format suivant");
	printMessage("\t\'score de " + match.first->getName() + "\'-\'score de " + match.second->getName() + "\' sans les guillemets.");
	std::getline(std::cin, buffer);
	if (!isScoreCorrect(buffer)) {
		printMessage("Le format des scores est incorrect!", WARNING);
		return ;
	}

	score.first = atoi(buffer.substr(0, buffer.find('-')).c_str());
	score.second = atoi(buffer.substr(buffer.find('-') + 1).c_str());
	match.first->addScoreMatch(match.second->getName(), score);
	match.second->addScoreMatch(match.first->getName(), std::pair<int, int>(score.second, score.first));
	match.first->addToListAlreadyPlayed(match.second);
	match.second->addToListAlreadyPlayed(match.first);
	tournament->addPlayerToWaitingQueue(match.first);
	tournament->addPlayerToWaitingQueue(match.second);
	tournament->removeMatch(match);
}

void	MATCH(Tournament* tournament) {
	std::string	buffer;

	printMessage("Pour commencer un nouveau match tapez \'DEBUT\'");
	printMessage("Pour finir un match tapez \'FIN\'.");
	std::getline(std::cin, buffer);
	if (buffer.compare("DEBUT") == 0)
		startMatch(tournament);
	else if (buffer.compare("FIN") == 0)
		finishMatch(tournament);
	else
		printMessage(buffer + " n'est pas un argument valide.", WARNING);
}