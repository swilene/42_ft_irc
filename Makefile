# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: saguesse <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/20 09:39:18 by saguesse          #+#    #+#              #
#    Updated: 2023/08/22 15:51:43 by saguesse         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -g3
RM = rm -rf

INC = -I include
HEADER = include/Server.hpp \
		 include/Client.hpp \
		 include/Messages.hpp

SRC_DIR = srcs
SRC_FILES = main.cpp \
			Server.cpp \
			Client.cpp \
			Messages.cpp
SRC = $(SRC_FILES:%=$(SRC_DIR)/%)

OBJ_DIR = objs
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) $(HEADER)

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(HEADER) $(INC)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INC)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

all: $(NAME)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re
