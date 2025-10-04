#pragma once

#include <string>
#include <vector>
#include <map>
#include "server.hpp"
#include "location.hpp"
#include "errors.hpp"


const char OUTSIDE = 0b0;
const char IN_SERVER = 0b1; 
const char IN_LOCATION = 0b10;
const char IN_DIRECTIVE = 0b100;

class ConfigParser {
/*
    // Méthodes principales de la FSM
    void processToken(const std::string &token);
    void handleOutsideState(const std::string &token);
    void handleServerState(const std::string &token);
    void handleLocationState(const std::string &token);
    void handleDirectiveState(const std::string &token);

    // Parsers de directives (exemples)
    void parseListen(Server &server, const std::string &value);
    void parseRoot(Server &server, const std::string &value);
    void parseLocation(Server &server, const std::string &value);
*/
    // Attributs
    char _state;
    std::string _content;
/*
    std::vector<Server> servers;
    Server *currentServer;
    Location *currentLocation;
    std::string currentDirective;
    std::map<std::string, Directive> directives;
    std::vector<std::string> tokens;
    size_t tokenIndex;
*/
    // Méthodes utilitaires
    void tokenize(const std::string &content);
 //   void registerDirectives();

    ConfigParser();
    ConfigParser operator=(const ConfigParser& src);

    public:

        ConfigParser(std::string& configFileContent);
        ~ConfigParser();

        std::vector<Server> parse();
};
