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
#include "Request.hpp"
#include "colors.hpp"
#include "helpers.hpp"
#include <sstream>

void	EventManager::monitorNewEvent(ssize_t nEvent)
{
	(void)nEvent;
	// // Source - https://stackoverflow.com/q
	// // Posted by SAK, modified by community. See post 'Timeline' for change history
	// // Retrieved 2025-12-07, License - CC BY-SA 4.0
	// std::stringstream ss;
	//
	// ss << nEvent << "events found !";
	// Monitor.editStatusLine(ss.str());
}

void	EventManager::monitorEventRecv(ssize_t count, Request &client)
{
	DashBoard.addRBytes(count);
	DashBoard.log(VIVID_MAGENTA + "FROM " + client.ip_str + ": "+nbrToString(count) + " bytes recv" + RESET);
}
