# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ikulik <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/02 14:20:42 by ikulik            #+#    #+#              #
#    Updated: 2026/02/18 17:54:56 by ikulik           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


BOLD = \033[1m
RED = \033[31;2m
GREEN = \033[32m
YELLOW = \033[33m
RESET = \033[0m

NAME		= webserv
TESTERS		= test

MAIN		= main.cpp Webserv.cpp

UTIL		= Basics.cpp

INIT		=

PARSE		=

ROUTERS		= Socket.cpp Connection.cpp TaskManager.cpp

SERVICES	= Client.cpp Executer.cpp Parser.cpp Poller.cpp Reader.cpp Sender.cpp

TEST		= test.cpp


PARSEDIR	= src/parse
UTILDIR		= src/utils
MAINDIR		= src/main
INITDIR		= src/init
SERVICEDIR	= src/service
TESTDIR		= src/tests
ROUTEDIR	= src/routers

MAINSRC		= $(addprefix $(MAINDIR)/, $(MAIN))
UTILSRC		= $(addprefix $(UTILDIR)/, $(UTIL))
PARSESRC	= $(addprefix $(PARSEDIR)/, $(PARSE))
SERVICESRC	= $(addprefix $(SERVICEDIR)/, $(SEERVICES))
INITSRC		= $(addprefix $(INITDIR)/, $(INIT))
TESTSRC		= $(addprefix $(TESTDIR)/, $(TEST))
ROUTESRC	= $(addprefix $(ROUTEDIR)/, $(ROUTERS))

SRCSDIR		= src
OBJDIR		= obj
INCLUDE		= include


SRCS		= $(MAINSRC) $(PARSESRC) $(UTILSRC) $(INITSRC) $(ROUTESRC) $(SERVICESRC)
OBJS		= $(SRCS:$(SRCSDIR)/%.cpp=$(OBJDIR)/%.o)

TESTSRCS	= $(TESTSRC) $(PARSESRC) $(UTILSRC) $(INITSRC) $(ROUTESRC) $(SERVICESRC)
TESTOBJS	= $(TESTSRCS:$(SRCSDIR)/%.cpp=$(OBJDIR)/%.o)

CFLAGS		= -Wall -Wextra -Werror -std=c++98
INCLUDES	= -I$(INCLUDE)
CC			= c++
TOTAL_SRCS	= $(words $(MAINSRC) $(PARSESRC) $(UTILSRC) $(INITSRC))

RM			= rm -rf
SRC_NUM		= 0



all: $(NAME)
tester: $(TESTERS)

$(NAME): $(OBJDIR) $(OBJS)
	@$(CC) $(OBJS) $(LIBRARY) $(INCLUDES) -o $(NAME)

$(TESTERS): $(OBJDIR) $(TESTOBJS)
	@$(CC) $(TESTOBJS) $(LIBRARY) $(INCLUDES) -o $(TESTERS)

$(OBJDIR)/%.o: $(SRCSDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(eval SRC_NUM := $(shell expr $(SRC_NUM) + 1))
	@$(eval PERCENT := $(shell printf "%.0f" $(shell echo "scale=4; $(SRC_NUM) / $(TOTAL_SRCS) * 100" | bc)))
	@printf "$(BOLD)\rCompiling $(NAME): ["
	@$(eval PROGRESS := $(shell expr $(PERCENT) / 5))
	@printf "$(GREEN)%0.s#$(RESET)$(BOLD)" $(shell seq 1 $(PROGRESS))
	@if [ $(PERCENT) -lt 100 ]; then printf "%0.s-" $(shell seq 1 $(shell expr 20 - $(PROGRESS))); fi
	@printf "] "
	@if [ $(PERCENT) -eq 100 ]; then printf "$(GREEN)"; fi
	@printf "%d%% $(RESET)" $(PERCENT)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)


$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@$(RM) $(OBJDIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re test
