#ifndef __PLAYER_HPP__
# define __PLAYER_HPP__

# include <string>
# include <vector>

class Tournament;

enum	e_status {
	WAITING,
	INGAME,
	FINISHED
};

class   Player {
	public:
		Player(const std::string name);
		~Player();

		bool	hasAlreadyPlayAgainst(Player* player);
		void	initMatch(Tournament* tournament, Player* player);
		void	findMatch(Tournament* tournament);

		void    setName(const std::string& name);
		void	setStatus(const int status);

		int			getStatus() const;
		std::string	getName() const;

		bool	operator==(const Player& ref) const;

	private:
		int						_status;
		std::string 			_name;
		std::vector<Player*>	_listPlayersAlreadyPlayed;
};

# include "Tournament.hpp"

#endif
