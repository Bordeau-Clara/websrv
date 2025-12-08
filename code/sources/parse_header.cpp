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

#include "../includes/parsing_header.hpp"
#include "../classes/Request/Request.hpp"
#include "../classes/Cgi/Cgi.hpp"
#include "statusCodes.hpp"

// string = expr + final CRLF
void	parse_header_type(Request *request)
{
	std::string				token;

	if (request->getHeader().empty())
		return;
	//cant be empty test with empty header (just DCRLF)
	//error if empty???

	if (!request->getToken(&token))//can't use this cause it skip ows
	{
		request->setStatus(BAD_REQUEST);
		return;
	}
	parse_request_line(request, token);
	if (!request->getStatus().empty())
		return;
	if (request->getState() == CGI)
		parse_cgi_header(request);
	else
		parse_header(request);
		

	//if CGI parse_header in cgi mode
	//state CGI but same parsing function?
}

void	parse_header(Request *request)
{
	std::string				token;

	if (request->getHeader().empty())
		return;
	//error if empty??? can it be empty?? Is it an error?? have to test it
	
	int type;
	while (1)
	{
		//look at parse_cgi_header for amelioration
		if (request->getHeader().empty())
		{
			streams.get(LOG_REQUEST) << "[HEADER EMPTIED IN PARSING]" << std::endl;
			break;
		}
		if (!request->getField(&type) || !request->getToken(&token))
		{
			request->setStatus(BAD_REQUEST);
			streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
				<< "invalid field or token" << std::endl
				<< "field type is :" << type << std::endl
				<< "token after field is :" << token
				<< std::endl;
			return;
		}
		if (type > 0 && type < 207 && Request::fctField[type] != NULL)
			(request->*Request::fctField[type])(token);
		else if (type < 0)
		{
			request->setStatus(BAD_REQUEST);
		//How to deal with expect? Does errors override expect?? Does expect override body??
		//->Put in a string and check at response construction?
		}
		//inverser condition if else pour supprimer else pour la clarter
	}
	//check_complete_header(event);
}

void	parse_cgi_header(Request *request)
{
	std::string				token;
	std::string				field;
	Cgi						cgi;

	if (request->getHeader().empty())
		return;
	
	while (1)
	{
		if (request->getHeader().empty())
		{
			streams.get(LOG_REQUEST) << "[HEADER EMPTIED IN PARSING]" << std::endl;
			break;
		}
		if (!request->getField(&field) || !request->getToken(&token))
		{
			request->setStatus(BAD_REQUEST);
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
