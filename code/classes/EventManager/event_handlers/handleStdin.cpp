/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverAcceptClient.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:24:34 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"

#include <fcntl.h>
#include <unistd.h>
#include "helpers.hpp"

void	EventManager::handleStdin(void)
{
	static char	buffer[BUFFER_SIZE + 1];

	ssize_t	rbytes = read(0, buffer, BUFFER_SIZE);
	if (!rbytes || rbytes == -1)
	{
		DashBoard.log(GREEN + "Stopping webserv"  + RESET);
	/**/streams.get(LOG_EVENT) << "[ENDING WEBSERV]" << std::endl
		/**/<< std::endl;
		_alive = false;
	}
}
