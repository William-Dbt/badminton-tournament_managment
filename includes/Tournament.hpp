#ifndef __TOURNAMENT_HPP__
# define __TOURNAMENT_HPP__

# define VERSION 1.0

# include <string>
# include <map>
# include "Player.hpp"

class   Tournament {
	public:
		Tournament();
		~Tournament();

		void	savePlayers();
		void	askCourtsNumber();

		void	run();

		void	addPlayer(const std::string name);
		void	removePlayer(const std::string name);
		void	showPlayers();

		unsigned int	getNumberOfPlayers() const;

		std::map<const std::string, Player*>&	getPlayersList();

	private:
		struct s_infos {
			unsigned int	nbCourts;
		};

		struct s_infos	_infos;

		std::map<const std::string, Player*>	_playersList;
};

#endif
