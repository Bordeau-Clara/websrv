# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/19 13:28:35 by cbordeau          #+#    #+#              #
#    Updated: 2025/10/19 14:29:40 by cbordeau         ###   LAUSANNE.ch        #
#                                                                              #
# **************************************************************************** #


NAME       = WebServ
CPP        = c++
CPPFLAGS   = -Wall -Wextra -Werror -std=c++98 -g3
INCLUDES   = -Iincludes -Iclasses/

# Dossiers
SRC_PATH   = sources
CLASS_PATH = classes
OBJ_PATH   = objs

# Sources
SOURCES = \
	$(SRC_PATH)/main.cpp \
	$(CLASS_PATH)//.cpp

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
