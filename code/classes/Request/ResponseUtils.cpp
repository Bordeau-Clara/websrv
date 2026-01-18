/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:18:01 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"
#include "Location.hpp"
#include "requestDefines.hpp"
#include "stateMachine.hpp"
#include "statusCodes.hpp"
#include <cstddef>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>
#include "helpers.hpp"

void	Request::generateRequestLine(void)
{
	this->_response.str.append("HTTP/1.1 " + getStatus().str + CRLF);
}

void	Request::appendConnection(void)
{
	if (this->_connection == KEEP_ALIVE)
		this->_response.str.append(CON_KEEP_ALIVE);
	else
		this->_response.str.append(CON_CLOSE);
}

void	Request::headerEnd(void)
{
	this->_response.str.append(CRLF);
}

bool	Request::findErrorPage(void)
{
	std::map<int, std::string>::const_iterator	errorPage = this->_location->getErrorPages().find(_status.code);

	if (errorPage == this->_location->getErrorPages().end())
		return (false);
	if (access(errorPage->second.c_str(), R_OK))
		return (false);
	_response.body = extractStr(errorPage->second.c_str());
	return (true);
}
