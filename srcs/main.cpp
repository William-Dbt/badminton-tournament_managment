#include <iostream>
#include "Tournament.hpp"

bool	g_bTournamentRunning = true;

static void	mainMessage() {
	std::cout << "########################################\n";
	std::cout << "USJM Badminton: Programme pour tournois\n";
	std::cout << "Version: " << VERSION << '\n';
	std::cout << "########################################\n" << std::endl;
}

int	main() {
	Tournament	tournament;

	mainMessage();
	tournament.savePlayers();
	tournament.askCourtsNumber();
	while (g_bTournamentRunning)
		tournament.run();

	return 0;
}
