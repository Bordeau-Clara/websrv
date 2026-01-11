# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/17 12:32:52 by aykrifa           #+#    #+#              #
#                                                                              #
# **************************************************************************** #

# directories
SRC_DIR   = sources
CLASS_DIR = classes
OBJ_DIR   = build

# Sources files listing
SRC_FILES = \
	main.cpp \
	utils/strTimestamp.cpp \
	utils/nbrToString.cpp \
	utils/hexToLong.cpp \
	utils/trimSlash.cpp \
	utils/extractStr.cpp \
	utils/recursiveReaddir.cpp \
	\

# Classes files listing
CLASS_FILES = \
	ArgChecker/ArgChecker.cpp \
	ConfigParser/ConfigParser.cpp \
		ConfigParser/tokenize.cpp \
		ConfigParser/run.cpp \
		ConfigParser/parseDirective.cpp \
		ConfigParser/helpers.cpp \
	FileStream/FileStream.cpp \
	Location/Location.cpp \
	Server/Server.cpp \
	Request/Request.cpp \
		Request/Getter.cpp \
		Request/RequestUtils.cpp \
		Request/Response.cpp \
			Request/parser/FieldsParser.cpp \
			Request/parser/FillBody.cpp \
			Request/parser/parseBuffer.cpp \
			Request/parser/parseHeader.cpp \
			Request/parser/parseHeaderUtils.cpp \
			Request/parser/stateMachine.cpp \
	Cgi/Cgi.cpp \
	EventManager/EventManager.cpp \
		EventManager/event_handlers/serverAcceptClient.cpp \
		EventManager/event_handlers/recvFromClient.cpp \
		EventManager/event_handlers/sendToClient.cpp \
		EventManager/event_handlers/handlePipe.cpp \
		EventManager/utils.cpp \
		EventManager/monitor.cpp \
	Logger/Logger.cpp \

CLASSES = $(addprefix $(CLASS_DIR)/, $(CLASS_FILES))
SOURCES = $(addprefix $(SRC_DIR)/,$(SRC_FILES))

# include directories
INC_DIR = \
	includes \
		$(CLASS_DIR)/ArgChecker \
		$(CLASS_DIR)/ConfigParser \
		$(CLASS_DIR)/FileStream \
		$(CLASS_DIR)/Location \
		$(CLASS_DIR)/Server \
		$(CLASS_DIR)/Request \
			$(CLASS_DIR)/Request/parser \
		$(CLASS_DIR)/Cgi \
		$(CLASS_DIR)/EventManager \
		$(CLASS_DIR)/Event \
		$(CLASS_DIR)/Logger \

##################################################################################
#compilation
NAME		= WebServ
CC			= c++
CFLAGS 		= -Wall -Wextra -Werror -std=c++98 -MMD -MP -g3
PREPROC		=
INCLUDES	= $(addprefix -I, $(INC_DIR))

# Objets (même structure que SOURCES ou CLASSES mais dans objs/)
OBJS = $(CLASSES:%.cpp=$(OBJ_DIR)/%.o) \
		$(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

DEPS = $(OBJS:.o=.d)

all: $(NAME)
.PHONY: all

# LINK
$(NAME): $(OBJS)
	$(CC) $(OBJS) \
		-o $@

#compilation
$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(INCLUDES) \
		$< -o $@

# COMMENT
-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR)
.PHONY: clean

fclean: clean
	rm -f $(NAME)
.PHONY: fclean

re: fclean
	make all
.PHONY: re

bear:
	bear -- make re
.PHONY: bear
