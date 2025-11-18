/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 14:38:54 by cbordeau          #+#    #+#             */
/*   Updated: 2025/11/18 15:38:42 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
const std::string DCRLF = "\r\n\r\n";
const std::string CRLF = "\r\n";
const std::string OWS = " \t";

typedef enum method
{
	GET = 0,
	POST,
	DELETE,
	OTHER,
} methode;

class Request
{
protected:
	int			_start; //pour chuncked request, pour verifier le temps
	
	std::string	_header;
	std::string	_body;
	std::string	_buffer;

	bool		_hEnd;
	bool		_bEnd;

	method 		_method;
	std::string	_uri;

	std::string	_host;
	std::string	_accept;
	std::string	_acceptEncoding;
	std::string	_cookies;
	std::string	_language;
	std::string	_authorization;

	bool		_connection;

public:
	Request();

	static void initFields();
	static std::string fields[207][3];
	static void (Request::*ptr[210])(std::string);

	void		appendHeader(std::string, int start, int end);
	void		appendBody(std::string, int start, int end);
	void		appendBuffer(std::string, int start, int end);
	void		set_hEnd(bool value);
	void		set_bEnd(bool value);

	std::string	getHeader();
	std::string	getBody();
	std::string	getBuffer();
	bool		get_hEnd();
	bool		get_bEnd();

	void		tokenize(std::string::size_type cursor, int mode);
	void		getToken(std::string *header, std::string::size_type *cursor);
	int			getField(std::string::size_type *cursor);

	void		parseHost(std::string);
	void		parseAccept(std::string);
	void		parseAcceptEncoding(std::string);
	void		parseCookies(std::string);
	void		parseLanguage(std::string);
	void		parseAuthorization(std::string);
	void		parseConnection(std::string);

	void		parseIfModifiedSince(std::string);

	void		parseExpect(std::string);
	void		parseContentType(std::string);
	void		parseContentLength(std::string);
	void		parseTransferEncoding(std::string);
};

class Get : public Request
{
private:
	bool		_ifModif;
	std::string	_ifModifiedSince;
public:
	Get();
};

class Post : public Request
{
private:
	int			_expect;
	std::string	_contentType;
	int			_contentLength;
	bool		_transfer_encoding;

public:
	Post();
};

class Delete : public Request
{
private:
	std::string	_contentType;
	int			_contentLength;
	bool		_transfer_encoding;
public:
	Delete();
};
