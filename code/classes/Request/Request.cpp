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
#include "Server.hpp"
#include "helpers.hpp"
#include <sys/socket.h>
#include <sstream>

Request::Request(Server &server) :Event(CLIENT), client_len(sizeof(sockaddr_in)), fd(-1), _server(server), _state(0), _status(Status(OK, 200)), _response(), _method(OTHER), _contentLength(0), _length(0), _connection(KEEP_ALIVE), _trailer(0)
{
	this->_cgi = NULL;
	this->fd = accept(server.getFd(), (struct sockaddr *)&this->client_addr, &this->client_len);
	if (this->fd == -1)
	{
		throw(std::runtime_error("Accept failed !"));
	}
	// extract ipv4 big endian
	uint32_t ip = ntohl(this->client_addr.sin_addr.s_addr);
	
	//each byte 
	this->ip_str = nbrToString((ip >> 24) & 0xFF) + "."
		+ nbrToString((ip >> 16) & 0xFF) + "."
		+ nbrToString((ip >> 8) & 0xFF) + "."
		+ nbrToString(ip & 0xFF) + "/"
		+ nbrToString(ntohs(this->client_addr.sin_port));
}

#include <unistd.h>
Request::~Request(void)
{
	/**/streams.get(LOG_EVENT) << "[REQUEST DESTRUCTOR CALLED]" << std::endl
		/**/<< std::endl;
	if (this->fd != -1)
		close(this->fd);
	if (this->_cgi)
	{
	/**/streams.get(LOG_EVENT) << "[CGI DELETED]" << std::endl
		/**/<< std::endl;
		delete _cgi;
		_cgi = NULL;
	}
}

void	Request::resetRequest()
{
	this->_body.clear();
	this->_header.clear();
	this->_status = Status(OK, 200);

	this->_requestedRessource.clear();
	this->_queryString.clear();
	this->_url.clear();
	this->_uri.clear();

	this->_state = 0;
	this->_method = OTHER;
	this->_contentLength = 0;
	this->_length = 0;
	this->_connection = KEEP_ALIVE;
	// this->_transferEncoding = 0;
	this->_trailer = 0;
	this->_host.clear();
	this->_cookies.clear();
	this->_contentType.clear();

	this->_response = Response();
	//add everything that has been modified
	if (this->_cgi)
	{
		delete _cgi;
		_cgi = NULL;
	}
}

void	Request::appendBuffer(char * str, int size)
{
	this->_buffer.append(str, size);
	streams.get(LOG_REQUEST) << "Request Buffer SIZE:"<< this->_buffer.size() << std::endl;
	streams.get(LOG_REQUEST) << "while we read:"<< size << std::endl;
}

void	Request::setStatus(const Status &status)
{
	streams.get(LOG_REQUEST) << "STATUS: " << status.code << std::endl;
	if (this->_status.code == 200)
		this->_status = status;
}

void	Request::setError(const Status &status)
{
		this->setStatus(status);
		this->setState(ERROR);
		this->setState(EXEC);
}

void	Request::fillHeader(std::string::size_type cursor)
{
	this->_header.append(this->_buffer, 0, cursor + 2);
	this->_buffer.erase(0, cursor + 4);
}

int	Request::getToken(std::string *token)
{
	int	Ows = 0;
	std::string::size_type	cursor = 0;

	while (this->_header[Ows] && OWS.find(this->_header[Ows]) != std::string::npos)
		Ows++;

	if (!moveCursor(&cursor, this->_header, CRLF))
	{
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "CRLF has not been find to complete token"
			<< std::endl;
		return 0;
	}
	token->assign(this->_header, Ows, cursor - Ows);
	cursor += 2;
	this->_header.erase(0, cursor);
	return 1;
}

int	Request::getStatusLine(std::string *token)
{
	std::string::size_type	cursor = 0;

	if (!moveCursor(&cursor, this->_header, CRLF))
	{
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "CRLF has not been find to complete token"
			<< std::endl;
		return 0;
	}
	token->assign(this->_header, 0, cursor);
	cursor += 2;
	this->_header.erase(0, cursor);
	return 1;
}

int	Request::getField(int *type)
{
	std::string field;
	std::string::size_type	cursor = 0;
	if (!moveCursor(&cursor, this->_header, ":"))
	{
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "':' has not been find to complete field"
			<< std::endl;
		return 0;
	}
	cursor += 1;
	field.assign(this->_header.substr(0, cursor));
	streams.get(LOG_REQUEST) << "[FIELD]" << std::endl
		<< field
		<< std::endl;
	*type = find_type(field);
	streams.get(LOG_REQUEST) << "[TYPE]" << std::endl
		<< *type
		<< std::endl;
	this->_header.erase(0, cursor);
	if (*type == 0)
		return 1;
	if (*type == -1)
		return 0;
	return *type;
}

//getField() pour CGI
int	Request::getField(std::string *field, int *type)
{
	std::string::size_type	cursor = 0;
	if (!moveCursor(&cursor, this->_header, ":"))
	{
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "':' has not been find to complete field"
			<< std::endl;
		return 0;
	}
	cursor += 1;
	field->assign(this->_header.substr(0, cursor));
	streams.get(LOG_REQUEST) << "[FIELD]" << std::endl
		<< *field
		<< std::endl;
	*type = find_type(*field);
	streams.get(LOG_REQUEST) << "[TYPE]" << std::endl
		<< *type
		<< std::endl;
	if (*type == -1)
		return 0;
	if (*type == 0)
		field->clear();
	this->_header.erase(0, cursor);
	return 1;
}

std::string Request::fields[TAB_SIZE][3] = {};
void (Request::*Request::fctField[TAB_SIZE])(std::string) = {NULL};

void Request::initFields()
{
	Request::fields[26][0] = "date";
	Request::fields[40][0] = "range";
	Request::fields[42][0] = "accept";
	Request::fields[48][0] = "from";
	Request::fields[50][0] = "pragma";
	Request::fields[52][0] = "cookie";
	Request::fields[58][0] = "host";
	Request::fields[66][0] = "origin";
	Request::fields[67][0] = "expect";
	Request::fields[76][0] = "trailer";
	Request::fields[98][0] = "if-match";
	Request::fields[98][1] = "if-range";
	Request::fields[102][0] = "connection";
	Request::fields[146][0] = "user-agent";
	Request::fields[147][0] = "accept-language";
	Request::fields[150][0] = "accept-encoding";
	Request::fields[150][1] = "cache-control";
	Request::fields[154][0] = "accept-charset";
	// Request::fields[164][0] = "authorization";
	Request::fields[169][0] = "content-range";
	Request::fields[187][0] = "if-none-match";
	Request::fields[189][0] = "content-length";
	Request::fields[189][1] = "content-language";
	Request::fields[191][0] = "content-type";
	Request::fields[201][0] = "transfer-encoding";
	Request::fields[205][0] = "if-modified-since";
	Request::fields[210][0] = "content-location";
	Request::fields[238][0] = "if-unmodified-since";

	Request::fctField[58] = &Request::parseHost;
	Request::fctField[52] = &Request::parseCookies;
	Request::fctField[76] = &Request::parseTrailer;
	Request::fctField[102] = &Request::parseConnection;
	Request::fctField[189] = &Request::parseContentLength;
	Request::fctField[191] = &Request::parseContentType;
	Request::fctField[201] = &Request::parseTransferEncoding;
}

std::ostream	&operator<<(std::ostream &lhs, const Request &rhs)
{
	lhs	<< "state="
		<< rhs.getState() << std::endl
		<< "status="
		<< rhs.getStatus().str << std::endl
		<< "method="
		<< METHODS[rhs.getMethod()] << std::endl
		<< "URI="
		<< rhs.getUri() << std::endl
		<< "Query string="
		<< rhs.getQueryString() << std::endl
		<< std::endl
		<< "Host= "
		<< rhs.getHost() << std::endl
		<< "Content Type="
		<< rhs.getContentType() << std::endl
		<< "Content Length="
		<< rhs.getContentLength() << std::endl
		<< "Cookies="
		<< rhs.getCookies() << std::endl
		<< "Transfer Encoding =";
	if (rhs.isState(CHUNKED))
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
