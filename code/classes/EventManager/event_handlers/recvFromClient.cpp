/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recvFromClient.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:26:41 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Event.hpp"
#include "EventManager.hpp"

#include "Request.hpp"
#include "Cgi.hpp"
#include <sys/epoll.h>

bool	EventManager::recvBuffer(Request &client)
{
	static char buffer[BUFFER_SIZE] = {0};

	ssize_t count = recv(client.fd, buffer, sizeof(buffer), 0); // kesako
	if (count == -1)
		throw (std::runtime_error("RECV KO"));
	if (count == 0) // client has closed connection
	{
		Monitor.printNewLine(RED + "END FROM "+client.ip_str+" connection:CLOSE (client disconnected)"  + RESET);
		EventDelete(client.fd);
		delete (Request *)getPtr();
		this->requests.remove((Request *)getPtr());
		return (false);
	}
	monitorEventRecv(count, String(buffer).substr(0, count), client);
	client.appendBuffer(buffer, count);
	return (true);
}

void	EventManager::recvFromClient(void)
{
	if (checkEvent() != CLIENT)
		/**/streams.get(LOG_EVENT) << "{EEEEEEEEEEEERRRRRRRRRRRRROOOOOOOOOOOOOOORRRRRRRRRRR}" << std::endl
			/**/<< std::endl;
	Request &client = *(Request *)getPtr();
	if (checkEvent() != CLIENT)
		/**/streams.get(LOG_EVENT) << "{EEEEEEEEEEEERRRRRRRRRRRRROOOOOOOOOOOOOOORRRRRRRRRRR}" << std::endl
			/**/<< std::endl;
	// si reception
	// si aucun element est recu
	// CECI n'est pas tres propre
	if (!recvBuffer(client)) // if nothing has been received
		return ;
	client.parseBuffer();
	if (!client.isState(EXEC))// if parsing is not finished
		return ;
	// (should be only send because error is send)
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
		Cgi	*cgi = client.getCgi();
		// cree les pipes
		Monitor.printNewLine("Initializing a pipe");
		cgi->init();
		// ecoute le pipe cgi
		// mute les envois clients
		EventModify(client.fd, 0, &client);
		//create pipe fork and send optional body through new pipe and fork here??
		cgi->start(*this);
		EventAdd(cgi->_responsePipe[0], EPOLLIN, cgi);

	}
	else // passe en emission
	{
		client.generateResponse();
		EventModify(client.fd, EPOLLOUT, &client);
	}
}
