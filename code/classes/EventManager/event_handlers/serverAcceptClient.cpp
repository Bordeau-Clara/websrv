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
#include "colors.hpp"
#include "helpers.hpp"

Request	&EventManager::requestAdd(Server &server)
{
	Request *client = new Request(server);
	requests.push_front(client);
	return (*client);
}

void	EventManager::serverAcceptClient(void)
{
	Server	&server = *(Server*)getPtr();
	Request	&client = requestAdd(server);

	DashBoard.log(VIVID_GREEN + "New client " + client.ip_str + " accepted !" + RESET);
	// Configurer le socket client en non-bloquant
	fcntl(client.fd, F_SETFL, O_NONBLOCK);

	// Ajouter le socket client à epoll
	EventAdd(client.fd, EPOLLIN, &client);
	client.editTime();
}
