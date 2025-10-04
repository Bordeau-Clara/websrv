#pragma once

#include <vector>
#include <string>

class Server {

	// ports range from 0 to 65535
	unsigned short	_port;
	// We can use bit masks for aloowed methods
	unsigned short	_methods;
	std::string	_host;
	std::string	_root_path;
	// Unsure about this one
	std::vector<std::string> _contents;	

//	std::vector<Clients> clients;

	public:

	Server();
	~Server();
	Server(const Server& src);

	Server& operator=(const Server& server);

	void addConnection();
	void endConnection();
};
