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

// LOCATION -> path -> GRAMMAR_OPEN -> METHODS -> * -> GRAMMAR_CLOSE
void parseLocation() {

}


void parseDirectives() {

}

// SERVER -> GRAMMAR_OPEN -> * {DIRECTIVE | LOCATION} -> GRAMMAR_CLOSE
void parseServer() {

}

std::vector<Server> ConfigParser::parse() {
	size_t serverCount = 0;
    std::vector<Server> servers;
	std::queue<std::string> tokens;

	std::cout << _content << std::endl;
	std::stringstream ss(_content);
	std::string tmp;

	while (ss >> tmp) {
		if (tmp == "server")
			serverCount++;
		tokens.push(tmp);
		std::cout << tmp << std::endl;
	}
	
	std::cout << "Parsed " << serverCount << " servers" << std::endl;


    return servers;
}

/*	INTUITION
 *
 *	Using an FSM with 4 differente states:
 *
 *		->	OUTSIDE			-	Next token must be a server token 
 *		->	IN_SERVER		-	We're parsing a server, can trasition to state IN_LOCATION
 *		->	IN_LOCATION		-	Must start with a LOCATION token
 *		->	IN_DIRECTIVE	-	.
 *
 *	We must make sure brackets are opened && closed
 *	Certain informations have to be present to get a valid server config (ip, port, adress)
 *	Some tokens need to fit boundaries (port >= 0 && <= 6535)
 *	*/
