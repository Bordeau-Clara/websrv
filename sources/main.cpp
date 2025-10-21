/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:43:59 by cuistobal         #+#    #+#             */
/*   Updated: 2025/10/21 16:46:47 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

volatile sig_atomic_t g_signal_received = 0;

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGQUIT) {
        g_signal_received = signal;
    }
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		return (USAGE_CODE);
	}
	signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
	(void)argv;
	try
	{
		// std::vector<ServerSocket>	ServerSocket;
		std::string	file = extractStr(argv[1]);
		commentFilter(file);
		std::vector<std::string>	token;
		tokenize(token, file);
		std::cout << file << std::endl;
        
/* DEBUG
		std::vector<Server>::iterator it = servers.begin();
		for ( ; it != servers.end(); it++) {
			it->print();
			std::vector<Location> locations = it->getLocations();
			std::vector<Location>::iterator it = locations.begin();
			for (; it != locations.end(); it++) {
				it->print();
			}	
		}
*/
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
		return (1);
	}
	return (SUCCESS);
}
