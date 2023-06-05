#include <iostream>
#include "utils.hpp"

void	printMessage(const std::string message, const int type) {
	if (type == WARNING)
		std::cout << "ATTENTION: ";
	else if (type == ERROR)
		std::cout << "ERREUR: ";

	std::cout << message << std::endl;
}

bool	isStringEmpty(std::string buffer) {
	std::string::iterator	it;

	for (it = buffer.begin(); it != buffer.end(); it++) {
		if (!isspace((*it)))
			return false;
	}
	return true;
}
