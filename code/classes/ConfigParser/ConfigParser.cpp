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
#include "Logger.hpp"
#include <sstream>

static std::string	extractStr(const char *file)
{
	//https://stackoverflow.com/questions/29310166/check-if-a-fstream-is-either-a-file-or-directory
	std::fstream		fs(file);
	if (fs.fail())
		throw (std::runtime_error("Cannot open '" + std::string(file) + '\''));

	std::ostringstream	ostrs;
	ostrs	<< fs.rdbuf();
	/**/Logger::print(LOG_CONFIGPARSER) << SEPARATOR + "|| Extracted str \nVV" << std::endl << ostrs;
	return (ostrs.str());
}

ConfigParser::ConfigParser(char *file)
{
	this->_str = extractStr(file);
}

int	ConfigParser::checkDirective(void)
{
	for (int i = 0; i != NONE; ++i)
	{
		if (get() == DIRECTIVE[i])
			return (i);
	}
	throw (std::runtime_error("Unrecognized token " + get()));
}

Location	ConfigParser::parseLocationLoop(Location &current)
{
	while (true)
	{
		next();
		if (end())
			throw (std::runtime_error("Location scope not closed by '}'"));
		int	directive = checkDirective();
		next();
		if (end())
			throw (std::runtime_error("Empty directive " + DIRECTIVE[directive]));
		switch (directive)
		{
			case ROOT:
			parseRoot(current);
			break ;

			case ALIAS:
			parseAlias(current);
			break ;

			case CLIENT_MAX_BODY_SIZE:
			parseClientMaxBodySize(current);
			break ;

			case CGI_SUFFIX:
			parseCgi(current);
			break ;

			case ALLOWED_METHODS:
			parseAllowedMethods(current);
			break ;

			case RETURN:
			parseReturn(current);
			break ;

			case AUTOINDEX:
			parseAutoIndex(current);
			break ;

			case ERROR_PAGE:
			parseErrorPages(current);
			break ;

			case POST_LOCATION:
			parsePostLocation(current);
			break ;

			case CLOSING_BRACKET:
			return (current);

			default :
			throw (std::runtime_error("Unauthorized directive in location scope \n-->" + get()));
		}
		next();
		if (get() != ";")
			throw (std::runtime_error("too much argument in directive " + DIRECTIVE[directive] + "\n-->" + get()));
	}
}

void	ConfigParser::parseLocation(std::map<std::string, Location> &locations)
{
	// save current location name
	std::vector<std::string>::iterator	name = _token_it;
	if (locations.find(*name) != locations.end())
		throw (std::runtime_error("location " + *name + " already exists"));
	if (*name == DIRECTIVE[CLOSING_BRACKET])
		throw (std::runtime_error("location need an path identifier"));
	Location	current;
	parseAlias(current);
	next();
	if (end() || get() != "{")
		throw (std::runtime_error("Missing bracket after location '" + *name + "'\n-->" + get()));
	parseLocationLoop(current);
	locations.insert(std::make_pair(*name, current));
}

std::map<std::string, Location>	ConfigParser::parseServerLoop(Server &current)
{
	std::map<std::string, Location> locations;
	while (true)
	{
		switch (checkDirective())
		{
			case LISTEN:
				parseListen(current);
				break ;
			case LOCATION:
				this->next();
				parseLocation(locations);
				break ;
			case CLOSING_BRACKET:
			return (locations);
			default :
				throw (std::runtime_error("Unauthorized directive in server scope :" + this->get()));
		}
	}
}


void	ConfigParser::parseServer(std::vector<Server> &servers)
{
	Server	current;
	// check if there is an opening bracket
	if (this->get() != "{")
	{
		throw (std::runtime_error("Unrecognized token " + this->get()));
	}
	this->next();

	// build location for current
	std::map<std::string, Location> locations = parseServerLoop(current);

	// check interface:port are unique
	// for (std::vector<Server>::iterator it1 = servers.begin(); it1 != servers.end(); this->next())
	// {
	// 	// if (current *it1)
	// 		// throw (std::runtime_error("Server may not have same port" + *it));
	// }
	servers.push_back(current);
}

inline void	ConfigParser::next(void)
{
	++_token_it;
}

bool	ConfigParser::end(void) const
{
	return(_token_it == _token_it_end);
}

std::string	ConfigParser::get(void)
{
	return(*_token_it);
}

std::vector<Server>	ConfigParser::run(void)
{
	std::vector<Server>	servers;

	for (this->tokenInit(); !this->end(); this->next())
	{
		switch (checkDirective())
		{
			case SERVER:
			this->next();
			parseServer(servers);
			break ;

			default :
				throw (std::runtime_error("Unauthorized directive in server scope :" + this->get()));
		}
	}
	return (servers);
}
