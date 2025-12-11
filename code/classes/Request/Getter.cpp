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
#include "stateMachine.hpp"

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

std::string	Request::getState() const
{
	std::string state;
	if (this->isState(SEND))
		state.append("SEND");
	else
		state.append("READ");

	if (this->isState(CGI))
		state.append(" + CGI");

	if (this->isState(HEADER))
		state.append(" + HEADER");
	else
		state.append(" + BODY");

	if (this->isState(CHUNKED))
	{
		state.append(" + CHUNKED");

		if (this->isState(CHUNK_SIZE))
			state.append(" + CHUNKSIZE");
		else
			state.append(" + OCTET");
		if (this->isState(TRAILERS))
			state.append(" + TRAILERS");

		if (this->isState(ERROR))
			state.append(" + ERROR");
	}
	return state;
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
