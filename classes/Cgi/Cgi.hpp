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

#include "../Request/Request.hpp"
#include <vector>
class Cgi
{
private:
	int							_pipe[2];
	std::vector<std::string>	_env;
	Request&					_client;
	std::string					_buffer;
	static void					(Request::*fctField[210])(std::string);

public:
	Cgi();
	Cgi(Request&); //copie de CGI_HEADER + prendre adresse de request

	void						createBasicEnv();

	void						addFields(std::string);
	void						getFieldFromUri();

	void				parseCgiHost(std::string field, std::string token);
	void				parseCgiContentType(std::string);
	void				parseCgiContentLength(std::string); //do after body parsing? if chunked do after?
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
