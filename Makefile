# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ikulik <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/02 14:20:42 by ikulik            #+#    #+#              #
#    Updated: 2026/02/18 18:13:32 by ikulik           ###   ########.fr        #
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

UTIL		= Basics.cpp MimeTypes.cpp

INIT		=

PARSE		= ConfigParser.cpp

ROUTERS		= Socket.cpp Connection.cpp TaskManager.cpp

SERVICES	= Client.cpp Executer.cpp Parser.cpp Poller.cpp Reader.cpp Sender.cpp RequestHandler.cpp CgiHandler.cpp

TEST		= test.cpp


PARSEDIR	= parse
UTILDIR		= utils
MAINDIR		= main
INITDIR		= init
SERVICEDIR	= services
TESTDIR		= tests
ROUTEDIR	= routers

SRCSDIR		= src
OBJDIR		= obj
INCLDIR		= include

MAINSRC		= $(addprefix $(SRCSDIR)/, $(addprefix $(MAINDIR)/, $(MAIN)))
UTILSRC		= $(addprefix $(SRCSDIR)/, $(addprefix $(UTILDIR)/, $(UTIL)))
PARSESRC	= $(addprefix $(SRCSDIR)/, $(addprefix $(PARSEDIR)/, $(PARSE)))
SERVICESRC	= $(addprefix $(SRCSDIR)/, $(addprefix $(SERVICEDIR)/, $(SERVICES)))
INITSRC		= $(addprefix $(SRCSDIR)/, $(addprefix $(INITDIR)/, $(INIT)))
TESTSRC		= $(addprefix $(SRCSDIR)/, $(addprefix $(TESTDIR)/, $(TEST)))
ROUTESRC	= $(addprefix $(SRCSDIR)/, $(addprefix $(ROUTEDIR)/, $(ROUTERS)))

MAININCL	= $(addprefix $(INCLDIR)/, $(MAINDIR))
UTILINCL	= $(addprefix $(INCLDIR)/, $(UTILDIR))
SERVICEINCL	= $(addprefix $(INCLDIR)/, $(SERVICEDIR))
INITINCL	= $(addprefix $(INCLDIR)/, $(INITDIRDIR))
ROUTEINCL	= $(addprefix $(INCLDIR)/, $(ROUTEDIR))
PARSEINCL	= $(addprefix $(INCLDIR)/, $(PARSEDIR))


SRCS		= $(MAINSRC) $(PARSESRC) $(UTILSRC) $(INITSRC) $(ROUTESRC) $(SERVICESRC)
OBJS		= $(SRCS:$(SRCSDIR)/%.cpp=$(OBJDIR)/%.o)

TESTSRCS	= $(TESTSRC) $(PARSESRC) $(UTILSRC) $(INITSRC) $(ROUTESRC) $(SERVICESRC)
TESTOBJS	= $(TESTSRCS:$(SRCSDIR)/%.cpp=$(OBJDIR)/%.o)

INCLUDE		= $(MAININCL) $(UTILINCL) $(SERVICEINCL) $(INITINCL) $(ROUTEINCL) $(PARSEINCL)

CFLAGS		= -Wall -Wextra -Werror -std=c++98
INCLUDES	= $(addprefix -I, $(INCLUDE))
CC			= c++
TOTAL_SRCS	= $(words $(MAINSRC) $(PARSESRC) $(UTILSRC) $(INITSRC) $(ROUTESRC) $(SERVICESRC))

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
