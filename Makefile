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
CONFIG_TEST = configs/42CGI.conf
TESTER_DIR = request_builder

build:
	$(MAKE) -j -C $(PROJECT_DIR)
.PHONY: build

run_multipleServers:
	$(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: run

# deprecated
request:
	$(MAKE) -C RequestGenerator
.PHONY: request

test:
	bash netcat.sh
.PHONY: test
#fin

run_42cgi:
	$(PROJECT_DIR)/$(BIN) $(CONFIG_TEST)
.PHONY: run_42cgi

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

debug:
	valgrind $(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: debug

debugFork:
	valgrind --trace-children=yes --track-fds=yes $(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: debug

debugLeaks:
	valgrind --trace-children=yes --track-fds=yes --leak-check=full --show-leak-kinds=all $(PROJECT_DIR)/$(BIN) $(CONFIG)
.PHONY: debug

val_42CGI:
	valgrind $(PROJECT_DIR)/$(BIN) $(CONFIG_TEST)
.PHONY: val_42CGI

post_42CGI:
	curl -v \
		-X POST http://localhost:8002/put_test/test.bla \
		-H "Content-Type: text/plain" \
		-H "Connection: close" \
		--data-ascii "C'est un TOUPPER ou juste ca inverse ??????"
.PHONY: post_42CGI

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

chunked:
	{ \
	  echo "--- Début du message ---"; \
	  sleep 4; \
	  echo "--- Milieu du message (2s plus tard) ---"; \
	  sleep 4; \
	  echo "--- Fin du message ---"; \
	} | curl -v  --no-buffer -X POST "http://localhost:8002/chunked.txt" \
		 -H "Transfer-Encoding: chunked" \
		 -H "Content-Type: text/plain" \
		 --data-binary @-
.PHONY: chunked

pretty:
	code/WebServ configs/pretty.conf
.PHONY: pretty

# SIEGE
SIEGE_PORT = 4000
SIEGE_LOCATION = /siege.html
SIEGE_CONFIG = configs/siege.conf
SIEGE_IP = localhost

siege_server_run:
	$(PROJECT_DIR)/$(BIN) $(SIEGE_CONFIG)

siege_stress_empty:
	siege -c 10 -t 10S http://$(SIEGE_IP):$(SIEGE_PORT)/$(SIEGE_LOCATION)

siege_stress_404:
	siege -c 10 -t 5S http://$(SIEGE_IP):$(SIEGE_PORT)/kk
