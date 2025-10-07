#pragma once

#include <queue>
#include <string>
#include <vector>
#include <climits>
#include <cstddef> 
#include <cstdlib>
#include <fstream>
#include <signal.h>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include "Logger.hpp"
#include "errors.hpp"
#include "Server.hpp"
#include "defines.hpp"
#include "utilities.hpp"
#include "ConfigParser.hpp"
#include "CustomException.hpp"

// Forward declarations for functions implemented in sources
//std::vector<Server>	parseConfig(const std::string& configFilePath, bool allowFallback);

// Parsing funcitons outside objects
std::vector<std::string> parseConfig(const std::string& configFilePath, bool allowFallback);
std::vector<std::string> parseStream(std::ifstream& configFile);

//

Server parseServer(std::string serverConfig);

void startServers(std::vector<Server>& servers);
void runServer();
void exitServer();
