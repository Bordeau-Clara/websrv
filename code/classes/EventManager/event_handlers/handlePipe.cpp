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
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "Server.hpp"
#include "Request.hpp"
#include "Cgi.hpp"
#include "colors.hpp"
#include "string.hpp"
#include "helpers.hpp"

void	EventManager::handlePipe()
{
	Monitor.printNewLine(VIVID_YELLOW + "Handling A PIPE ..."  + RESET);
	Cgi &cgi = *(Cgi *)getPtr();
	static char buffer[BUFFER_SIZE] = {0};
	ssize_t count = read(cgi._responsePipe[0], buffer, sizeof(buffer));
	if (count == -1)
		throw (std::runtime_error("RECV KO"));
	if (count)
	{
		cgi._buffer.append(buffer, count);
		Monitor.printNewLine(BRIGHT_YELLOW + "RECV FROM PIPE " + nbrToString(count) + " BYTES !" + RESET);
		/**/streams.get(LOG_EVENT) << "RECEIVED:" +cgi._buffer << std::endl
			/**/<< std::endl;
		return ;
	}
	// else -> eof
	Monitor.printNewLine(ORANGE + "ENDOF PIPE"  + RESET);
	//treat info and put into cgi.request.response
	cgi.parseBuffer();
	//DEL event cgi
	EventDelete(cgi._responsePipe[0]);
	close(cgi._responsePipe[0]);
	/**/streams.get(LOG_EVENT) << "{FD}" << cgi._client->fd << std::endl
		/**/<< std::endl;
	cgi._client->setState(EXEC);
	//MOD request back to EPOLLOUT
	EventModify(cgi._client->fd, EPOLLOUT, cgi._client);
}
