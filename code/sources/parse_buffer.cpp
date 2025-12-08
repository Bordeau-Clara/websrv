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

#include "../includes/parsing_header.hpp"
#include "../classes/Request/Request.hpp"
#include "../classes/Cgi/Cgi.hpp"
#include "statusCodes.hpp"
#include <iostream>


void	parse_buffer(Request *request)
{
		streams.get(LOG_REQUEST) << "[BUFFER BEFORE PARSING]" << std::endl
			<< request->getBuffer()
			<< std::endl;
	//can a \r or \n be alone in header???
	std::string::size_type cursor = 0;
	//header is full in buffer
	if (request->getState() == HEADER && move_cursor(&cursor, request->getBuffer(), DCRLF))
	{
		request->fillHeader(cursor);
		parse_header_type(request);
		if (!request->getStatus().empty())
			return;
		if (request->getContentLength() == 0 && request->getTransferEncoding() != CHUNKED)
			request->setState(SEND);
		else if (request->getTransferEncoding() == CHUNKED)
			request->setState(CHUNK_SIZE);
		else
			request->setState(BODY);
	}

	if (request->getState() == BODY || request->getState() == CHUNK_SIZE || request->getState() == TRAILERS)
	{
		//fill body according to content-length and transfer-encoding (chunked)
		if (request->getTransferEncoding() == CHUNKED)
			request->fillChunkedBody();
		else
			request->fillBody();
	}
	if (request->getState() == SEND && request->getContentLength() != request->getBody().length())
	{
		request->setStatus(BAD_REQUEST);
	}
	printRequest(request);
}
