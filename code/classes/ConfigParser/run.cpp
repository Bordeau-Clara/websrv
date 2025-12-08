/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 11:58:41 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "logfiles.hpp"
#include "tokens.hpp"

Location	ConfigParser::parseLocationLoop(Location &current)
{
	streams.get(LOG_DIRECTIVE) << "SCOPE LOCATION" << std::endl;
	while (true)
	{
		checkDirective();
		switch (getDirective())
		{
			case ROOT:
			next();
			parseRoot(current);
			break ;

			case ALIAS:
			next();
			parseAlias(current);
			break ;

			case CLIENT_MAX_BODY_SIZE:
			next();
			parseClientMaxBodySize(current);
			break ;

			case CGI_SUFFIX:
			next();
			parseCgi(current);
			break ;

			case ALLOWED_METHODS:
			next();
			parseAllowedMethods(current);
			break ;

			case RETURN:
			next();
			parseReturn(current);
			break ;

			case AUTOINDEX:
			next();
			parseAutoIndex(current);
			break ;

			case ERROR_PAGE:
			next();
			parseErrorPages(current);
			break ;

			case POST_LOCATION:
			next();
			parsePostLocation(current);
			break ;

			case CLOSING_BRACKET:
			return (current);

			default :
			throw (std::runtime_error("Unauthorized directive in location scope \n-->" + get()));
		}
		next();
		if (end())
			throw (std::runtime_error("Unexpected end context location not closed by '}'"));
	}
}

void	ConfigParser::parseLocation(std::map<std::string, Location> &locations)
{
	// save current location name
	std::string	name = get();
	if (locations.find(name) != locations.end())
		throw (std::runtime_error("location " + name + " already exists"));
	if (name == DIRECTIVE[CLOSING_BRACKET])
		throw (std::runtime_error("location need an path identifier"));

	Location	current;
	current.setAlias(name);
	next();
	if (end() || get() != "{")
		throw (std::runtime_error("Missing bracket after location '" + name + "'\n-->" + get()));
	next();
	if (end())
		throw (std::runtime_error("Unexpected end context location not closed by '}'"));
	parseLocationLoop(current);
	locations.insert(std::make_pair(name, current));
}

std::map<std::string, Location>	ConfigParser::parseServerLoop(Server &current)
{
	std::map<std::string, Location> locations;
	while (true)
	{
		checkDirective();
		switch (getDirective())
		{
			case LISTEN:
				next();
				parseListen(current);
				break ;
			case LOCATION:
				next();
				parseLocation(locations);
				break ;
			case CLOSING_BRACKET:
			return (locations);
			default :
				throw (std::runtime_error("Unauthorized directive in server scope :" + DIRECTIVE[this->getDirective()]));
		}
		next();
	}
}

void	ConfigParser::parseServer(std::vector<Server> &servers)
{
	Server	current;
	// check if there is an opening bracket
	if (checkDirective() != OPENING_BRACKET)
	{
		throw (std::runtime_error("Missing opening bracket instead of " + this->get()));
	}
	next();
	if (end())
		throw (std::runtime_error("Unexpected end context server not closed by '}'"));

	// build location for current
	std::map<std::string, Location> locations = parseServerLoop(current);
	// should check interface:port are unique
	// should check if at least one location
	current.setLocationsMap(locations);
	servers.push_back(current);
}

std::vector<Server>	ConfigParser::run(void)
{
	std::vector<Server>	servers;

	for (this->tokenInit(); !this->end(); this->next())
	{
		switch (checkDirective())
		{
			case SERVER:
			next();
			parseServer(servers);
			if (end())
				return (servers);
			break ;

			default :
				throw (std::runtime_error("Unauthorized directive in global scope :" + this->get()));
		}
	}
	streams.get(LOG_CONFIGPARSER) << std::endl << "END";
	return (servers);
}
