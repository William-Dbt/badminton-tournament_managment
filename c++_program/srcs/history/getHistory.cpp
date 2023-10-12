#include <iostream>
#include <fstream>
#include <unistd.h>
#include "utils.hpp"
#include "Tournament.hpp"

static bool	isIgnoreLine(std::string& buffer) {
	if (buffer.compare("{") == 0)
		return true;

	if (buffer[0] == '/' && buffer[1] == '/')
		return true;

	if (isStringEmpty(buffer))
		return true;

	return false;
}

static std::string&	removeStringWhitespaces(std::string& buffer) {
	std::string::iterator	it = buffer.begin();

	while (it != buffer.end()) {
		if (std::isspace(*it)) {
			it = buffer.erase(it);
			continue ;
		}
		it++;
	}
	return buffer;
}

static void	getPlayerList(Tournament* tournament, std::fstream& file) {
	std::string	buffer;

	if (tournament->getNumberOfPlayers() > 0) {
		printMessage("Il semblerait que des joueurs soient déjà enregistrés et donc que l'objet \'playerList\' soit dupliqué!", ERROR);
		exit(EXIT_FAILURE);
	}
	while (std::getline(file, buffer)) {
		removeStringWhitespaces(buffer);
		if (buffer.compare("playerList") == 0
			|| isIgnoreLine(buffer))
			continue ;

		if (buffer.compare("}") == 0)
			break ;

		tournament->addPlayer(buffer);
	}
	if (tournament->getNumberOfPlayers() == 0) {
		printMessage("Il semblerait qu'aucun joueur n'ai été trouvé à ajouter au tournoi.", ERROR);
		exit(EXIT_FAILURE);
	}
}

static void	getTournamentInfos(Tournament* tournament, std::fstream& file) {
	std::string	buffer;
	std::string	parameter;

	if (tournament->getNumberOfCourts() != 0) {
		printMessage("Il semblerait que le nombre de terrains soit déjà défini! Attention au duplications d'objets.", ERROR);
		exit(EXIT_FAILURE);
	}
	while (std::getline(file, buffer)) {
		removeStringWhitespaces(buffer);
		if (buffer.compare("tournamentInfos") == 0
			|| isIgnoreLine(buffer))
			continue ;

		if (buffer.compare("}") == 0)
			break ;

		if (buffer.find(':') == std::string::npos) {
			printMessage("Le format des paramètres de cet objet doit être de la forme \'paramètre:valeur\'. Cette ligne sera ignoré.\nligne: " + buffer, WARNING);
			continue ;
		}
		parameter = buffer.substr(0, buffer.find(':'));
		if (parameter.compare("nbCourts") == 0) {
			parameter = buffer.substr(buffer.find(':') + 1);
			if (!isStringNumeric(parameter)) {
				printMessage("Le paramètre \'nbCourts\' doit être un nombre/chiffre!", ERROR);
				exit(EXIT_FAILURE);
			}
			tournament->setCourts(atoi(parameter.c_str()));
		}
		else
			printMessage("Le paramètre " + parameter + " n'est pas connu du programme. Il ne sera pas pris en compte", WARNING);
	}
	if (tournament->getNumberOfCourts() == 0) {
		printMessage("Le nombre de terrains n'a pas été défini dans le fichier de log.", ERROR);
		exit(EXIT_FAILURE);
	}
}

static void	getMatchesInProgress(Tournament* tournament, std::fstream& file) {
	std::string	buffer;
	std::string	playerName[2];
	Player*		players[2];

	if (tournament->getNumberOfPlayingMatches() != 0) {
		printMessage("Il semblerait que des matchs aient déjà été défini. Attention aux doublons d'objets dans les fichiers de logs!", ERROR);
		exit(EXIT_FAILURE);
	}
	while (std::getline(file, buffer)) {
		removeStringWhitespaces(buffer);
		if (buffer.compare("matchesInProgress") == 0
			|| isIgnoreLine(buffer))
			continue ;

		if (buffer.compare("}") == 0)
			break ;

		if (buffer.find(':') == std::string::npos) {
			printMessage("Le format des paramètres de cet objet doit être de la forme \'joueur1:joueur2\'. Cette ligne sera ignoré.\nligne: " + buffer, WARNING);
			continue ;
		}
		playerName[0] = buffer.substr(0, buffer.find(':'));
		playerName[1] = buffer.substr(buffer.find(':') + 1);
		players[0] = tournament->findPlayer(playerName[0], true);
		if (!players[0]) {
			printMessage("Le joueur " + playerName[0] + " n'a pas été trouvé dans la liste des joueurs.", ERROR);
			exit(EXIT_FAILURE);
		}
		players[1] = tournament->findPlayer(playerName[1], true);
		if (!players[1]) {
			printMessage("Le joueur " + playerName[1] + " n'a pas été trouvé dans la liste des joueurs.", ERROR);
			exit(EXIT_FAILURE);
		}
		players[0]->initMatch(tournament, players[1], false);
	}
}

static bool	isPlayerAlreadyRegistred(Player* player, std::vector<Player*>&	registredPlayers) {
	std::vector<Player*>::iterator	it;

	for (it = registredPlayers.begin(); it != registredPlayers.end(); it++)
		if ((*it)->getName() == player->getName())
			return true;

	return false;
}

static void	getPlayersHistory(Tournament* tournament, std::fstream& file, std::string buffer) {
	std::string				parameter;
	Player*					player;
	Player*					opponent;
	std::pair<int, int>		score;
	bool					against = false;
	std::vector<Player*>	registredPlayers;

	do {
		removeStringWhitespaces(buffer);
		if (isIgnoreLine(buffer))
			continue ;

		if (buffer.compare("}") == 0)
			break ;

		player = tournament->findPlayer(buffer, true);
		if (!player) {
			printMessage("Le joueur " + buffer + " n'est pas connu du tournoi.", ERROR);
			exit(EXIT_FAILURE);
		}
		if (isPlayerAlreadyRegistred(player, registredPlayers)) {
			printMessage("Il semblerait que le joueur " + player->getName() + " soit défini plus d'une fois!", ERROR);
			exit(EXIT_FAILURE);
		}
		registredPlayers.push_back(player);
		while (std::getline(file, buffer)) {
			removeStringWhitespaces(buffer);
			if (buffer.find(":") != std::string::npos) {
				if (buffer.substr(0, buffer.find(":")).compare("status") == 0) {
					parameter = buffer.substr(buffer.find(":") + 1);
					if (parameter.compare("auto") == 0)
						continue ;

					if (parameter.compare("stopped") != 0 && parameter.compare("finished") != 0) {
						printMessage("Le status " + parameter + " n'est pas connu!", ERROR);
						exit(EXIT_FAILURE);
					}
					(parameter.compare("stopped") == 0) ? player->setStatus(STOPPED) : player->setStatus(FINISHED);
				}
			}
			if (buffer.compare("scoreHistory") == 0)
				break ;
		}
		while (std::getline(file, buffer)) {
			removeStringWhitespaces(buffer);
			if (buffer.compare("scoreHistory") == 0 
				|| isIgnoreLine(buffer))
				continue ;

			if (buffer.compare("}") == 0)
				break ;

			if (!against && buffer.substr(0, buffer.find(':')).compare("against") == 0) {
				opponent = tournament->findPlayer(buffer.substr(buffer.find(':') + 1), true);
				if (!opponent) {
					printMessage("Le joueur " + buffer.substr(buffer.find(':') + 1) + "n'est pas inscrit au tournoi.", ERROR);
					exit(EXIT_FAILURE);
				}
				player->addToListAlreadyPlayed(opponent);
				opponent->addToListAlreadyPlayed(player);
				against = true;
			}
			else if (against && buffer.substr(0, buffer.find(':')).compare("score") == 0) {
				score.first = atoi(buffer.substr(buffer.find(':') + 1, buffer.find('-')).c_str());
				score.second = atoi(buffer.substr(buffer.find('-') + 1).c_str());
				player->addScoreMatch(opponent->getName(), score);
				against = false;
			}
			else {
				printMessage("Une erreur s'est produite lors de la lecture de l'historique des scores du joueur " + player->getName() + ".", ERROR);
				exit(EXIT_FAILURE);
			}
		}
	} while (std::getline(file, buffer));
}

void	getHistory(Tournament* tournament) {
	std::string		buffer;
	std::fstream	file;

	std::map<const std::string, Player*>::iterator	it;

	printMessage("Indiquez le nom du fichier d'historique que vous souhaitez utiliser: ", -1, false);
	std::getline(std::cin, buffer);
	file.open(buffer, std::fstream::in);
	if (!file.is_open()) {
		printMessage("Une erreur est survenue lors de l'ouverture du fichier " + buffer + "!", ERROR);
		exit(EXIT_FAILURE);
	}
	while (std::getline(file, buffer)) {
		if (isIgnoreLine(buffer))
			continue ;

		removeStringWhitespaces(buffer);
		if (buffer.compare("playerList") == 0)
			getPlayerList(tournament, file);
		else if (buffer.compare("tournamentInfos") == 0)
			getTournamentInfos(tournament, file);
		else if (buffer.compare("matchesInProgress") == 0)
			getMatchesInProgress(tournament, file);
		else
			getPlayersHistory(tournament, file, buffer);
	}
	for (it = tournament->getPlayersList().begin(); it != tournament->getPlayersList().end(); it++)
		if (tournament->isPlayerInWaitingQueue((*it).second) && ((*it).second->getStatus() == STOPPED || (*it).second->getStatus() == FINISHED))
			tournament->removePlayerFromWaitingQueue((*it).second);
}
