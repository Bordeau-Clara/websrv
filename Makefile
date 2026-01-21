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

post:
	curl -v \
		-X POST http://localhost:8002/test \
		-H "Content-Type: text/plain" \
		-H "Connection: close" \
		--data-binary @Makefile 
		#--data-binary @www/default/img/smile
.PHONY: post

delete:
	curl -v \
		-X DELETE http://localhost:8002/test.txt
.PHONY: delete

test:
	bash netcat.sh
.PHONY: test

debug:
	valgrind $(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: debug
