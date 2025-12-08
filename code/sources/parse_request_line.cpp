/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request_line.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 11:33:51 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing_header.hpp"

void	parse_request_line(Request *request, std::string token)
{
	//parser methode
	//reconstituer/parser URI
	//check HTTP/1.1
	
	std::string::size_type cursor= 0;

	if (move_cursor(&cursor, token, " "))
	{
		request->parseMethod(token.substr(0, cursor));
		token.erase(0, cursor + 1);
		if (!request->getStatus().empty())
			return;
	}
	else
	{
		request->setStatus(BAD_REQUEST);
		return;
	}
	if (move_cursor(&cursor, token, " "))
	{
		request->parseURI(token.substr(0, cursor));
		token.erase(0, cursor + 1);
		if (!request->getStatus().empty())
			return;
	}
	else
	{
		request->setStatus(BAD_REQUEST);
		return;
	}
	if (token.compare("HTTP/1.1"))
	{
		request->setStatus(BAD_REQUEST);
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Wrong HTTP protocol:" << token
			<< std::endl;
	}
}
