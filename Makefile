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
CONFIG = configs/default.conf
TESTER_DIR = request_builder

build:
	$(MAKE) -j -C $(PROJECT_DIR)
.PHONY: build

run:
	$(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: run

request:
	$(MAKE) -C RequestGenerator
.PHONY: request

test:
	bash netcat.sh
.PHONY: test
