#include <iostream>
#include "utils.hpp"
#include "Tournament.hpp"

// nbMatchsConsidered refers to the maximum number of matchs per player that we will check
// isAllPlayerTaken, did we consider stopped-players in the total sum?
typedef struct	s_historyVars {
	bool			isStoppedPlayerConsidered;
	unsigned int	nbMatchsConsidered;
	
	int	firstScore;
	int	secondScore;
	int	thirdScore;
}	t_historyVars;

t_historyVars	g_sTournamentVars;

static void	getNbOfMatchesConsidered(std::pair<int, int>& nbOfMatches) {
	std::string	buffer;

	printMessage("De combien de matchs voulez-vous vous soucier pour le classement final? ", -1, false);
	while (std::getline(std::cin, buffer)) {
		if (isStringEmpty(buffer) || !isStringNumeric(buffer)) {
			printMessage("Votre réponse peut contenir que des chiffres!", WARNING);
			continue ;
		}
		if (std::stoi(buffer) < nbOfMatches.first || std::stoi(buffer) > nbOfMatches.second) {
			printMessage("Le nombre de matchs doit être compris entre " + std::to_string(nbOfMatches.first) + " et " + std::to_string(nbOfMatches.second) + "!", WARNING);
			continue ;
		}
		break ;
	}
	g_sTournamentVars.nbMatchsConsidered = std::stoi(buffer);
}

static void	getTournamentVars(Tournament* tournament) {
	std::string			buffer;
	std::pair<int, int>	nbOfMatches;

	printMessage("Dois-t'on prendre en compte les matchs des joueurs arrêté? (les matchs contre les joueurs arrêté compterons toujours)");
	printMessage("Répondre par O/OUI/oui ou N/NON/non");
	while (std::getline(std::cin, buffer)) {
		if (!isOui(buffer) && !isNon(buffer)) {
			printMessage("Je n'ai pas compris votre réponse.", WARNING);
			printMessage("Répondre par O/OUI/oui ou N/NON/non");
			continue ;
		}
		if (isOui(buffer))
			g_sTournamentVars.isStoppedPlayerConsidered = true;
		else if (isNon(buffer))
			g_sTournamentVars.isStoppedPlayerConsidered = false;

		break ;
	}
	nbOfMatches = tournament->getNumberOfMaxMinPlayedMatches(g_sTournamentVars.isStoppedPlayerConsidered);
	printMessage("\nVoici le nombre minimum de match effectué par un (ou plusieurs) joueurs: " + std::to_string(nbOfMatches.first) + ".");
	printMessage("Et le nombre maximum: " + std::to_string(nbOfMatches.second) + ".\n");
	getNbOfMatchesConsidered(nbOfMatches);
}

static void	getTotalScorePerPlayer(Tournament* tournament, std::vector< std::pair<std::string, int> >&	totalScorePerPlayer) {
	std::map<const std::string, Player*>::iterator	it;

	for (it = tournament->getPlayersList().begin(); it != tournament->getPlayersList().end(); it++) {
		if (!g_sTournamentVars.isStoppedPlayerConsidered && (*it).second->getStatus() == STOPPED)
			continue ;

		totalScorePerPlayer.push_back(std::make_pair((*it).first, (*it).second->getTotalScore(g_sTournamentVars.nbMatchsConsidered)));
	}
}

static void	showFirstPlayers(std::vector< std::pair<std::string, int> >& totalScorePerPlayer) {
	int	tmpMaxScore = -1;

	std::vector< std::pair<std::string, int> >				firstPlayers;
	std::vector< std::pair<std::string, int> >::iterator	it;

	for (it = totalScorePerPlayer.begin(); it != totalScorePerPlayer.end(); it++) {
		if (tmpMaxScore == -1) {
			tmpMaxScore = (*it).second;
			continue ;
		}
		if ((*it).second > tmpMaxScore)
			tmpMaxScore = (*it).second;
	}
	for (it = totalScorePerPlayer.begin(); it != totalScorePerPlayer.end(); it++)
		if ((*it).second == tmpMaxScore)
			firstPlayers.push_back(*it);

	printMessage("Avec un score de " + std::to_string(tmpMaxScore) + " la première place reviens à:");
	for (it = firstPlayers.begin(); it != firstPlayers.end(); it++)
		std::cout << (*it).first << std::endl;

	g_sTournamentVars.firstScore = tmpMaxScore;
}

static void	showSecondPlayers(std::vector< std::pair<std::string, int> >& totalScorePerPlayer) {
	int	tmpMaxScore = -1;

	std::vector< std::pair<std::string, int> >				secondPlayers;
	std::vector< std::pair<std::string, int> >::iterator	it;

	for (it = totalScorePerPlayer.begin(); it != totalScorePerPlayer.end(); it++) {
		if (tmpMaxScore == -1) {
			if ((*it).second < g_sTournamentVars.firstScore)
				tmpMaxScore = (*it).second;

			continue ;
		}
		if ((*it).second > tmpMaxScore && (*it).second < g_sTournamentVars.firstScore)
			tmpMaxScore = (*it).second;
	}
	if (tmpMaxScore == -1)
		return ;

	for (it = totalScorePerPlayer.begin(); it != totalScorePerPlayer.end(); it++)
		if ((*it).second == tmpMaxScore)
			secondPlayers.push_back(*it);

	printMessage("Avec un score de " + std::to_string(tmpMaxScore) + " la deuxième place reviens à:");
	for (it = secondPlayers.begin(); it != secondPlayers.end(); it++)
		std::cout << (*it).first << std::endl;

	g_sTournamentVars.secondScore = tmpMaxScore;
}

static void	showThirdPlayers(Tournament* tournament, std::vector< std::pair<std::string, int> >& totalScorePerPlayer) {
	int	tmpMaxScore = -1;

	std::vector< std::pair<std::string, int> >				thirdPlayers;
	std::vector< std::pair<std::string, int> >::iterator	it;

	if (tournament->getNumberOfPlayers() < 3)
		return ;

	for (it = totalScorePerPlayer.begin(); it != totalScorePerPlayer.end(); it++) {
		if (tmpMaxScore == -1) {
			if ((*it).second < g_sTournamentVars.secondScore)
				tmpMaxScore = (*it).second;

			continue ;
		}
		if ((*it).second > tmpMaxScore && (*it).second < g_sTournamentVars.secondScore)
			tmpMaxScore = (*it).second;
	}
	if (tmpMaxScore == -1)
		return ;

	for (it = totalScorePerPlayer.begin(); it != totalScorePerPlayer.end(); it++)
		if ((*it).second == tmpMaxScore)
			thirdPlayers.push_back(*it);

	printMessage("Avec un score de " + std::to_string(tmpMaxScore) + " la troisième place reviens à:");
	for (it = thirdPlayers.begin(); it != thirdPlayers.end(); it++)
		std::cout << (*it).first << std::endl;

	g_sTournamentVars.thirdScore = tmpMaxScore;
}

void	showEndHistory(Tournament* tournament) {
	std::vector< std::pair<std::string, int> >	totalScorePerPlayer;

	printMessage("Voici les statistiques du tournoi:");
	getTournamentVars(tournament);
	getTotalScorePerPlayer(tournament, totalScorePerPlayer);

	printMessage("\nVoici la liste des joueurs ainsi que de leurs points:");
	std::vector< std::pair<std::string, int> >::iterator	it;
	for (it = totalScorePerPlayer.begin(); it != totalScorePerPlayer.end(); it++)
		std::cout << (*it).first << " => " << (*it).second << " points." << std::endl;

	std::cout << std::endl;
	showFirstPlayers(totalScorePerPlayer);
	std::cout << std::endl;
	showSecondPlayers(totalScorePerPlayer);
	std::cout << std::endl;
	showThirdPlayers(tournament, totalScorePerPlayer);
}
