# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/29 08:40:27 by hyunghki          #+#    #+#              #
#    Updated: 2024/08/29 08:40:28 by hyunghki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= webserv
CXXFLAGS= -Wall -Werror -Wextra -std=c++98
LIBS=
INCLUDE= \
-Iincludes/rules \
-Iincludes/utils

SRC_DIR= ./srcs/

SRCS= $(SRC_DIR)main.cpp \
	  $(SRC_DIR)utils/FileManager.cpp \
	  $(SRC_DIR)utils/NetInfo.cpp \
	  $(SRC_DIR)rules/SyntaxChecker.cpp \
	  $(SRC_DIR)rules/ARule.cpp \
	  $(SRC_DIR)rules/MainRule.cpp \
	  $(SRC_DIR)rules/HttpRule.cpp \
	  $(SRC_DIR)rules/ServerRule.cpp \
	  $(SRC_DIR)rules/LocationRule.cpp \
	  $(SRC_DIR)rules/MethodRule.cpp

OBJS= $(SRCS:.cpp=.o)

ifdef DEBUG
	DEBUG_FLAG = -D_DEBUG
endif

all: $(NAME)

.PHONY: re clean fclean debug

re:
	make fclean
	make all

clean:
	rm -f $(OBJS)

fclean:
	make clean
	rm -f $(NAME)

debug:
	@make DEBUG=1 all

$(NAME): $(OBJS)
	c++ -o $@ $^

%.o: %.cpp
	c++ $(CXXFLAGS) -o $@ -c $< $(INCLUDE) $(DEBUG_FLAG)
