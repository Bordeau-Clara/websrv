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
#include "string.hpp"
#include "helpers.hpp"

void	EventManager::handlePipe()
{
	Cgi &cgi = *(Cgi *)getPtr();
	static char buffer[BUFFER_SIZE] = {0};
	ssize_t count = read(cgi._responsePipe[0], buffer, sizeof(buffer));
	if (count == -1)
		throw (std::runtime_error("RECV KO"));
	if (count == 0) // client has closed connection
	{
		Monitor.printNewLine(RED + "ENDOF PIPE"  + RESET);
		EventDelete(cgi._responsePipe[0]);
		delete (Request *)getPtr();
		return ;
	}
	cgi._buffer.append(buffer, count);
	Monitor.printNewLine("RECV FROM PIPE" + nbrToString(count) + "BYTES !");
	/**/streams.get(LOG_EVENT) << "RECEIVED:" +cgi._buffer << std::endl
		/**/<< std::endl;
	//treat info and put into cgi.request.response
	//when finished DEL event cgi and EPOLL_CTL_MOD en EPOLLOUT cgi.request
}
