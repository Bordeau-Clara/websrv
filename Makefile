# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/11 08:28:48 by aykrifa           #+#    #+#              #
#                                                                              #
# **************************************************************************** #

PROJECT_DIR = code
BIN = WebServ
CONFIG = configs/multipleServers.conf

build:
	$(MAKE) -j -C $(PROJECT_DIR)
.PHONY: build

run:
	$(PROJECT_DIR)/$(BIN)
.PHONY: run

run_multiple:
	$(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: run_multiple

debug:
	valgrind $(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: debug

debugFork:
	valgrind --trace-children=yes --track-fds=yes $(PROJECT_DIR)/$(BIN)
.PHONY: debug

debugLeaks:
	valgrind --trace-children=yes --track-fds=yes --leak-check=full --show-leak-kinds=all $(PROJECT_DIR)/$(BIN)
.PHONY: debug

# SIEGE
SIEGE_PORT = 4000
SIEGE_LOCATION = /siege.html
SIEGE_CONFIG = configs/siege.conf
SIEGE_IP = localhost

siege_run:
	$(PROJECT_DIR)/$(BIN) $(SIEGE_CONFIG)

siege_test:
	siege -c 10 -t 10S http://$(SIEGE_IP):$(SIEGE_PORT)/$(SIEGE_LOCATION)

siege_test_404:
	siege -c 10 -t 5S http://$(SIEGE_IP):$(SIEGE_PORT)/kk
