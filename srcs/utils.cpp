#include <iostream>
#include "utils.hpp"

void	printMessage(const std::string message, const int type, const bool newLine) {
	if (type == WARNING)
		std::cout << CYELLOW << "ATTENTION: ";
	else if (type == ERROR)
		std::cout << CRED << "ERREUR: ";
	else
		std::cout << CBOLD;

	std::cout << message;
	if (newLine)
		std::cout << std::endl;

	std::cout << CRESET;
}

bool	isStringEmpty(std::string buffer) {
	std::string::iterator	it;

	for (it = buffer.begin(); it != buffer.end(); it++) {
		if (!isspace((*it)))
			return false;
	}
	return true;
}

bool	isStringNumeric(std::string buffer) {
	std::string::iterator	it;

	for (it = buffer.begin(); it != buffer.end(); it++)
		if (!isdigit(*it))
			return false;

	return true;
}

bool	isOui(std::string buffer) {
	if (buffer.compare("O") == 0)
		return true;

	if (buffer.compare("OUI") == 0)
		return true;

	if (buffer.compare("oui") == 0)
		return true;

	return false;
}

bool	isNon(std::string buffer) {
	if (buffer.compare("N") == 0)
		return true;

	if (buffer.compare("NON") == 0)
		return true;

	if (buffer.compare("non") == 0)
		return true;

	return false;
}
