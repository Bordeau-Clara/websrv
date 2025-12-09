/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 14:29:54 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */


#include "EventManager.hpp"
#include <sstream>

void	EventManager::monitorNewEvent(ssize_t nEvent)
{
	// Source - https://stackoverflow.com/q
	// Posted by SAK, modified by community. See post 'Timeline' for change history
	// Retrieved 2025-12-07, License - CC BY-SA 4.0
	std::stringstream ss;

	ss << nEvent << "events found !";
	Monitor.editStatusLine(ss.str());
}

void	EventManager::monitorEventRecv(ssize_t count, String recv)
{
	std::stringstream ss;
	ss << " " << count << " bytes recv";
	Monitor.printNewLine(ss.str());
	streams.get(LOG_EVENT) << " " << count << " bytes recv" << std::endl
	<< recv << std::endl; 
}
