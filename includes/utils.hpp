#ifndef __UTILS_HPP__
# define __UTILS_HPP__

# include <string>

# define CRESET		"\x1B[0m"
# define CBOLD		"\x1b[1m"
# define CRESETB	CRESET << CBOLD
# define CGRAY		"\x1B[90m"
# define CRED		"\x1B[31m"
# define CYELLOW	"\x1B[33m"

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
