/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header_utils.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 13:39:14 by cbordeau          #+#    #+#             */
/*   Updated: 2025/10/21 15:12:31 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing_header.hpp"

int	move_cursor(std::string::size_type *cursor, std::string str, std::string toFind)
{
	*cursor = str.find(toFind);
	if (*cursor != std::string::npos)
		return 1;
	else
		return 0;
}

void	tokenize(std::string *buffer, Request *request, std::string::size_type cursor, int mode)
{
	if (mode == 0)
	{
		request->set_hEnd(1);
		request->appendHeader(*buffer, 0, cursor);
		buffer->erase(0, cursor + 3);
	}
	if (mode == 1)
	{
		request->set_bEnd(1);
		request->appendBody(*buffer, 0, cursor);
		buffer->erase(0, cursor + 3);
	}
}

void	tokenize(std::string *buffer, Request *request, int mode)
{
	if (mode == 0)
	{
		request->appendHeader(*buffer, 0, buffer->length());
		buffer->erase(0, buffer->length());
	}
	if (mode == 1)
	{
		request->appendBody(*buffer, 0, buffer->length());
		buffer->erase(0, buffer->length());
	}
}

void	get_token(std::string *header, std::string *token, std::string::size_type *cursor)
{
	*cursor = header->find(CRLF);
	if (*cursor != std::string::npos)
	{
		token->assign(*header, 0, *cursor);
		*cursor += 2;
		header->erase(0, *cursor);
	}
	// else
	// 	;
		//throw error;
}
