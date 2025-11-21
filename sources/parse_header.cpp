/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:43:32 by cbordeau          #+#    #+#             */
/*   Updated: 2025/11/21 14:17:02 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing_header.hpp"
#include "../classes/Request/Request.hpp"
#include <iostream>

void	parse_buffer(Request *request)
{
	std::string::size_type cursor = 0;
	//header is full in buffer
	if (request->getState() == HEADER && move_cursor(&cursor, request->getBuffer(), DCRLF))
	{
		request->fillHeader(cursor);
		parse_header(request);
	}
	if (request->getState() == BODY)
	{
		//fill body according to content-length and transfer-encoding (chunked)
		if (request->getTransferEncoding() == CHUNKED)
			request->fillChunkedBody();
		else
			request->fillBody();
	}
	std::cout << "++++++++++++++++" << std::endl;
	std::cout << "header is : " << request->getHeader() << std::endl;
	std::cout << "body is : " << request->getBody() << std::endl;
	std::cout << "buffer is : " << request->getBuffer() << std::endl;
}

void	parse_header(Request *request)
{
	std::string::size_type	cursor = 0;
	std::string				token;

	if (request->getHeader().empty())
		return;
	request->getToken(&token, &cursor);
	//parse_request(token, event);
	//if CGI parse_header in cgi mode
	//state CGI but same parsing function?
	
	int type;
	while (1)
	{
		if (request->getHeader().empty())
		{
			std::cout << "header emptied" << std::endl;
			break;
		}
		cursor = request->getHeader().find(":");
		if (cursor != std::string::npos)
		{
			type = request->getField(&cursor);
		}
		else
		{
			std::cout << ": not found" << std::endl;
			break;
		}
			//throw error;
		if (type < 0)
		{
			std::cout << "Type = -1" << std::endl;
			break;
		}
		request->getToken(&token, &cursor);
		if (Request::fctField[type] != NULL)
			(request->*Request::fctField[type])(token);
		else
			std::cout << "Invalid index is " << type << std::endl;
		
	}
	//check_complete_header(event);
}
