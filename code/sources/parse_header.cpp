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

// string = expr + final CRLF
void	parse_header_type(Request *request)
{
	std::string				token;

	if (request->getHeader().empty())
		return;
	//cant be empty
	//error if empty???

	request->getToken(&token);//can't use this cause it skip ows
	// std::cout << GREEN << "Request line is:" + token << WHITE << std::endl;
	parse_request_line(request, token);
	if (request->getState() == CGI)
		parse_cgi_header(request);
	else
		parse_header(request);
		
	//what if error in request line

	//if CGI parse_header in cgi mode
	//state CGI but same parsing function?
}

void	parse_header(Request *request)
{
	std::string				token;

	if (request->getHeader().empty())
		return;
	//error if empty???
	
	int type;
	while (1)
	{
		//look at parse_cgi_header for amelioration
		if (request->getHeader().empty())
		{
			std::cout << "header emptied" << std::endl;
			break;
		}
		if (!request->getField(&type) || !request->getToken(&token))
		{
			std::cout << RED << "Error in field or token" << WHITE << std::endl;
			return; //throw error?
			//edit status ici ou dans les fonction du if??
		}
		if (Request::fctField[type] != NULL)
			(request->*Request::fctField[type])(token);
		else
			std::cout << "Invalid index is " << type << std::endl;//not necessarly, field can be valid but no function
		//inverser condition if else pour supprimer else pour la clarter
		
	}

	//check_complete_header(event);
	
	if (request->getTransferEncoding() == CHUNKED)
		request->setState(CHUNK_SIZE);
	else
		request->setState(BODY);
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
			std::cout << "header emptied" << std::endl;
			break;
		}
		if (!request->getField(&field) | !request->getToken(&token))
		{
			std::cout << "invalid field or token" << std::endl;
			return;
			//edit status and return
		}
		// request->getField(&field, &cursor);//why don't getField() do the search?
		// request->getToken(&token, &cursor);// should skip the ows qnd not parsers
		cgi.addFields(field, token);
	}
	//check_complete_header(event); //if content_length absent -> add it
	if (request->getTransferEncoding() == CHUNKED)
		request->setState(CHUNK_SIZE);
	else
		request->setState(BODY);
}
//class request devient client et request est le constructeur de client (clien herite de request)
//les fonctions de parsing vont dans request pour la lisibilite
//
//construction reponse:
//si cgi -> recuperer header + body => comment savoir si cgi? Encore une variable? Peut pas avec state car passe en send quand cgi a fini
//state "SEND_CGI"? Et donc va check la class cgi qui ce trouve dan client?
//else a construire a partir des variable et uri
