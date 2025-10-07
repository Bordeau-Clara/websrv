#include "Server.hpp"
#include "Logger.hpp"

Server::Server(const Server& other) : _config(other._config) {}

Server::~Server() {}

Server::Server(const Config& cfg) : _config(cfg) {}

Server& Server::operator=(const Server& server) {
	if (this != &server)
		_config = server._config;
	return *this;
}

void	addConnection() {

}

void	endConnection() {

}

void Server::print() {

	std::cout << "PRINTING SERVER CONFIG" << std::endl;
	std::cout << _config.getPort() << std::endl;
	std::cout << _config.getName() << std::endl;
	std::cout << _config.getRoot() << std::endl;

	std::map<int, std::string> errorPages= _config.getErrorPages();	
	std::map<int, std::string>::iterator i = errorPages.begin();	
	for ( ; i != errorPages.end(); i++) {
		std::pair<int, std::string> tmp = *i;
		std::cout << tmp.first << ": " << tmp.second << std::endl;
	}

	std::vector<Location> locs = _config.getLocations();
	std::vector<Location>::iterator it = locs.begin();    
	for ( ; it != locs.end(); it++) {
		it->print();
		std::cout << "next" << std::endl;
	}
}

std::vector<Location> Server::getLocations() const {
	return _config.getLocations();
}

#include <string>
#include <vector>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>


inline std::string toString(int number) {
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

void Server::startServer() {
	Logger::logMsg(INFO, SERVER_START);

	struct serverSocket;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	struct addrinfo *record;
	struct addrinfo *results;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	std::string	port = toString(_config.getPort());

	if (getaddrinfo(NULL, port.c_str(), &hints, &results) != 0) {
		std::cerr << "Can't get server's adress" << std::endl;
		_exit(-1);
	}

	for (record = results; record != NULL; record = record->ai_next) {
		_socket = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
		if (_socket == -1)
			continue;
		int enable = 1;
		setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		if (bind(_socket, record->ai_addr, record->ai_addrlen) == 0)
			break;
		close(_socket);
	}

	freeaddrinfo(results);

	Logger::logMsg(INFO, SERVER_START_SUCESS);
	return ;	
}

void Server::listening() {
	std::string port = toString(_config.getPort());
	std::string msg = SERVER_LISTENING + port; 
	Logger::logMsg(INFO, msg);

	if (listen(_socket, 1024) == -1) {
		std::cerr << "I'm deaf" << std::endl;
		_exit(1);
	}

	std::vector<int> clients;

	while (1) {

		int clientSocket;
		struct sockaddr clientAdress;
		socklen_t clientAdressLenght = sizeof(clientAdress);

		if ((clientSocket = accept(_socket, &clientAdress, &clientAdressLenght)) < 0) {
			std::cerr << "Aled" << std::endl;
			_exit(1);
		}

		int received;
			
		char buffer[_config.getMaxBodySize()] = {0};
		received = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (received == -1) {
			std::cerr << "I,m blind" << std::endl;
			_exit(1);
		} else {
			std::cout << "Message: " << buffer << std::endl;
		}

		close(clientSocket);

	}

	close(_socket);
}

void Server::stopServer() {
	Logger::logMsg(INFO, SERVER_STOP);
	return ;
}
