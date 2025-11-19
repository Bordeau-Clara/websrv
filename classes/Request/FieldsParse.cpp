/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FieldsParse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 14:24:44 by cbordeau          #+#    #+#             */
/*   Updated: 2025/11/19 17:38:38 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <iostream>

void	Request::parseHost(std::string str)
{
	std::cout << "Host is : " << str << std::endl;
}

void	Request::parseAccept(std::string str)
{
	//content types the sender is able to anderstand
	//the server uses content negociation to select one of the proposals
	//inform the client with Content-Type response
	//usefull only if cgi, accept elements are passed as <CGI spec> argument
	//
	//format xxx, xxx;q=nb.nb
	//format to be checked
	//q is between 0 and 1, with a max of 3 decimals, by default 1
	std::cout << "Accept is : " << str << std::endl;
}

void	Request::parseAcceptEncoding(std::string str)
{
	std::cout << "Accept Encoding is : " << str << std::endl;
}

void	Request::parseCookies(std::string str)
{
	std::cout << "Cookies is : " << str << std::endl;
}

void	Request::parseLanguage(std::string str)
{
	std::cout << "Language is : " << str << std::endl;
}

void	Request::parseAuthorization(std::string str)
{
	std::cout << "Authorization is : " << str << std::endl;
}

void	Request::parseConnection(std::string str)
{
	std::cout << "Connection is : " << str << std::endl;
}

void	Request::parseIfModifiedSince(std::string str)
{
	std::cout << "If Modified Since is : " << str << std::endl;
}

void	Request::parseExpect(std::string str)
{
	std::cout << "Expect is : " << str << std::endl;
}

void	Request::parseContentType(std::string str)
{
	std::cout << "Content Type is : " << str << std::endl;
}

void	Request::parseContentLength(std::string str)
{
	std::cout << "Content Length is : " << str << std::endl;
}

void	Request::parseTransferEncoding(std::string str)
{
	std::cout << "Transfer Encoding is : " << str << std::endl;
}
