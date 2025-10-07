# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/19 13:28:35 by cbordeau          #+#    #+#              #
#    Updated: 2025/10/07 13:59:43 by cbordeau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME       = WebServ
CPP        = c++
CPPFLAGS   = -Wall -Wextra -Werror -std=c++98 -g3
INCLUDES   = -Iincludes -Iclasses/Config -Iclasses/ConfigParser -Iclasses/CustomException -Iclasses/Locations -Iclasses/Logger -Iclasses/Server

# Dossiers
SRC_PATH   = sources
CLASS_PATH = classes
OBJ_PATH   = objs

# Sources
SOURCES = \
	$(SRC_PATH)/main.cpp \
	$(SRC_PATH)/execution/startServer.cpp \
	$(SRC_PATH)/parsing/parseServerConfigurationFile.cpp \
	$(SRC_PATH)/parsing/parseStream.cpp \
	$(SRC_PATH)/utilities/errors.cpp \
	$(SRC_PATH)/utilities/memory_utils.cpp \
	$(CLASS_PATH)/Config/Config.cpp \
	$(CLASS_PATH)/ConfigParser/ConfigParser.cpp \
	$(CLASS_PATH)/CustomException/CustomException.cpp \
	$(CLASS_PATH)/Locations/Locations.cpp \
	$(CLASS_PATH)/Logger/Logger.cpp \
	$(CLASS_PATH)/Server/Server.cpp

# Objets (même structure que SOURCES mais dans objs/)
OBJS = $(addprefix $(OBJ_PATH)/,$(SOURCES:.cpp=.o))
DEPS = $(OBJS:.o=.d)

# Règle principale
all: $(NAME)

# Édition de lien
$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $(OBJS)

# Compilation des .o (recréation des dossiers automatiquement)
$(OBJ_PATH)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Inclusion des dépendances
-include $(DEPS)

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

bear:
	bear -- make re

.PHONY: all clean fclean re
