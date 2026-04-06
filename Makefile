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
TESTERS		= config_parser.cpp config_tokenizer.cpp

MAIN		= main.cpp Webserv.cpp

UTIL		= Basics.cpp MimeTypes.cpp Heirarchy.cpp

INIT		=

PARSE		= ConfigParser.cpp ConfigSetters.cpp ConfigTokenizer.cpp FormatVerification.cpp

ROUTERS		= Socket.cpp Connection.cpp TaskManager.cpp

SERVICES	= Client.cpp Executer.cpp Parser.cpp Poller.cpp Sender.cpp RequestHandler.cpp CgiHandler.cpp

TEST		= test.cpp


PARSEDIR	= parse
UTILDIR		= utils
MAINDIR		= main
INITDIR		= init
SERVICEDIR	= services
TESTDIR		= tests/src
TESTMAINDIR	= tests/obj
TESTBINDIR	= tests/bin
ROUTEDIR	= routers

SRCSDIR		= src
OBJDIR		= obj
INCLDIR		= include

MAINSRC		= $(addprefix $(SRCSDIR)/, $(addprefix $(MAINDIR)/, $(MAIN)))
UTILSRC		= $(addprefix $(SRCSDIR)/, $(addprefix $(UTILDIR)/, $(UTIL)))
PARSESRC	= $(addprefix $(SRCSDIR)/, $(addprefix $(PARSEDIR)/, $(PARSE)))
SERVICESRC	= $(addprefix $(SRCSDIR)/, $(addprefix $(SERVICEDIR)/, $(SERVICES)))
INITSRC		= $(addprefix $(SRCSDIR)/, $(addprefix $(INITDIR)/, $(INIT)))
ROUTESRC	= $(addprefix $(SRCSDIR)/, $(addprefix $(ROUTEDIR)/, $(ROUTERS)))
TESTSRC		= $(addprefix $(TESTDIR)/, $(TESTERS))


SRCS		= $(MAINSRC) $(PARSESRC) $(UTILSRC) $(INITSRC) $(ROUTESRC) $(SERVICESRC)
OBJS		= $(SRCS:$(SRCSDIR)/%.cpp=$(OBJDIR)/%.o)

TESTMAINS	= $(TESTSRC:$(TESTDIR)/%.cpp=$(TESTMAINDIR)/%.o)
TESTBINS	= $(addprefix $(TESTBINDIR)/, $(basename $(notdir $(TESTMAINS))))
TESTOBJS	= $(filter-out obj/main/main.o, $(OBJS))

INCLUDE		= $(MAININCL) $(UTILINCL) $(SERVICEINCL) $(INITINCL) $(ROUTEINCL) $(PARSEINCL)

CFLAGS		= -Wall -Wextra -Werror -std=c++98
INCLUDES	= $(addprefix -I, $(INCLDIR)) -g3
CC			= c++
TOTAL_SRCS	= $(words $(MAINSRC) $(PARSESRC) $(UTILSRC) $(INITSRC) $(ROUTESRC) $(SERVICESRC))

RM			= rm -rf
SRC_NUM		= 0



all: $(NAME)
test: $(TESTOBJS) $(TESTBINS)


$(NAME): $(OBJDIR) $(OBJS)
	@$(CC) $(OBJS) $(LIBRARY) $(INCLUDES) -o $(NAME)

$(TESTBINDIR)/%: $(TESTDIR)/%.cpp $(TESTOBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

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
	@if [ $(PERCENT) -eq 100 ]; then printf "\n"; fi
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)


$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@$(RM) $(OBJDIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re test
