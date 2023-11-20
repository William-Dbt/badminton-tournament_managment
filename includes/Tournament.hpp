#ifndef __TOURNAMENT_HPP__
# define __TOURNAMENT_HPP__

# define VERSION 1.4

# include <string>
# include <map>
# include <vector>
# include "Player.hpp"

typedef std::pair< std::pair<Player*, Player*>, std::pair<Player*, Player*> >	playersMatchDouble;
typedef std::vector< playersMatchDouble >										vectorMatchsDouble;

enum	e_mode {
	ALL_SIMPLE = 0,
	ALL_DOUBLE
};

class   Tournament {
	public:
		Tournament();
		~Tournament();

		bool	isCourtsFull();

		bool	startWithHistory();
		void	savePlayers();
		void	askCourtsNumber();
		void	initFirstMatchs();
		void	managment();

		void	addPlayer(const std::string name);
		void	removePlayer(const std::string name, const bool isTournamentStarted = true);
		Player*	findPlayer(const std::string name, const bool evenIfNotParticipate = false);
		void	showPlayers(const bool printNumberOfPlayers = true, const bool printNotParticipatingPlayers = false);

		void						addMatch(Player* player1, Player* player2);
		void						removeMatch(std::pair<Player*, Player*> match);
		void						removeDoubleMatch(playersMatchDouble match);
		std::pair<Player*, Player*>	findMatchByPlayer(Player* player);
		playersMatchDouble			findDoubleMatchByPlayer(Player* player);
		void						showMatchs(bool showMatchs = true, bool showWaitingList = true);

		void	addDoubleMatch(Player* player1, Player* player2, Player* player3, Player* player4);

		bool	isPlayerInWaitingQueue(Player* player);
		void	addPlayerToWaitingQueue(Player* player);
		void	removePlayerFromWaitingQueue(Player* player);

		unsigned int		getNumberOfPlayingMatches() const;
		unsigned int		getNumberOfPlayers(bool takeStoppedPlayers = true);
		unsigned int		getNumberOfWaitingPlayers() const;
		std::pair<int, int>	getNumberOfMaxMinPlayedMatches(bool considereStoppedPlayers);

		void	setCourts(unsigned int courts);
		void	setMode(unsigned int mode);

		unsigned int	getMode() const;
		unsigned int	getNumberOfCourts() const;

		std::vector< std::pair<Player*, Player*> >&	getMatchsInProgress();
		std::vector<Player*>&						getWaitingQueue();
		std::map<const std::string, Player*>&		getPlayersList();

	private:
		struct s_infos {
			unsigned int	nbCourts;
		};
		struct s_infos	_infos;

		unsigned int	_mode;

		std::map<const std::string, Player*>			_playersList;
		std::vector< std::pair<Player*, Player*> >		_matchsInProgress;
		std::vector<Player*>							_waitingQueue;
		std::map<std::string, void (*)(Tournament*)>	_commands;

		vectorMatchsDouble	_doubleMatchsInProgress;
};

void	MATCH(Tournament* tournament);
void	INFOS(Tournament* tournament);
void	PLAYER(Tournament* tournament);
void	HISTORY(Tournament* tournament);
void	STATS(Tournament* tournament);
void	FINISH(Tournament* tournament);

#endif
