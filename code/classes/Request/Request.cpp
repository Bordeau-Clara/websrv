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

Request::Request(Server &server) :Event(CLIENT), client_len(sizeof(sockaddr_in)), fd(-1), _status(), _state(HEADER), _method(OTHER), _server(server), _contentLength(0), _length(0), _transferEncoding(0), _connection(KEEP_ALIVE), _trailer(0)
{
	this->fd = accept(server.getFd(), (struct sockaddr *)&this->client_addr, &this->client_len);
	if (this->fd == -1)
	{
		throw(std::runtime_error("Accept failed !"));
	}
}

#include <unistd.h>
Request::~Request(void)
{
	if (this->fd != -1)
		close(this->fd);
}

void	Request::resetRequest()
{
	this->_body.clear();
	this->_header.clear();
	this->_status.clear();
	this->_state = HEADER;
	this->_method = OTHER;
	this->_contentLength = 0;
	this->_length = 0;
	this->_transferEncoding = 0;
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

void	Request::setStatus(std::string code)
{
	this->_status.assign(code);
}

void	Request::fillHeader(std::string::size_type cursor)
{
	// this->_state = BODY;
	this->_header.append(this->_buffer, 0, cursor + 2);
	this->_buffer.erase(0, cursor + 4);
}

int	Request::getToken(std::string *token)
{
	streams.get(LOG_REQUEST) << "Token before assign is " << *token << std::endl;
	int	Ows = 0;
	std::string::size_type	cursor = 0;

	while (this->_header[Ows] && OWS.find(this->_header[Ows]) != std::string::npos)
		Ows++;

	if (!move_cursor(&cursor, this->_header, CRLF))
	{
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "CRLF has not been find to complete token"
			<< std::endl;
		return 0;
		//OR Edit status and return? How to deal with expect? Put in a string and check at response construction?
	}
	token->assign(this->_header, Ows, cursor - Ows);
	cursor += 2;
	this->_header.erase(0, cursor);
	streams.get(LOG_REQUEST) <<"header=" << _header << std::endl;
	return 1;
}

int	Request::getField(int *type)
{
	std::string field;
	std::string::size_type	cursor = 0;
	if (!move_cursor(&cursor, this->_header, ":"))
	{
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "':' has not been find to complete field"
			<< std::endl;
		return 0;
		//OR Edit status and return? How to deal with expect? Put in a string and check at response construction?
	}
	cursor += 1;
	field.assign(this->_header.substr(0, cursor));
	*type = find_type(field);
	this->_header.erase(0, cursor);
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
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "':' has not been find to complete field"
			<< std::endl;
		return 0;
	}
	cursor += 1;
	field->assign(this->_header.substr(0, cursor));
	this->_header.erase(0, cursor);
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
		this->_status = BAD_REQUEST;
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Bad method identified: " << str
			<< std::endl;
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
	
	//resolve uri
	//deal with errors
	this->_uri.assign(str);
	resolveURL();
}

std::ostream	&operator<<(std::ostream &lhs, const Request &rhs)
{
	lhs	<< "state="
		<< rhs.getState() << std::endl
		<< "status="
		<< rhs.getStatus() << std::endl
		<< "method="
		<< rhs.getMethod() << std::endl
		<< "URI="
		<< rhs.getUri() << std::endl
		<< "Query string="
		<< rhs.getQueryString() << std::endl
		<< std::endl
		<< "Host= "
		<< rhs.getHost() << std::endl
		<< "Content Type="
		<< rhs.getContentType() << std::endl
		<< "Expect="
		<< rhs.getExpect() << std::endl
		<< "Content Length="
		<< rhs.getContentLength() << std::endl
		<< "Transfer Encoding =";
	if (rhs.getTransferEncoding() == CHUNKED)
		lhs << "chunked" << std::endl;
	else
		lhs << "normal body" << std::endl;
	lhs << "Connection =";
	if (rhs.getConnection() == KEEP_ALIVE)
		lhs << "keep-alive" << std::endl;
	else
		lhs << "close" << std::endl;
	lhs << std::endl
		<< "Body="
		<< rhs.getBody() << std::endl;
	return (lhs);
}
