/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:39:19 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Location.hpp"
#include "requestDefines.hpp"
#include "stateMachine.hpp"
#include "statusCodes.hpp"
#include <cstddef>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>

#include "helpers.hpp"

void	Request::generateResponse()
{
	//3 reponses possible -> erreur, normal(get, post, delete), cgi
	//errors
	if (this->isState(ERROR))
		this->buildErrorResponse();
	//CGI because we will fill response dynamically as soon as we recv in pipe and treat what we receive
	// else if (!this->_response.empty())
	else if (isState(CGI))
	{
		return;
	}
	else if (this->_location->getReturn().code)
	{
		this->generateRequestLine();
		//buildRedir();
		_response.str.append("Location:" + this->_location->getReturn().str + CRLF);
		this->_response.str.append(CON_LEN + "0" + CRLF);
		this->appendConnection();
		this->headerEnd();
	}
	// static request
	else
	{
		//first have to exec to see if no error
		//-> is there error possible for these method??
		//GET -> open fail?? throw + stop server, method not allowed + bad uri?? delt with in parseUri()
		//POST -> open fail?? throw + stop server, method not allowed + bad uri?? delt with in parseUri()
		//DELETE -> open fail?? throw + stop server, method not allowed + bad uri?? delt with in parseUri()
				//for open fail should we access() in parseUri() to check rights???

		//attention -> pas le meme code pour post (201 Created) et delete (204 No Content), 3xx pour redirections
		switch(this->_method)
		{
			case GET:
				this->buildGetResponse();
				break;
			case POST:
				this->buildPostResponse();
				break;
			case DELETE:
				this->buildDeleteResponse();
				break;
			default:
				break;
		}
	}
}

void	Request::buildErrorResponse()
{
	//ne pas oublier DCRLF a la fin du header
	//open fichier erreur et mettre body dans une autre chaine
	//pour pouvoir connaitre sa longueur et l'ajouter au header
	//ou compter la taille du body qui est direct append a la reponse
	//et insert Content-length a response.find(CRLF) donc apres la status line
	generateRequestLine();
	this->_response.str.append(TEXT_HTML_TYPE);
	if (_status.code == 404)
		this->appendConnection();
	else
	{
		this->_response.str.append(CON_CLOSE);
		this->_connection = CLOSE;
	}
	// check if error page exists
	// if not or if access fail fall back on our default error page
	// if yes append page to body
	// this->_response.str.append(extractStr(_requestedRessource.c_str()));
	this->_response.body.clear();
	if (!findErrorPage())
	{
		this->_response.body.append(_status.str);
		if (!this->_location)
			this->_response.body.append("No location");
	}
	this->_response.str.append(CON_LEN + nbrToString(_response.body.length()) + CRLF);
	this->_response.str.append(CRLF);
	this->_response.str.append(_response.body);
}

#include <sys/stat.h>

void	Request::buildGetResponse()
{
	//open file from url if not autoindexed yet
	if (_response.body.empty())
		_response.body = extractStr(_requestedRessource.c_str());

	this->generateRequestLine();
	// size of body
	this->_response.str.append(CON_LEN + nbrToString(_response.body.size()) + CRLF);
	this->appendConnection();
	//add Content-type
	//add Date ??
	this->headerEnd();
	this->_response.str.append(_response.body.data(), _response.body.size());
}

void	Request::buildPostResponse()
{
	//error : method not allowed
	//c la merde
	if (!canBuildOnDir(_requestedRessource))
	{
		this->setError(Status(FORBIDDEN, 403));
		this->buildErrorResponse();
		return ;
	}
	std::ofstream	postFile(_requestedRessource.c_str());

	if (!postFile.is_open())
	{
		this->setError(Status(INTERNAL_SERVER_ERROR, 500));
		this->buildErrorResponse();
		return ;
	}
	postFile.write(this->_body.data(), this->_body.size());
	this->setStatus(Status("201 Created", 201));
	//+ Location header field that provides an identifier for the primary ressource created
	this->generateRequestLine();
	this->appendConnection();
	//a modifier
	this->_response.str.append("Location:" + _requestedRessource + CRLF);
	this->headerEnd();
}

void	Request::buildDeleteResponse()
{
	if (access(_requestedRessource.c_str(), F_OK))// if ressource does not exist
	{
		this->setError(Status(NOT_FOUND, 404));
		return ;
	}
	if (access(_requestedRessource.c_str(), W_OK))// if ressource cannot be wrote
	{
		this->setError(Status(FORBIDDEN, 403));
		return ;
	}
	std::remove(_requestedRessource.c_str());
	this->setStatus(Status(NO_CONT, 204));
	this->generateRequestLine();
	this->appendConnection();
	this->headerEnd();
}
