#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <time.h>
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

static void	saveWaitingQueue(std::fstream& file, std::vector<Player*> waitingQueue) {
	std::vector<Player*>::iterator	it;

	file << "waitingQueue\n";
	file << "{\n";
	for (it = waitingQueue.begin(); it != waitingQueue.end(); it++)
		file << '\t' << (*it)->getName() << '\n';

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
		file << "\tname:" << player->getName() << "\n\n";
		file << "\tscoreHistory\n";
		file << "\t{\n";
		scoreHistory = (*it).second->getScoreHistory();
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
	std::fstream	file;

	file.open(getPathOfFile(), std::fstream::out);
	savePlayers(file, tournament->getPlayersList());
	saveTournament(file, tournament->getNumberOfCourts());
	saveMatchesInProgress(file, tournament->getMatchsInProgress());
	saveWaitingQueue(file, tournament->getWaitingQueue());
	savePlayersHistory(file, tournament->getPlayersList());
}
