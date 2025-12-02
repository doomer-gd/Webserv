# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ikulik <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/02 14:20:42 by ikulik            #+#    #+#              #
#    Updated: 2025/12/02 16:14:10 by ikulik           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


BOLD = \033[1m
RED = \033[31;2m
GREEN = \033[32m
YELLOW = \033[33m
RESET = \033[0m

NAME		= webserv

MAIN		= main.cpp Webserv.cpp

UTIL		=

PARSE		=


PARSEDIR	= src/parse
UTILDIR		= src/utils
MAINDIR		= src/main

MAINSRC		= $(addprefix $(MAINDIR)/, $(MAIN))
UTILSRC		= $(addprefix $(UTILDIR)/, $(UTIL))
PARSESRC	= $(addprefix $(PARSEDIR)/, $(PARSE))

SRCSDIR		= src
OBJDIR		= obj
INCLUDE		= include


SRCS		= $(MAINSRC) $(PARSESRC) $(UTILSRC)
OBJS		= $(SRCS:$(SRCSDIR)/%.cpp=$(OBJDIR)/%.o)

CFLAGS		= -Wall -Wextra -Werror -std=c++98
INCLUDES	= -I$(INCLUDE)
CC			= c++
TOTAL_SRCS	= $(words $(MAINSRC) $(PARSESRC) $(UTILSRC))

RM			= rm -rf
SRC_NUM		= 0



all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	@$(CC) $(OBJS) $(LIBRARY) $(INCLUDES) $(MFLAGS) -o $(NAME)

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

.PHONY: all clean fclean re
