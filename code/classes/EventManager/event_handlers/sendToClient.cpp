/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendToClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:28:00 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"
#include "Request.hpp"
#include "Cgi.hpp"
#include "colors.hpp"
#include "helpers.hpp"
#include <cerrno>

bool	EventManager::sendBuffer(Request &client)
{
	std::string toSend = client._response.get(BUFFER_SIZE);

	if (send(client.fd, toSend.data(), toSend.size(), 0) == -1)
	{
		DashBoard.log(VIVID_RED + "Unexpected connection end from " + client.ip_str +  " send: "+ strerror(errno) + RESET);
		EventDelete(client.fd);
		this->requests.remove((Request *)getPtr());
		delete (Request *)getPtr();
		return (false);
	}
	ssize_t count = toSend.size();
	DashBoard.addTBytes(count);
	DashBoard.log(VIVID_BLUE + "SEND TO " + client.ip_str + ": " + nbrToString(count) + " bytes" + RESET);
	return (client._response.transmissionComplete());
}

void	EventManager::sendToClient(void)
{
	Request &client = *(Request *)getPtr();

	if (!sendBuffer(client))
		return;

	if (client.getConnection() == KEEP_ALIVE)
	{
		DashBoard.log(VIVID_CYAN + "FROM "+client.ip_str+" connection:KEEPALIVE (end of the request)"  + RESET);
		client.resetRequest();
		EventModify(client.fd, EPOLLIN, &client);
	}
	else
	{
		DashBoard.log(VIVID_CYAN + "END FROM "+client.ip_str+" connection:CLOSE (end of the request)"  + RESET);
		EventDelete(client.fd);
		this->requests.remove((Request *)getPtr());
		delete (Request *)getPtr();
	}
}
