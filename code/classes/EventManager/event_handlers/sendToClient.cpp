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

void	EventManager::sendBuffer(Request &client)
{// Send to client function
	//have to send by small buffers to not exceed the socket's buffer
	/**/streams.get(LOG_EVENT) << "[ENVOI]" << std::endl
		/**/<< std::endl;
	std::string toSend = "HTTP/1.1 " + (client.getStatus().empty() ? "200 OK" : client.getStatus());
	toSend.append("\r\nContent-Type: text/plain\r\nContent-Length:12\r\nConnection:close\r\n\r\nHello, World!");
	if (send(client.fd, toSend.c_str(), toSend.length(), 0) == -1)
		throw (std::runtime_error("SEND"));
}

void	EventManager::sendToClient(void)
{
	// si on est en emission
	Request &client = *(Request *)getPtr();

	sendBuffer(client);
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
