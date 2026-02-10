/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FillBody.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:41:39 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Location.hpp"
#include "stateMachine.hpp"
#include "statusCodes.hpp"

void	Request::fillBody()
{
	if ((this->_body.size() + this->_buffer.size()) <= this->_contentLength)
	{
		this->_body.append(this->_buffer, 0, this->_buffer.size());
		this->_buffer.erase(0, this->_buffer.size());
	}
	else
	{
		this->_body.append(this->_buffer, 0, this->_contentLength - this->_body.size());
		this->_buffer.erase(0, this->_contentLength - this->_body.size());
	}
	if (this->_body.size() == this->_contentLength)
	{
		this->setState(EXEC);
	}
}

void	Request::fillChunkedBody()
{
	static unsigned long	chunk_size = 0;
	std::string::size_type	cursor = 0;

	while(1)
	{
		if (this->_body.size() > this->_location->getClientMaxBodySize())
		{
			this->setError(Status(PAYLOAD_TOO_LARGE, 413));
			return;
		}
		if (this->isState(CHUNK_SIZE))
		{
			if (!moveCursor(&cursor, this->_buffer, CRLF))
				break;
			chunk_size = getChunkLength(cursor);
			if (isState(EXEC))
				return;
			continue;
		}
		if (this->isState(OCTET) && this->_buffer.size() >= chunk_size + 2)
		{
			this->putChunkInBody(chunk_size);
			if (isState(EXEC))
				return;
			continue;
		}
		if (this->isState(TRAILERS))
		{
			if (!moveCursor(&cursor, this->_buffer, DCRLF))
				break;
			setTrailers(cursor);
			return;
		}
		break;
	}
}

#include "helpers.hpp"

unsigned long	Request::getChunkLength(std::string::size_type cursor)
{
	std::string				line;
	unsigned long	chunk_size = 0;

	line.assign(this->_buffer.substr(0, cursor));
	this->_buffer.erase(0, line.size() + 2);
	chunk_size = hexToLong(line);
	if (isState(ERROR))
		return 0;
	if (chunk_size == 0 && this->_trailer)
		this->setState(TRAILERS);
	else if (chunk_size == 0)
	{
		this->_buffer.erase(0, 2);
		this->setState(EXEC);
		this->_contentLength = this->_body.size();
		if (isState(CGI))
		{
			this->_cgi->addFields("Content-Length:", nbrToString(this->_contentLength));
		}
		return chunk_size;
	}
	else
		this->setState(OCTET);
	return chunk_size;
}

const std::string hex = "0123456789abcdefABCDEF";
unsigned long Request::hexToLong(std::string line)
{
	unsigned long chunk_size;
	const char* semicolon = std::strchr(line.data(), ';');

	if (line.empty())
	{
			this->setError(Status(BAD_REQUEST, 400));
			return 0;
	}
	//verifie que tout les chiffre font parti de la base 16
	for(std::string::iterator it = line.begin(); *it != ';' && it != line.end(); ++it)
	{
		if (hex.find(*it) == std::string::npos)
		{
			this->setError(Status(BAD_REQUEST, 400));
			return 0;
		}
	}

	chunk_size = std::strtoul(line.data(), semicolon ? (char**)&semicolon : NULL, 16);

	return chunk_size;
}

void	Request::putChunkInBody(unsigned long chunk_size)
{
	this->_body.append(this->_buffer, 0, chunk_size);
	if (this->_buffer[chunk_size ] != '\r' && this->_buffer[chunk_size + 1] != '\n')
	{
		this->setError(Status(BAD_REQUEST, 400));
		return;
	}
	this->_buffer.erase(0, chunk_size + 2);
	this->setState(CHUNK_SIZE);
}

void	Request::setTrailers(std::string::size_type cursor)
{
	this->_buffer.erase(0, cursor + 3);
	this->setState(EXEC);
	this->_contentLength = this->_body.size();
	if (isState(CGI))
	{
		this->_cgi->addFields("Content-Length:", nbrToString(this->_contentLength));
	}
}
