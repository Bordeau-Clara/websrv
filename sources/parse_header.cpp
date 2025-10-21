/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:43:32 by cbordeau          #+#    #+#             */
/*   Updated: 2025/10/21 13:43:32 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing_header.hpp"

void	parse_header(std::string *header, Request *request);


void	parse_buffer(std::string *buffer, Request *request)
{
	std::string::size_type cursor = 0;
	//header is full in buffer
	if (move_cursor(&cursor, *buffer, DCRLF) && !request->get_hEnd())
	{
		tokenize(buffer, request, cursor, 0);
		//body is full in buffer
		if (move_cursor(&cursor, *buffer, DCRLF))
		{
			tokenize(buffer, request, cursor, 1);
			request->appendBuffer(*buffer, 0, buffer->length());
			//parse_body(request->getBody, request);
		}
		else
			tokenize(buffer, request, 1);
		parse_header(request->getHeader(), request);
		//will throw, try parse_buffer catch
	}
	else if (!request->get_hEnd())
		tokenize(buffer, request, 0);
	if (request->get_hEnd() && !request->get_bEnd() && move_cursor(&cursor, *buffer, DCRLF))
	{
		tokenize(buffer, request, cursor, 1);
		//parse_body(request->getBody, request);
	}
	else if (request->get_hEnd())
		tokenize(buffer, request, 1);
}

int	find_type(std::string str, int end)
{
	//map.at(token)?
	//strncmp(str, tab[i], end);
	return -1;
}

void	parse_header(std::string *header, Request *request)
{
	std::string::size_type	cursor = 0;
	std::string				token;

	get_token(header, &token, &cursor);
	//parse_request(token, event);
	int type;
	while (1)
	{
		cursor = header->find(":");
		if (cursor != std::string::npos)
		{
			type = find_type(*header, cursor);
			cursor += 1;
			header->erase(0, cursor);
			cursor = header->find(CRLF);
		}
		else
			;
			//throw error;
		if (type < 0)
			break;
		get_token(header, &token, &cursor);
		//fct[type](token, event);
		
	}
	//check_complete_header(event);
}
