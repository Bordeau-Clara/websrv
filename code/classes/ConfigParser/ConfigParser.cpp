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
#include <sstream>

static std::string	extractStr(const char *file)
{
	//https://stackoverflow.com/questions/29310166/check-if-a-fstream-is-either-a-file-or-directory
	std::fstream		fs(file);
	if (fs.fail())
		throw (std::runtime_error("Cannot open '" + std::string(file) + '\''));

	std::ostringstream	ostrs;
	ostrs	<< fs.rdbuf();
	/**/streams.get(LOG_CONFIGPARSER) << SEPARATOR + "VV Extracted str VV" << std::endl << ostrs.str() << std::endl;
	return (ostrs.str());
}

ConfigParser::ConfigParser(char *file): _last_directive(NONE)
{
	this->_str = extractStr(file);
}
