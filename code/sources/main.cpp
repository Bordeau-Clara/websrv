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

int	main(int argc, char **argv)
{
	try
	{
		Logger::add(LOG_CONFIGPARSER);
		ArgChecker::checkargs(argc);
		ConfigParser	parser(argv[1]);
		parser.run();
	}
	catch (std::exception	&e)
	{
		std::cerr << "Exception caught :"<< e.what() << std::endl;
	}
	Logger::delete_streams();
}
