#ifndef __PLAYER_HPP__
# define __PLAYER_HPP__

# include <string>
# include <vector>

class Tournament;

enum	e_status {
	WAITING = 0,
	INGAME,
	STOPPED,
	FINISHED
};

class   Player {
	public:
		Player(const std::string name);
		~Player();

		bool	hasAlreadyPlayAgainstOrWith(Player* player);
		void	addToListAlreadyPlayed(Player* player);

		void	initMatch(Tournament* tournament, Player* player, bool showMessage = true);
		void	initDoubleMatch(Tournament* tournament);
		void	findMatch(Tournament* tournament);
		bool	findDoubleMatch(Tournament* tournament, Player* exceptPlyr);
		void	addScoreMatch(std::string opponent, std::pair<int, int> score);
		void	addDoubleScoreMatch(Player* opponent1, Player* opponent2, std::pair<int, int> score);

		void			showScoreHistory();
		unsigned int	getTotalScore(int limitOfMatches = 0, Tournament* tournament = NULL);
		int				getNbOfMatches(Tournament* tournament = NULL, bool takeStoppedPlayers = true);
		int				getNbOfMatchesWon(Tournament* tournament);

		void    setName(const std::string& name);
		void	setStatus(const int status);
		void	setPartner(Player* partner);

		int																				getStatus() const;
		std::string																		getName() const;
		std::vector< std::pair<std::string, std::pair<int, int> > >&					getScoreHistory();
		std::vector< std::pair< std::pair< Player*, Player*>, std::pair<int, int> > >&	getDoubleScoreHistory();

		Player*	getPartner();

		bool	operator==(const Player& ref) const;

	private:
		int						_status;
		std::string 			_name;
		std::vector<Player*>	_listPlayersAlreadyPlayed;

		Player*	_partner;

		std::vector< std::pair< std::string, std::pair<int, int> > >					_scoreHistory;
		std::vector< std::pair< std::pair< Player*, Player*>, std::pair<int, int> > >	_doubleScoreHistory;
};

# include "Tournament.hpp"

#endif
