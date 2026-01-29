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
#include <cmath>
#include <fcntl.h>
#include <unistd.h>
#include "helpers.hpp"

// string = expr + final CRLF
void	Request::parseHeader(void)
{
	std::string				token;

	if (!this->getToken(&token))//can't use this cause it skip ows
	{
		this->setError(Status(BAD_REQUEST, 400));
		return;
	}
	parseRequestLine(token);
	if (this->isState(ERROR))
		return;
	if (this->isState(CGI))
	{
		this->_cgi->getFieldFromUri();
		parseHeaderCgi();
	}
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
		this->setError(Status(BAD_REQUEST, 400));
		return;
	}
	this->parseMethod(token.substr(0, cursor));
	if (isState(ERROR))
		return;
	token.erase(0, cursor + 1);
	if (!moveCursor(&cursor, token, " "))
	{
		this->setError(Status(BAD_REQUEST, 400));
		return;
	}
	this->parseURI(token.substr(0, cursor));
	if (isState(ERROR))
		return;
	token.erase(0, cursor + 1);
	if (token.compare("HTTP/1.1"))
	{
		this->setError(Status(BAD_REQUEST, 400));
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
		this->setError(Status(BAD_REQUEST, 400));
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "Bad method identified: " << str
			<< std::endl;
	}
}

#include <sys/stat.h>

void	Request::parseURI(std::string str)
{
	std::string::size_type cursor = 0;
	//or # anchor???
	if (moveCursor(&cursor, str, "?"))
	{
		this->_queryString.assign(str.substr(cursor + 1));
		str.erase(cursor);
	}
	trimDots(str);
{//resolve uri
	this->_uri.assign(str);
	streams.get(LOG_REQUEST) << "[urlSolver]" << "start with:<" << str + '>' << std::endl;
	this->_location = this->_server.urlSolver(str);
	streams.get(LOG_REQUEST) << "[urlSolver]" << "end" << std::endl;
}
{//deal with location errors
	// 404 not found
	if (!this->_location)
	{
		this->setError(Status(NOT_FOUND, 404));
		return ;
	}
	// 400 bad request (not authorized request)
	if (!this->_location->getMethods()[this->getMethod()])
	{
		this->setError(Status(NOT_ALLOWED, 405));
		streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
			<< "un authorized method " + METHODS[getMethod()] + " in location " + this->_uri
			<< std::endl;
		return ;
	}
}
	streams.get(LOG_EVENT) << "Location found, method authorized" << std::endl;
	if (this->_location->getReturn().code) // redirection
		this->setStatus(Status(nbrToString(this->_location->getReturn().code) + " See Other", this->_location->getReturn().code));
	else
		checkURI(str);
}

void	Request::checkURI(std::string	&remainder)
{
	//Todo:
	// verifier CGI == chercher dans la map suffixe - executant
	//  si trouve
		// access --> fichier ressource demandee 
		// access --> executant (deprecated)
	// 	
	streams.get(LOG_REQUEST) << "Solving remainder "<< "<"+remainder+">" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = this->_location->getCgiSuffix().begin();
		it != this->_location->getCgiSuffix().end(); it++)
	{
		if (it->first.size() < remainder.size() && remainder.rfind(it->first) == remainder.size() - it->first.size())
		{
			streams.get(LOG_REQUEST) << "[Cgi detected]"<< remainder.rfind(it->first) << std::endl;
			this->_cgi = new Cgi(this);
			this->_cgi->_exec = it->second;
			this->_cgi->_arg.push_back(this->_cgi->_exec);
			_requestedRessource = _location->getRoot() + "/" +_location->getAlias() + "/" + remainder;
			trimSlash(_requestedRessource);
			if (access(_requestedRessource.c_str(), F_OK))// if cannot read index
			{
				streams.get(LOG_REQUEST) << "cgi file not found" << std::endl;
				this->setError(Status(NOT_FOUND, 404));
				return ;
			}
			this->_cgi->_arg.push_back(_requestedRessource);
			setState(CGI);
			return ;
		}
	}
	if (this->_method == GET)
	{
		// fusionner root + alias + remainder pour access
		// ERRATUM > il fau pas access mais stat en premier
		// si rien ou slash sans rien alors verifier index
		// ERRATUM SI STAT RENVOIE DIR VERIFIER INDEX
		_requestedRessource = _location->getRoot() + "/" +_location->getAlias() + "/" + remainder;
		trimSlash(_requestedRessource);

		struct stat	statbuf;
		if (stat(_requestedRessource.c_str(), &statbuf) == -1) // does not exist
		{
			this->setError(Status(NOT_FOUND, 404));
			return ;
		}
		if ((statbuf.st_mode & S_IFMT) == S_IFDIR) // path is a directory
		{
			_requestedRessource = _location->getRoot() + "/" + _location->getAlias() + "/" + remainder + "/" + _location->getIndex();
			trimSlash(_requestedRessource);
			streams.get(LOG_REQUEST) << "path is a directory, checking index :" + _requestedRessource<< std::endl;
			if (access(_requestedRessource.c_str(), R_OK))// if cannot read index
			{
				streams.get(LOG_REQUEST) << " index missing, testing auto index "<< std::endl;
				if (_location->getAutoindex() == false) // if no auto index error
				{
					streams.get(LOG_REQUEST) << "error no auto index"<< std::endl;
					this->setError(Status(NOT_FOUND, 404));
					return ;
				}
				_requestedRessource = _location->getRoot() + "/" + _location->getAlias() + "/" + remainder;
				trimSlash(_requestedRessource);
				streams.get(LOG_REQUEST) << "building auto index with :"<< _requestedRessource<< std::endl;
				if (!recursiveReaddir(""))
				{
					streams.get(LOG_REQUEST) << "error auto index failed"<< std::endl;
					this->setError(Status(NOT_FOUND, 404));
					return ;
				}
				streams.get(LOG_REQUEST) << "auto index :<" + _response.body + ">" << std::endl;
				//
			}
		}
		else if ((statbuf.st_mode & S_IFMT) == S_IFREG) // path is a regular file
		{
			streams.get(LOG_EVENT) << "file: " << _requestedRessource << std::endl;
			if (access(_requestedRessource.c_str(), R_OK))// if ressource cannot be read
			{
				this->setError(Status(NOT_FOUND, 404));
				return ;
			}
		}
		else // bad request not dir or reg
		{
			this->setError(Status(NOT_FOUND, 404));
			return ;
		}
		//reminder.find_last_of(".") to fill _response.contentType
	}
	if (this->_method == POST)
	// _requested ressource will be post location + remainder
	// then try to see if we can open file (create or append)
	{
		_requestedRessource = this->_location->getPostDirectory() + remainder;
			streams.get(LOG_REQUEST) << "[REQUESTED RESSOURCE]" << std::endl
				<< _requestedRessource << std::endl
				<< std::endl;
		if(!canBuildOnDir(_requestedRessource))
		{
			this->setError(Status(FORBIDDEN, 403));
			return ;
		}
	}
	if (this->_method == DELETE)
	// _requested ressource will be post location + remainder
	// TBD
	{
		_requestedRessource = this->_location->getPostDirectory() + remainder;
		struct stat	statbuf;
		if (stat(_requestedRessource.c_str(), &statbuf) == -1) // does not exist
		{
			this->setError(Status(NOT_FOUND, 404));
			return ;
		}
		if ((statbuf.st_mode & S_IFMT) == S_IFDIR) // path is a directory
		{
			this->setError(Status(FORBIDDEN, 403));
			return ;
		}
		if (access(_requestedRessource.c_str(), W_OK))// if ressource cannot be wrote
		{
			this->setError(Status(FORBIDDEN, 403));
			return ;
		}
	}
}

// void	Request::buildIndex()
// {
// }

void	Request::isCGI(void)
{
	const std::map<std::string, std::string>	&CgiSuffixes = _location->getCgiSuffix();
	for (std::map<std::string, std::string>::const_iterator it = CgiSuffixes.begin(); it != CgiSuffixes.end(); it++)
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
			this->setError(Status(BAD_REQUEST, 400));
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
			this->setError(Status(BAD_REQUEST, 400));
		//How to deal with expect? Does errors override expect?? Does expect override body??
		//->Put in a string and check at response construction?
		}
		//inverser condition if else pour supprimer else pour la clarter
	}
	//check_complete_header(event);
	//if POST ->Content Length obligatoire ->411
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
			this->setError(Status(BAD_REQUEST, 400));
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
			this->setError(Status(BAD_REQUEST, 400));
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
