/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:51:13 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "webserv.hpp"

#define MAX_EVENTS 10

int event_loop(std::vector<Server> servers) {
    // 2. Créer une instance epoll
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

	for(std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		// Structure pour les événements
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = it->getFd();
		event.data.ptr = &(*it);

		// 3. Ajouter le socket serveur à epoll
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event) == -1) {
			perror("epoll_ctl: server_fd");
			exit(EXIT_FAILURE);
		}
	}

    // Boucle principale
	struct epoll_event events[MAX_EVENTS];

	//2.TANT QUE (serveur en marche)
    while (1)
	{
		//a.n = epoll_wait
        int nEvent = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nEvent == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

		//b.POUR CHAQUE event dans events [0,n)
		for (int i = 0; i < nEvent; ++i)
		{
			// 1. SI(event.fd == server_socket)
			if (1 /*events[i].data.ptr is a server ?*/)
			{
				//TANT QUE accept()
				while (1)
				{
					Request *client = new Request(*(Server*)events[i].data.ptr);
					client->fd = accept(events[i].data.fd, (struct sockaddr *)&client->client_addr, (socklen_t*)&client->client_len);
					if (client->fd == -1)
					{
						perror("accept");
						break;
					}

					// Configurer le socket client en non-bloquant
					fcntl(client->fd, F_SETFL, O_NONBLOCK);

					// Ajouter le socket client à epoll
					struct epoll_event event;
					event.events = EPOLLIN;
					event.data.fd = client->fd;
					event.data.ptr = client;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client->fd, &event) == -1) 
					{
						perror("epoll_ctl: client_fd");
						close(client->fd);
						continue;
					}
					printf("Nouvelle connexion acceptée: %d\n", client->fd);
				}
			}
			else
			{
				// Données prêtes à être lues sur un socket client
				char buffer[1024];
				ssize_t count = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
				if (count <= 0) 
				{
					// Fermeture ou erreur de connexion
					close(events[i].data.fd);
					printf("Connexion fermée\n");
				}
				else
				{
					// Traiter les données reçues (ex: envoyer une réponse)
					send(events[i].data.fd, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!", 48, 0);
					close(events[i].data.fd);
				}
			}
		}
    }

    // close(server_fd);
    return 0;
}

