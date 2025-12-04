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
#include <iostream>

void	parse_buffer(Request *request)
{
	//can a \r or \n be alone in header???
	std::string::size_type cursor = 0;//cursor has to be here for fill header
	//header is full in buffer
	if (request->getState() == HEADER && move_cursor(&cursor, request->getBuffer(), DCRLF))
	{
		request->fillHeader(cursor);
		parse_header_type(request);
	}
	// if (request->getState() & (BODY | CHUNK_SIZE | TRAILERS))
	if (request->getState() == BODY || request->getState() == CHUNK_SIZE || request->getState() == TRAILERS)
	{
		//fill body according to content-length and transfer-encoding (chunked)
		if (request->getTransferEncoding() == CHUNKED)
			request->fillChunkedBody();
		else
			request->fillBody();
	}
	std::cout << "++++++++++++++++" << std::endl;
	std::cout << "header is :" << request->getHeader() << std::endl;
	std::cout << "body is :" << request->getBody() << std::endl;
	std::cout << "buffer is :" << request->getBuffer() << std::endl;
	std::cout << "++++++++++++++++" << std::endl;
}
