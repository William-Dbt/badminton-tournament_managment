#include <iostream>
#include <map>
#include "utils.hpp"
#include "Player.hpp"

Player*	g_firstOpponent = NULL;

Player::Player(const std::string name) : _status(-1), _name(name), _partner(NULL) {}

Player::~Player() {}

bool	Player::hasAlreadyPlayAgainstOrWith(Player* player) {
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

	if (this->hasAlreadyPlayAgainstOrWith(player))
		return ;

	this->_listPlayersAlreadyPlayed.push_back(player);
}

void	Player::initMatch(Tournament* tournament, Player* secondPlayer, bool showMessage) {
	if (secondPlayer == NULL)
		return ;

	if (tournament->isPlayerInWaitingQueue(this))
		tournament->removePlayerFromWaitingQueue(this);

	if (tournament->isPlayerInWaitingQueue(secondPlayer))
		tournament->removePlayerFromWaitingQueue(secondPlayer);

	this->_status = INGAME;
	secondPlayer->setStatus(INGAME);
	if (showMessage)
		printMessage("Match trouvé entre " + this->_name + " et " + secondPlayer->getName() + "!");

	tournament->addMatch(this, secondPlayer);
}

void	Player::initDoubleMatch(Tournament* tournament) {
	Player*	fstCouple_first = this;
	Player*	fstCouple_second = this->getPartner();

	Player*	scndCouple_first = g_firstOpponent;
	Player*	scndCouple_second = g_firstOpponent->getPartner();

	std::string	buffer;

	if (tournament->isPlayerInWaitingQueue(fstCouple_first))
		tournament->removePlayerFromWaitingQueue(fstCouple_first);

	if (tournament->isPlayerInWaitingQueue(fstCouple_second))
		tournament->removePlayerFromWaitingQueue(fstCouple_second);

	if (tournament->isPlayerInWaitingQueue(scndCouple_first))
		tournament->removePlayerFromWaitingQueue(scndCouple_first);

	if (tournament->isPlayerInWaitingQueue(scndCouple_second))
		tournament->removePlayerFromWaitingQueue(scndCouple_second);

	this->_status = INGAME;
	fstCouple_second->setStatus(INGAME);
	scndCouple_first->setStatus(INGAME);
	scndCouple_second->setStatus(INGAME);
	std::cout << CBOLD << "Match trouvé entre ";
	std::cout << CYELLOW << this->_name << " et " << fstCouple_second->getName();
	std::cout << CRESET << CBOLD << " contre " << CYELLOW << scndCouple_first->getName() << " et " << scndCouple_second->getName();
	std::cout << CRESET << CBOLD << ".\n" << CRESET;

	tournament->addDoubleMatch(fstCouple_first, fstCouple_second, scndCouple_first, scndCouple_second);
	g_firstOpponent = NULL;
}

void	Player::findMatch(Tournament* tournament) {
	std::vector<Player*>::iterator	it;

	if (tournament->isCourtsFull())
		return ;

	if (tournament->getNumberOfWaitingPlayers() <= 1)
		return ;

	for (it = tournament->getWaitingQueue().begin(); it != tournament->getWaitingQueue().end(); it++) {
		if (tournament->getMode() == ALL_DOUBLE && tournament->getNumberOfWaitingPlayers() < 4)
			return ;

		if ((*it) == this)
			continue ;

		if (this->hasAlreadyPlayAgainstOrWith((*it)))
			continue ;

		if (tournament->getMode() == ALL_SIMPLE)
			return this->initMatch(tournament, (*it));
		else {
			if (!this->findDoubleMatch(tournament, (*it)))
				continue ;
			else
				return this->initDoubleMatch(tournament);
		}
	}
}

bool	Player::findDoubleMatch(Tournament* tournament, Player* exceptPlyr) {
	Player*							player = NULL;
	std::vector<Player*>::iterator	it;

	for (it = tournament->getWaitingQueue().begin(); it != tournament->getWaitingQueue().end(); it++) {
		if ((*it) == this)
			continue ;

		if ((*it) == exceptPlyr)
			continue ;

		if (player != NULL && (*it) == player)
			continue ;

		if (player == NULL) {
			player = (*it);
			continue ;
		}
		if (!player->hasAlreadyPlayAgainstOrWith((*it))) {
			this->setPartner(exceptPlyr);
			exceptPlyr->setPartner(this);
			player->setPartner((*it));
			(*it)->setPartner(player);
			g_firstOpponent = player;
			return true;
		}
	}
	return false;
}

void	Player::addScoreMatch(std::string opponent, std::pair<int, int> score) {
	this->_scoreHistory.push_back(std::make_pair(opponent, score));
}

void	Player::addDoubleScoreMatch(Player* opponent1, Player* opponent2, std::pair<int, int> score) {
	this->_doubleScoreHistory.push_back(std::make_pair(std::make_pair(opponent1, opponent2), score));
}

void	Player::showScoreHistory() {
	std::pair<int, int>														score;
	std::vector< std::pair<std::string, std::pair<int, int> > >::iterator	it;

	printMessage("----------------------------------------");
	if (this->getNbOfMatches() == 0)
		std::cout << "Le joueur " << this->_name << " n'a effectué aucun match." << std::endl;
	else {
		printMessage("Historique des matchs de " + this->_name + "\n");
		for (it = this->_scoreHistory.begin(); it != this->_scoreHistory.end(); it++) {
			score = (*it).second;
			if (score.first > score.second)
				std::cout << "GAGNÉ ";
			else
				std::cout << "PERDU ";

			std::cout << "contre " << (*it).first << '.';
			std::cout << " (" << score.first << " à " << score.second << ")\n" << std::endl;
		}
		std::cout << "Nombre de matchs effectué: " << this->getNbOfMatches() << '.' << std::endl;
		std::cout << "Score total accumulé: " << this->getTotalScore() << '.' << std::endl;
	}
	printMessage("----------------------------------------");
}

unsigned int	Player::getTotalScore(int limitOfMatches) {
	int				i = 0;
	unsigned int	totalScore = 0;

	std::pair<int, int>														score;
	std::vector< std::pair<std::string, std::pair<int, int> > >::iterator	it;

	if (this->getNbOfMatches() == 0)
		return 0;

	for (it = this->_scoreHistory.begin(); it != this->_scoreHistory.end(); it++) {
		if (limitOfMatches && i++ == limitOfMatches)
			break ;

		score = (*it).second;
		totalScore += score.first;
	}
	return totalScore;
}

int	Player::getNbOfMatches(Tournament* tournament, bool takeStoppedPlayers) {
	int		nbOfMatches = 0;
	Player*	opponent;

	if (tournament->getMode() == ALL_DOUBLE)
		return this->_doubleScoreHistory.size();

	if (takeStoppedPlayers)
		return this->_scoreHistory.size();

	if (!tournament)
		return 0;

	std::vector< std::pair<std::string, std::pair<int, int> > >::iterator	it;

	for (it = this->_scoreHistory.begin(); it != this->_scoreHistory.end(); it++) {
		opponent = tournament->findPlayer((*it).first, true);
		if (opponent != NULL && opponent->getStatus() != STOPPED)
			nbOfMatches++;
	}
	return nbOfMatches;
}

void    Player::setName(const std::string& name) {
    this->_name = name;
}

void	Player::setStatus(const int status) {
	this->_status = status;
}

void	Player::setPartner(Player* player) {
	this->_partner = player;
}

int	Player::getStatus() const {
	return this->_status;
}

std::string	Player::getName() const {
	return this->_name;
}

std::vector< std::pair<std::string, std::pair<int, int> > >&	Player::getScoreHistory() {
	return this->_scoreHistory;
}

Player*	Player::getPartner() {
	return this->_partner;
}

bool	Player::operator==(const Player &ref) const {
	return (this->_name == ref._name);
}
