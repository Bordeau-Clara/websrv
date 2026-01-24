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
	if (isState(HEADER) && moveCursor(&cursor, this->getBuffer(), DCRLF))
	{
		streams.get(LOG_REQUEST) << "[PARSING HEADER]" << std::endl
			<< std::endl;
		this->fillHeader(cursor);
		parseHeader();
		setEndOfHeaderState();
		if (isState(EXEC))
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
		//can this happen?? Does it not juste create a phantom client and we will wait for body indefinately
		if (this->isState(EXEC) && this->getContentLength() != this->getBody().length())
		{
			this->setError(Status(BAD_REQUEST, 400));
		}
	}
	printRequest(this);
}
