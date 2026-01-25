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
CONFIG_TEST = configs/test.conf
TESTER_DIR = request_builder

build:
	$(MAKE) -j -C $(PROJECT_DIR)
.PHONY: build

run:
	$(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: run

corr:
	$(PROJECT_DIR)/$(BIN) $(CONFIG_TEST)
.PHONY: run

request:
	$(MAKE) -C RequestGenerator
.PHONY: request

post:
	curl -v \
		-X POST http://localhost:8002/test \
		-H "Content-Type: text/plain" \
		-H "Connection: close" \
		--data-binary @www/default/img/smile
		#--data-binary @Makefile 
.PHONY: post

delete:
	curl -v \
		-X DELETE http://localhost:8002/test.txt
.PHONY: delete

get:
	firefox localhost:8002/
.PHONY: get

test:
	bash netcat.sh
.PHONY: test

debug:
	valgrind $(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: debug

corrval:
	valgrind $(PROJECT_DIR)/$(BIN) $(CONFIG_TEST)
.PHONY: run
