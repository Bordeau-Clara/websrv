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

class EventManager
{

	public:
		EventManager(std::vector<Server>&);
		~EventManager(void);
		void			run(void);

	private: // all computation of webserv is in this class

		Request			&requestAdd(Server&);
		// EPOLLOUT
		void			sendToClient(void);
			void			sendBuffer(Request&);
		// EPOLLIN
		void			recvFromClient(void);
			bool			recvBuffer(Request&);
		void			handlePipe(void);
		void			serverAcceptClient(void);
	
		// Logger
		Logger			Monitor;
		void			monitorNewEvent(ssize_t);
		void			monitorEventRecv(ssize_t, String);

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
		std::list<Request*>	requests;
		void(EventManager::*epollinHandler[3])(void);// jumptable
		int					_fd;
		struct epoll_event	_events[MAX_EVENTS];
		int					_nEvent;
		int					_it;
};

static const int	BUFFER_SIZE = 1024;
