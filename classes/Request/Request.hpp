/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 14:38:54 by cbordeau          #+#    #+#             */
/*   Updated: 2025/10/21 13:23:05 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

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
	int _start; //pour chuncked request, pour verifier le temps
	
	std::string	_header;
	std::string	_body;
	std::string	_bufferNextRequest;

	bool _hEnd;
	bool _bEnd;

	method _method;
	std::string	_uri;

	std::string	_host;
	std::string	_accept;
	std::string	_acceptEncoding;
	std::string	_cookies;
	std::string	_language;
	std::string	_authorization;

	bool	_connection;

public:
	Request();

	void	appendHeader(std::string, int start, int end);
	void	appendBody(std::string, int start, int end);
	void	appendBuffer(std::string, int start, int end);
	void	set_hEnd(bool value);
	void	set_bEnd(bool value);

	std::string	*getHeader();
	std::string	*getBody();
	bool		get_hEnd();
	bool		get_bEnd();
};

class Get : public Request
{
private:
	bool			_ifModif;
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
