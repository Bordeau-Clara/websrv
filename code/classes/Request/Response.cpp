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

#include "helpers.hpp"
void	Request::buildErrorResponse()
{
	this->_response.str.append(TEXT_HTML_TYPE);
	if (_status.code == 404)
	{
		if (this->_connection == KEEP_ALIVE)
			this->_response.str.append(CON_KEEP_ALIVE);
		else
			this->_response.str.append(CON_CLOSE);
		//ne pas oublier DCRLF a la fin du header
		//open fichier et mettre body dans une autre chaine
		//pour pouvoir connaitre sa longueur et l'ajouter au header
		//ou compter la taille du body qui est direct append a la reponse
		//et insert Content-length a response.find(CRLF) donc apres la status line
	}
	else
	{
		this->_response.str.assign(CON_CLOSE);
		//ne pas oublier DCRLF a la fin du header
		//open fichier erreur et mettre body dans une autre chaine
		//pour pouvoir connaitre sa longueur et l'ajouter au header
		//ou compter la taille du body qui est direct append a la reponse
		//et insert Content-length a response.find(CRLF) donc apres la status line
	}
	this->_response.str.append(CRLF);
	if (!this->_location)
	{
		this->_response.str.append("No location");
		return ;
	}
	const std::map<int, std::string>	ErrorPages = this->_location->getErrorPages();
	if (ErrorPages.find(_status.code) == ErrorPages.end())
	{
		this->_response.str.append("Error" + nbrToString(_status.code));
		return ;
	}
	// this->_response.str.append(extractStr(_requestedRessource.c_str()));
}

#include <sys/stat.h>

void	Request::buildGetResponse()
{
	//open file from url if not autoindexed yet
	if (_response.body.empty())
		_response.body = extractStr(_requestedRessource.c_str());

	// size of body
	this->_response.str.append(CON_LEN + nbrToString(_response.body.size()) + CRLF);
	//add Content-type
	//add Date ??
	this->_response.str.append(CRLF);
	this->_response.str.append(_response.body);
}

void	Request::buildPostResponse()
{
	//error : method not allowed
	//c la merde
}

void	Request::buildDeleteResponse()
{
	//error : method not allowed
	//open file from url
	//in header: status line (200 OK ou Expect)
	//if DELETE autorise dans la location && url est un fichier
		//if fichier existe
			//supprimer fichier -> 204 No Content
		//else
			//404 Not Found
	//if DELETE pas autorise ou url est un dossier
		//403 Forbidden
	

	//faire une cgi (script bash) qui delete un fichier
}

void	Request::generateResponse()
{
	//3 reponses possible -> erreur, normal(get, post, delete), cgi
	
	//errors
	if (this->isState(ERROR))
		buildErrorResponse();
	//CGI because we will fill response dynamically as soon as we recv in pipe and treat what we receive
	// else if (!this->_response.empty())
	// else if (isState(CGI))
	// {
	// }
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
		this->_response.str.append("HTTP/1.1 " + getStatus().str + CRLF);
		if (this->_connection == KEEP_ALIVE)
			this->_response.str.append(CON_KEEP_ALIVE);
		else
			this->_response.str.append(CON_CLOSE);
		switch(this->_method)
		{
			case GET:
				buildGetResponse();
				break;
			case POST:
				buildPostResponse();
				break;
			case DELETE:
				buildDeleteResponse();
				break;
			default:
				break;
		}
	}
}
