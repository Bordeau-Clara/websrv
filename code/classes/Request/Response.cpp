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

void	Request::buildErrorResponse()
{
	if (_status.compare(0, 3, "404"))
	{
		this->_response.assign("HTTP/1.1 " + this->_status + CRLF
			+ TEXT_HTML_TYPE);
		if (this->_connection == KEEP_ALIVE)
			this->_response.append(CON_KEEP_ALIVE);
		else
			this->_response.append(CON_CLOSE);
		//ne pas oublier DCRLF a la fin du header
		//open fichier et mettre body dans une autre chaine
		//pour pouvoir connaitre sa longueur et l'ajouter au header
		//ou compter la taille du body qui est direct append a la reponse
		//et insert Content-length a response.find(CRLF) donc apres la status line
	}
	else
	{
		this->_response.assign("HTTP/1.1 " + this->_status + CRLF
			+ TEXT_HTML_TYPE + CON_CLOSE);
		//ne pas oublier DCRLF a la fin du header
		//open fichier erreur et mettre body dans une autre chaine
		//pour pouvoir connaitre sa longueur et l'ajouter au header
		//ou compter la taille du body qui est direct append a la reponse
		//et insert Content-length a response.find(CRLF) donc apres la status line
	}
}

void	Request::generateResponse()
{
	//3 reponses possible -> erreur, normal(get, post, delete), cgi
	
	//errors
	if (this->isState(ERROR))
		buildErrorResponse();
	//CGI because we will fill response dynamically as soon as we recv in pipe and treat what we receive
	// else if (!this->_response.empty())
	else if (isState(CGI))
	{
	}
	// static request
	else
	{
		//si GET
			//open file from url
			//in header: status line (200 OK ou Expect), content-length, content-type (if body)
		//si POST
			//c la merde
			//
		//si DELETE
			//open file from url
			//in header: status line (200 OK ou Expect)
			//if DELETE autorise dans la location && url est un fichier
				//if fichier existe
					//supprimer fichier -> 204 No Content
				//else
					//404 Not Found
			//if DELETE pas autorise ou url est un dossier
				//403 Forbidden
	}
}
