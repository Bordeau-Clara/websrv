/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 14:38:54 by cbordeau          #+#    #+#             */
/*   Updated: 2025/10/19 15:31:16 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

typedef enum method
{
	GET = 0,
	POST,
	DELETE,
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
};

class Get : public Request
{
	std::string	_ifModifiedSince;
};

class Post : public Request
{
	int			_expect;
	std::string	_contentType;
	int			_contentLength;
	bool		_transfer_encoding;
};

class Delete : public Request
{
	std::string	_contentType;
	int			_contentLength;
	bool		_transfer_encoding;
};
