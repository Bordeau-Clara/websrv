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
#include <unistd.h>
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
	~Cgi()
	{
		if (_responsePipe[1] != -1)
		{
			close(_responsePipe[1]);
			_responsePipe[1] = -1;
		}
		if (_responsePipe[0] != -1)
		{
			close(_responsePipe[0]);
			_responsePipe[0] = -1;
		}
	}

	int						start(EventManager &webServ);
	pid_t					_pid;

	void						createBasicEnv();
	std::vector<char *>			strToArray(std::vector<std::string>);
	void						addFields(std::string field, std::string token);//check for host, type, length and or add
	void						getFieldFromUri();
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

//Liste des fields possible a verifier pour la CGI
//pragma
//cache-control
//date
//upgrade (ne pas envoyer a la cgi mais valide)
//accept-charset
//from
//if-mach
//if-none-match
//if-range
//if-unmodified-since
//max-forward (ne pas envoyer mais a valider)
//proxy-authorization (ne pas envoyer mais a valider)
//range
//referer
//TE
//user-agent
//
//lie au body
//content-encoding
//content-location
//content-language
//content-range
