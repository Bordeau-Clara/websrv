#pragma once

#include <string>
#include <vector>
#include <cstddef> 
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "Logger.hpp"
#include "errors.hpp"
#include "defines.hpp"
#include "utilities.hpp"
#include "customException.hpp"

// Forward declarations for functions implemented in sources
int parseConfig(const std::string& configFilePath, bool allowFallback);
void startServer();
void runServer();
void exitServer();

