#pragma once

#include <map>
#include <string>
#include <vector>

#include "location.hpp"

class Config {

	// ports range from 0 to 65535
	unsigned short	_port;
	unsigned int _maxBodySize;
	std::string	_name;
	std::string	_root;
	std::vector<Location> _locations;	
	std::vector<std::string> _contents;	
	std::map<int, std::string> _errorPages;

	public :

		Config(unsigned short port, 
				unsigned int maxBodySize,
				std::string root,
				std::vector<Location> locations,
				std::vector<std::string> content,
				std::map<int, std::string> errorPages,
				std::string name="");

		~Config();

		Config& operator=(const Config& config);

		unsigned short getPort() const;
		unsigned int getMaxBodySize() const;
		std::string getName() const;
		std::string getRoot() const;
		std::vector<Location> getLocations() const;
		std::vector<std::string> getContent() const;
		std::map<int, std::string> getErrorPages() const;
};
