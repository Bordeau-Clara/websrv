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
// #include "../Request/Request.hpp"

Cgi::Cgi(): _env(CGI_HEADER)
{
}

std::string	httpToCgiHeader(std::string field)
{
	for (int i = 0; field[i] != std::string::npos; i++)
	{
	}
	return (HTTP + field);
}

void	Cgi::addFields(std::string field, std::string token)
{
	std::string	variable;

	//check for host/type/length to be parse separated cause dont have HTTP_ preposition

	variable.assign(httpToCgiHeader(field));
}

void	Cgi::getFieldFromUri(Request *request)
{
	(void)request;
}
