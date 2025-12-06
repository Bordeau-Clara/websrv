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

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

#define MAX_EVENTS 10
#include <sys/epoll.h>

class	Location;

class	Server;
class EventManager
{
	public:
		EventManager(std::vector<Server>&);
		~EventManager(void){};
		void			run(void);
	
		void				getNewEvent(void);
		void				*getPtr(void);
		struct epoll_event	&getEvent(void);
		void				eventNext(void);
		bool				checkEvent(void);

		void			serverAccept(void);
		void			handleClient(void);
	private:
		int					_fd;
		struct epoll_event	_events[MAX_EVENTS];
		int					_nEvent;
		int					_it;
};

static const int	BUFFER_SIZE = 1024;
