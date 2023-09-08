# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: saguesse <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/25 12:13:17 by saguesse          #+#    #+#              #
#    Updated: 2023/08/29 19:40:29 by saguesse         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -g3

RM = rm -rf

HEADER_DIR = -I include
HEADER_FILES = include/Server.hpp \
			   include/Client.hpp \
			   include/Messages.hpp \
			   include/Channel.hpp

SRCS_DIR = srcs
SRCS_FILES = main.cpp \
			 Server.cpp \
			 Client.cpp \
			 Messages.cpp \
			 MsgJoin.cpp \
			 MsgMode.cpp \
			 MsgNick.cpp \
			 MsgPart.cpp \
			 MsgPing.cpp \
			 MsgPrivmsg.cpp \
			 MsgQuit.cpp \
			 MsgTopic.cpp \
			 MsgInvite.cpp \
			 Channel.cpp
SRCS = $(SRCS_FILES:%=$(SRCS_DIR)/%)

OBJS_DIR = objs
OBJS = $(SRCS:$(SRCS_DIR)/.cpp=$(OBJS_DIR)/.o)

$(OBJS_DIR)%.o : $(SRCS_DIR)/%.cpp $(HEADER_FILES)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(NAME): $(OBJS_DIR) $(OBJS) $(HEADER_FILES)
	$(CXX) -o $(NAME) $(CXXFLAGS) $(OBJS) $(HEADER_DIR)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

all: $(NAME)

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re
