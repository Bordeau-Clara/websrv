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

#include "ConfigParser.hpp"
#include "Location.hpp"
#include "tokens.hpp"

void	ConfigParser::parseListen(Server &current)
{
	(void)current;
}

void	ConfigParser::parseRoot(Location &current)
{
	if (get().at(0) != '/')
		throw (std::runtime_error("root must be a valid path syntax\n-->" + get()));
	current.setRoot(get());
}

void	ConfigParser::parseAlias(Location &current)
{
	if (get().at(0) != '/')
		throw (std::runtime_error("alias must be a valid path syntax\n-->" + get()));
	current.setAlias(get());
}

#include <cstdlib>
void	ConfigParser::parseClientMaxBodySize(Location &current)
{
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
}

void	ConfigParser::parseCgi(Location &current)
{
	std::set<std::string> suffixes = current.getCgiSuffix();
	for (;get() != ";" && !end(); next())
	{
		if (get().at(0) != '.')
			throw (std::runtime_error("CGI suffix must start with '.'\n-->" + get()));
		suffixes.insert(get());
	}
	if (end())
		throw (std::runtime_error("Unclosed directive CGI \n-->edit"));
	_token_it--;
	current.setCgiSuffixSet(suffixes);
}

void	ConfigParser::parseAllowedMethods(Location &current)
{
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
		throw (std::runtime_error("Unclosed directive methods \n-->" + *(--_token_it)));
	_token_it--;
	current.setMethods(methods);
}

void	ConfigParser::parseReturn(Location &current)
{
	if (0)
		throw (std::runtime_error(DIRECTIVE[RETURN] + " must be a valid url\n-->" + get()));
	current.setRedirect(get());
}

void	ConfigParser::parseAutoIndex(Location &current)
{
	bool	autoindex;
	if (get() == "on" || get() == "ON")
		autoindex = true;
	else if (get() == "off" || get() == "OFF")
		autoindex = false;
	else
		throw (std::runtime_error("In directive " + DIRECTIVE[AUTOINDEX] + " :unrecognized token\n-->" + get()));
	current.setAutoindex(autoindex);
}

void	ConfigParser::parseErrorPages(Location &current)
{
	static const int	codes[] = {404};
	static const int	max = sizeof(codes) / sizeof(int);

	std::vector<std::string>::iterator it_start = _token_it;

	int	size = 0;
	for (;get() != ";" && !end(); next())
		size++;
	if (end())
		throw (std::runtime_error("Unclosed directive " + DIRECTIVE[ERROR_PAGE] + " \n-->" + *(--_token_it)));
	if (size < 2)
		throw (std::runtime_error("Directive" + DIRECTIVE[ERROR_PAGE] + "need at least 2 arguments\n-->" + *(--_token_it)));

	// it at last argument
	--_token_it;
	// should be the location of each error_pages
	if (get().at(0) != '/')
		throw (std::runtime_error(DIRECTIVE[ERROR_PAGE] + " must be a valid path syntax\n-->" + get()));
	for (; it_start < _token_it; it_start++)
	{
		int	nb;
		try// extraire le nb de la str
		{
			nb = std::strtol(it_start->c_str(), NULL, 10);
		}
		catch (std::invalid_argument const& ex)
		{
			throw (std::runtime_error("max_client_body_size must be a number argument\n-->" + *it_start));
		}
		catch (std::out_of_range const& ex)
		{
			throw (std::runtime_error("max_client_body_size must not exceed int value\n-->" + *it_start));
		}
		for (int i = 0; i < max + 1; i++)
		{
			if (i == max)
				throw ("Unrecognized error page code \n-->" + *it_start);
			if (codes[i] == nb)
				current.setErrorPage(nb, get());
		}
	}
}

void	ConfigParser::parsePostLocation(Location &current)
{
	if (get().at(0) != '/')
		throw (std::runtime_error("post_location must be a valid path syntax\n-->" + get()));
	current.setRoot(get());
}
