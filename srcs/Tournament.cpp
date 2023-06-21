#include <iostream>
#include <cstdlib>
#include <exception>
#include <unistd.h>
#include "utils.hpp"
#include "Tournament.hpp"

void	getHistory(Tournament* tournament);
void	startMatch(Tournament* tournament);

bool	g_bFinishTournament = false;

Tournament::Tournament() {
	this->_commands["MATCH"] = MATCH;
	this->_commands["INFOS"] = INFOS;
	this->_commands["PLAYER"] = PLAYER;
	this->_commands["HISTORY"] = HISTORY;
	this->_commands["FINISH"] = FINISH;
}

Tournament::~Tournament() {
	std::map<const std::string, Player*>::iterator	it;

	for (it = this->_playersList.begin(); it != this->_playersList.end(); it++)
		delete (*it).second;
}

bool	Tournament::isCourtsFull() {
	if (this->_matchsInProgress.size() >= this->_infos.nbCourts)
		return true;

	return false;
}

bool	Tournament::startWithHistory() {
	std::string	buffer;

	printMessage("Voulez-vous commencer le tournoi à partir d'un historique existant? (O:oui/N:non) ", -1, false);
	std::getline(std::cin, buffer);
	if (isOui(buffer))
		return true;
	else if (isNon(buffer))
		return false;

	printMessage("Impossible de comprendre votre choix, commecement d'un nouveau tournoi!\n", WARNING);
	return false;	
}

bool	isValidName(std::string name) {
	std::string::iterator	it;

	if (name.empty())
		return false;

	if (name.size() == 0)
		return false;

	if (isStringEmpty(name))
		return false;

	for (it = name.begin(); it != name.end(); it++) {
		if (it == name.begin() && (*it) == '-')
			continue ;

		if (!isalpha((*it)) && (*it) != '.') {
			printMessage("Le nom du joueur ne peut contenir que des lettres et le caractère \'.\'.", WARNING);
			return false;
		}
	}
	return true;
}

void	Tournament::savePlayers() {
	std::string	buffer;

	if (this->getNumberOfPlayers() == 0) {
		printMessage("\nEntrez le nom des joueurs participant au tournoi");
		printMessage("Si vous souhaitez enlever un participant retapez son nom avec le signe \'-\' devant celui-ci.");
		printMessage("Une fois terminé, tapez \"FIN\".");
	}
	while (std::getline(std::cin, buffer)) {
		if (!isValidName(buffer))
			continue ;

		if (buffer.compare("FIN") == 0)
			break ;

		if (buffer[0] == '-')
			this->removePlayer(buffer.c_str() + 1, false);
		else
			this->addPlayer(buffer);
	}
	if (this->getNumberOfPlayers() <= 1) {
		printMessage("Pas assez de joueurs inscrits pour lancer le tournoi.", ERROR);
		exit(EXIT_FAILURE);
	}
	this->showPlayers();

	printMessage("Y-a t'il une erreur dans la liste? (O:oui/N:non) ", -1, false);
	while (std::getline(std::cin, buffer)) {
		if (isOui(buffer)) {
			printMessage("Si vous souhaitez enlever un participant retapez son nom avec le signe \'-\' devant celui-ci.");
			printMessage("Pour en rajouter, tapez juste son nom. Entrez \'FIN\' une fois les modifications terminées.");
			return savePlayers();
		}
		else if (isNon(buffer))
			break ;
		else
			printMessage("Vous pouvez répondre uniquement avec oui (O/OUI/oui) ou non (N/NON/non).", WARNING);
	}
}

void	Tournament::askCourtsNumber() {
	std::string	buffer;

	std::cout << std::endl;
	printMessage("Combien de terrains sont disponibles? ", -1, false);
	while (std::getline(std::cin, buffer)) {
		if (!isStringNumeric(buffer)) {
			printMessage("Vous ne pouvez entrer que des nombres!", WARNING);
			continue ;
		}
		else {
			this->_infos.nbCourts = atoi(buffer.c_str());
			break ;
		}
	}
	std::cout << std::endl << "Nombre de terrains: " << this->_infos.nbCourts << '.' << std::endl;
	printMessage("Souhaitez vous faire une modification sur le nombre de terrains? (O:oui/N:non) ", -1, false);
	while (std::getline(std::cin, buffer)) {
		if (isOui(buffer))
			return this->askCourtsNumber();
		else if (isNon(buffer))
			break ;
		else
			printMessage("Vous pouvez répondre uniquement avec oui (O/OUI/oui) ou non (N/NON/non).", WARNING);
	}
	if (this->_infos.nbCourts < 1) {
		printMessage("Pas assez de terrains disponible pour lancer le tournoi.", ERROR);
		exit(EXIT_FAILURE);
	}
}

void	Tournament::initFirstMatchs() {
	std::map<const std::string, Player*>::iterator	it;

	for (it = this->_playersList.begin(); it != this->_playersList.end(); it++) {
		if ((*it).second->getStatus() != WAITING && (*it).second->getStatus() != -1)
			continue ;

		(*it).second->findMatch(this);
	}
}

void	Tournament::managment() {
	std::string												buffer;
	std::map<std::string, void (*)(Tournament*)>::iterator	it;

	printMessage("\nPour gérer la suite du tournoi, plusieurs commandes sont à votre disposition:");
	for (it = this->_commands.begin(); it != this->_commands.end(); it++)
		printMessage("\t" + (*it).first);

	while (!g_bFinishTournament && std::getline(std::cin, buffer)) {
		if (isStringEmpty(buffer))
			continue ;

		try {
			this->_commands.at(buffer)(this);
		}
		catch (std::exception &e) {
			printMessage("La commande \'" + buffer + "\' n'existe pas!", WARNING);
		}
		if (g_bFinishTournament)
			break ;

		printMessage("\nPour gérer le tournoi, plusieurs commandes sont à votre disposition: ");
		for (it = this->_commands.begin(); it !=  this->_commands.end(); it++)
			printMessage("\t" + (*it).first);
	}
}

void	Tournament::addPlayer(const std::string name) {
	Player*	player;

	if (this->_playersList.find(name) != this->_playersList.end())
		return (printMessage("Le joueur " + name + " existe déjà.", ERROR));

	player = new Player(name);
	this->_playersList.insert(std::make_pair(name, player));
	this->addPlayerToWaitingQueue(player);
}

void	Tournament::removePlayer(const std::string name, const bool isTournamentStarted) {
	bool		isMatchStopped = false;
	std::string	buffer;
	Player*		player;
	std::pair<Player*, Player*>	match;

	std::map<const std::string, Player*>::iterator	itPlayer;

	itPlayer = this->_playersList.find(name);
	if (itPlayer == this->_playersList.end())
		return (printMessage("Le joueur \'" + name + "\' n'existe pas.", WARNING));

	player = (*itPlayer).second;
	if (player->getStatus() == FINISHED || player->getStatus() == STOPPED)
		return printMessage("Le joueur ne participe plus au tournoi, impossible de le supprimer.", ERROR);

	if (isTournamentStarted) {
		printMessage("Êtes-vous sûr de vouloir supprimer le joueur " + name + "? (O:oui/N:non)");
		printMessage("Si le joueur est dans un match, ce dernier ne sera pas pris en compte pour les deux joueurs.");
		std::getline(std::cin, buffer);
		if (!isOui(buffer))
			return ;
	}
	match = this->findMatchByPlayer(player);
	if (match.first != NULL) {
		printMessage("Suppression du match entre " + match.first->getName() + " et " + match.second->getName() + ".");
		this->removeMatch(match);
		if (player != match.first)
			this->addPlayerToWaitingQueue(match.first);
		else
			this->addPlayerToWaitingQueue(match.second);

		isMatchStopped = true;
	}
	if (this->isPlayerInWaitingQueue(player))
		this->removePlayerFromWaitingQueue(player);

	player->setStatus(STOPPED);
	if (!isTournamentStarted || player->getScoreHistory().empty()) {
		delete this->_playersList[name];
		this->_playersList.erase(name);
	}
	printMessage("Le joueur " + name + " a été enlevé du tournoi!");
	if (this->getNumberOfPlayers() <= 1) {
		g_bFinishTournament = true;
		return printMessage("\nIl n'y a plus assez de joueur en liste pour continuer le tournoi!");
	}
	if (isMatchStopped) {
		printMessage("\nUn match en cours a été stoppé, esseyons d'en lancer un autre!");
		startMatch(this);
	}
}

Player*	Tournament::findPlayer(const std::string name, const bool evenIfNotParticipate) {
	std::map<const std::string, Player*>::iterator	it;

	it = this->_playersList.find(name);
	if (it == this->_playersList.end())
		return NULL;

	if (!evenIfNotParticipate)
		if ((*it).second->getStatus() == STOPPED || (*it).second->getStatus() == FINISHED)
			return NULL;

	return (*it).second;
}

void	Tournament::showPlayers(const bool printNumberOfPlayers, const bool printNotParticipatingPlayers) {
	std::map<const std::string, Player*>::iterator	it;

	std::cout << "\nListe des joueurs: \n";
	for (it = this->_playersList.begin(); it != this->_playersList.end(); it++) {
		if (!printNotParticipatingPlayers && ((*it).second->getStatus() == STOPPED || (*it).second->getStatus() == FINISHED))
			continue ;

		std::cout << "- " << (*it).first;
		if ((*it).second->getStatus() == FINISHED)
			std::cout << " (a fini tous ses matchs)";
		else if ((*it).second->getStatus() == STOPPED)
			std::cout << " (ne participe plus au tournoi)";

		std::cout << '\n';
	}
	if (printNumberOfPlayers)
		std::cout << "Nombre total de joueurs: " << this->getNumberOfPlayers() << '\n';

	std::cout << std::endl;
}

void	Tournament::addMatch(Player* player1, Player* player2) {
	if (player1 == NULL || player2 == NULL)
		return ;

	this->_matchsInProgress.push_back(std::make_pair(player1, player2));
}

void	Tournament::removeMatch(std::pair<Player*, Player*> match) {
	std::vector< std::pair<Player*, Player*> >::iterator	it;

	for (it = this->_matchsInProgress.begin(); it != this->_matchsInProgress.end(); it++)
		if ((*it) == match)
			break ;

	this->_matchsInProgress.erase(it);
}

std::pair<Player*, Player*>	Tournament::findMatchByPlayer(Player* player) {
	std::vector< std::pair<Player*, Player*> >::iterator	it;

	if (player == NULL)
		return std::pair<Player*, Player*>(NULL, NULL);

	if (this->findPlayer(player->getName()) == NULL)
		return std::pair<Player*, Player*>(NULL, NULL);

	for (it = this->_matchsInProgress.begin(); it != this->_matchsInProgress.end(); it++)
		if ((*it).first->getName() == player->getName() || (*it).second->getName() == player->getName())
			return (*it);

	return std::pair<Player*, Player*>(NULL, NULL);
}

void	Tournament::showMatchs(bool showMatchs, bool showWaitingList) {
	std::vector< std::pair<Player*, Player*> >::iterator	it;
	std::vector<Player*>::iterator							itQueue;

	std::cout << "\n----------------------------------------\n";
	if (showMatchs) {
		if (this->_matchsInProgress.size() == 0)
			std::cout << "Aucun match en cours.\n";
		else {
			std::cout << "Liste des matchs en cours:\n";
			for (it = this->_matchsInProgress.begin(); it != this->_matchsInProgress.end(); it++) {
				std::cout << '\t' << (*it).first->getName() << " contre " << (*it).second->getName() << '\n';
			}
		}
	}
	if (showWaitingList) {
		if (showMatchs)
			std::cout << std::endl;

		if (this->_waitingQueue.size() == 0)
			std::cout << "Aucun joueur en attente de match.\n";
		else {
			std::cout << "Liste des joueurs en attente:\n";
			for (itQueue = this->_waitingQueue.begin(); itQueue != this->_waitingQueue.end(); itQueue++)
				std::cout << "\t- " << (*itQueue)->getName() << '\n';
		}
	}
	std::cout << "----------------------------------------" << std::endl;
}

bool	Tournament::isPlayerInWaitingQueue(Player* player) {
	std::vector<Player*>::iterator	it;

	if (player == NULL)
		return false;

	for (it = this->_waitingQueue.begin(); it != this->_waitingQueue.end(); it++)
		if ((*it) == player)
			return true;

	return false;
}

void	Tournament::addPlayerToWaitingQueue(Player* player) {
	if (player == NULL)
		return ;

	if (player->getStatus() == WAITING)
		return ;

	this->_waitingQueue.push_back(player);
	player->setStatus(WAITING);
}

void	Tournament::removePlayerFromWaitingQueue(Player* player) {
	std::vector<Player*>::iterator	it;

	if (player == NULL)
		return ;

	if (player->getStatus() == INGAME)
		return ;

	if (this->_waitingQueue.size() == 0)
		return ;

	if (!this->isPlayerInWaitingQueue(player))
		return ;

	for (it = this->_waitingQueue.begin(); it != this->_waitingQueue.end(); it++)
		if ((*it) == player)
			break ;

	this->_waitingQueue.erase(it);
}

void	Tournament::setCourts(unsigned int courts) {
	this->_infos.nbCourts = courts;
}

unsigned int	Tournament::getNumberOfPlayingMatches() const {
	return this->_matchsInProgress.size();
}

unsigned int	Tournament::getNumberOfPlayers() {
	unsigned int	i = 0;

	std::map<const std::string, Player*>::iterator	it;

	for (it = this->_playersList.begin(); it != this->_playersList.end(); it++)
		if ((*it).second->getStatus() != STOPPED && (*it).second->getStatus() != FINISHED)
			i++;

	return i;
}

unsigned int	Tournament::getNumberOfWaitingPlayers() const {
	return this->_waitingQueue.size();
}

unsigned int	Tournament::getNumberOfCourts() const {
	return this->_infos.nbCourts;
}

std::vector< std::pair<Player*, Player*> >&	Tournament::getMatchsInProgress() {
	return this->_matchsInProgress;
}

std::vector<Player*>&	Tournament::getWaitingQueue() {
	return this->_waitingQueue;
}

std::map<const std::string, Player*>&	Tournament::getPlayersList() {
	return this->_playersList;
}
