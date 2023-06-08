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
	if (secondPlayer == NULL)
		return ;

	if (tournament->isPlayerInWaitingQueue(this))
		tournament->removePlayerFromWaitingQueue(this);

	if (tournament->isPlayerInWaitingQueue(secondPlayer))
		tournament->removePlayerFromWaitingQueue(secondPlayer);

	this->_status = INGAME;
	secondPlayer->setStatus(INGAME);
	printMessage("Match trouvé entre " + this->_name + " et " + secondPlayer->getName() + "!");
	tournament->addMatch(this, secondPlayer);
}

void	Player::findMatch(Tournament* tournament) {
	std::vector<Player*>::iterator	it;

	if (tournament->isCourtsFull())
		return ;

	if (tournament->getNumberOfWaitingPlayers() <= 1)
		return ;

	for (it = tournament->getWaitingQueue().begin(); it != tournament->getWaitingQueue().end(); it++) {
		if ((*it) == this)
			continue ;

		if (this->hasAlreadyPlayAgainst((*it)))
			continue ;

		return initMatch(tournament, (*it));
	}
}

void	Player::addScoreMatch(std::string opponent, std::pair<int, int> score) {
	this->_scoreHistory.push_back(std::make_pair(opponent, score));
}

void	Player::showScoreHistory() {
	std::pair<int, int>														score;
	std::vector< std::pair<std::string, std::pair<int, int> > >::iterator	it;

	printMessage("########################################");
	printMessage("Historique des matchs de " + this->_name + "\n");
	for (it = this->_scoreHistory.begin(); it != this->_scoreHistory.end(); it++) {
		score = (*it).second;
		if (score.first > score.second)
			std::cout << "GAGNÉ ";
		else
			std::cout << "PERDU ";

		std::cout << "contre " << (*it).first << '.';
		std::cout << " (" << score.first << " à " << score.second << ')' << std::endl;
	}
	printMessage("########################################");
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

std::vector< std::pair<std::string, std::pair<int, int> > >	Player::getScoreHistory() const {
	return this->_scoreHistory;
}

bool	Player::operator==(const Player &ref) const {
	return (this->_name == ref._name);
}
