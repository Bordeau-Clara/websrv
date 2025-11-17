/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:30:23 by cbordeau          #+#    #+#             */
/*   Updated: 2025/11/17 17:09:29 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../HeaderParser/HeaderParser.hpp"
#include <cstring>

Request::Request() : _hEnd(0), _bEnd(0), _method(OTHER), _connection(1)
{
}

void	Request::appendHeader(std::string str, int start, int end)
{
	this->_header.append(str, start, end);
}

void	Request::appendBody(std::string str, int start, int end)
{
	this->_body.append(str, start, end);
}

void	Request::appendBuffer(std::string str, int start, int end)
{
	this->_buffer.append(str, start, end);
}

void	Request::set_hEnd(bool value)
{
	this->_hEnd = value;
}

void	Request::set_bEnd(bool value)
{
	this->_bEnd = value;
}

std::string	Request::getHeader()
{
	return this->_header;
}

std::string	Request::getBody()
{
	return this->_body;
}

std::string	Request::getBuffer()
{
	return this->_buffer;
}

bool		Request::get_hEnd()
{
	return this->_hEnd;
}

bool		Request::get_bEnd()
{
	return this->_bEnd;
}

void	Request::tokenize(std::string::size_type cursor, int mode)
{
	if (mode == 0)
	{
		this->_hEnd = 1;
		this->_header.append(this->_buffer, 0, cursor);
		this->_buffer.erase(0, cursor + 3);
	}
	if (mode == 1)
	{
		this->_bEnd = 1;
		this->_body.append(this->_buffer, 0, cursor);
		this->_buffer.erase(0, cursor + 3);
	}
}

void	Request::getToken(std::string *token, std::string::size_type *cursor)
{
	*cursor = this->_header.find(CRLF);
	if (*cursor != std::string::npos)
	{
		token->assign(this->_header, 0, *cursor);
		*cursor += 2;
		this->_header.erase(0, *cursor);
	}
	// else
	// 	;
		//throw error;
}

int	find_type(std::string str)
{
	int index = 0;
	for (int i = 0; str[i] !=':'; i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			index += str[i] - 97;
		}
		else
			index += str[i];
	}
	if (index <= 0 || index > 207)
		return -1;
	for (int i = 0; i < 3; i++)
	{
	if (!HeaderParsing::fields[index][i].empty() && !HeaderParsing::fields[index][i].compare(str))
		return index + i;

	}
	return -1;
}

int	Request::getField(std::string::size_type *cursor)
{
	int	type;
	type = find_type(this->_header.substr(0, *cursor));
	// *cursor += 1;
	this->_header.erase(0, *cursor);
	// *cursor = this->_header.find(CRLF);
	return type;
}
