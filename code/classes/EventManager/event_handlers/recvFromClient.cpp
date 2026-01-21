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

#include "EventManager.hpp"

#include "Request.hpp"
#include "Cgi.hpp"

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
	Request &client = *(Request *)getPtr();
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
		//client.startCgi();
		//create pipe fork and send optional body through new pipe and fork here??

		// ecoute le pipe cgi
		EventAdd(client.getCgi()._responsePipe[1], EPOLLIN, &client.getCgi());
		// mute les envois clients
		EventModify(client.fd, 0, &client);
	}
	else // passe en emission
	{
		client.generateResponse();
		EventModify(client.fd, EPOLLOUT, &client);
	}
}
