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
#include "requestDefines.hpp"
#include "stateMachine.hpp"
#include "statusCodes.hpp"
#include <cstddef>
#include <fcntl.h>
#include <fstream>

void	Request::buildErrorResponse()
{
	this->_response.str.assign(TEXT_HTML_TYPE);
	if (_status.compare(0, 3, "404"))
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
	//appendbody
}

#include "helpers.hpp"
#include <sys/stat.h>
std::string	extractStr(const char *file);

void	Request::buildGetResponse()
{
	//open file from url
	//in header: status line (200 OK ou Expect), content-length, content-type, date
	
	struct stat fileStat;
	if (stat(this->_requestedRessource.c_str(), &fileStat) == 0)
	{
		this->_response.str.append(CON_LEN + nbrToString(fileStat.st_size) + CRLF);
	}
	//add Content-type
	//add Date ??
	this->_response.str.append(CRLF);
	std::string		body = extractStr(_requestedRessource.c_str());
	this->_response.str.append(body);
	// this->_responseCursor = 0;
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
		this->_response.str.append("HTTP/1.1 " + (this->_status.empty() ? "200 OK" : this->_status) + CRLF);
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
