/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:43:32 by cbordeau          #+#    #+#             */
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

void	parse_request(Request *request, std::string token)
{
	//parser methode
	//reconstituer/parser URI
	//check HTTP/1.1
	
	std::string::size_type cursor= 0;

	if (move_cursor(&cursor, token, " "))
	{
		request->parseMethod(token.substr(0, cursor));
		token.erase(0, cursor + 1);
	}
	else
	{
		//error, throw ?
	}
	if (move_cursor(&cursor, token, " "))
	{
		request->parseURI(token.substr(0, cursor));
		token.erase(0, cursor + 1);
	}
	else
	{
		//error, throw ?
	}
	if (token.compare("HTTP/1.1"))
	{
		std::cout << "Wrong HTTP protocol :" + token << std::endl;
		//error, throw
	}
}

void	parse_header(Request *request)
{
	std::string::size_type	cursor = 0;
	std::string				token;

	if (request->getHeader().empty())
		return;
	request->getToken(&token, &cursor);
	parse_request(request, token);
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
		if (move_cursor(&cursor, request->getHeader(), ":"))
			type = request->getField(&cursor);
		else
		{
			std::cout << ": not found" << std::endl;
			break; //throw error?
		}
		if (type < 0)
		{
			std::cout << "Type = -1" << std::endl;
			break; //throw error?
		}
		request->getToken(&token, &cursor);
		if (Request::fctField[type] != NULL)
			(request->*Request::fctField[type])(token);
		else
			std::cout << "Invalid index is " << type << std::endl;
		
	}
	//check_complete_header(event);
	if (request->getTransferEncoding() == CHUNKED)
		request->setState(CHUNK_SIZE);
	else
		request->setState(BODY);
}
