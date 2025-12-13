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
	// si reception
	if (getEvent().events & EPOLLIN)
	{
		// si aucun element est recu
		// CECI n'est pas tres propre
		if (!recvFromClient(client))
			return ;
		client.parseBuffer();
		// si parsing est fini
		if (client.isState(SEND) || client.isState(ERROR))
		{
			// streams.print(LOG_EVENT) << "[CLIENT switching sending mode]" << std::endl

			// checke si la reponse attendue est celle dune cgi
			if (client.isState(CGI))// && !client.isState(ERROR)
			{
				//mettre l'event en dormant (EPOLLONESHOT) faut il d'abord le mettre en EPOLLOUT?
				//-> .events = 0 est plus propre car EPOLLONESHOT est fais pour bloquer apres la reception d'un event
				//comme client va etre mis dans l'event de la cgi(ou inversement) on aura le fd pour le reactiver
				//lancer cgi et add l'event en EPOLLIN
				//quand cgi finit on le met dans la rep du client et on le reactive et on le passe en EPOLLOUT
				//
				//client.startCgi();
				//create pipe fork and send optional body through new pipe and fork here??

				// ecoute le pipe cgi
				EventAdd(client.getCgi()._responsePipe[1], EPOLLIN, &client.getCgi());
				// mute les envois clients
				EventModify(client.fd, 0, &client);
			}
			else
			{
				// passe en emission
				EventModify(client.fd, EPOLLOUT, &client);
			}
		}
	}
	// si on est en emission
	else if (getEvent().events & EPOLLOUT)
	{
		/**/streams.get(LOG_EVENT) << "[ENVOI]" << std::endl
			/**/<< std::endl;

		//have to send by small buffers to not exceed the socket's buffer
		{// Send to client function
			std::string toSend = "HTTP/1.1 " + (client.getStatus().empty() ? "200 OK" : client.getStatus());
			toSend.append("\r\nContent-Type: text/plain\r\nContent-Length:12\r\nConnection:close\r\n\r\nHello, World!");
			if (send(client.fd, toSend.c_str(), toSend.length(), 0) == -1)
				throw (std::runtime_error("SEND"));
		}
		/**/streams.get(LOG_EVENT) << "[SUCCESS]" << std::endl
			/**/<< std::endl;

		// Close pas la connexion et garde en ecoute le socket client pour les prochaines requetes
		/* /!\ ATTENTION /!\
		 * le buffer peut contenir une ou plusieurs requete entiere apres la premiere reponse
		 * ce qui ferait que epoll ne rapellera jamais ce client
		 * sauf si il demande une enieme requete (ultra sale)
		*/
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
