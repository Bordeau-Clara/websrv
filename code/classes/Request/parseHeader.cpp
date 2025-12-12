/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 11:36:44 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Cgi.hpp"
#include "statusCodes.hpp"
#include "Location.hpp"
#include "Server.hpp"

// string = expr + final CRLF
void	Request::parseHeaderType(void)
{
	std::string				token;

	if (!this->getToken(&token))//can't use this cause it skip ows
	{
		this->setStatus(BAD_REQUEST);
		this->setState(ERROR);
		return;
	}
	parseRequestLine(token);
	if (this->isState(ERROR))
		return;
	if (this->isState(CGI))
		parseCgiHeader();
	else
		parseHeader();
		

	//if CGI parse_header in cgi mode
	//state CGI but same parsing function?
}

//parse method
//reconstitue/parse URI
//check HTTP/1.1
void	Request::parseRequestLine(std::string token)
{
	std::string::size_type cursor= 0;

	if (moveCursor(&cursor, token, " "))
	{
		this->parseMethod(token.substr(0, cursor));
		token.erase(0, cursor + 1);
		if (isState(ERROR))
			return;
	}
	else
	{
		this->setStatus(BAD_REQUEST);
		this->setState(ERROR);
		return;
	}
	if (moveCursor(&cursor, token, " "))
	{
		this->parseURI(token.substr(0, cursor));
		token.erase(0, cursor + 1);
		if (isState(ERROR))
			return;
	}
	else
	{
		this->setStatus(BAD_REQUEST);
		this->setState(ERROR);
		return;
	}
	if (token.compare("HTTP/1.1"))
	{
		this->setStatus(BAD_REQUEST);
		this->setState(ERROR);
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Wrong HTTP protocol:" << token
			<< std::endl;
	}
}

void	Request::parseMethod(std::string str)
{
	if (str == "GET")
		this->_method = GET;
	else if (str == "POST")
		this->_method = POST;
	else if (str == "DELETE")
		this->_method = DELETE;
	else
	{
		this->_status = BAD_REQUEST;
		this->setState(ERROR);
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Bad method identified: " << str
			<< std::endl;
	}
}

void	Request::parseURI(std::string str)
{
	std::string::size_type cursor = 0;
	//or # anchor???
	if (moveCursor(&cursor, str, "?"))
	{
		this->_queryString.assign(str.substr(cursor + 1));
		str.erase(cursor);
	}
	
	this->_uri.assign(str);
	//resolve uri
	this->_location = this->_server.urlSolver(str);
	//deal with errors
	if (!this->_location)
	{
		this->setState(ERROR);
		this->_status = "404";
	}
	this->_url.assign(str);
	// si rien ou slash sans rien alors verifier index
		// access -->file (index)
	// sinon verifier CGI
		// si CGI
			// access --> executable
		// sinon
			// access --> file
	// fusionner root + alias + URL pour access
	// access reussie passe a la suite
	// sinon 404
	// this->isCGI();
}

void	Request::isCGI(void)
{
	const std::set<std::string>	&CgiSuffixes = _location->getCgiSuffix();
	for (std::set<std::string>::const_iterator it = CgiSuffixes.begin(); it != CgiSuffixes.end(); it++)
	{
		std::cout << "Salut" << std::endl;
	}
}

void	Request::parseHeader(void)
{
	std::string				token;

	if (this->getHeader().empty())
		return;
	//error if empty??? can it be empty?? Is it an error?? have to test it
	
	int type;
	while (1)
	{
		//look at parse_cgi_header for amelioration
		if (this->getHeader().empty())
		{
			streams.get(LOG_REQUEST) << "[HEADER EMPTIED IN PARSING]" << std::endl;
			break;
		}
		if (!this->getField(&type) || !this->getToken(&token))
		{
			this->setStatus(BAD_REQUEST);
			this->setState(ERROR);
			streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
				<< "invalid field or token" << std::endl
				<< "field type is :" << type << std::endl
				<< "token after field is :" << token
				<< std::endl;
			return;
		}
		if (type > 0 && type < 207 && Request::fctField[type] != NULL)
			(this->*Request::fctField[type])(token);
		else if (type < 0)
		{
			this->setStatus(BAD_REQUEST);
			this->setState(ERROR);
		//How to deal with expect? Does errors override expect?? Does expect override body??
		//->Put in a string and check at response construction?
		}
		//inverser condition if else pour supprimer else pour la clarter
	}
	//check_complete_header(event);
}

void	Request::parseCgiHeader(void)
{
	std::string				token;
	std::string				field;
	Cgi						cgi;

	if (this->getHeader().empty())
		return;
	
	while (1)
	{
		if (this->getHeader().empty())
		{
			streams.get(LOG_REQUEST) << "[HEADER EMPTIED IN PARSING]" << std::endl;
			break;
		}
		if (!this->getField(&field) || !this->getToken(&token))
		{
			this->setStatus(BAD_REQUEST);
			this->setState(ERROR);
			streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
				<< "invalid field or token" << std::endl
				<< "field is :" << field << std::endl
				<< "token after field is :" << token
				<< std::endl;
			return;
		}
		cgi.addFields(field, token);
	}
	//check_complete_header(event); //if content_length absent -> add it
}
//class request devient client et request est le constructeur de client (clien herite de request)
//les fonctions de parsing vont dans request pour la lisibilite
//
//construction reponse:
//si cgi -> recuperer header + body => comment savoir si cgi? Encore une variable? Peut pas avec state car passe en send quand cgi a fini
//state "SEND_CGI"? Et donc va check la class cgi qui ce trouve dan client?
//else a construire a partir des variable et uri
