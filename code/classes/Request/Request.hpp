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

#include "FileStream.hpp"
#include "logfiles.hpp"
#include "Status.hpp"

extern FileStream	streams;
int				find_type(std::string str);

const bool			KEEP_ALIVE = true;
const bool			CLOSE = false;
const unsigned int			TAB_SIZE = 240;

class Location;
class Server;
struct FileEntry;
#include "Cgi.hpp"

struct Response
{
	std::string				body;
	std::string				str;
	std::string::size_type	cursor;
	Response(void): body(), str(), cursor(0){}
	// get next str to send
	// move cursor
	const std::string		get(std::string::size_type size)
	{
		std::string sub = this->str.substr(this->cursor, size);
		cursor += sub.size();
		return (sub);
	}
	// check if cursor reach size of str to transmitt
	bool	transmissionComplete(void) const
	{
		return (this->str.size() == cursor);
	}
};

class Request: public Event
{
public://epollloop variable for accept
	struct sockaddr_in client_addr;
	socklen_t client_len;
	std::string	ip_str;
	int fd;
public:
	Request(Server&);
	~Request(void);

	const Location*	_location;
	Server&			_server;


	static std::string	fields[TAB_SIZE][3];
	static void			(Request::*fctField[TAB_SIZE])(std::string);
	static void				initFields();

	void					resetRequest();
private:
	int				_start; //pour chuncked request, pour verifier le temps

private:
	uint8_t			_state;
public:
	void					setState(parsing_state new_state);
	bool					isState(parsing_state new_state) const;
	std::string				getState() const;
	void					setEndOfHeaderState();

private:
	Status			_status; //to put in response
public:
	void					setStatus(const Status &);
	const Status			&getStatus() const;
	void					setError(const Status &);
	
private:
	std::string		_header;
public:
	std::string				getHeader() const;
	void					fillHeader(std::string::size_type cursor);
	void					parseHeader(void);
	void					parseHeaderRegular(void);
	void					parseHeaderCgi(void);
	int						getToken(std::string *header);
	int						getField(int *type);
	int						getField(std::string *field, int *type);
	int						getStatusLine(std::string *header);
	void					checkFullHeader();

private:
	std::string		_body;
public:
	std::string				getBody() const;
	void					fillBody();
	void					fillChunkedBody();
	unsigned long			getChunkLength(std::string::size_type cursor);
	void					putChunkInBody(unsigned long chunk_size);
	unsigned long			hexToLong(std::string line);
	void					setTrailers(std::string::size_type cursor);

private:
	std::string		_buffer;
public:
	std::string				getBuffer() const;
	void					appendBuffer(char *, int size);
	void					parseBuffer(void);
	void					skipCrlf(void);

public:
	struct Response	_response;
public:
	// void					collectEntries(std::string, std::vector<FileEntry>&, std::string);
	// void					generateTreeHTML(std::string, int, std::string&);
	void					generateAllFoldersContent(std::string, std::string&);
	void					generateSidebarHTML(std::string, int, std::string&);
	bool					recursiveReaddir(std::string);
	void					generateResponse();
	void					buildErrorResponse();
	bool					findErrorPage();
	void					buildGetResponse();
	void					buildPostResponse();
	void					buildDeleteResponse();
// utils
	void					generateRequestLine();
	void					appendConnection(void);
	void					appendContentType(void);
	void					appendCookie(void);
	void					headerEnd(void);

private:
	method 			_method;
	std::string		_uri;
	std::string		_url;//need to keep the URI for the CGI so put the full file path here
	std::string		_requestedRessource;
	std::string		_queryString;
public:
	void					parseRequestLine(std::string token);
	method					getMethod() const;
	std::string				getUri() const;
	std::string				getQueryString() const;
	void					parseMethod(std::string);
	void					parseURI(std::string);
	void					checkURI(std::string&);
	void					isCGI(void);

private:
	Cgi					*_cgi;
	time_t				_lastTimeStamp;
public:
	void				editTime(void);
	time_t				getLastTime(void);
	bool				timeOut(unsigned int);
	Cgi						*getCgi();

//faire une classe de header
private:
	std::string		_host; //inutile mais obligatoire
	std::string		_cookies;
	std::string		_contentType; //que utile pour POST
	unsigned long	_contentLength;
	bool			_length;
	bool			_connection;
	bool			_trailer;

public:
	std::string				getHost() const;
	std::string				getCookies() const;
	std::string				getContentType() const;
	unsigned long			getContentLength() const;
	bool					getTransferEncoding() const;
	bool					getConnection() const;
	bool					getTrailer() const;

	void					parseHost(std::string);
	void					parseCookies(std::string);
	void					parseConnection(std::string);
	void					parseContentType(std::string);
	void					parseContentLength(std::string);
	void					parseTransferEncoding(std::string);
	void					parseTrailer(std::string);
};

std::ostream	&operator<<(std::ostream &lhs, const Request &rhs);

int		moveCursor(std::string::size_type *cursor, std::string str, std::string toFind);
int		moveCursor(std::string::size_type *cursor, std::string str, int start, std::string toFind);
void	printRequest(Request *request);
