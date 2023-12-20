#include <iostream>
#include "utils.hpp"
#include "Tournament.hpp"

extern bool g_bFinishTournament;

void	startMatch(Tournament* tournament) {
	unsigned int					nbWaitingPlayers = tournament->getNumberOfWaitingPlayers();
	std::vector<Player*>&			waitingQueue = tournament->getWaitingQueue();
	std::vector<Player*>::iterator	it;

	if (tournament->getNumberOfPlayingMatches() == 0 && tournament->getNumberOfWaitingPlayers() == 0) {
		printMessage("Plus aucun joueur est en attente de match, arrêt du tournoi!", WARNING);
		g_bFinishTournament = true;
		return ;
	}
	if ((tournament->getMode() == ALL_SIMPLE && nbWaitingPlayers < 2)
		|| (tournament->getMode() == ALL_DOUBLE && nbWaitingPlayers < 4))
		return (printMessage("Il n'y a pas assez de joueur en file d'attente pour lancer un match!", WARNING));

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

static void	finishSimpleMatch(Tournament* tournament, std::string playerName) {
	std::string					buffer;
	std::pair<Player*, Player*>	match;
	std::pair<int, int>			score;

	match = tournament->findMatchByPlayer(tournament->findPlayer(playerName));
	if (match.first == NULL)
		return (printMessage("Le participant \'" + playerName + "\' n'a été trouvé dans aucun match!", WARNING));

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
	if (match.first->getNbOfMatches(tournament) == (int)(tournament->getNumberOfPlayers() - 1)) {
		std::cout << CBOLD << "\nLe joueur " << CYELLOW << match.first->getName() << CRESETB << " a fini tous ses matchs!" << std::endl;
		match.first->setStatus(FINISHED);
	}
	else
		tournament->addPlayerToWaitingQueue(match.first);

	if (match.second->getNbOfMatches(tournament) == (int)(tournament->getNumberOfPlayers() - 1)) {
		std::cout << CBOLD << "Le joueur " << CYELLOW << match.second->getName() << CRESETB << " a fini tous ses matchs!" << std::endl;
		match.second->setStatus(FINISHED);
	}
	else
		tournament->addPlayerToWaitingQueue(match.second);

	tournament->removeMatch(match);
	printMessage("Les deux joueurs ont été ajouté dans la file d'attente (si ils n'ont pas fini tous leurs matchs), le score est enregistré.\n");
	printMessage("Lancement d'un nouveau match!");
	startMatch(tournament);
}

static void	finishDoubleMatch(Tournament* tournament, std::string playerName) {
	std::string			buffer;
	playersMatchDouble	match;
	std::pair<int, int>	score;

	match = tournament->findDoubleMatchByPlayer(tournament->findPlayer(playerName));
	if (match.first.first == NULL)
		return (printMessage("Le participant \'" + playerName + "\' n'a été trouvé dans aucun match!", WARNING));

	printMessage("Match trouvé!");
	std::cout << CYELLOW << match.first.first->getName() << " et " << match.first.second->getName();
	std::cout << CRESET << " contre ";
	std::cout << CYELLOW << match.second.first->getName() << " et " << match.second.second->getName() << CRESET << '\n' << std::endl;

	printMessage("Indiquez le score des deux équipes au format suivant:");
	std::cout << "\'Score de " << CYELLOW << match.first.first->getName() << CRESET << " et " << CYELLOW << match.first.second->getName() << CRESET << '\'';
	std::cout << '-';
	std::cout << "\'Score de " << CYELLOW << match.second.first->getName() << CRESET << " et " << CYELLOW << match.second.second->getName() << CRESET << '\'';
	std::cout << " sans les guillemets." << std::endl;
	std::getline(std::cin, buffer);
	if (!isScoreCorrect(buffer))
		return (printMessage("Le format des scores est incorrect!", WARNING));

	score.first = atoi(buffer.substr(0, buffer.find('-')).c_str());
	score.second = atoi(buffer.substr(buffer.find('-') + 1).c_str());

	match.first.first->addDoubleScoreMatch(match.second.first, match.second.second, score);
	match.first.second->addDoubleScoreMatch(match.second.first, match.second.second, score);
	match.second.first->addDoubleScoreMatch(match.first.first, match.first.second, std::pair<int, int>(score.second, score.first));
	match.second.second->addDoubleScoreMatch(match.first.first, match.first.second, std::pair<int, int>(score.second, score.first));

	match.first.first->addToListAlreadyPlayed(match.first.second);
	match.first.second->addToListAlreadyPlayed(match.first.first);
	match.second.first->addToListAlreadyPlayed(match.second.second);
	match.second.second->addToListAlreadyPlayed(match.second.first);

	tournament->addPlayerToWaitingQueue(match.first.first);
	tournament->addPlayerToWaitingQueue(match.first.second);
	tournament->addPlayerToWaitingQueue(match.second.first);
	tournament->addPlayerToWaitingQueue(match.second.second);

	tournament->removeDoubleMatch(match);
	printMessage("Les joueurs ont été ajoutés dans la file d'attente, le score est enregistré.\n");
	printMessage("Lancement d'un nouveau match!");
	startMatch(tournament);
}

void	MATCH(Tournament* tournament) {
	std::string	buffer;

	if (tournament->getNumberOfPlayingMatches() == 0)
		return (printMessage("Il n'y a aucun match de lancé pour le moment, esseyons d'en lancer."), startMatch(tournament));

	tournament->showMatchs(true, false);
	printMessage("Entrez le nom d'un joueur participant au match à arrêter: ", -1, false);
	std::getline(std::cin, buffer);
	if (tournament->getMode() == ALL_SIMPLE)
		finishSimpleMatch(tournament, buffer);
	else
		finishDoubleMatch(tournament, buffer);
}
