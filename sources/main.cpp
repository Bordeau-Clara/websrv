/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:43:59 by cuistobal         #+#    #+#             */
/*   Updated: 2025/10/07 09:03:22 by chrleroy         ###   ########.fr       */
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

int main(int argc, char **argv) {

	Logger	logger;

	if (argc > 2) {
		logger.logMsg(ERROR, USAGE);
		return USAGE_CODE;
	}

	signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);

	std::string configFile = argc == 2 ? argv[1] : DEFAULT_CONFIG;	
	bool allowFallback = (argc == 2);
	std::vector<std::string> configFileContent;
    std::vector<Server> servers;
	try {
		configFileContent = parseConfig(configFile, allowFallback);
        ConfigParser configParser(configFileContent);
        servers = configParser.parse();
        
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
		startServers(servers);
/*
		runServer();
	
		exitServer();
*/

	} catch (const CustomException& e) {
		std::string msg = std::string(e.what());
		if (!e.get_nested_message().empty()) {
			msg += " ";
			msg += e.get_nested_message();
		}
		return errorMessage(msg.c_str(), e.get_code());
	} catch (const std::exception& e) {
		return errorMessage(e.what(), OPEN_ERROR_CODE);
	}	
	return SUCCESS;
}
