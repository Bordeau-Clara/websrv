#pragma once

#include <map>
#include <string>
#include <vector>

#include "errors.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "Locations.hpp"

class	Server;
class	Location;

class ConfigParser
{
	static std::vector<std::string>	_server;
	static std::vector<std::string>	_location;
	Location						_globalLocationTemplate;
	Location						_locationTemplate;
    ConfigParser();
    ConfigParser operator=(const ConfigParser& src);

    public:

        ConfigParser(std::vector<std::string>& configFileContent);
        ~ConfigParser();
		Server parseServer(const std::vector<std::string>& block);
};
