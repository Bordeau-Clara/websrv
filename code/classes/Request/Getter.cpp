/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Getter.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:00:07 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::string	Request::getHeader()
{
	return this->_header;
}

std::string	Request::getBody() const
{
	return this->_body;
}

std::string	Request::getBuffer()
{
	return this->_buffer;
}

parsing_state	Request::getState()
{
	return this->_state;
}

bool	Request::getTransferEncoding()
{
	return this->_transferEncoding;
}

bool	Request::getConnection()
{
	return this->_connection;
}

int	Request::getContentLength()
{
	return this->_contentLength;
}

method	Request::getMethod() const
{
	return this->_method;
}

std::string	Request::getUri() const
{
	return this->_uri;
}

std::string	Request::getQueryString() const
{
	return this->_queryString;
}
