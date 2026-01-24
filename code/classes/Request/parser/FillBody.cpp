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
	
	//tester erreur avec content Length trop grand et trop petit
	
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
	if (chunk_size == 0 && this->_trailer)
		this->setState(TRAILERS);
	else if (chunk_size == 0)
	{
		this->_buffer.erase(0, 2);
		this->setState(EXEC);
		this->_contentLength = this->_body.size();
		return CHUNK_SIZE;
	}
	else
		this->setState(OCTET);
	return CHUNK_SIZE;
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
}

//version with logger

// void	Request::fillChunkedBody()
// {
// 		streams.get(LOG_REQUEST) << "[PARSING CHUNKED BODY]" << std::endl
// 			<< std::endl;
// 	std::string				line;
// 	static unsigned long	chunk_size = 0;
// 	std::string::size_type	cursor = 0;
//
// 	while(1)
// 	{
// 		if (this->_body.size() > this->_location->getClientMaxBodySize())
// 		{
// 			this->setError(Status(PAYLOAD_TOO_LARGE, 413));
// 			return;
// 		}
// 		if (this->isState(CHUNK_SIZE))
// 		{
// 			if (!moveCursor(&cursor, this->_buffer, CRLF))
// 				break;
// 			line.assign(this->_buffer.substr(0, cursor));
// 			this->_buffer.erase(0, line.size() + 2);
// 			chunk_size = hexToLong(line);
// 			if (chunk_size == 0 && this->_trailer)
// 				this->setState(TRAILERS);
// 			else if (chunk_size == 0)
// 			{
// 				this->_buffer.erase(0, 2);
// 				this->setState(EXEC);
// 				this->_contentLength = this->_body.size();
// 				streams.get(LOG_REQUEST) << "[STATE]" << std::endl
// 					<< "Client state has been put in SEND mode (1)"
// 					<< std::endl;
// 				return;
// 			}
// 			else
// 				this->setState(OCTET);
// 			continue;
// 		}
// 		if (this->isState(OCTET) && this->_buffer.size() >= chunk_size + 2)
// 		{
// 			streams.get(LOG_REQUEST) << "[READING OCTET]" << std::endl
// 				<< std::endl;
// 			this->_body.append(this->_buffer, 0, chunk_size);
// 			if (this->_buffer[chunk_size ] != '\r' && this->_buffer[chunk_size + 1] != '\n')
// 			{
// 				this->setError(Status(BAD_REQUEST, 400));
// 				streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
// 					<< "Number of octet till CRLF is not equal to the number of octet to read"
// 					<< std::endl;
// 				return;
// 			}
// 			this->_buffer.erase(0, chunk_size + 2);
// 			this->setState(CHUNK_SIZE);
// 			continue;
// 		}
// 		if (this->isState(TRAILERS))
// 		{
// 			if (!moveCursor(&cursor, this->_buffer, DCRLF))
// 				break;
// 			this->_buffer.erase(0, cursor + 3);
// 			this->setState(EXEC);
// 			this->_contentLength = this->_body.size();
// 			streams.get(LOG_REQUEST) << "[STATE]" << std::endl
// 				<< "Client state has been put in SEND mode (2)"
// 				<< std::endl;
// 			return;
// 		}
// 		break;
// 	}
// }
