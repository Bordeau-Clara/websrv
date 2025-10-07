#include "Config.hpp"

Config::~Config(){}

Config::Config(unsigned short port,
				unsigned int maxBodySize,
				std::string root,
				std::vector<Location> locations,
				std::vector<std::string> content,
				std::map<int, std::string> errorPages,
				std::string name) {
	_port = port;
	_maxBodySize = maxBodySize;
	_root = root;
	_locations = locations;
	_contents = content;
	_errorPages = errorPages;
	_name = name;
}

Config& Config::operator=(const Config& src) {
	if (this == &src)
		return *this;
	_port = src._port;
	_maxBodySize = src._maxBodySize;
	_root =  src._root;
	_locations =  src._locations;
	_contents =  src._contents;
	_errorPages =  src._errorPages;
	return *this;
}

unsigned short Config::getPort() const {
	return _port;
}

unsigned int Config::getMaxBodySize() const {
	return _maxBodySize;
}

std::string Config::getName() const {
	return _name;
}

std::string Config::getRoot() const {
	return _root;
}

std::vector<Location> Config::getLocations() const {
	return _locations;
}

std::vector<std::string> Config::getContent() const {
	return _contents;
}

std::map<int, std::string> Config::getErrorPages() const {
	return _errorPages;
}
