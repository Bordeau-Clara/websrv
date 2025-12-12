/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleEvent.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:16:29 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"

#include <cerrno>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "Server.hpp"
#include "Request.hpp"
#include "Cgi.hpp"
#include "string.hpp"

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
	EventAdd(client.fd, EPOLLIN, &client);
	streams.get(LOG_EVENT) << "Nouvelle connexion acceptée: " << client.fd
		<< std::endl;
}

bool	EventManager::recvFromClient(Request &client)
{
	static char buffer[BUFFER_SIZE + 1] = {0};

	ssize_t count = recv(client.fd, buffer, sizeof(buffer), 0); // kesako
	if (count == -1)
		throw (std::runtime_error("RECV KO"));
	if (count == 0)
	{
		Monitor.printNewLine(RED + "connection is CLOSE" + WHITE);
		EventDelete(client.fd);
		delete (Request *)getPtr();
		return (false);
	}
	monitorEventRecv(count, String(buffer).substr(0, count));
	client.appendBuffer(buffer, 0, count);
	return (true);
}

void	EventManager::handleClient()
{
	Request &client = *(Request *)getPtr();
	if (getEvent().events & EPOLLIN)
	{
		if (recvFromClient(client))
			client.parseBuffer();
		if (client.isState(SEND) || client.isState(ERROR))
		{
			// streams.print(LOG_EVENT) << "[CLIENT switching sending mode]" << std::endl
			if (client.isState(CGI))
			{
				//put 1 << 1 a 1 pour running_cgi -> inutile
				//mettre l'event en dormant (EPOLLONESHOT) faut il d'abord le mettre en EPOLLOUT?
				//-> .events = 0 est plus propre car EPOLLONESHOT est fais pour bloquer apres la reception d'un event
				//comme client va etre mis dans l'event de la cgi(ou inversement) on aura le fd pour le reactiver
				//lancer cgi et add l'event en EPOLLIN
				//quand cgi finit on le met dans la rep du client et on le reactive et on le passe en EPOLLOUT

				struct epoll_event cgi;
				cgi.data.ptr = &client.getCgi();
				Cgi& CGI = *(Cgi*)cgi.data.ptr;
				//create pipe fork and send optional body through new pipe and fork here??
				epoll_ctl(this->_fd, EPOLL_CTL_MOD, CGI._responsePipe[1], &cgi);//bon cote du pipe??
				getEvent().events = 0;
				epoll_ctl(this->_fd, EPOLL_CTL_MOD, client.fd, &getEvent());
			}
			else
			{
				EventModify(client.fd, EPOLLOUT, &client);
			}
		}
	}
	else if (getEvent().events & EPOLLOUT)
	{
		streams.get(LOG_EVENT) << "[ENVOI]" << std::endl
			<< std::endl;
		//have to send by small buffers to not exceed the socket's buffer
		std::string toSend = "HTTP/1.1 " + (client.getStatus().empty() ? "200 OK" : client.getStatus());
		toSend.append("\r\nContent-Type: text/plain\r\nContent-Length:12\r\nConnection:close\r\n\r\nHello, World!");
		if (send(client.fd, toSend.c_str(), toSend.length(), 0) == -1)
			throw (std::runtime_error("SEND"));
		streams.get(LOG_EVENT) << "[SUCCESS]" << std::endl
			<< std::endl;
		if (client.getConnection() == KEEP_ALIVE)
		{
			Monitor.printNewLine(RED + "connection is KEEP ALIVE" + WHITE);
			client.resetRequest();
			EventModify(client.fd, EPOLLIN, &client);
		}
		else
		{
			Monitor.printNewLine(RED + "connection is CLOSE" + WHITE);
			EventDelete(client.fd);
			delete (Request *)getPtr();
		}
	}
}

void	EventManager::handlePipe()
{
	//recv pipe
	//treat info and put into cgi.request.response
	//when finished DEL event cgi and EPOLL_CTL_MOD en EPOLLOUT cgi.request
}
