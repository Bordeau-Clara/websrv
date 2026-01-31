/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 14:22:22 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"
#include <iostream>
#include <string>
#include "helpers.hpp"
#include "logfiles.hpp"
#include "webserv.hpp"

FileStream	streams;
const std::string MIMES_PATH = "mime.types";

int	main(int argc, char **argv)
{
	try// to open stream log file
	{
		streams.add(LOG_CONFIGPARSER);
		streams.add(LOG_SERVER);
		streams.add(LOG_DIRECTIVE);
		streams.add(LOG_LOCATION);
		streams.add(LOG_EVENT);
		streams.add(LOG_REQUEST);
	}
	catch (std::exception	&e)// open issue
	{
		std::cerr << "stream initialisation issue :"<< e.what() << std::endl;
		return (1);
	}

	std::map<std::string,std::string> mimes;
	mimeTypesMapFill(MIMES_PATH.c_str(), mimes);
	for (std::map<std::string, std::string>::iterator it = mimes.begin()
		; it != mimes.end(); it++)
	{
		streams.get(LOG_LOCATION) << it->first + '=' + it->second << std::endl;
	}
	std::vector<Server>	servers;
	try// to fill server vector with config file
	{
		ArgChecker::checkargs(argc);
		ConfigParser	parser(argv[1]);
		servers = parser.run();
	}
	catch (std::exception	&e)// parse error exception
	{
		std::cerr << "configParser Exception caught :"<< e.what() << std::endl;
		return (1);
	}
	printServerInfo(servers);
	Request::initFields();
	try// init epoll loop then run webserv
	{
		EventManager	Webserv(servers);
		Webserv.run();
	}
	catch (std::exception &e)// epoll system call issue or runtime issues
	{
		std::cerr << "le WEBSERV BOBO :" << e.what() << std::endl;
	}
}
