/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 13:46:47 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Logger.hpp"
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <list>

#define MAX_EVENTS 10
#include <sys/epoll.h>

#include "FileStream.hpp"
extern FileStream	streams;

class	Logger;
class	Server;
class	Request;

#include "Event.hpp"
struct	StdinEvent : public Event
{
	StdinEvent() : Event(STDIN){}
	std::string	buf;
};

class EventManager
{

	public:
		EventManager(std::vector<Server>&);
		~EventManager(void);
		void			run(void);
		std::list<Request*>	requests;

	private: // all computation of webserv is in this class


		Request			&requestAdd(Server&);
		// EPOLLOUT
		void			sendToClient(void);
		// Send to client function
			//have to send by small buffers to not exceed the socket's buffer
			//Should send buffer size byte of respone and increment response cursor by buffersize.
			//return false if request not fully sent true if fully sent
			bool			sendBuffer(Request&);
		// EPOLLIN
		void			recvFromClient(void);
			bool			recvBuffer(Request&);
		void			handlePipe(void);
		void			serverAcceptClient(void);
		void			handleStdin(void);
	
		// Logger
		Logger			Monitor;
		void			monitorNewEvent(ssize_t);
		void			monitorEventRecv(ssize_t, String, Request&);

		// utils
		void				getNewEvent(void);
		void				*getPtr(void);
		struct epoll_event	&getEvent(void);
		void				eventNext(void);
		int					checkEvent(void);
		bool				eventIs(uint32_t);
		// Epoll encapsulation
		void				EventAdd(int, uint32_t, void*);
		void				EventModify(int, uint32_t, void*);
		void				EventDelete(int);
		// allocated request vector
		void(EventManager::*epollinHandler[4])(void);// jumptable
		int					_fd;
		struct epoll_event	_events[MAX_EVENTS];
		int					_nEvent;
		int					_it;
		StdinEvent			_stdin;
		bool				_alive;

		void				zombieCheck(void);
		time_t				lastZombieCheck;
};

static const int	BUFFER_SIZE = 1024;
