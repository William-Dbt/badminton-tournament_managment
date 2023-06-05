#ifndef __PLAYER_HPP__
# define __PLAYER_HPP__

# include <string>

class   Player {
	public:
		Player(const std::string name);
		~Player();

		void    setName(const std::string& name);

	private:
		std::string _name;
};

#endif
