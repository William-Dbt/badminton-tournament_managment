#ifndef __TOURNAMENT_HPP__
# define __TOURNAMENT_HPP__

# define VERSION 1.0

# include <string>
# include <map>
# include <vector>
# include "Player.hpp"

class   Tournament {
	public:
		Tournament();
		~Tournament();

		bool	isCourtsFull();

		void	savePlayers();
		void	askCourtsNumber();
		void	initFirstMatchs();
		void	managment();

		void	addPlayer(const std::string name);
		void	removePlayer(const std::string name);
		Player*	findPlayer(const std::string name);
		void	showPlayers();

		void						addMatch(Player* player1, Player* player2);
		void						removeMatch(std::pair<Player*, Player*> match);
		std::pair<Player*, Player*>	findMatchByPlayer(Player* player);
		void						showMatchs(bool showMatchs = true, bool showWaitingList = true);

		bool	isPlayerInWaitingQueue(Player* player);
		void	addPlayerToWaitingQueue(Player* player);
		void	removePlayerFromWaitingQueue(Player* player);

		unsigned int								getNumberOfPlayingMatches() const;
		unsigned int								getNumberOfPlayers() const;
		unsigned int								getNumberOfWaitingPlayers() const;
		unsigned int								getNumberOfCourts() const;
		std::vector< std::pair<Player*, Player*> >&	getMatchsInProgress();
		std::vector<Player*>&						getWaitingQueue();
		std::map<const std::string, Player*>&		getPlayersList();

	private:
		struct s_infos {
			unsigned int	nbCourts;
		};

		struct s_infos	_infos;

		std::map<const std::string, Player*>			_playersList;
		std::vector< std::pair<Player*, Player*> >		_matchsInProgress;
		std::vector<Player*>							_waitingQueue;
		std::map<std::string, void (*)(Tournament*)>	_commands;
};

void	MATCH(Tournament* tournament);
void	INFOS(Tournament* tournament);
void	HISTORY(Tournament* tournament);
void	FINISH(Tournament* tournament);

#endif
