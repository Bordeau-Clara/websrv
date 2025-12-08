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

std::string	Request::getHeader() const
{
	return this->_header;
}

std::string	Request::getBody() const
{
	return this->_body;
}

std::string	Request::getBuffer() const
{
	return this->_buffer;
}

parsing_state	Request::getState() const
{
	return this->_state;
}

std::string	Request::getStatus() const
{
	return this->_status;
}

std::string	Request::getHost() const
{
	return this->_host;
}

std::string	Request::getContentType() const
{
	return this->_contentType;
}

std::string	Request::getExpect() const
{
	return this->_expect;
}

bool	Request::getTransferEncoding() const
{
	return this->_transferEncoding;
}

bool	Request::getConnection() const
{
	return this->_connection;
}

unsigned long	Request::getContentLength() const
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
