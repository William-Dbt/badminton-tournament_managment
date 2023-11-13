#ifndef __UTILS_HPP__
# define __UTILS_HPP__

# include <string>

enum messageType {
	WARNING,
	ERROR
};

void	printMessage(const std::string message, const int type = -1, const bool newLine = true);
bool	isStringEmpty(std::string buffer);
bool	isStringNumeric(std::string buffer);
bool	isOui(std::string buffer);
bool	isNon(std::string buffer);

#endif
