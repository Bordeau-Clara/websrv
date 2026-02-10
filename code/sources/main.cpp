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
#include "webserv.hpp"

std::map<std::string,std::string>	mimes;

int	main(int argc, char **argv)
{
	// mimes parser
	try// init mimes
	{
		mimeTypesMapFill(MIMES_PATH.c_str(), mimes);
	}
	catch (std::exception &e)// open or parsing mimes issues
	{
		std::cerr << MIMES_PATH << " error:" << e.what() << std::endl;
	}
	
	// config parser
	std::vector<Server>	servers;
	try// to fill server vector with config file
	{
		std::string	config;
	
		if (argc == 1)
		{
			config = DEFAULT_CONF_PATH;
			std::cerr << "warning: no config file.. falling back on " + DEFAULT_CONF_PATH << std::endl;
		}
		else if (argc >= 3)
			throw (TooMuchArgs());
		else
			config = argv[1];
		ConfigParser	parser(config.c_str());
		servers = parser.run();
	}
	catch (std::exception	&e)// parse error exception
	{
		std::cerr << "configParser Exception caught :"<< e.what() << std::endl;
		return (1);
	}
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
