#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "location.hpp"

class Server {

	// ports range from 0 to 65535
	unsigned short	_port;
	unsigned int _maxBodySize;
	std::string	_name;
	std::string	_root;
	// Unsure about this one
	std::vector<Location> _locations;	
	std::vector<std::string> _contents;	
	std::map<int, std::string> _errorPages;

//	std::vector<Clients> clients;

	Server();

	public:

	Server(
			unsigned short& port, 
			unsigned int& maxBodySize,
			std::string& root, 
			std::vector<Location>& locations,
			std::map<int, std::string>& errorPages,
			std::string name=""); 
	Server(const Server& src);

	~Server();

	Server& operator=(const Server& server);

	void print();
	std::vector<Location> getLocations() const;	

	void startServer();
	void stopServer();

	void addConnection();
	void endConnection();
};
