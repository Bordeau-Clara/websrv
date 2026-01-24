/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:29:17 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include "define_cgi.hpp"

class Request;
class Cgi
{
private:
	std::vector<std::string>	_env;
	std::vector<std::string>	_arg;
	std::vector<std::string>	_exec;
	bool						_contentLength;

	std::string					_buffer;
	Request&					_client;


public:
	Cgi();
	int							_responsePipe[2];
	int							_bodyPipe[2];
	Cgi(Request&); //prendre adresse de request

	void						createBasicEnv();

	void						addFields(std::string field, std::string token);//check for host, type, length and or add
	void						getFieldFromUri(Request *request);//to call in constructor -> no
	//do function in request createCgi() to add uri, methode, query without getters

	// void				parseCgiHost(std::string field, std::string token);
	// void				parseCgiContentType(std::string);
	// void				parseCgiContentLength(std::string); //do after body parsing? if chunked do after?
};

	//variable static:
				//GATEWAY_INTERFACE
				//SERVER_PROTOCOL
				//SERVER_SOFTWARE (voir chat)
	//leave empty:
				//AUTH_TYPE
				//HOST_NAME ?
				//SERVER_PORT ?
				//REMOTE_IDENT
				//REMOTE_USER
	//request line ->
				//REQUEST_URI
				//PATH_INFO = SCRIPT_NAME
				//PATH_TRANSLATED
				//QUERY_STRING
				//METHOD
	//a parser du header:
				//REMOTE_HOST
				//CONTENT_LENGTH
				//CONTENT_TYPE
	//a obtenir autrement:
				//REMOTE_ADDR -> ipv4 du client
