#include <iostream>
#include "utils.hpp"
#include "Tournament.hpp"

extern bool	g_bFinishTournament;

void	FINISH(Tournament* tournament) {
	std::string	buffer;

	printMessage("Souahitez-vous vraiment terminer le tournoi en cours? (O:oui/N:non)", WARNING, false);
	std::getline(std::cin, buffer);
	if (!isOui(buffer))
		return ;

	(void)tournament;
	g_bFinishTournament = true;
}
