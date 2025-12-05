/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 12:56:49 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include "../Request/Request.hpp"

Cgi::Cgi(): _env(CGI_HEADER), _contentLength(0)
{
}

std::string	httpToCgiHeader(std::string field)
{
	for (int i = 0; !field[i]; i++)
	{
		if (field[i] == '-')
			field[i] = '_';
		if (field[i] >= 'a' && field[i] <= 'z')
			field[i] += 32;
	}
	if (!field.compare("CONTENT_TYPE") || !field.compare("CONTENT_LENGTH"))
		return (field + "=");
	return (HTTP + field + "=");
}

void	Cgi::addFields(std::string field, std::string token)
{
	std::string	variable;

	//check for host/type/length to be parse separated cause dont have HTTP_ preposition

	variable.assign(httpToCgiHeader(field));
	variable.append(token);
	this->_env.push_back(variable);
}

void	Cgi::getFieldFromUri(Request *request)
{
	std::string	token;
	token.assign(REQUEST_URI);
	token.append("=" + request->getUri());
	this->_env.push_back(token);
	token.assign(QUERY_STRING);
	token.append("=" + request->getQueryString());
	this->_env.push_back(token);
	token.assign(METHOD);
	if (request->getMethod() == GET)
		token.append("=GET");
	else if (request->getMethod() == POST)
		token.append("=POST");
	else if (request->getMethod() == DELETE)
		token.append("=DELETE");
	this->_env.push_back(token);
}
