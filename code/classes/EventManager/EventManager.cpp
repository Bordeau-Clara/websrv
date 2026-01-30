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
#include <ctime>
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
EventManager::EventManager(std::vector<Server> &servers): Monitor(MONITOR_START), _alive(true), lastZombieCheck(std::time(NULL))
{
    // 2. Créer une instance epoll
	Monitor.popStatus("Creating an epoll instance");
    _fd = epoll_create1(0);
    if (_fd == -1)
	{
        perror("epoll_create1");
		throw (std::runtime_error("ERROR"));
    }
	Monitor.printNewLine("Opening Server fd...");
	for(std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		it->startListen();
		Monitor.printNewLine("Listening Succeed !");
		// 3. Ajouter le socket serveur à epoll
		EventAdd(it->getFd(), EPOLLIN, &*it);
		Monitor.printNewLine("Adding listening socket to epoll Succeed !");
	}
	// stdin event add
	EventAdd(STDIN_FILENO, EPOLLIN, &this->_stdin);
	// building jumptable thx to gemini
	epollinHandler[0] = &EventManager::serverAcceptClient;
	epollinHandler[1] = &EventManager::recvFromClient;
	epollinHandler[2] = &EventManager::handlePipe;
	epollinHandler[3] = &EventManager::handleStdin;
}

EventManager::~EventManager(void)
{
	/**/streams.get(LOG_EVENT) << "[EVENT MANAGER DETRUCTOR CALLED]" << std::endl
		/**/<< std::endl;
	close (this->_fd);
	for (std::list<Request*>::iterator it = requests.begin(); it != requests.end(); it++)
	{
		delete (*it);
	}
}

void	EventManager::run(void)
{
	Monitor.printNewLine("STARTING ..");
	while (_alive)
	{
		// for each events
		for (getNewEvent(); getPtr(); eventNext())
		{
			if (eventIs(EPOLLIN)) // retriving which func it will call in th ejumptable epollinHandler
				(this->*epollinHandler[checkEvent()])();
			else if (eventIs(EPOLLOUT) && checkEvent() == CLIENT) // EPOLLOUT can only be for client send queue
				sendToClient();
			else if (eventIs(EPOLLHUP) && checkEvent() == PIPE)
				handlePipe();
			else
				throw (std::runtime_error("Unrecognized event"));
			if (_alive == false)
				break;
		}
		zombieCheck();
	}
}
