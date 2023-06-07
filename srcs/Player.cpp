#include <iostream>
#include <map>
#include "utils.hpp"
#include "Player.hpp"

Player::Player(const std::string name) : _status(-1), _name(name) {}

Player::~Player() {}

bool	Player::hasAlreadyPlayAgainst(Player* player) {
	std::vector<Player*>::iterator	it;

	if (this->_listPlayersAlreadyPlayed.size() == 0)
		return false;

	for (it = this->_listPlayersAlreadyPlayed.begin(); it != this->_listPlayersAlreadyPlayed.end(); it++)
		if (player == (*it))
			return true;

	return false;
}

void	Player::addToListAlreadyPlayed(Player* player) {
	if (player == NULL)
		return ;

	if (this->hasAlreadyPlayAgainst(player))
		return ;

	this->_listPlayersAlreadyPlayed.push_back(player);
}

void	Player::initMatch(Tournament* tournament, Player* secondPlayer) {
	this->_status = INGAME;
	secondPlayer->setStatus(INGAME);
	printMessage("Match trouvé entre " + this->_name + " et " + secondPlayer->getName() + "!");
	tournament->addMatch(this, secondPlayer);
}

void	Player::findMatch(Tournament* tournament) {
	std::map<const std::string, Player*>&			playersList = tournament->getPlayersList();
	std::map<const std::string, Player*>::iterator	it;

	if (tournament->isCourtsFull())
		return ;

	for (it = playersList.begin(); it != playersList.end(); it++) {
		if (this->_status != WAITING && this->_status != -1)
			break ;

		if ((*it).second == this)
			continue ;

		if ((*it).second->getStatus() != WAITING && (*it).second->getStatus() != -1)
			continue ;

		if (hasAlreadyPlayAgainst((*it).second))
			continue ;

		this->initMatch(tournament, (*it).second);
	}
}

void	Player::addScoreMatch(std::string opponent, std::pair<int, int> score) {
	this->_scoreHistory.push_back(std::make_pair(opponent, score));
}

void    Player::setName(const std::string& name) {
    this->_name = name;
}

void	Player::setStatus(const int status) {
	this->_status = status;
}

int	Player::getStatus() const {
	return this->_status;
}

std::string	Player::getName() const {
	return this->_name;
}

bool	Player::operator==(const Player &ref) const {
	return (this->_name == ref._name);
}
