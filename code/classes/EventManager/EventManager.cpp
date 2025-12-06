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

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>

#include "EventManager.hpp"
#include "Server.hpp"
#include "Request.hpp"

EventManager::EventManager(std::vector<Server> &servers)
{
    // 2. Créer une instance epoll
    _fd = epoll_create1(0);
    if (_fd == -1)
	{
        perror("epoll_create1");
		throw (std::runtime_error("ERROR"));
    }

	for(std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		// Structure pour les événements
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = it->getFd();
		event.data.ptr = &(*it);

		// 3. Ajouter le socket serveur à epoll
		if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, event.data.fd, &event) == -1)
		{
			perror("epoll_ctl: server_fd");
			throw (std::runtime_error("ERROR"));
		}
	}
}

void	EventManager::getNewEvent(void)
{
	//a.n = epoll_wait
	_nEvent = epoll_wait(this->_fd, this->_events, MAX_EVENTS, -1);
	if (_nEvent == -1)
	{
		perror("epoll_wait");
		throw (std::runtime_error("ERROR"));
	}
	_it = 0;
}

epoll_data_t	EventManager::getData(void)
{
	return (_events[_it].data);
}

void	*EventManager::getPtr(void)
{
	if (_it == _nEvent)
		return (NULL);
	return (_events[_it].data.ptr);
}

void	EventManager::eventNext(void)
{
	_it++;
}

void	EventManager::serverAccept(Server &current)
{
	//TANT QUE accept()
	while (1)
	{
		Request *client = new Request(current);
		client->fd = accept(getData().fd, (struct sockaddr *)&client->client_addr, &client->client_len);
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
		if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, client->fd, &event) == -1) 
		{
			perror("epoll_ctl: client_fd");
			close(client->fd);
			continue;
		}
		printf("Nouvelle connexion acceptée: %d\n", client->fd);
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
			if (1 /*events[i].data.ptr is a server ?*/) // SERVER
			{
				serverAccept(*(Server*)getPtr());
			}
			else// CLIENT
			{
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
			}
		}
    }
}
