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
#include "parsing_header.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <sys/socket.h>


// Request::Request() : _status(), _state(HEADER), _method(OTHER), _connection(1), _trailer(0)
// {
// }

Request::Request(Server &server) :Event(CLIENT), client_len(sizeof(sockaddr_in)), _status(), _state(HEADER), _method(OTHER), _server(server), _contentLength(0), _connection(KEEP_ALIVE), _trailer(0)
{
}

void	Request::resetRequest()
{
	this->_body.assign("");
	this->_status.assign("");
	this->_state = HEADER;
	this->_method = OTHER;
	this->_contentLength = 0;
	this->_connection = KEEP_ALIVE;
	this->_trailer = 0;

	//add everything that has been modified
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
	if (*type == 0)
		return 1;
	if (*type == -1)
		return 0;
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

// 
void Request::resolveURL(void)
{
	//DO NOT modify request->_uri put it in _path
	;
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
	resolveURL();
}

std::ostream	&operator<<(std::ostream &lhs, const Request &rhs)
{
	lhs << "method="
		<< rhs.getMethod() << std::endl
		<< "URI="
		<< rhs.getUri() << std::endl
		<< "Query string="
		<< rhs.getQueryString() << std::endl
		<< "Body="
		<< rhs.getBody() << std::endl;
	return (lhs);
}
