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
		std::cout << "Entrez le nom des joueurs participant au tournoi\n";
		std::cout << "Si vous souhaitez enlever un participant retapez son nom avec le signe \'-\' devant celui-ci.\n";
		std::cout << "Une fois terminé, tapez \"FIN\"." << std::endl;
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

	buffer.clear();
	std::cout << "Y-a t'il une erreur dans la liste? (O/N)" << std::endl;
	while (std::getline(std::cin, buffer)) {
		if (buffer.compare("O") == 0) {
			std::cout << "Si vous souhaitez enlever un participant retapez son nom avec le signe \'-\' devant celui-ci.\n";
			std::cout << "Pour en rajouter, tapez juste son nom. Entrez \'FIN\' une fois les modifications terminées." << std::endl;
			return savePlayers();
		}
		else if (buffer.compare("N") == 0)
			break ;
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
