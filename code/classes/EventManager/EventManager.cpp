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
#include "logfiles.hpp"
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
		// Structure pour les événements
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.ptr = &(*it);

		// 3. Ajouter le socket serveur à epoll
		if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, it->getFd(), &event) == -1)
		{
			perror("epoll_ctl: server_fd");
			throw (std::runtime_error("ERROR"));
		}
		Monitor.printNewLine("Adding listening socket to epoll Succeed !");
	}
}

EventManager::~EventManager(void)
{
	close (this->_fd);
	for (std::list<Request*>::iterator it = requests.begin(); it != requests.end(); it++)
	{
		delete (*it);
	}
}

Request	&EventManager::requestAdd(Server &server)
{
	Request *client = new Request(server);
	requests.push_front(client);
	return (*client);
}

void	EventManager::serverAccept(void)
{
	Server &server = *(Server*)getPtr();
	Request	&client = requestAdd(server);
	// Configurer le socket client en non-bloquant
	fcntl(client.fd, F_SETFL, O_NONBLOCK);

	// Ajouter le socket client à epoll
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.ptr = &client;
	if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, client.fd, &event) == -1) 
	{
		perror("epoll_ctl: client_fd");
		throw(std::runtime_error("Epoll ctl add fail failed !"));
	}
	streams.get(LOG_EVENT) << "Nouvelle connexion acceptée: " << client.fd
		<< std::endl;
}

#include "parsing_header.hpp"
void	EventManager::handleClient()
{
	char buffer[BUFFER_SIZE + 1] = {0};
	Request &client = *(Request *)getPtr();
	if (getEvent().events & EPOLLIN)
	{
		{
			ssize_t count = recv(client.fd, buffer, sizeof(buffer), 0); // kesako
			if (count == -1)
				throw (std::runtime_error("RECV KO"));
			if (count == 0) // gerer client deconecte
			{
				Monitor.printNewLine(RED + "connection is CLOSE" + WHITE);
				epoll_ctl(this->_fd, EPOLL_CTL_DEL, client.fd, &getEvent());
				delete (Request *)getPtr(); //vraiment pas sur de la syntaxe
				return ;
			}
			monitorEventRecv(count, String(buffer).substr(0, count));
			client.appendBuffer(buffer, 0, count);
		}
		parse_buffer(&client);
		if (client.getState() == SEND)
		{
			// streams.print(LOG_EVENT) << "[CLIENT switching sending mode]" << std::endl
			getEvent().events = EPOLLOUT;
			epoll_ctl(this->_fd, EPOLL_CTL_MOD, client.fd, &getEvent());
		}
	}
	else if (getEvent().events & EPOLLOUT)
	{
		streams.get(LOG_EVENT) << "[ENVOI]" << std::endl
			<< std::endl;
		std::string toSend = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length:12\r\nConnection:close\r\n\r\nHello, World!";
		if (send(client.fd, toSend.c_str(), toSend.length(), 0) == -1)
			throw (std::runtime_error("SEND"));
		streams.get(LOG_EVENT) << "[SUCCESS]" << std::endl
			<< std::endl;
		if (client.getConnection() == KEEP_ALIVE)
		{
			Monitor.printNewLine(RED + "connection is KEEP ALIVE" + WHITE);
			getEvent().events = EPOLLIN;
			client.resetRequest();
			epoll_ctl(this->_fd, EPOLL_CTL_MOD, client.fd, &getEvent());
			// event == eppollin
			// epoll_ctl(MOD EPOLLIN) 
			//reinitialiser client ?????? C CLARA LA FOLLE
		}
		else
		{
			Monitor.printNewLine(RED + "connection is CLOSE" + WHITE);
			epoll_ctl(this->_fd, EPOLL_CTL_DEL, client.fd, &getEvent());
			delete (Request *)getPtr(); //vraiment pas sur de la syntaxe
			//else a verifier

			// close(events[i].data.fd);
			// epoll ctl delete
		}
	}
}

void	EventManager::run(void)
{
	Monitor.popStatus("STARTING ..");
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
