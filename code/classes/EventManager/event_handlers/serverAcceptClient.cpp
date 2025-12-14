/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverAcceptClient.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:24:34 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"

#include <fcntl.h>

#include "Server.hpp"
#include "Request.hpp"

Request	&EventManager::requestAdd(Server &server)
{
	Request *client = new Request(server);
	requests.push_front(client);
	return (*client);
}

void	EventManager::serverAcceptClient(void)
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
