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
#include <string>
const std::string	DCRLF = "\r\n\r\n";
const std::string	CRLF = "\r\n";
const std::string	OWS = " \t";
const bool			CHUNKED = 1;

typedef enum method
{
	GET = 0,
	POST,
	DELETE,
	OTHER,
} method;

typedef enum parsing_state
{
	HEADER = 0,
	CGI,
	BODY,
	CHUNK_SIZE,
	TRAILERS,
	SEND,
} parsing_state;

class Request
{
private:
	int					_start; //pour chuncked request, pour verifier le temps
	
	std::string			_header;
	std::string			_body;
	std::string			_buffer;

	parsing_state		_state;

	method 				_method;
	std::string			_uri;
	std::string			_queryString;

public:
	Request();

	static std::string	fields[207][3];
	static void			(Request::*fctField[210])(std::string);
	static void			initFields();

	void				setState(parsing_state value);

	std::string			getHeader();
	std::string			getBody();
	std::string			getBuffer();
	parsing_state		getState();

	void				fillHeader(std::string::size_type cursor);
	void				fillBody();
	void				fillChunkedBody();
	void				appendBuffer(std::string, int start, int end);

	void				parseMethod(std::string);
	void				parseURI(std::string);

	void				getToken(std::string *header, std::string::size_type *cursor);
	int					getField(std::string::size_type *cursor);

private:
	std::string			_host;
	std::string			_accept;
	std::string			_acceptEncoding;
	std::string			_cookies;
	std::string			_language;
	std::string			_authorization;
	std::string			_ifModifiedSince;
	std::string			_contentType;
	int					_expect;
	unsigned long		_contentLength;
	bool				_transferEncoding;
	bool				_connection;
	bool				_ifModif;
	bool				_trailer;

public:

	bool				getTransferEncoding();
	bool				getTrailer();

	void				parseHost(std::string);
	void				parseAccept(std::string);
	void				parseAcceptEncoding(std::string);
	void				parseCookies(std::string);
	void				parseLanguage(std::string);
	void				parseAuthorization(std::string);
	void				parseConnection(std::string);
	void				parseIfModifiedSince(std::string);
	void				parseExpect(std::string);
	void				parseContentType(std::string);
	void				parseContentLength(std::string);
	void				parseTransferEncoding(std::string);
	void				parseTrailer(std::string);
};
