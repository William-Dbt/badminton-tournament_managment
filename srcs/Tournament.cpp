#include <iostream>
#include <cstdlib>
#include "utils.hpp"
#include "Tournament.hpp"

Tournament::Tournament() {}

Tournament::~Tournament() {
	std::map<const std::string, Player*>::iterator	it;

	for (it = this->_playersList.begin(); it != this->_playersList.end(); it++)
		delete (*it).second;
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
			printMessage("Vous ne pouvez entrer que des nombres!", ERROR);
			continue ;
		}
		else {
			this->_infos.nbCourts = atoi(buffer.c_str());
			break ;
		}
	}
	std::cout << std::endl << "Nombre de terrains: " << this->_infos.nbCourts << '.' << std::endl;
	printMessage("Souhaitez vous faire une modification sur le nombre de terrains? O:oui/N:non");
	while (std::getline(std::cin, buffer)) {
		if (isOui(buffer))
			return this->askCourtsNumber();
		else if (isNon(buffer))
			break ;
		else
			printMessage("Vous pouvez répondre uniquement avec oui (O/OUI/oui) ou non (N/NON/non).", WARNING);
	}
}

void	Tournament::run() {
	std::map<const std::string, Player*>::iterator	it;

	for (it = this->_playersList.begin(); it != this->_playersList.end(); it++) {
		if ((*it).second->getStatus() != WAITING)
			continue ;

		std::cout << "=> " << (*it).first << std::endl;
		(*it).second->findMatch(this);
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
		printMessage("Le joueur " + name + " n'existe pas.", ERROR);
		return ;
	}
	delete this->_playersList[name];
	this->_playersList.erase(name);
}

void	Tournament::showPlayers() {
	std::map<const std::string, Player*>::iterator	it;

	std::cout << "\nListe des joueurs: \n";
	for (it = this->_playersList.begin(); it != this->_playersList.end(); it++)
		std::cout << "- " << (*it).first << '\n';

	std::cout << "Nombre total de joueurs: " << this->getNumberOfPlayers() << '\n' << std::endl;
}

unsigned int	Tournament::getNumberOfPlayers() const {
	return this->_playersList.size();
}

std::map<const std::string, Player*>&	Tournament::getPlayersList() {
	return this->_playersList;
}
