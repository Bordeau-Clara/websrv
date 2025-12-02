/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:37:55 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

FileStream	streams;

int	main(int argc, char **argv)
{
	std::vector<Server>	servers;
	try
	{
		streams.add(LOG_CONFIGPARSER);
		streams.add(LOG_SERVER);
		streams.add(LOG_DIRECTIVE);
		streams.add(LOG_LOCATION);
		ArgChecker::checkargs(argc);
		ConfigParser	parser(argv[1]);
		servers = parser.run();
	}
	catch (std::exception	&e)
	{
		std::cerr << "Exception caught :"<< e.what() << std::endl;
	}
	printServerInfo(servers);
}
