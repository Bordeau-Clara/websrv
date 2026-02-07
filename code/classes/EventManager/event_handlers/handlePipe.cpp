/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePipe.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:29:10 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"

#include <cerrno>
#include <cstring>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "Server.hpp"
#include "Request.hpp"
#include "Cgi.hpp"
#include "colors.hpp"
#include "statusCodes.hpp"
#include "string.hpp"
#include "helpers.hpp"

void	EventManager::handlePipe()
{
	Cgi &cgi = *(Cgi *)getPtr();
	static char buffer[BUFFER_SIZE] = {0};
	ssize_t count = read(cgi._responsePipe[0], buffer, sizeof(buffer));
	if (count > 0)
	{
		cgi._buffer.append(buffer, count);
		Monitor.printNewLine(BRIGHT_YELLOW + "READ from cgi " + nbrToString(count) + " BYTES !" + RESET);
		/**/streams.get(LOG_EVENT) << "RECEIVED:" +cgi._buffer << std::endl
			/**/<< std::endl;
		return ;
	}
	if (count == -1) // read error : cgi broken ?
	{
		Monitor.printNewLine(VIVID_RED + "Unexpected broken CGI pipe: "+ strerror(errno) + RESET);
		cgi._client->setError(Status(GATEWAY_TIMEOUT, 504));
		cgi._client->buildErrorResponse();
	}
	else // else -> eof
	{
		Monitor.printNewLine(ORANGE + "CGI successfully ended"  + RESET);
		//treat info and put into cgi.request.response
		cgi.parseBuffer();
		/**/streams.get(LOG_EVENT) << "{FD}" << cgi._client->fd << std::endl
			/**/<< std::endl;
		cgi._client->setState(EXEC);
	}
	//DEL event cgi
	EventDelete(cgi._responsePipe[0]);
	close(cgi._responsePipe[0]);
	//MOD request back to EPOLLOUT
	EventModify(cgi._client->fd, EPOLLOUT, cgi._client);
}
