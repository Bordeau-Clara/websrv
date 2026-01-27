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
	Monitor.popStatus("Polling...");
	_nEvent = epoll_wait(this->_fd, this->_events, MAX_EVENTS, 1000);
	if (_nEvent == -1)
	{
		perror("epoll_wait");
		throw (std::runtime_error("ERROR"));
	}
	_it = 0;
	// monitorNewEvent(_nEvent);
	if (!getPtr())
		return ;
	printEvent();
}

void	*EventManager::getPtr(void)
{
		// /**/streams.get(LOG_EVENT) << "[_it]" << _it  << std::endl
		// /**/<< "[_nevents]" << _nEvent  << std::endl
		// 	/**/<< std::endl;
	if (_it == _nEvent)
		return (NULL);
	return (_events[_it].data.ptr);
}

#include "helpers.hpp"
void	EventManager::printEvent(void)
{
	int	type = checkEvent();
	switch (type)
	{
		case SRV:
		Monitor.printNewLine("handling server");
		break;
		case CLIENT:
		Monitor.printNewLine("handling client");
		break;
		case PIPE:
		Monitor.printNewLine("handling pipe");
		break;
		default:
		Monitor.printNewLine("stdin");
		;
	}
	// Monitor.printNewLine("" + nbrToString(getEvent().data.ptr) + " TYPE " + nbrToString(checkEvent()));
}

void	EventManager::eventNext(void)
{
	_it++;
	if (!getPtr())
		return ;
	printEvent();
}

bool	EventManager::eventIs(uint32_t mode)
{
		/**/streams.get(LOG_EVENT) << "[EVENT POLL TYPE]" << getEvent().events  << std::endl
		/**/<< "[EVENT PTR IS]" << this->checkEvent()  << std::endl
		/**/<< "[PTR IS]" << getEvent().data.ptr  << std::endl
			/**/<< std::endl;
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
#include "helpers.hpp"
void	EventManager::EventAdd(int event_fd, uint32_t event_mode, void *event_ptr)
{
		/**/streams.get(LOG_EVENT) << "[EVENT ADD FD]" << event_fd  << std::endl
			/**/<< std::endl;
		Monitor.printNewLine("Adding " + nbrToString(event_fd) + "in mode " + nbrToString(event_mode));
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
		/**/streams.get(LOG_EVENT) << "[EVENT MODIFY FD]" << event_fd  << std::endl
			/**/<< std::endl;
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
		/**/streams.get(LOG_EVENT) << "[EVENT DELETE FD]" << event_fd  << std::endl
			/**/<< std::endl;
		// Structure pour les événements
		if (epoll_ctl(this->_fd, EPOLL_CTL_DEL, event_fd, NULL) == -1)
		{
			perror("epoll_ctl: DEL");
			throw (std::runtime_error("ERROR"));
		}
}
