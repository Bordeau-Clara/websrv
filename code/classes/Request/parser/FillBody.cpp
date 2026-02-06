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
		streams.get(LOG_REQUEST) << "[PARSING NORMAL BODY]" << std::endl
			<< std::endl;
	
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
		streams.get(LOG_REQUEST) << "[STATE]" << std::endl
			<< "Client state has been put in SEND mode"
			<< std::endl;
	}
}

void	Request::fillChunkedBody()
{
		streams.get(LOG_REQUEST) << "[PARSING CHUNKED BODY]" << std::endl
			<< std::endl;
	static unsigned long	chunk_size = 0;
	std::string::size_type	cursor = 0;

	while(1)
	{
		if (this->_body.size() > this->_location->getClientMaxBodySize())
		{
		streams.get(LOG_REQUEST) << "[CHUNK BODY]" << std::endl
			<< "body size exceded mak body size"
			<< std::endl;
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
			streams.get(LOG_REQUEST) << "[CHUNK SIZE]" << std::endl
				<< "no chunk size"
				<< std::endl;
			this->setError(Status(BAD_REQUEST, 400));
			return 0;
	}
	//verifie que tout les chiffre font parti de la base 16
	for(std::string::iterator it = line.begin(); *it != ';' && it != line.end(); ++it)
	{
		if (hex.find(*it) == std::string::npos)
		{
			streams.get(LOG_REQUEST) << "[CHUNK SIZE]" << std::endl
				<< "character not in hexa base"
				<< std::endl;
			this->setError(Status(BAD_REQUEST, 400));
			return 0;
		}
	}

	chunk_size = std::strtoul(line.data(), semicolon ? (char**)&semicolon : NULL, 16);

		streams.get(LOG_REQUEST) << "[CHUNK SIZE]" << std::endl
			<< "octet to read: " << chunk_size
			<< std::endl;
	return chunk_size;
}

void	Request::putChunkInBody(unsigned long chunk_size)
{
		streams.get(LOG_REQUEST) << "[CHUNK READING]" << std::endl
			<< "size:" << chunk_size
			<< std::endl;
	this->_body.append(this->_buffer, 0, chunk_size);
	if (this->_buffer[chunk_size ] != '\r' && this->_buffer[chunk_size + 1] != '\n')
	{
		streams.get(LOG_REQUEST) << "[CHUNK READING]" << std::endl
			<< "no CRLF at buffer[chunk-size]; char is:" << _buffer[chunk_size]
			<< "body is:" << _body
			<< std::endl;
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
}
