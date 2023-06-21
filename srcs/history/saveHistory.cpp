#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <time.h>
#include "utils.hpp"
#include "Tournament.hpp"

static std::string	getCurrentDateTime() {
	time_t		now = time(NULL);
	struct tm	timeStruct;
	char		buffer[80];
	timeStruct = *localtime(&now);

	strftime(buffer, sizeof(buffer), "%d%m%Y_%H%M%S", &timeStruct);
	return buffer;
}

static std::string	getPathOfFile() {
	std::string	buffer;

	buffer = "/var/logs/usjm_tournament/";
	buffer += getCurrentDateTime();
	buffer += "_tournament.log";
	return buffer;
}

static void	savePlayers(std::fstream& file, std::map<const std::string, Player*>& playerList) {
	std::map<const std::string, Player*>::iterator	it;

	file << "playerList\n";
	file << "{\n";
	for (it = playerList.begin(); it != playerList.end(); it++)
		file << '\t' << (*it).first << '\n';

	file << "}\n\n";
}

static void	saveTournament(std::fstream& file, unsigned int nbOfCourts) {
	file << "tournamentInfos\n";
	file << "{\n";
	file << '\t' << "nbCourts:" << nbOfCourts << '\n';
	file << "}\n\n";
}

static void	saveMatchesInProgress(std::fstream& file, std::vector< std::pair<Player*, Player*> > matchesInProgress) {
	std::vector< std::pair<Player*, Player*> >::iterator	it;

	file << "matchesInProgress\n";
	file << "{\n";
	for (it = matchesInProgress.begin(); it != matchesInProgress.end(); it++)
		file << '\t' << (*it).first->getName() << ':' << (*it).second->getName() << '\n';

	file << "}\n\n";
}

static void	savePlayersHistory(std::fstream& file, std::map<const std::string, Player*> playerList) {
	Player*											player;
	std::map<const std::string, Player*>::iterator	it;

	std::vector< std::pair< std::string, std::pair<int, int> > >			scoreHistory;
	std::vector< std::pair< std::string, std::pair<int, int> > >::iterator	itScore;

	for (it = playerList.begin(); it != playerList.end(); it++) {
		player = (*it).second;
		file << (*it).first << '\n';
		file << "{\n";
		file << "\tstatus:";
		if (player->getStatus() == FINISHED || player->getStatus() == STOPPED)
			(player->getStatus() == FINISHED) ? file << "finished\n" : file << "stopped\n";
		else
			file << "auto\n";

		file << '\n';
		file << "\tscoreHistory\n";
		file << "\t{\n";
		scoreHistory = player->getScoreHistory();
		for (itScore = scoreHistory.begin(); itScore != scoreHistory.end(); itScore++) {
			file << "\t\tagainst:" << (*itScore).first << '\n';
			file << "\t\tscore:" << (*itScore).second.first << '-' << (*itScore).second.second << '\n';
			if (itScore + 1 != scoreHistory.end())
				file << '\n';
		}
		file << "\t}\n";
		file << "}\n";
		if (++it != playerList.end())
			file << '\n';

		--it;
	}
}

void	saveHistory(Tournament* tournament) {
	std::string		filePathName;
	std::fstream	file;

	filePathName = getPathOfFile();
	file.open(getPathOfFile(), std::fstream::out);
	if (!file.is_open())
		return printMessage("Impossible de créer le fichier " + filePathName + ".", ERROR);

	savePlayers(file, tournament->getPlayersList());
	saveTournament(file, tournament->getNumberOfCourts());
	saveMatchesInProgress(file, tournament->getMatchsInProgress());
	savePlayersHistory(file, tournament->getPlayersList());
	printMessage("Les logs du tournoi sont enregistrés dans le fichier suivant: " + filePathName + ".\n");
}
