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

		void	savePlayers();
		void	askCourtsNumber();

		void	initFirstMatchs();

		void	addPlayer(const std::string name);
		void	removePlayer(const std::string name);
		void	showPlayers();

		void	addMatch(Player* player1, Player* player2);
		bool	isPlayerInWaitingQueue(Player* player);
		void	addPlayerToWaitingQueue(Player* player);
		void	removePlayerFromWaitingQueue(Player* player);
		void	showMatchs();

		unsigned int	getNumberOfPlayers() const;

		std::map<const std::string, Player*>&	getPlayersList();

	private:
		struct s_infos {
			unsigned int	nbCourts;
		};

		struct s_infos	_infos;

		std::map<const std::string, Player*>		_playersList;
		std::vector< std::pair<Player*, Player*> >	_matchsInProgress;
		std::vector<Player*>						_waitingQueue;
};

#endif
