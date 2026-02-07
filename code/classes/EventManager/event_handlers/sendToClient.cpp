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

bool	EventManager::sendBuffer(Request &client)
{
	std::string toSend = client._response.get(BUFFER_SIZE);

	if (send(client.fd, toSend.data(), toSend.size(), 0) == -1)
	{
		Monitor.printNewLine(VIVID_RED + "FROM "+client.ip_str+" connection:CLOSE (client ended connection while sending)"  + WHITE);
		EventDelete(client.fd);
		this->requests.remove((Request *)getPtr());
		delete (Request *)getPtr();
	}
	Monitor.printNewLine(VIVID_BLUE + "SEND TO " + client.ip_str + ": " + nbrToString(toSend.size()) + " bytes" + RESET);
	return (client._response.transmissionComplete());
}

void	EventManager::sendToClient(void)
{
	Request &client = *(Request *)getPtr();

	if (!sendBuffer(client))
		return;
	/**/streams.get(LOG_EVENT) << "[SUCCESS]" << std::endl
		/**/<< std::endl;

	if (client.getConnection() == KEEP_ALIVE)
	{
		Monitor.printNewLine(VIVID_CYAN + "FROM "+client.ip_str+" connection:KEEPALIVE (end of the request)"  + WHITE);
		client.resetRequest();
		EventModify(client.fd, EPOLLIN, &client);
	}
	else
	{
		Monitor.printNewLine(VIVID_CYAN + "END FROM "+client.ip_str+" connection:CLOSE (end of the request)"  + WHITE);
		EventDelete(client.fd);
		this->requests.remove((Request *)getPtr());
		delete (Request *)getPtr();
	}
}
