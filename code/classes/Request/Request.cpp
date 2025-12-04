/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:30:23 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../../includes/parsing_header.hpp"

Request::Request() : _status(), _state(HEADER), _method(OTHER), _connection(1), _trailer(0)
{
}

void	Request::appendBuffer(std::string str, int start, int end)
{
	this->_buffer.append(str, start, end);
}

void	Request::setState(parsing_state value)
{
	this->_state = value;
}

void	Request::fillHeader(std::string::size_type cursor)
{
	// this->_state = BODY;
	this->_header.append(this->_buffer, 0, cursor + 2);
	this->_buffer.erase(0, cursor + 4);
}

void	Request::fillBody()
{
	if ((this->_body.size() + this->_buffer.size()) <= this->_contentLength)
	{
		this->_body.append(this->_buffer, 0, this->_buffer.size());
		this->_buffer.erase(0, this->_buffer.size());
	}
	else
	{
		this->_body.append(this->_buffer, 0, this->_contentLength - this->_body.size());
		this->_buffer.erase(0, this->_contentLength - this->_body.size());
	}
	if (this->_body.size() == this->_contentLength)
		this->_state = SEND;
}

int	Request::getToken(std::string *token)
{
	// std::cout << "Token  before assign is " << *token << std::endl;
	int	Ows = 0;
	std::string::size_type	cursor = 0;

	while (this->_header[Ows] && OWS.find(this->_header[Ows]) != std::string::npos)
		Ows++;

	if (!move_cursor(&cursor, this->_header, CRLF))
	{
		std::cout << RED << "Cursor is at NULL" << WHITE << std::endl;
		return 0;
		//throw error;
		//OR Edit status and return? How to deal with expect? Put in a string and check at response construction?
	}
	token->assign(this->_header, Ows, cursor);
	// std::cout << "Token after assign is " << *token << std::endl;
	cursor += 2;
	this->_header.erase(0, cursor);
	return 1;
}

int	Request::getField(int *type)
{
	std::string field;
	std::string::size_type	cursor = 0;
	if (!move_cursor(&cursor, this->_header, ":"))
	{
		std::cout << RED << ": not found" << WHITE << std::endl;
		return 0;
		//OR Edit status and return? How to deal with expect? Put in a string and check at response construction?
	}
	cursor += 1;
	field.assign(this->_header.substr(0, cursor));
	*type = find_type(field);
	// *cursor += 1;
	this->_header.erase(0, cursor);
	// *cursor = this->_header.find(CRLF);
	return *type;
}

int	Request::getField(std::string *field)
{
	std::string::size_type	cursor = 0;
	if (!move_cursor(&cursor, this->_header, ":"))
	{
		std::cout << RED << ": not found" << WHITE << std::endl;
		return 0;
		//OR Edit status and return? How to deal with expect? Put in a string and check at response construction?
	}
	cursor += 1;
	field->assign(this->_header.substr(0, cursor));
	// *cursor += 1;
	this->_header.erase(0, cursor);
	// *cursor = this->_header.find(CRLF);
	return 1;
}

void	Request::fillChunkedBody()
{
	std::string				line;
	static unsigned long	chunk_size = 0;
	std::string::size_type	cursor = 0;

	while(1)
	{
		if (this->_state == CHUNK_SIZE && move_cursor(&cursor, this->_buffer, CRLF))
		{
			line.assign(this->_buffer.substr(0, cursor));
			this->_buffer.erase(0, line.size() + 2);
			chunk_size = hexToLong(line);
			if (chunk_size == 0 && this->_trailer)
				this->_state = TRAILERS;
			else if (chunk_size == 0)
				this->_state = SEND;
			else
				this->_state = BODY;
			continue;
		}
		if (this->_state == BODY && this->_buffer.size() >= chunk_size + 2)
		{
			//put chunk_size octets in body
			this->_body.append(this->_buffer, 0, chunk_size);
			if (this->_buffer[chunk_size ] != '\r' && this->_buffer[chunk_size + 1] != '\n')
			{
				//trow 400 Bad request
				std::cout << RED << "Number of octet not in adequation whith chunk size" << WHITE << std::endl;
			}
			//erase chunk_size octet + 2 from buffer
			this->_buffer.erase(0, chunk_size + 2);
			this->_state = CHUNK_SIZE;
			continue;
		}
		if (this->_state == TRAILERS && move_cursor(&cursor, this->_buffer, DCRLF))
		{
			this->_buffer.erase(0, cursor + 3);
			this->_state = SEND;
			continue;
		}
		break;
	}
}

void	Request::parseMethod(std::string str)
{
	if (str.compare("GET"))
		this->_method = GET;
	else if (str.compare("POST"))
		this->_method = POST;
	else if (str.compare("DELETE"))
		this->_method = DELETE;
	else
	{
		std::cout << "BAD METHOD" << std::endl;
		;//throw 400
		//ou edit status
	}
}

std::string resolveURI(std::string str)
{
	//DO NOT modify request->_uri put it in _path
	return str;
}

void	Request::parseURI(std::string str)
{
	std::string::size_type cursor = 0;
	//or # anchor???
	if (move_cursor(&cursor, str, "?"))
	{
		this->_queryString.assign(str.substr(cursor + 1));
		str.erase(cursor);
	}
	// std::cout << "uri is:" + str << std::endl;
	// std::cout << "query is is:" + this->_queryString << std::endl;
	
	//resolve uri
	this->_uri.assign(str);
	this->_path = resolveURI(str);
}
