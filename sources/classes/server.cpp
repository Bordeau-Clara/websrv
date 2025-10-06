#include "server.hpp"
#include "Logger.hpp"

Server::Server() {}

Server::Server(const Server& other) {
	_port = other._port;
	_name = other._name;
	_root = other._root;
	_locations = other._locations;
}

Server::~Server() {

}

Server::Server(
		unsigned short& port, 
		std::string& name, 
		std::string& root,
		std::vector<Location>& locations) {
	_port = port;
	_name = name;
	_root = root;
	_locations = locations;
}

Server& Server::operator=(const Server& server) {
	if (this == &server)
		return *this;
	_port = server._port;
	_name = server._name;
	_root = server._root;
	_locations = server._locations;
	return *this;
}

void	addConnection() {

}

void	endConnection() {

}

void Server::print() {

	std::cout << "PRINTING SERVER CONFIG" << std::endl;
	std::cout << _port << std::endl;
	std::cout << _name << std::endl;
	std::cout << _root << std::endl;
	std::vector<Location>::iterator it = _locations.begin();	
	for ( ; it != _locations.end(); it++) {
		it->print();
		std::cout << "next" << std::endl;
	}
}

std::vector<Location> Server::getLocations() const {
	return _locations;
}

void Server::startServer() {
	Logger::logMsg(INFO, SERVER_START);

	Logger::logMsg(INFO, SERVER_START_SUCESS);
	return ;	
}

void Server::stopServer() {
	Logger::logMsg(INFO, SERVER_STOP);
	return ;
}
