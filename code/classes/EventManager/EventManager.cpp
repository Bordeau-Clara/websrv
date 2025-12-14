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

#include "Event.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Cgi.hpp"
#include "string.hpp"

static const String	MONITOR_START = "Starting Webserv...";
EventManager::EventManager(std::vector<Server> &servers): Monitor(MONITOR_START)
{
    // 2. Créer une instance epoll
	Monitor.popStatus("Creating an epoll instance");
    _fd = epoll_create1(0);
    if (_fd == -1)
	{
        perror("epoll_create1");
		throw (std::runtime_error("ERROR"));
    }
	Monitor.popStatus("Opening Server fd...");
	for(std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		it->startListen();
		Monitor.printNewLine("Listening Succeed !");
		// 3. Ajouter le socket serveur à epoll
		EventAdd(it->getFd(), EPOLLIN, &*it);
		Monitor.printNewLine("Adding listening socket to epoll Succeed !");
	}
	// building jumptable thx to gemini
	epollinHandler[0] = &EventManager::serverAcceptClient;
	epollinHandler[1] = &EventManager::recvFromClient;
	epollinHandler[2] = &EventManager::handlePipe;
}

EventManager::~EventManager(void)
{
	close (this->_fd);
	for (std::list<Request*>::iterator it = requests.begin(); it != requests.end(); it++)
	{
		delete (*it);
	}
}

void	EventManager::run(void)
{
	Monitor.popStatus("STARTING ..");
    while (1)
	{
		// for each events
		for (getNewEvent(); getPtr(); eventNext())
		{
			if (eventIs(EPOLLIN)) // retriving which func it will call in th ejumptable epollinHandler
				(this->*epollinHandler[checkEvent()])();
			else // EPOLLOUT can only be for client send queue
				sendToClient();
		}
		Monitor.printNewLine("");
    }
}
