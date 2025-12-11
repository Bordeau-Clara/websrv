/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 14:38:54 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <execution>
#include <netinet/in.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include "requestDefines.hpp"
#include "Event.hpp"
#include "stateMachine.hpp"
#include "statusCodes.hpp"

//C comme ca que ca marche ???
#include "FileStream.hpp"
#include "logfiles.hpp"

extern FileStream	streams;
int				find_type(std::string str);
unsigned long	hexToLong(std::string line);

// const bool			CHUNKED = 1;
const bool			KEEP_ALIVE = 1;
const bool			CLOSE = 0;
// typedef enum parsing_state
// {
// 	HEADER = 0,
// 	CGI,
// 	BODY,
// 	CHUNK_SIZE,
// 	TRAILERS,
// 	SEND,
// 	// SEND_CGI, -> so that resonse builder knows what to parse and how
// 	// or put variable in cgi or request or if *cgi==NULL?
// } parsing_state;

class Location;
class Server;
class Cgi;

class Request: public Event
{
public://epollloop variable for accept
	struct sockaddr_in client_addr;
	socklen_t client_len;
	int fd;
private:
	int					_start; //pour chuncked request, pour verifier le temps
	std::string			_status; //to put in response
	
	std::string			_header;
	std::string			_body;
	std::string			_buffer;

	std::string			_response;

	uint8_t				_state;

	method 				_method;
	std::string			_uri;
	std::string			_url;//need to keep the URI for the CGI so put the full file path here
	std::string			_queryString;

	Cgi*				_cgi;

	const Location*		_location;
	Server&				_server;

	Request(Request&);
	Request();
public:
	Request(Server&);
	~Request(void);

	static std::string	fields[207][3];
	static void			(Request::*fctField[210])(std::string);
	static void			initFields();

	void				resetRequest();

	void				setState(parsing_state new_state);
	bool				isState(parsing_state new_state) const;
	void				setStatus(std::string code);

	std::string			getHeader() const;
	std::string			getBody() const;
	std::string			getBuffer() const;
	std::string			getState() const;
	std::string			getStatus() const;

	method				getMethod() const;
	std::string			getUri() const;
	std::string			getQueryString() const;

	void				fillHeader(std::string::size_type cursor);
	void				fillBody();
	void				fillChunkedBody();
	void				appendBuffer(std::string, int start, int end);

	void				parseMethod(std::string);
	void				parseURI(std::string);

	int					getToken(std::string *header);
	int					getField(int *type);
	int					getField(std::string *field);

	void	parseBuffer(void);
	void	parseHeaderType(void);
	void	parseHeader(void);
	void	parseCgiHeader(void);
	void	parseRequestLine(std::string token);

	void	generateResponse();
	void	buildErrorResponse();
	void	isCGI(void);

private:
	std::string			_host; //inutile mais obligatoire
	std::string			_cookies;
	std::string			_contentType; //que utile pour POST
	std::string			_expect;
	unsigned long		_contentLength;
	bool				_length;
	bool				_connection;
	bool				_trailer;
	// std::string			_authorization;
	// std::string			_accept; //ignorable ou 406
	// std::string			_ifModifiedSince;
	// bool				_ifModif;

public:

	std::string			getHost() const;
	std::string			getContentType() const;
	std::string			getExpect() const;
	unsigned long		getContentLength() const;
	bool				getTransferEncoding() const;
	bool				getConnection() const;
	bool				getTrailer() const;

	void				parseHost(std::string);
	void				parseCookies(std::string);
	void				parseConnection(std::string);
	void				parseExpect(std::string);
	void				parseContentType(std::string);
	void				parseContentLength(std::string);
	void				parseTransferEncoding(std::string);
	void				parseTrailer(std::string);
	// void				parseAuthorization(std::string);
	// void				parseIfModifiedSince(std::string);
};


//faire surcharge de << pour imprimer toute la classe
std::ostream	&operator<<(std::ostream &lhs, const Request &rhs);

static const int	MAX_BODY_SIZE = 8192; //juste la le temps de le resoudre dans la config du serveur
int		moveCursor(std::string::size_type *cursor, std::string str, std::string toFind);
void	printRequest(Request *request);
