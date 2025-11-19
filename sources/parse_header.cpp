/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:43:32 by cbordeau          #+#    #+#             */
/*   Updated: 2025/11/19 17:37:23 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing_header.hpp"
#include "../classes/Request/Request.hpp"
#include <iostream>

void	parse_buffer(Request *request)
{
	std::string::size_type cursor = 0;
	//header is full in buffer
	if (move_cursor(&cursor, request->getBuffer(), DCRLF) && !request->get_hEnd())
	{
		request->tokenize(cursor, HEADER);
		parse_header(request);
		//body is full in buffer
		if (move_cursor(&cursor, request->getBuffer(), DCRLF))
		{
			request->tokenize(cursor, BODY);
		}
	}
	if (request->get_hEnd() && !request->get_bEnd() && move_cursor(&cursor, request->getBuffer(), DCRLF))
	{
		request->tokenize(cursor, BODY);
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

	// std::cout << "header at the beginning is " << request->getHeader() << std::endl;
	if (request->getHeader().empty())
		return;
	request->getToken(&token, &cursor);
	//parse_request(token, event);
	//if CGI parse_header in cgi mode
	
	// std::cout << "header after first getToken is " << request->getHeader() << std::endl;
	// std::cout << "Token is " << token << std::endl;
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
			// std::cout << "Type is " << type << std::endl;
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
		// std::cout << "header after first getField is " << request->getHeader() << std::endl;
		request->getToken(&token, &cursor);
		// std::cout << "Token is " << token << std::endl;
		if (Request::ptr[type] != NULL)
			(request->*Request::ptr[type])(token);
		else
			std::cout << "Invalid index is " << type << std::endl;
		
	}
	//check_complete_header(event);
}
