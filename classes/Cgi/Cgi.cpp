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

void	Cgi::addFields(std::string field, std::string token)
{
	(void)field;
	(void)token;
}

void	Cgi::getFieldFromUri(Request *request)
{
	(void)request;
}
