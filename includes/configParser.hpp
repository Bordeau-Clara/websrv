#pragma once

#include <map>
#include <string>
#include <vector>

#include "errors.hpp"
#include "config.hpp"
#include "server.hpp"
#include "location.hpp"

class ConfigParser {

	std::vector<Config> _configs;
	std::vector<std::string> _content;
   // void tokenize(const std::string &content);

    ConfigParser();
    ConfigParser operator=(const ConfigParser& src);

    public:

        ConfigParser(std::vector<std::string>& configFileContent);
        ~ConfigParser();

        std::vector<Server> parse();

		Server parseServer(const std::vector<std::string>& block);
};
