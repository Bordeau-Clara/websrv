/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:30:23 by cbordeau          #+#    #+#             */
/*   Updated: 2025/10/21 14:22:10 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() : _hEnd(0), _bEnd(0), _method(OTHER), _connection(1)
{
}

void	Request::appendHeader(std::string str, int start, int end)
{
	this->_header.append(str, start, end);
}

void	Request::appendBody(std::string str, int start, int end)
{
	this->_body.append(str, start, end);
}

void	Request::appendBuffer(std::string str, int start, int end)
{
	this->_bufferNextRequest.append(str, start, end);
}

void	Request::set_hEnd(bool value)
{
	this->_hEnd = value;
}

void	Request::set_bEnd(bool value)
{
	this->_bEnd = value;
}

std::string	*Request::getHeader()
{
	return &this->_header;
}

std::string	*Request::getBody()
{
	return &this->_body;
}

std::string	*Request::getBuffer()
{
	return &this->_bufferNextRequest;
}

bool		Request::get_hEnd()
{
	return this->_hEnd;
}

bool		Request::get_bEnd()
{
	return this->_bEnd;
}
