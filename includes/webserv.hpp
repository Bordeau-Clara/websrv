#pragma once

#include <queue>
#include <string>
#include <vector>
#include <cstddef> 
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include "Logger.hpp"
#include "errors.hpp"
#include "server.hpp"
#include "defines.hpp"
#include "utilities.hpp"
#include "configParser.hpp"
#include "customException.hpp"

// Forward declarations for functions implemented in sources
//std::vector<Server>	parseConfig(const std::string& configFilePath, bool allowFallback);
std::vector<std::string> parseConfig(const std::string& configFilePath, bool allowFallback);
Server parseServer(std::string serverConfig);

void startServers(std::vector<Server>& servers);
void runServer();
void exitServer();
