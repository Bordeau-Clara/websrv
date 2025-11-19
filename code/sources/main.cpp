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
	(void)argv;
	try
	{
		ArgChecker::checkargs(argc);
		ConfigParser::run(argv[1]);
	}
	catch (std::exception	&e)
	{
		std::cerr << e.what() << std::endl;
	}
}
