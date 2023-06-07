#include <iostream>
#include <cstdlib>
#include <exception>
#include <unistd.h>
#include "utils.hpp"
#include "Tournament.hpp"

Tournament::Tournament() {
	this->_commands["MATCH"] = MATCH;
	this->_commands["INFOS"] = INFOS;
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

void	Tournament::savePlayers() {
	std::string	buffer;

	// Enregistrer tous les joueurs qui participeront au match
	if (this->getNumberOfPlayers() == 0) {
		printMessage("Entrez le nom des joueurs participant au tournoi");
		printMessage("Si vous souhaitez enlever un participant retapez son nom avec le signe \'-\' devant celui-ci.");
		printMessage("Une fois terminé, tapez \"FIN\".");
	}
	while (std::getline(std::cin, buffer)) {
		if (buffer.empty() || buffer.size() == 0 || isStringEmpty(buffer))
			continue ;

		if (buffer.compare("FIN") == 0)
			break ;

		if (buffer[0] == '-') {
			this->removePlayer(buffer.c_str() + 1);
		}
		else
			this->addPlayer(buffer);
	}
	std::cout << std::endl;
	if (this->getNumberOfPlayers() <= 1) {
		printMessage("Pas assez de joueurs inscrits pour lancer le tournoi.", ERROR);
		exit(EXIT_FAILURE);
	}
	this->showPlayers();

	std::cout << "Y-a t'il une erreur dans la liste? (O:oui/N:non)" << std::endl;
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

static bool	isStringNumeric(std::string buffer) {
	std::string::iterator	it;

	for (it = buffer.begin(); it != buffer.end(); it++)
		if (!isdigit(*it))
			return false;

	return true;
}

void	Tournament::askCourtsNumber() {
	std::string	buffer;

	std::cout << std::endl;
	printMessage("Combien de terrains sont disponibles?");
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
	printMessage("Souhaitez vous faire une modification sur le nombre de terrains? (O:oui/N:non)");
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
	for (it = this->_playersList.begin(); it != this->_playersList.end(); it++)
		if ((*it).second->getStatus() != INGAME)
			this->addPlayerToWaitingQueue((*it).second);
}

void	Tournament::managment() {
	std::string												buffer;
	std::map<std::string, void (*)(Tournament*)>::iterator	it;

	printMessage("\nPour gérer la suite du tournoi, plusieurs commandes sont à votre disposition: ");
	for (it = this->_commands.begin(); it != this->_commands.end(); it++)
		printMessage("\t" + (*it).first);

	std::cout << std::endl;
	while (std::getline(std::cin, buffer)) {
		try {
			this->_commands.at(buffer)(this);
		}
		catch (std::exception &e) {
			printMessage("La commande " + buffer + " n'existe pas!", WARNING);
		}
		printMessage("\nPour gérer le tournoi, plusieurs commandes sont à votre disposition: ");
		for (it = this->_commands.begin(); it != this->_commands.end(); it++)
			printMessage("\t" + (*it).first);
	}
}

void	Tournament::addPlayer(const std::string name) {
	if (this->_playersList.find(name) != this->_playersList.end()) {
		printMessage("Le joueur " + name + " existe déjà.", ERROR);
		return ;
	}
	this->_playersList.insert(std::make_pair(name, new Player(name)));
}

void	Tournament::removePlayer(const std::string name) {
	if (this->_playersList.find(name) == this->_playersList.end()) {
		printMessage("Le joueur " + name + " n'existe pas.", WARNING);
		return ;
	}
	delete this->_playersList[name];
	this->_playersList.erase(name);
}

Player*	Tournament::findPlayer(const std::string name) {
	std::map<const std::string, Player*>::iterator	it;

	it = this->_playersList.find(name);
	if (it == this->_playersList.end())
		return NULL;

	return (*it).second;
}

void	Tournament::showPlayers() {
	std::map<const std::string, Player*>::iterator	it;

	std::cout << "\nListe des joueurs: \n";
	for (it = this->_playersList.begin(); it != this->_playersList.end(); it++)
		std::cout << "- " << (*it).first << '\n';

	std::cout << "Nombre total de joueurs: " << this->getNumberOfPlayers() << '\n' << std::endl;
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

void	Tournament::showMatchs() {
	std::vector< std::pair<Player*, Player*> >::iterator	it;
	std::vector<Player*>::iterator							itQueue;

	std::cout << "\n----------------------------------------\n";
	if (this->_matchsInProgress.size() == 0)
		std::cout << "Aucun match en cours.\n";
	else {
		std::cout << "Liste des matchs en cours:\n";
		for (it = this->_matchsInProgress.begin(); it != this->_matchsInProgress.end(); it++) {
			std::cout << '\t' << (*it).first->getName() << " contre " << (*it).second->getName() << '\n';
		}
	}
	std::cout << std::endl;
	if (this->_waitingQueue.size() == 0)
		std::cout << "Aucun joueur en attente de match.\n";
	else {
		std::cout << "Liste des joueurs en attente:\n";
		for (itQueue = this->_waitingQueue.begin(); itQueue != this->_waitingQueue.end(); itQueue++)
			std::cout << "\t- " << (*itQueue)->getName() << '\n';
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

	if (player->getStatus() != WAITING)
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

unsigned int	Tournament::getNumberOfPlayers() const {
	return this->_playersList.size();
}

unsigned int	Tournament::getNumberOfWaitingPlayers() const {
	return this->_waitingQueue.size();
}

std::map<const std::string, Player*>&	Tournament::getPlayersList() {
	return this->_playersList;
}
