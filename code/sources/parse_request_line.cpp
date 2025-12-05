/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request_line.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 11:33:51 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing_header.hpp"

void	parse_request_line(Request *request, std::string token)
{
	//parser methode
	//reconstituer/parser URI
	//check HTTP/1.1
	
	std::string::size_type cursor= 0;

	if (move_cursor(&cursor, token, " "))
	{
		request->parseMethod(token.substr(0, cursor));
		token.erase(0, cursor + 1);
	}
	else
	{
		//error, throw ?
	}
	if (move_cursor(&cursor, token, " "))
	{
		request->parseURI(token.substr(0, cursor));
		token.erase(0, cursor + 1);
	}
	else
	{
		//error, throw ?
	}
	if (token.compare("HTTP/1.1"))
	{
		std::cout << RED << "Wrong HTTP protocol :" + token << WHITE << std::endl;
		//error, throw
	}
}
