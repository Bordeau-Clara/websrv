#include "webserv.hpp"

void startServers(std::vector<Server>& servers) {
	std::vector<Server>::iterator it = servers.begin();
	for(; it != servers.end(); it++) {
		it->startServer();
		it->listening();
	}
}
