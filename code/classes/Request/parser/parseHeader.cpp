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
#include "logfiles.hpp"
#include "requestDefines.hpp"
#include "statusCodes.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <unistd.h>
#include "helpers.hpp"

// string = expr + final CRLF
void	Request::parseHeader(void)
{
	std::string				token;

	if (!this->getToken(&token))//can't use this cause it skip ows
	{
		this->setStatus(Status(BAD_REQUEST, 400));
		// buildErrorResponse();
		this->setState(ERROR);
		this->setState(EXEC);
		return;
	}
	parseRequestLine(token);
	if (this->isState(ERROR))
		return;
	if (this->isState(CGI))
		parseHeaderCgi();
	else
		parseHeaderRegular();
		

	//if CGI parse_header in cgi mode
	//state CGI but same parsing function?
}

//parse method
//reconstitue/parse URI
//check HTTP/1.1
void	Request::parseRequestLine(std::string token)
{
	std::string::size_type cursor= 0;

	if (!moveCursor(&cursor, token, " "))
	{
		this->setStatus(Status(BAD_REQUEST, 400));
		this->setState(ERROR);
		this->setState(EXEC);
		return;
	}
	this->parseMethod(token.substr(0, cursor));
	if (isState(ERROR))
		return;
	token.erase(0, cursor + 1);
	if (!moveCursor(&cursor, token, " "))
	{
		this->setStatus(Status(BAD_REQUEST, 400));
		this->setState(ERROR);
		this->setState(EXEC);
		return;
	}
	this->parseURI(token.substr(0, cursor));
	if (isState(ERROR))
		return;
	token.erase(0, cursor + 1);
	if (token.compare("HTTP/1.1"))
	{
		this->setStatus(Status(BAD_REQUEST, 400));
		this->setState(ERROR);
		this->setState(EXEC);
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Wrong HTTP protocol:" << token
			<< std::endl;
		return ;
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
		this->setStatus(Status(BAD_REQUEST, 400));
		this->setState(ERROR);
		this->setState(EXEC);
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
	

	//resolve uri
	this->_uri.assign(str);
	streams.get(LOG_REQUEST) << "[urlSolver]" << "start with:<" << str + '>' << std::endl;
	this->_location = this->_server.urlSolver(str);
	streams.get(LOG_REQUEST) << "[urlSolver]" << "end" << std::endl;

	streams.get(LOG_EVENT) << "Okay" << std::endl;
	//deal with location errors
	{
		// 404 not found
		if (!this->_location)
		{
			this->setState(EXEC);
			this->setState(ERROR);
			this->setStatus(Status(NOT_FOUND, 404));
			return ;
		}
		// 400 bad request (not authorized request)
		if (!this->_location->getMethods()[this->getMethod()])
		{
			this->setState(EXEC);
			this->setState(ERROR);
			this->setStatus(Status(BAD_REQUEST, 400));
			streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
				<< "un authorized method " + METHODS[getMethod()] + " in location " + this->_uri
				<< std::endl;
			return ;
		}
	}

//Todo:
// verifier CGI
	// si CGI
		// access --> executable

	streams.get(LOG_EVENT) << "Okay" << std::endl;
	streams.get(LOG_EVENT) << "Solving remainder "<< "<"+str+">" << std::endl;
	streams.get(LOG_REQUEST) << "Solving remainder "<< "<"+str+">" << std::endl;
	if (this->_method == GET)
	{// fusionner root + alias + remainder pour access
		// si rien ou slash sans rien alors verifier index
		if ((str.empty() || str == "/")) // index ressource
		{
			_requestedRessource = _location->getRoot() + _location->getAlias() + "/" + _location->getIndex();
			streams.get(LOG_REQUEST) << "concatenation :" + _requestedRessource<< std::endl;
			trimSlash(_requestedRessource);
			streams.get(LOG_REQUEST) << " Empty remainder, testing index:" + _requestedRessource<< std::endl;
			if (access(_requestedRessource.c_str(), R_OK))// if no index no auto index
			{
				streams.get(LOG_REQUEST) << " index missing, testing auto index "<< std::endl;
				if (_location->getAutoindex() == false)
				{
					streams.get(LOG_REQUEST) << "error no auto index"<< std::endl;
					this->setState(EXEC);
					this->setState(ERROR);
					this->setStatus(Status(NOT_FOUND, 404));
					return ;
				}
				// should handle auto index here
				_requestedRessource = _location->getRoot() + _location->getAlias();
				trimSlash(_requestedRessource);
				streams.get(LOG_REQUEST) << "building auto index"<< std::endl;
				if (!recursiveReaddir(""))
				{
					streams.get(LOG_REQUEST) << "error auto index failed"<< std::endl;
					this->setState(EXEC);
					this->setState(ERROR);
					this->setStatus(Status(NOT_FOUND, 404));
					return ;
				}
				streams.get(LOG_REQUEST) << "auto index :<" + _response.body + ">" << std::endl;
				//
			}
		}
		else // regular file
		{
			_requestedRessource = _location->getRoot() + _location->getAlias() + "/" + str;
			trimSlash(_requestedRessource);
			streams.get(LOG_EVENT) << "file: " << _requestedRessource << std::endl;
			if (access(_requestedRessource.c_str(), R_OK))// if ressource cannot be read
			{
				this->setState(EXEC);
				this->setState(ERROR);
				this->setStatus(Status(NOT_FOUND, 404));
				return ;
			}
		}
	}
	if (this->_method == POST)
	// _requested ressource will be post location + remainder
	// then try to see if we can open file (create or append)
	{
		// WORK IN PROGRESS
	}
	if (this->_method == DELETE)
	// _requested ressource will be post location + remainder
	// TBD
	{
	}
}

// void	Request::buildIndex()
// {
// }

void	Request::isCGI(void)
{
	const std::set<std::string>	&CgiSuffixes = _location->getCgiSuffix();
	for (std::set<std::string>::const_iterator it = CgiSuffixes.begin(); it != CgiSuffixes.end(); it++)
	{
		std::cout << "Salut" << std::endl;
	}
}

void	Request::parseHeaderRegular(void)
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
			this->setStatus(Status(BAD_REQUEST, 400));
			this->setState(ERROR);
			this->setState(EXEC);
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
			this->setStatus(Status(BAD_REQUEST, 400));
			this->setState(ERROR);
			this->setState(EXEC);
		//How to deal with expect? Does errors override expect?? Does expect override body??
		//->Put in a string and check at response construction?
		}
		//inverser condition if else pour supprimer else pour la clarter
	}
	//check_complete_header(event);
}

void	Request::parseHeaderCgi(void)
{
	std::string	token;
	std::string	field;
	int			type;

	// this->_cgi = new Cgi(*this);

	if (this->getHeader().empty())
		return;
	
	while (1)
	{
		if (this->getHeader().empty())
		{
			streams.get(LOG_REQUEST) << "[HEADER EMPTIED IN PARSING]" << std::endl;
			break;
		}
		if (!this->getField(&field, &type) || !this->getToken(&token))
		{
			this->setStatus(Status(BAD_REQUEST, 400));
			this->setState(ERROR);
			this->setState(EXEC);
			streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
				<< "invalid field or token" << std::endl
				<< "field is :" << field << std::endl
				<< "token after field is :" << token
				<< std::endl;
			return;
		}
		if (type > 0 && type < 207 && Request::fctField[type] != NULL)
			(this->*Request::fctField[type])(token);
		else if (type < 0)
		{
			this->setStatus(Status(BAD_REQUEST, 400));
			this->setState(ERROR);
			this->setState(EXEC);
		//How to deal with expect? Does errors override expect?? Does expect override body??
		//->Put in a string and check at response construction?
		}
		if (!field.empty())
			this->_cgi->addFields(field, token);
	}
	//check_complete_header(event); //if content_length absent -> add it
}
//construction reponse:
//si cgi -> recuperer header + body => comment savoir si cgi? Encore une variable? Peut pas avec state car passe en send quand cgi a fini
//state "SEND_CGI"? Et donc va check la class cgi qui ce trouve dan client?
//else a construire a partir des variable et uri
