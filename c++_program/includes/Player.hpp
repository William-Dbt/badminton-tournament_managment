#ifndef __PLAYER_HPP__
# define __PLAYER_HPP__

# include <string>
# include <vector>

class Tournament;

enum	e_status {
	WAITING = 0,
	INGAME,
	STOPPED,
	FINISHED	//TODO
};

class   Player {
	public:
		Player(const std::string name);
		~Player();

		bool	hasAlreadyPlayAgainst(Player* player);
		void	addToListAlreadyPlayed(Player* player);

		void	initMatch(Tournament* tournament, Player* player, bool showMessage = true);
		void	findMatch(Tournament* tournament);
		void	addScoreMatch(std::string opponent, std::pair<int, int> score);

		void			showScoreHistory();
		unsigned int	getTotalScore(int limitOfMatches = 0);
		int				getNbOfMatches(Tournament* tournament = NULL, bool takeStoppedPlayers = true);

		void    setName(const std::string& name);
		void	setStatus(const int status);

		int																getStatus() const;
		std::string														getName() const;
		std::vector< std::pair<std::string, std::pair<int, int> > >&	getScoreHistory();

		bool	operator==(const Player& ref) const;

	private:
		int						_status;
		std::string 			_name;
		std::vector<Player*>	_listPlayersAlreadyPlayed;

		std::vector< std::pair< std::string, std::pair<int, int> > >	_scoreHistory;
};

# include "Tournament.hpp"

#endif