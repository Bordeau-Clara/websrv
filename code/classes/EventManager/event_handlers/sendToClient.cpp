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
#include "helpers.hpp"

bool	EventManager::sendBuffer(Request &client)
{
	/**/streams.get(LOG_EVENT) << "before throwing" << std::endl;
	std::string toSend = client._response.get(BUFFER_SIZE);
	/**/streams.get(LOG_EVENT) << "before throwing" << std::endl;

	if (send(client.fd, toSend.c_str(), toSend.size(), 0) == -1)
		throw (std::runtime_error("SEND"));
	Monitor.printNewLine("SEND TO "+client.ip_str+ ": " + nbrToString(toSend.size())+" bytes");
	return (client._response.transmissionComplete());
}

void	EventManager::sendToClient(void)
{
	// si on est en emission
	Request &client = *(Request *)getPtr();
	/**/streams.get(LOG_EVENT) << "[resonse]" << client._response.cursor << std::endl
	/**/<< "[resonse]" << client._response.str.size() << std::endl
	/**/<< "[resonse]" << client._response.str << std::endl
		/**/<< std::endl;

	if (!sendBuffer(client))
		return;
	// shoul return if false
	// continue if true then checkif shoukld close request or keepalive
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
		Monitor.printNewLine(RED + "FROM "+client.ip_str+" connection:KEEPALIVE (end of the request)"  + WHITE);
		client.resetRequest();
		EventModify(client.fd, EPOLLIN, &client);
	}
	else
	{
		Monitor.printNewLine(RED + "END FROM "+client.ip_str+" connection:CLOSE (end of the request)"  + WHITE);
		EventDelete(client.fd);
		this->requests.remove((Request *)getPtr());
		delete (Request *)getPtr();
	}
}
