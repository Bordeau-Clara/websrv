/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FieldsParse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 14:24:44 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Location.hpp"
#include "stateMachine.hpp"
#include "statusCodes.hpp"
#include <cstdlib>
#include <iostream>

//format: <host>:<port>
//<port> optional
void	Request::parseHost(std::string str)
{
	if (!this->_host.empty())
	{
		setError(Status("400 Bad Request: Multiple Host Header", 400));
		return ;
	}
	this->_host = str;
}

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

void Request::parseCookies(std::string str) 
{
	//contains stored HTTP cookies associated with the server 
	//(previously sent by the server with the Set-Cookie header
	//format: name=value; name2=value2; name3=value3
	//
	// On utilise un stringstream pour découper par ';'
	std::stringstream ss(str);
	std::string segment;

	while (std::getline(ss, segment, ';'))
	{
		// 1. Nettoyage des espaces au début (après le ';')
		size_t start = segment.find_first_not_of(" ");
		if (start == std::string::npos)// segment vide erreur
		{
			setError(Status("400 Bad Request: Malformed Cookie Header", 400));
			return ;
		}
		segment = segment.substr(start);
		// 2. Vérification du format clé=valeur
		size_t sep = segment.find('=');
		if (sep == std::string::npos || sep == 0 || sep == segment.size() - 1)
		{
			setError(Status("400 Bad Request: Malformed Cookie Header", 400));
			return ;
		}
		std::string key = segment.substr(0, sep);
		std::string value = segment.substr(sep + 1);
		// 3. Extraction de SESSIONID
		if (key == SID)
			this->_cookies = value;
	}
	streams.get(LOG_REQUEST) << "[cookies]" << std::endl << this->_cookies
		<< std::endl;
}

//keep-alive ou close
void	Request::parseConnection(std::string str)
{
	streams.get(LOG_REQUEST) << "parseConnection<" + str + ">" << std::endl;
	if (!str.compare("keep-alive"))
	{
		streams.get(LOG_REQUEST) << "[connection is keep alive]" << std::endl
			<< std::endl;
		this->_connection = KEEP_ALIVE;
	}
	if (!str.compare("close"))
	{
		streams.get(LOG_REQUEST) << "[connection is close]" << std::endl
			<< std::endl;
		this->_connection = CLOSE;
	}
}

void	Request::parseContentType(std::string str)
{
	this->_contentType.assign(str);
}

void	Request::parseContentLength(std::string str)
{
	//if chunked and content length -> 400
	if (isState(CHUNKED))
	{
		this->setError(Status(BAD_REQUEST, 400));
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Cannot have Content-Length and Transfer-encoding at the same time"
			<< std::endl;
		return;
	}
	this->_length = 1;
	this->_contentLength = std::strtol(str.c_str(), NULL, 10);
	if (this->_contentLength > this->_location->getClientMaxBodySize())
	{
		this->setError(Status(PAYLOAD_TOO_LARGE, 413));
	}
}

void	Request::parseTransferEncoding(std::string str)
{
	//format xxx, xxx
	//encodage dans lequel le body va etre envoyer
	//dans notre cas chunked est le seul a gerer
	//pour les autres types 501 Not Implemented
	//
	//lire ligne: "50\r\n" => taille hex
	//lire 80 bytes de body
	//lire CRLF
	//lire next chunk
	//fin si chunk size = 0
	//lire final CRLF + trailers eventuels
	//reconstruire le body
	//
	//pour cgi pas de VE HTTP_TRANSFER_ENCODING
	//=> trouver la taille du body reconstitue et creer CONTNENT_LENGTH
	//if chunked and content length -> 400
	if (this->_length == 1)
	{
		this->setError(Status(BAD_REQUEST, 400));
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Cannot have Content-Length and Transfer-encoding at the same time"
			<< std::endl;
		return;
	}
	if (str.find("chunked") != std::string::npos)
	{
		setState(CHUNKED);
	}
	else
	{
		this->setError(Status(BAD_REQUEST, 400));
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Only accept chunked encoding"
			<< std::endl;
	}
}

void	Request::parseTrailer(std::string str)
{
	//indique quels headers apparaitront apres un message chunked
	//viennent apres le chunk final (chunk size = 0)
	//a stocker qqpart pour pouvoir les supprimer de la fin du body
	//ne surtout pas les envoyer a la cgi
	
	(void)str;
	this->_trailer = 1;
}
