/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 10:47:56 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <limits>
#include <stdexcept>

#include "ConfigParser.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "logfiles.hpp"
#include "tokens.hpp"

static const int LOCALHOST = 0x7F000001;
static const char *DIGITS = "0123456789";

static unsigned int	parse_ipv4(std::string str)
{
	if (str == "localhost")
		return (LOCALHOST);
	if (str.empty())
		return (0);
	unsigned int ipv4;
	try// extract nb from str
	{
		unsigned long int	nb[4];
		char				*end;
		const char			*p = str.c_str();
		
		for (int i = 0; i < 4; i++)
		{
			nb[i] = std::strtol(p, &end, 10);
			if (p == end)// does not go forward
				throw (std::invalid_argument("missing number"));
			if (nb[i] > std::numeric_limits<unsigned char>::max()) // exceed 255
				throw (std::out_of_range("each number cannot exceed 255"));
			if (i != 3 && *end != '.') // is not ended by .
				throw (std::invalid_argument("each number should be separated by dot"));
			else if (*end)// trail garbage
				throw (std::invalid_argument("garbage found at the end of interface"));
			p =+ end + 1; // go after .
		}
		ipv4 = nb[0] << 24
		| nb[1] << 16
		| nb[2] << 8
		| nb[3];
	}
	catch (std::invalid_argument const& ex)
	{
		throw (std::runtime_error("listen interface format invalid:" + std::string(ex.what()) + "\n-->" + str));
	}
	catch (std::out_of_range const& ex)
	{
		throw (std::runtime_error("listen interface too large:" + std::string(ex.what()) + "\n-->" + str));
	}
	return (ipv4);
}

static unsigned short	parse_port(std::string str)
{
	if (str.find_first_not_of(DIGITS) != std::string::npos)
		throw (std::runtime_error("listen port must be only digits\n-->" + str));
	unsigned long int nb;
	try// extract nb from str
	{
		nb = std::strtol(str.c_str(), NULL, 10);
	}
	catch (std::invalid_argument const& ex)
	{
		throw (std::runtime_error("listen port must be a number argument\n-->" + str));
	}
	catch (std::out_of_range const& ex)
	{
		throw (std::runtime_error("listen port must not exceed short int value\n-->" + str));
	}
	if (nb > std::numeric_limits<unsigned short>::max())
		throw (std::runtime_error("listen port must not exceed short int value\n-->" + str));
	return (static_cast<unsigned short>(nb));
}

void	ConfigParser::parseListen(Server &current)
{
	streams.print(LOG_DIRECTIVE) << "[" + DIRECTIVE[getDirective()] + "]"<< std::endl;
	if (end())
		throw (std::runtime_error("Empty directive " + DIRECTIVE[getDirective()]));

	std::string	port, interface;
	
	std::string::size_type two_dots = get().find(':');
	if (two_dots == std::string::npos)
	{
		interface = "";
		port = get();
	}
	else
	{
		port = get().substr(two_dots + 1);
		if (port.empty())
			throw (std::runtime_error("listen must specify a port\n-->" + get()));
		interface = get().substr(0, two_dots);
		if (interface.empty())
			throw (std::runtime_error("listen must specify an ipv4 interface before ':'\n-->" + get()));
	}

	current.setPort(parse_port(port));
	current.setInterface(parse_ipv4(interface));
	next();
	if (get() != ";")
		throw (std::runtime_error("too much argument in directive " + DIRECTIVE[getDirective()] + "\n-->" + get()));
	streams.print(LOG_DIRECTIVE) << "directive " + DIRECTIVE[getDirective()] + " succeed" << std::endl;
}

void	ConfigParser::parseRoot(Location &current)
{
	streams.print(LOG_DIRECTIVE) << "[" + DIRECTIVE[getDirective()] + "]"<< std::endl;
	if (end())
		throw (std::runtime_error("Empty directive " + DIRECTIVE[getDirective()]));

	// if (get().at(0) != '/')
	// 	throw (std::runtime_error("root must be a valid path syntax\n-->" + get()));
	current.setRoot(get());
	next();
	if (get() != ";")
		throw (std::runtime_error("too much argument in directive " + DIRECTIVE[getDirective()] + "\n-->" + get()));
	streams.print(LOG_DIRECTIVE) << "directive " + DIRECTIVE[getDirective()] + " succeed" << std::endl;
}

void	ConfigParser::parseAlias(Location &current)
{
	streams.print(LOG_DIRECTIVE) << "[" + DIRECTIVE[getDirective()] + "]"<< std::endl;
	if (end())
		throw (std::runtime_error("Empty directive " + DIRECTIVE[getDirective()]));

	// if (get().at(0) != '/')
	// 	throw (std::runtime_error("alias must be a valid path syntax\n-->" + get()));
	current.setAlias(get());
	next();
	if (get() != ";")
		throw (std::runtime_error("too much argument in directive " + DIRECTIVE[getDirective()] + "\n-->" + get()));
	streams.print(LOG_DIRECTIVE) << "directive " + DIRECTIVE[getDirective()] + " succeed" << std::endl;
}

void	ConfigParser::parseClientMaxBodySize(Location &current)
{
	streams.print(LOG_DIRECTIVE) << "[" + DIRECTIVE[getDirective()] + "]"<< std::endl;
	if (end())
		throw (std::runtime_error("Empty directive " + DIRECTIVE[getDirective()]));

	int i;
	try
	{
		i = std::strtol(get().c_str(), NULL, 10);
    }
	catch (std::invalid_argument const& ex)
	{
		throw (std::runtime_error("max_client_body_size must be a number argument\n-->" + get()));
	}
	catch (std::out_of_range const& ex)
	{
		throw (std::runtime_error("max_client_body_size must not exceed int value\n-->" + get()));
	}
	current.setClientMaxBodySize(i);
	next();
	if (get() != ";")
		throw (std::runtime_error("too much argument in directive " + DIRECTIVE[getDirective()] + "\n-->" + get()));
	streams.print(LOG_DIRECTIVE) << "directive " + DIRECTIVE[getDirective()] + " succeed" << std::endl;
}

void	ConfigParser::parseCgi(Location &current)
{
	streams.print(LOG_DIRECTIVE) << "[" + DIRECTIVE[getDirective()] + "]"<< std::endl;
	if (end())
		throw (std::runtime_error("Empty directive " + DIRECTIVE[getDirective()]));

	std::set<std::string> suffixes = current.getCgiSuffix();
	for (;get() != ";" && !end(); next())
	{
		if (get().at(0) != '.')
			throw (std::runtime_error("CGI suffix must start with '.'\n-->" + get()));
		suffixes.insert(get());
	}
	if (end())
		throw (std::runtime_error("Unclosed directive CGI \n-->edit"));
	current.setCgiSuffixSet(suffixes);
	streams.print(LOG_DIRECTIVE) << "directive " + DIRECTIVE[getDirective()] + " succeed" << std::endl;
}

void	ConfigParser::parseAllowedMethods(Location &current)
{
	streams.print(LOG_DIRECTIVE) << "[" + DIRECTIVE[getDirective()] + "]"<< std::endl;
	if (end())
		throw (std::runtime_error("Empty directive " + DIRECTIVE[getDirective()]));

	bool	methods[3]= {
					current.getMethods()[GET],
					current.getMethods()[POST],
					current.getMethods()[DELETE]
					};
	for (;get() != ";" && !end(); next())
	{
		int i;
		for (i = 0; i < 3; i++)
		{
			if (get() == METHODS[i])
			{
				methods[i] = true;
				break ;
			}
		}
		if (i == 3)
			throw (std::runtime_error("Unrecognized http method\n-->" + get()));
	}
	if (end())
		throw (std::runtime_error("Expected ';' at end of" + DIRECTIVE[getDirective()] + "\n-->" + *(--_token_it)));
	current.setMethods(methods);
	streams.print(LOG_DIRECTIVE) << "directive " + DIRECTIVE[getDirective()] + " succeed" << std::endl;
}

void	ConfigParser::parseReturn(Location &current)
{
	streams.print(LOG_DIRECTIVE) << "[" + DIRECTIVE[getDirective()] + "]"<< std::endl;
	if (end())
		throw (std::runtime_error("Empty directive " + DIRECTIVE[getDirective()]));

	//TBD
	if (0)
		throw (std::runtime_error(DIRECTIVE[RETURN] + " must be a valid url\n-->" + get()));
	current.setRedirect(get());
	next();
	if (get() != ";")
		throw (std::runtime_error("too much argument in directive " + DIRECTIVE[getDirective()] + "\n-->" + get()));
	streams.print(LOG_DIRECTIVE) << "directive " + DIRECTIVE[getDirective()] + " succeed" << std::endl;
}

void	ConfigParser::parseAutoIndex(Location &current)
{
	streams.print(LOG_DIRECTIVE) << "[" + DIRECTIVE[getDirective()] + "]"<< std::endl;
	if (end())
		throw (std::runtime_error("Empty directive " + DIRECTIVE[getDirective()]));

	bool	autoindex;
	if (get() == "on" || get() == "ON")
		autoindex = true;
	else if (get() == "off" || get() == "OFF")
		autoindex = false;
	else
		throw (std::runtime_error("In directive " + DIRECTIVE[AUTOINDEX] + " :unrecognized token\n-->" + get()));
	current.setAutoindex(autoindex);
	next();
	if (get() != ";")
		throw (std::runtime_error("Expected ';' at end of" + DIRECTIVE[getDirective()] + "\n-->" + *(--_token_it)));
	streams.print(LOG_DIRECTIVE) << "directive " + DIRECTIVE[getDirective()] + " succeed" << std::endl;
}

void	ConfigParser::parseErrorPages(Location &current)
{
	streams.print(LOG_DIRECTIVE) << "[" + DIRECTIVE[getDirective()] + "]"<< std::endl;
	if (end())
		throw (std::runtime_error("Empty directive " + DIRECTIVE[getDirective()]));

	static const int	codes[] = {404};
	static const int	max = sizeof(codes) / sizeof(int);

	std::vector<std::string>::iterator it_start = _token_it;
	std::vector<std::string>::iterator page;
	{
		int	size = 0;
		for (;get() != ";" && !end(); next())
		{
			size++;
			page = _token_it;// get last argument (the page)
						// should be the location of each error_pages
		}
		if (end())
			throw (std::runtime_error("Expected ';' at end of" + DIRECTIVE[getDirective()] + "\n-->" + *(--_token_it)));
		if (size < 2)
			throw (std::runtime_error("Directive" + DIRECTIVE[ERROR_PAGE] + "need at least 2 arguments\n-->" + *(--_token_it)));
	}

	//TBD
	// if ((page->at(0) != '/'))
	// 	throw (std::runtime_error(DIRECTIVE[ERROR_PAGE] + " must be a valid path syntax\n-->" + *page));
	// for each token until 
	for (; it_start < _token_it; it_start++)
	{
		int	nb;
		try// extraire le nb de la str
		{
			nb = std::strtol(it_start->c_str(), NULL, 10);
		}
		catch (std::invalid_argument const& ex)
		{
			throw (std::runtime_error("error page code must be a number argument\n-->" + *it_start));
		}
		catch (std::out_of_range const& ex)
		{
			throw (std::runtime_error("error page code must not exceed int value\n-->" + *it_start));
		}
		for (int i = 0; i < max + 1; i++)
		{
			if (i == max)
				throw ("Unrecognized error page code \n-->" + *it_start);
			if (codes[i] == nb)
			{
				current.setErrorPage(nb, *page);
				streams.print(LOG_DIRECTIVE) << nb << ": " + *page << std::endl;
			}
		}
	}
	streams.print(LOG_DIRECTIVE) << "directive " + DIRECTIVE[getDirective()] + " succeed" << std::endl;
}

void	ConfigParser::parsePostLocation(Location &current)
{
	streams.print(LOG_DIRECTIVE) << "[" + DIRECTIVE[getDirective()] + "]"<< std::endl;
	if (end())
		throw (std::runtime_error("Empty directive " + DIRECTIVE[getDirective()]));

	if (get().at(0) != '/')
		throw (std::runtime_error("post_location must be a valid path syntax\n-->" + get()));
	current.setRoot(get());
	next();
	if (get() != ";")
		throw (std::runtime_error("too much argument in directive " + DIRECTIVE[getDirective()] + "\n-->" + get()));
	streams.print(LOG_DIRECTIVE) << "directive " + DIRECTIVE[getDirective()] + " succeed" << std::endl;
}
