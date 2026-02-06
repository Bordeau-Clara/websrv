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
#include "stateMachine.hpp"

bool	EventManager::recvBuffer(Request &client)
{
	static char buffer[BUFFER_SIZE] = {0};

	ssize_t count = recv(client.fd, buffer, sizeof(buffer), 0); // kesako
	if (count == -1)
	{
		std::cerr << "Erreur recv: " << strerror(errno) << " (code: " << errno << ")"<< std::endl;
		if (errno == 104)
		{
			Monitor.printNewLine(RED + "END FROM "+client.ip_str+" connection:CLOSE (client disconnected)"  + RESET);
			EventDelete(client.fd);
			delete (Request *)getPtr();
			this->requests.remove((Request *)getPtr());
			return (false);
		}
		throw (std::runtime_error("RECV KO"));
	}
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
	client.editTime();
	if (!recvBuffer(client)) // if nothing has been received
		return ;
	client.parseBuffer();
	if (!client.isState(EXEC))// if parsing is not finished
		return ;
	/**/streams.get(LOG_EVENT) << "end of the parsing" << std::endl
		/**/<< std::endl;
	if (client.isState(CGI) && !client.isState(ERROR))
	{
		/**/streams.get(LOG_EVENT) << "{IN STATE CGI/ IN IF DANS RECVFROMCLIENT}" << std::endl
			/**/<< std::endl;
		client.setState(READ);
		Cgi	*cgi = client.getCgi();
		//pipe
		Monitor.printNewLine("Initializing a pipe");
		cgi->init();
		// desarme client
		EventModify(client.fd, 0, &client);
		//fork
		if (!cgi->start(*this))
		{
			this->_alive = false;
			return;
		}
		// listen to cgi pipe
		EventAdd(cgi->_responsePipe[0], EPOLLIN, cgi);
		client.editTime();
	}
	else // regular mode
	{
		client.generateResponse();
		EventModify(client.fd, EPOLLOUT, &client);
	}
}
