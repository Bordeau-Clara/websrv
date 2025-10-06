#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "location.hpp"

class Server {

	// ports range from 0 to 65535
	unsigned short	_port;
	std::string	_name;
	std::string	_root;
	// Unsure about this one
	std::vector<Location> _locations;	
	std::vector<std::string> _contents;	

//	std::vector<Clients> clients;

	Server();

	public:

	Server(
			unsigned short& port, 
			std::string& name, 
			std::string& root, 
			std::vector<Location>& locations);
	~Server();
	Server(const Server& src);

	Server& operator=(const Server& server);

	void print();
	std::vector<Location> getLocations() const;	

	void startServer();
	void stopServer();

	void addConnection();
	void endConnection();
};
