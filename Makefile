# Simple Makefile for webserv project

NAME = webserv

CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98

INCLUDES = -Iincludes

SRCS = $(shell find sources -name "*.cpp")
OBJS = $(SRCS:.cpp=.o)

ARGS ?=

all: $(NAME)

$(NAME): $(OBJS)
	@ echo "L3t'5 pl@y w3b53rv (:"
	@ $(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS)

%.o: %.cpp
	@ $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@ echo "L3t'5 cl34n up (:"
	@ rm -f $(OBJS)

fclean: clean
	@ rm -f $(NAME)

re: fclean all

run: re 
	./$(NAME) $(ARGS)
	@ $(MAKE) fclean

.PHONY: clean, fclean, re, run