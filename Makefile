################################################################################
# => TEXT
################################################################################

END		= \033[0m
BOLD	= \033[1m

RED		= \033[91m
GREEN	= \033[92m
YELLOW	= \033[93m
CYAN	= \033[96m

################################################################################
# => VARIABLES
################################################################################

NAME		= tournament

PATHSRCS	= srcs/
PATHHEADERS	= includes/
PATHNULL	= /dev/null

SRCS		= $(addprefix ${PATHSRCS}, main.cpp Tournament.cpp Player.cpp utils.cpp) \
			$(addprefix ${PATHSRCS}commands/, MATCH.cpp INFOS.cpp FINISH.cpp HISTORY.cpp) \
			$(addprefix ${PATHSRCS}history/, saveHistory.cpp)


OBJS		= ${SRCS:.cpp=.o}

CC			= c++
INC			= -I${PATHHEADERS}

CFLAGS		= -Wall -Wextra -Werror

RM			= rm -rf
MAKE		= make -C

################################################################################
# => RULES
################################################################################

.cpp.o:
				@echo "${BOLD}${YELLOW}Compiling:${END}\t$<"
				@${CC} ${CFLAGS} ${INC} -c $< -o ${<:.cpp=.o} >${PATHNULL}

all:		${NAME}

${NAME}:	${OBJS}
				@echo "${BOLD}${GREEN}Building:${END}\t${NAME}"
				@${CC} ${OBJS} -o ${NAME} >${PATHNULL}
				@sh ./init_folder.sh >${PATHNULL}

clean:
				@echo "${BOLD}${RED}Removing:${END}\tAll .o files"
				@${RM} ${OBJS} >${PATHNULL}

fclean:		clean
				@echo "${BOLD}${RED}Removing:${END}\t${NAME}"
				@${RM} ${NAME} >${PATHNULL}

re:			fclean all

.PHONY:		all clean fclean re

################################################################################