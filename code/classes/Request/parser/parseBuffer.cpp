/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_buffer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:43:32 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Cgi.hpp"
#include "statusCodes.hpp"
#include <iostream>


void	Request::parseBuffer(void)
{
	streams.get(LOG_REQUEST) << "[BUFFER BEFORE PARSING]" << std::endl
		<< this->getBuffer()
		<< std::endl;
	//can a \r or \n be alone in header???
	std::string::size_type cursor = 0;
	//header is full in buffer
	// if (this->getState() == HEADER && moveCursor(&cursor, this->getBuffer(), DCRLF))
	if (isState(HEADER) && moveCursor(&cursor, this->getBuffer(), DCRLF))
	{
		streams.get(LOG_REQUEST) << "[PARSING HEADER]" << std::endl
			<< std::endl;
		this->fillHeader(cursor);
		parseHeader();
		if (isState(ERROR))
		{
			printRequest(this);
			return;
		}
		//requete sans body
		if (this->getContentLength() == 0 && this->_length == 0 && !this->isState(CHUNKED))
		{
			this->setState(EXEC);
			printRequest(this);
			return;
		}
		this->setState(BODY);
		if (this->isState(CHUNKED))
			this->setState(CHUNK_SIZE);
	}

	if (isState(ERROR))
	{
		printRequest(this);
		return;
	}
	if (this->isState(BODY))
	{
		streams.get(LOG_REQUEST) << "[PARSING BODY]" << std::endl
			<< std::endl;
		if (this->isState(CHUNKED))
			this->fillChunkedBody();
		else
			this->fillBody();
	}
	if (this->isState(EXEC) && this->getContentLength() != this->getBody().length())
	{
		this->setStatus(Status(BAD_REQUEST, 400));
		this->setState(ERROR);
		this->setState(EXEC);
	}
	printRequest(this);
}
