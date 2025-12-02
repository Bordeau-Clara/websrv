/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 14:05:30 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

static const std::string	WHITESPACES = "\f\r\n\t\v ";
static const std::string	OPERAND = "{};";

static const std::string	DIRECTIVE[] =
	{
		"server",				// {}
		"location",				// expr{}
		"listen",				// short (port)
		"error_page",			// int
		"client_max_body_size",	// int
		"allow_methods",		//
		"return",				// URL (redirection)
		"root",					// path (debut du path)
		"alias",				// expr (remplace location)
		"autoindex",			// on |off
		"cgi_ext",			// .expr
		"post_directory",		// path
		"{",					//
		"}",					//
		";"						//
	};

static const int SERVER					= 0;
static const int LOCATION				= 1;
static const int LISTEN					= 2;
static const int ERROR_PAGE				= 3;
static const int CLIENT_MAX_BODY_SIZE	= 4;
static const int ALLOWED_METHODS		= 5;
static const int RETURN			    	= 6;
static const int ROOT					= 7;
static const int ALIAS					= 8;
static const int AUTOINDEX				= 9;
static const int CGI_SUFFIX				= 10;
static const int POST_LOCATION			= 11;
static const int OPENING_BRACKET		= 12;
static const int CLOSING_BRACKET		= 13;
static const int SEMICOLOM				= 14;
static const int NONE					= 15;
