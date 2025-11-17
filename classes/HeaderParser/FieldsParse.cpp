/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FieldsParse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 14:24:44 by cbordeau          #+#    #+#             */
/*   Updated: 2025/11/17 16:41:47 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "HeaderParser.hpp"
#include <iostream>

void	Request::parseHost(std::string str, Request* request)
{
	std::cout << "Host is : " << str << std::endl;
}

void	Request::parseAccept(std::string, Request*)
{
}

void	Request::parseAcceptEncoding(std::string, Request*)
{
}

void	Request::parseCookies(std::string, Request*)
{
}

void	Request::parseLanguage(std::string, Request*)
{
}

void	Request::parseAuthorization(std::string, Request*)
{
}

void	Request::parseConnection(std::string, Request*)
{
}

void	Request::parseIfModifiedSince(std::string, Request*)
{
}

void	Request::parseExpect(std::string, Request*)
{
}

void	Request::parseContentType(std::string, Request*)
{
}

void	Request::parseContentLength(std::string, Request*)
{
}

void	Request::parseTransferEncoding(std::string, Request*)
{
}
