/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:23:47 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "tokens.hpp"
// #include "Logger.hpp"

int	ConfigParser::checkDirective(std::string &token)
{
	for (int i = 0; i != NONE; ++i)
	{
		if (token == DIRECTIVE[i])
			return (i);
	}
	throw (std::runtime_error("Unrecognized token " + token));
}

Location	ConfigParser::parseLocationLoop(std::map<std::string, Location> &locations,
							std::vector<std::string>::iterator &it,
							std::vector<std::string>::iterator &it_end,
							std::vector<std::string>::iterator &name)
{
	(void)locations;
	Location	current;
	parseAlias(current, name, it_end);
	while (true)
	{
		if (++it == it_end)
			throw (std::runtime_error("Location scope not closed by '}'"));
		int	directive = checkDirective(*it);
		if (++it == it_end)
			throw (std::runtime_error("Empty directive " + DIRECTIVE[directive]));
		switch (directive)
		{
			case ROOT:
			parseRoot(current, it, it_end);
			break ;

			case ALIAS:
			parseAlias(current, it, it_end);
			break ;

			case CLIENT_MAX_BODY_SIZE:
			parseClientMaxBodySize(current, it, it_end);
			break ;

			case CGI_SUFFIX:
			parseCgi(current, it, it_end);
			break ;

			case ALLOWED_METHODS:
			parseAllowedMethods(current, it, it_end);
			break ;

			case RETURN:
			parseReturn(current, it, it_end);
			break ;

			case AUTOINDEX:
			parseAutoIndex(current, it, it_end);
			break ;

			case ERROR_PAGE:
			parseErrorPages(current, it, it_end);
			break ;

			case POST_LOCATION:
			parsePostLocation(current, it, it_end);
			break ;

			case CLOSING_BRACKET:
			return (current);

			default :
			throw (std::runtime_error("Unauthorized directive in location scope \n-->" + *it));
		}
		it++;
		if (*it != ";")
			throw (std::runtime_error("too much argument in directive " + DIRECTIVE[directive] + "\n-->" + *it));
	}
}

void	ConfigParser::parseLocation(std::map<std::string, Location> &locations,
							   std::vector<std::string>::iterator &it,
							   std::vector<std::string>::iterator &it_end)
{
	// save current location name
	std::vector<std::string>::iterator	name = it;
	if (locations.find(*name) != locations.end())
		throw (std::runtime_error("location " + *name + " already exists"));
	if (*name == DIRECTIVE[CLOSING_BRACKET])
		throw (std::runtime_error("location need an path identifier"));

	it++;
	if (it == it_end || *it != "{")
		throw (std::runtime_error("Missing bracket after location '" + *name + "'\n-->" + *it ));
	Location current = parseLocationLoop(locations, it, it_end, name);
	locations.insert(std::make_pair(*name, current));
}

std::map<std::string, Location>	ConfigParser::parseServerLoop(Server &current,
							   std::vector<std::string>::iterator &it,
							   std::vector<std::string>::iterator &it_end)
{
	std::map<std::string, Location> locations;
	while (true)
	{
		switch (checkDirective(*it))
		{
			case LISTEN:
				parseListen(current, it, it_end);
				break ;
			case LOCATION:
				it++;
				parseLocation(locations, it, it_end);
				break ;
			case CLOSING_BRACKET:
			return (locations);
			default :
				throw (std::runtime_error("Unauthorized directive in server scope :" + *it));
		}
	}
}


void	ConfigParser::parseServer(std::vector<Server> &servers,
							   std::vector<std::string>::iterator &it,
							   std::vector<std::string>::iterator &it_end)
{
	Server	current;
	// check if there is an opening bracket
	if (*it != "{")
	{
		throw (std::runtime_error("Unrecognized token " + *it));
	}
	it++;

	// build location for current
	std::map<std::string, Location> locations = parseServerLoop(current, it, it_end);

	// check interface:port are unique
	for (std::vector<Server>::iterator it1 = servers.begin(); it1 != servers.end(); it++)
	{
		// if (current *it1)
			// throw (std::runtime_error("Server may not have same port" + *it));
	}
	servers.push_back(current);
}

void	ConfigParser::run(char *file)
{
	std::vector<std::string>	token;
	std::vector<Server>	servers;

	tokenize(token, file);
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator it_end = token.end();
	for (it = token.begin(); it != it_end; ++it)
	{
		switch (checkDirective(*it))
		{
			case SERVER:
			it++;
			parseServer(servers, it, it_end);
			break ;

			default :
				throw (std::runtime_error("Unauthorized directive in server scope :" + *it));
		}
	}
}
