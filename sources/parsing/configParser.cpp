#include "webserv.hpp"

ConfigParser::ConfigParser(std::string& configFileContent) {
    _state = OUTSIDE;
    _content = configFileContent;
}

ConfigParser::~ConfigParser() {

}

/*
ConfigParser::tokenize() {

}
*/

std::vector<Server> ConfigParser::parse() {
    std::vector<Server> servers;

    return servers;
}
