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

# Sources listing
SRC_FILES = \
	main.cpp \
	time.cpp \
	\
	parse_buffer.cpp \
	parse_header.cpp \
	parse_request_line.cpp \
	parse_header_utils.cpp \

# Classes listing
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
	\
	Request/Request.cpp \
	Request/Getter.cpp \
	Request/HeaderTab.cpp \
	Request/FieldsParser.cpp \
	Request/FillBody.cpp \
	Request/RequestUtils.cpp \
	Cgi/Cgi.cpp \
	\
	EventManager/EventManager.cpp \
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
