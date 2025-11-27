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

void	ConfigParser::parseListen(Server &current,
								std::vector<std::string>::iterator &it,
								std::vector<std::string>::iterator &it_end)
{
	(void)current;
	(void)it;
	(void)it_end;
}

void	ConfigParser::parseRoot(Location &current,
								std::vector<std::string>::iterator &it,
								std::vector<std::string>::iterator &it_end)
{
	(void)it_end;
	if (it->at(0) != '/')
		throw (std::runtime_error("root must be a valid path syntax\n-->" + *it));
	current.setRoot(*it);
}

void	ConfigParser::parseAlias(Location &current,
								std::vector<std::string>::iterator &it,
								std::vector<std::string>::iterator &it_end)
{
	(void)it_end;
	if (it->at(0) != '/')
		throw (std::runtime_error("alias must be a valid path syntax\n-->" + *it));
	current.setAlias(*it);
}

#include <cstdlib>
void	ConfigParser::parseClientMaxBodySize(Location &current,
							   std::vector<std::string>::iterator &it,
							   std::vector<std::string>::iterator &it_end)
{
	(void)it_end;
	int i;
	try
	{
		i = std::strtol(it->c_str(), NULL, 10);
    }
	catch (std::invalid_argument const& ex)
	{
		throw (std::runtime_error("max_client_body_size must be a number argument\n-->" + *it));
	}
	catch (std::out_of_range const& ex)
	{
		throw (std::runtime_error("max_client_body_size must not exceed int value\n-->" + *it));
	}
	current.setClientMaxBodySize(i);
}

void	ConfigParser::parseCgi(Location &current,
							   std::vector<std::string>::iterator &it,
							   std::vector<std::string>::iterator &it_end)
{
	std::set<std::string> suffixes = current.getCgiSuffix();
	for (;*it != ";" && it != it_end; it++)
	{
		if (it->at(0) != '.')
			throw (std::runtime_error("CGI suffix must start with '.'\n-->" + *it));
		suffixes.insert(*it);
	}
	if (it == it_end)
		throw (std::runtime_error("Unclosed directive CGI \n-->" + *(--it)));
	it--;
	current.setCgiSuffixSet(suffixes);
}

void	ConfigParser::parseAllowedMethods(Location &current,
							   std::vector<std::string>::iterator &it,
							   std::vector<std::string>::iterator &it_end)
{
	bool	methods[3]= {
					current.getMethods()[GET],
					current.getMethods()[POST],
					current.getMethods()[DELETE]
					};
	for (;*it != ";" && it != it_end; it++)
	{
		int i;
		for (i = 0; i < 3; i++)
		{
			if (*it == METHODS[i])
			{
				methods[i] = true;
				break ;
			}
		}
		if (i == 3)
			throw (std::runtime_error("Unrecognized http method\n-->" + *it));
	}
	if (it == it_end)
		throw (std::runtime_error("Unclosed directive methods \n-->" + *(--it)));
	it--;
	current.setMethods(methods);
}

void	ConfigParser::parseReturn(Location &current,
							   std::vector<std::string>::iterator &it,
							   std::vector<std::string>::iterator &it_end)
{
	(void)it_end;
	if (0)
		throw (std::runtime_error(DIRECTIVE[RETURN] + " must be a valid url\n-->" + *it));
	current.setRedirect(*it);
}

void	ConfigParser::parseAutoIndex(Location &current,
							   std::vector<std::string>::iterator &it,
							   std::vector<std::string>::iterator &it_end)
{
	(void)it_end;
	bool	autoindex;
	if (*it == "on" || *it == "ON")
		autoindex = true;
	else if (*it == "off" || *it == "OFF")
		autoindex = false;
	else
		throw (std::runtime_error("In directive " + DIRECTIVE[AUTOINDEX] + " :unrecognized token\n-->" + *it));
	current.setAutoindex(autoindex);
}

void	ConfigParser::parseErrorPages(Location &current,
							   std::vector<std::string>::iterator &it,
							   std::vector<std::string>::iterator &it_end)
{
	static const int	codes[] = {404};
	static const int	max = sizeof(codes) / sizeof(int);

	std::vector<std::string>::iterator it_start = it;

	int	size = 0;
	for (;*it != ";" && it != it_end; it++)
		size++;
	if (it == it_end)
		throw (std::runtime_error("Unclosed directive " + DIRECTIVE[ERROR_PAGE] + " \n-->" + *(--it)));
	if (size < 2)
		throw (std::runtime_error("Directive" + DIRECTIVE[ERROR_PAGE] + "need at least 2 arguments\n-->" + *(--it)));

	// it at last argument
	it--;
	// should be the location of each error_pages
	if (it->at(0) != '/')
		throw (std::runtime_error(DIRECTIVE[ERROR_PAGE] + " must be a valid path syntax\n-->" + *it));
	for (; it_start < it; it_start++)
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
				current.setErrorPage(nb, *it);
		}
	}
}

void	ConfigParser::parsePostLocation(Location &current,
							std::vector<std::string>::iterator &it,
							std::vector<std::string>::iterator &it_end)
{
	(void)it_end;
	if (it->at(0) != '/')
		throw (std::runtime_error("post_location must be a valid path syntax\n-->" + *it));
	current.setRoot(*it);
}
