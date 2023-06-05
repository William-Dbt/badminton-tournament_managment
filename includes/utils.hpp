#ifndef __UTILS_HPP__
# define __UTILS_HPP__

enum messageType {
	WARNING,
	ERROR
};

void	printMessage(const std::string message, const int type = -1);
bool	isStringEmpty(std::string buffer);

#endif
