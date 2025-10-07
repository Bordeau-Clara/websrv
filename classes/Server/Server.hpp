#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "Config.hpp"
#include "Locations.hpp"

class Server {

	Config _config;
	int _socket;

	public:

	Server(const Config& cfg);
	Server(const Server& src);

	~Server();

	Server& operator=(const Server& server);

	void print();
	std::vector<Location> getLocations() const;    

	// Access config
	const Config& getConfig() const;

	void startServer();
	void stopServer();
	void listening();

	void addConnection();
	void endConnection();
};
