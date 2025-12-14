/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 12:33:01 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>

#include "Event.hpp"

void	EventManager::getNewEvent(void)
{
	Monitor.editStatusLine("Polling...");
	_nEvent = epoll_wait(this->_fd, this->_events, MAX_EVENTS, 1000);
	if (_nEvent == -1)
	{
		perror("epoll_wait");
		throw (std::runtime_error("ERROR"));
	}
	_it = 0;
	// monitorNewEvent(_nEvent);
}

void	*EventManager::getPtr(void)
{
	if (_it == _nEvent)
		return (NULL);
	return (_events[_it].data.ptr);
}

void	EventManager::eventNext(void)
{
	_it++;
	if (getPtr())
		Monitor.printNewLine("next event");
}

bool	EventManager::eventIs(uint32_t mode)
{
	return (getEvent().events & mode);
}

struct epoll_event	&EventManager::getEvent(void)
{
	return(_events[_it]);
}

// check if it's a server or client
int	EventManager::checkEvent()
{
	return (static_cast<Event*>(getPtr())->_type);
}

void	EventManager::EventAdd(int event_fd, uint32_t event_mode, void *event_ptr)
{
		// Structure pour les événements
		struct epoll_event event;
		event.events = event_mode;
		event.data.ptr = event_ptr;

		if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, event_fd, &event) == -1)
		{
			perror("epoll_ctl: ADD");
			throw (std::runtime_error("ERROR"));
		}
}

void	EventManager::EventModify(int event_fd, uint32_t event_mode, void *event_ptr)
{
		// Structure pour les événements
		struct epoll_event event;
		event.events = event_mode;
		event.data.ptr = event_ptr;

		if (epoll_ctl(this->_fd, EPOLL_CTL_MOD, event_fd, &event) == -1)
		{
			perror("epoll_ctl: MOD");
			throw (std::runtime_error("ERROR"));
		}
}

void	EventManager::EventDelete(int event_fd)
{
		// Structure pour les événements
		if (epoll_ctl(this->_fd, EPOLL_CTL_DEL, event_fd, NULL) == -1)
		{
			perror("epoll_ctl: DEL");
			throw (std::runtime_error("ERROR"));
		}
}
