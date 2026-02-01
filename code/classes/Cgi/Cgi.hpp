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

#include <sched.h>
#include <string>
#include <vector>
#include "EventManager.hpp"
#include "define_cgi.hpp"
#include "Event.hpp"

class Request;
class Cgi : public Event
{
private:


public:
	std::vector<std::string>	_env;
	std::vector<std::string>	_arg;
	std::string					_exec;

	std::string					_buffer;
	void							parseBuffer();

	std::string					_header;
	void							fillHeader(std::string::size_type cursor);
	void							parseHeader();
	void							appendStatus();
	void							appendContentLen();
	void							setConnection();
	std::string					_headerLimiter;
	std::string							findLimiter();

	std::string::size_type		_length;

	Request						*_client;

	Cgi():Event(PIPE){}
	void						init(void);
	int						_responsePipe[2];
	int						_bodyPipe[2];
	Cgi(Request*); //prendre adresse de request

	int						start(EventManager &webServ);
	pid_t					_pid;

	void						createBasicEnv();
	std::vector<char *>			strToArray(std::vector<std::string>); //to implement
	void						addFields(std::string field, std::string token);//check for host, type, length and or add
	void						getFieldFromUri();//to call in constructor -> no
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
