# Simple Makefile for webserv project

NAME = webserv

CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98

INCLUDES = -Iincludes

SRCS_ALL = $(shell find sources -name "*.cpp")
# Exclude logger.cpp which contains a different Logger implementation
SRCS = $(filter-out sources/logger.cpp,$(SRCS_ALL))
OBJS = $(SRCS:.cpp=.o)

.PHONY: all re clean fclean

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
