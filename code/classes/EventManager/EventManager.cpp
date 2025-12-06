/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 13:50:38 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"

#include <cerrno>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>

#include "Server.hpp"
#include "Request.hpp"
#include "FileStream.hpp"
#include "logfiles.hpp"

extern FileStream	streams;

EventManager::EventManager(std::vector<Server> &servers)
{
    // 2. Créer une instance epoll
	streams.print(LOG_EVENT) << "epoll_create : "
		<< std::endl;
    _fd = epoll_create1(0);
    if (_fd == -1)
	{
        perror("epoll_create1");
		throw (std::runtime_error("ERROR"));
    }
	streams.print(LOG_EVENT) << _fd
		<< std::endl;

	for(std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		it->startListen();
		// Structure pour les événements
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.ptr = &(*it);

		// 3. Ajouter le socket serveur à epoll
		streams.print(LOG_EVENT) << "if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, event.data.fd, &event) == -1)"
			<< std::endl;
		if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, it->getFd(), &event) == -1)
		{
			streams.print(LOG_EVENT) << "ERRNO : " << errno 
				<< " EBADF: " << EBADF
				<< " EINVAL" << EINVAL
				<< " ELOOP" << ELOOP
				<< " EPERM" << EPERM
				<< std::endl;
			perror("epoll_ctl: server_fd");
			throw (std::runtime_error("ERROR"));
		}
	}
}

void	EventManager::serverAccept(void)
{
	Server &server = *(Server*)getPtr();
	//TANT QUE accept()
	while (1)
	{
		Request *client = new Request(server);
		streams.print(LOG_EVENT) << "accept(getData().fd, (struct sockaddr *)&client->client_addr, &client->client_len);"
			<< std::endl;
		client->fd = accept(server.getFd(), (struct sockaddr *)&client->client_addr, &client->client_len);
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
		event.data.ptr = client;
		if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, client->fd, &event) == -1) 
		{
			perror("epoll_ctl: client_fd");
			close(client->fd);
			continue;
		}
		streams.print(LOG_EVENT) << "Nouvelle connexion acceptée: " << client->fd
			<< std::endl;
	}
}

// Request *client = (Request *)getPtr();
// if current evnet == epollin
	// ssize_t count = recv(getData().fd, buffer, sizeof(buffer), 0); // kesako
	//client .appendBuffer(buffer);
	//parse_buffer(client);
	//if client.status = SEND 
	// mettre en EPOLLOUT
// if EPOLLOUT
	// send
	// if connexion == KEEPALIVE 
		// event == eppollin
		// epoll_ctl(MOD EPOLLIN) 
		//reinitialiser client ?????? C CLARA LA FOLLE
	//else
		//epoll ctl delete close fd mais sa va jamais arriver
#include "parsing_header.hpp"
void	EventManager::handleClient()
{
	static char buffer[BUFFER_SIZE + 1] = {0};
	Request &client = *(Request *)getPtr();
	if (getEvent().events == EPOLLIN)
	{
		ssize_t count = recv(client.fd, buffer, sizeof(buffer), 0); // kesako
		if (count == -1)
			throw (std::runtime_error("RECV KO"));
		//if count == 0 check time pour client fantome
		streams.print(LOG_EVENT) << "[RECV]" << std::endl
			<< std::string(buffer).substr(0, count + 1)
			<< std::endl;
		
		client.appendBuffer(buffer, 0, count + 1);
		parse_buffer(&client);
		if (client.getState() == SEND)
		{
			// streams.print(LOG_EVENT) << "[CLIENT READY FO SEND]" << std::endl
			// 	<< client
			// 	<< std::endl;
			// mettre en EPOLLOUT
			getEvent().events = EPOLLOUT;
			epoll_ctl(this->_fd, EPOLL_CTL_MOD, client.fd, &getEvent());
			//streams.print();
		}
	}
	else
	{
		streams.print(LOG_EVENT) << "[ENVOI]" << std::endl
			<< std::endl;
		if (send(client.fd, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!", 48, 0) == -1)
			throw (std::runtime_error("SEND"));
		streams.print(LOG_EVENT) << "[SUCCESS]" << std::endl
			<< std::endl;
		// if connexion == KEEPALIVE 
			// event == eppollin
			// epoll_ctl(MOD EPOLLIN) 
			//reinitialiser client ?????? C CLARA LA FOLLE
		//else
			// close(events[i].data.fd);
			// epoll ctl delete
	}
}

void	EventManager::run(void)
{
    while (1)
	{
		//b.POUR CHAQUE event dans events [0,n)
		for (getNewEvent(); getPtr(); eventNext())
		{
			// 1. SI(event.fd == server_socket)
			if (checkEvent() == SRV) // SERVER
			{
				serverAccept();
			}
			else// CLIENT
			{
				handleClient();
			}
		}
		sleep(1);
    }
}
