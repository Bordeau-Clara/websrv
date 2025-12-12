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
	std::string	token;
	std::string	field;
	int			type;

	this->_cgi = new Cgi(*this);

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
			this->setStatus(BAD_REQUEST);
			this->setState(ERROR);
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
			this->setStatus(BAD_REQUEST);
			this->setState(ERROR);
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
