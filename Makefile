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

test:
	bash netcat.sh
.PHONY: test

debug:
	valgrind $(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: debug

debugFork:
	valgrind --trace-children=yes --track-fds=yes $(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: debug

debugLeaks:
	valgrind --trace-children=yes --track-fds=yes --leak-check=full --show-leak-kinds=all $(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: debug

corrval:
	valgrind $(PROJECT_DIR)/$(BIN) $(CONFIG_TEST)
.PHONY: corrval

corrpost:
	curl -v \
		-X POST http://localhost:8002/put_test/test.bla \
		-H "Content-Type: text/plain" \
		-H "Connection: close" \
		--data-ascii "C'est un TOUPPER ou juste ca inverse ??????"
.PHONY: corrpost

# Liste des navigateurs par ordre de préférence
BROWSERS := firefox firefox-esr google-chrome brave-browser chromium

# Détection du premier navigateur disponible
SELECTED_BROWSER := $(shell for b in $(BROWSERS); do if command -v $$b >/dev/null 2>&1; then echo $$b; break; fi; done)

# Vérification si un navigateur a été trouvé, sinon on affiche une erreur propre
ifeq ($(SELECTED_BROWSER),)
    BROWSER_CMD := echo "Erreur: Aucun navigateur trouvé ($(BROWSERS))"
else
    BROWSER_CMD := $(SELECTED_BROWSER)
endif

# Règles
get:
	$(BROWSER_CMD) localhost:8002/
.PHONY: get

cgi-php:
	$(BROWSER_CMD) localhost:8002/cgi-bin/wow.php
.PHONY: cgi-php

cgi-py:
	$(BROWSER_CMD) localhost:8002/cgi-bin/wow.py
.PHONY: cgi-py

cgi-sh:
	$(BROWSER_CMD) localhost:8002/cgi-bin/wow.sh
.PHONY: cgi-sh

cgi-not_found:
	$(BROWSER_CMD) localhost:8002/cgi-bin/prout.sh
.PHONY: cgi-sh

cgi-time_out:
	$(BROWSER_CMD) localhost:8002/cgi-bin/infiniteLoop.sh
.PHONY: cgi-time_out

cgi-cannot_exec:
	$(BROWSER_CMD) localhost:8002/cgi-bin/cannot.sh
.PHONY: cgi-cannot_exec
