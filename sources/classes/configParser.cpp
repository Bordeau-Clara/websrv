#include "webserv.hpp"

ConfigParser::ConfigParser(std::vector<std::string>& configTokens) {
    _state = OUTSIDE;
    _content = configTokens;
}

ConfigParser::~ConfigParser() {
}
/*
static inline bool checkPath(std::string path) {
	try {
		
	} catch {

		return false;
	}
	return true;
}
*/

// Extrait un bloc "server" du vecteur de tokens
std::vector<std::vector<std::string> > splitServerBlocks(const std::vector<std::string>& tokens) {
    std::vector<std::vector<std::string> > blocks;
    std::vector<std::string>::const_iterator it = tokens.begin();

    while (it != tokens.end()) {
        if (*it == "server") {
            std::vector<std::string> block;
            block.push_back(*it);
            ++it;
            if (it == tokens.end() || *it != "{") {
                throw CustomException("Expected '{' after 'server'", 1);
            }
            block.push_back(*it);
            ++it;
            int braceLevel = 1;
            while (it != tokens.end() && braceLevel > 0) {
                if (*it == "{") braceLevel++;
                else if (*it == "}") braceLevel--;
                block.push_back(*it);
                ++it;
            }
            if (braceLevel != 0) {
                throw CustomException("Unclosed server block", 1);
            }
            blocks.push_back(block);
        } else {
            ++it;
        }
    }
    return blocks;
}

Location parseLocation(const std::vector<std::string>& locationBlock) {
	std::string path, root;
	std::vector<std::string> methods;
    std::vector<std::string>::const_iterator it = locationBlock.begin();

    if (it == locationBlock.end()) {
        throw CustomException("Empty location block", 1);
    }
    path = *it;
    ++it;
    if (it == locationBlock.end() || *it != "{") {
        throw CustomException("Expected '{' after location path", 1);
    }
    ++it;
    while (it != locationBlock.end() && *it != "}") {
        std::string directive = *it;
        ++it;
        if (directive == "root") {
            if (it == locationBlock.end()) {
                throw CustomException("Missing path after 'root' in location", 1);
            }
            root = *it;
            ++it;
        } else if (directive == "allow_methods") {
            while (it != locationBlock.end() && *it != ";" && *it != "}") {
                methods.push_back(*it);
                ++it;
            }
            if (it != locationBlock.end() && *it == ";") {
                ++it;
            }
        }
		// Additional missing directives here -> autoindex, index
    }
	Location location(path, methods);
    return location;
}

std::vector<std::string> extractLocationBlock(
    std::vector<std::string>::const_iterator& it,
    const std::vector<std::string>::const_iterator& end) {
    std::vector<std::string> locationBlock;
    if (it == end) {
        throw CustomException("Expected 'location' keyword", 1);
    }
    if (it == end || it->empty() || (*it)[0] != '/') {
        throw CustomException("Expected path (starting with '/') after 'location'", 1);
    }
    locationBlock.push_back(*it);
    ++it;
    if (it == end || *it != "{") {
        throw CustomException("Expected '{' after location path", 1);
    }
    locationBlock.push_back(*it);
    ++it;
    int braceLevel = 1;
    while (it != end && braceLevel > 0) {
        const std::string& token = *it;
        if (token == "{") {
            braceLevel++;
        } else if (token == "}") {
            braceLevel--;
        }
        locationBlock.push_back(token);
        ++it;
    }
    if (braceLevel != 0) {
        throw CustomException("Unclosed location block", 1);
    }
    return locationBlock;
}

// Parse un bloc "server" (vecteur de tokens) et retourne un objet Server
Server ConfigParser::parseServer(const std::vector<std::string>& block) {
	unsigned short port;
	unsigned int client_max_body_size;
	std::string root, name;
	std::vector<Location> locations;
	std::map<int, std::string> errorPages;
    std::vector<std::string>::const_iterator it = block.begin();

    if (it == block.end() || *it != "server" || *(it + 1) != "{") {
        throw CustomException("Invalid server block", 1);
    }
    it += 2;  // Sauter "server" et "{"
    while (it != block.end() && *it != "}") {
        std::string directive = *it;
        ++it;
        if (directive == "listen") {
            if (it == block.end())
				throw CustomException("Missing port after 'listen'", 1);
            int tmp = atoi((*it).c_str());
			if (tmp < 0 || tmp > 65535)
				throw CustomException(INVALID_PORT, 1);
			port = tmp;
            ++it;
        } else if (directive == "root") {
            if (it == block.end())
				throw CustomException("Missing path after 'root'", 1);
			root = *it;
            ++it;
        } else if (directive == "server_name") {
            if (it == block.end())
				throw CustomException("Missing name after 'server_name'", 1);
            name = *it;
            ++it;
        } else if (directive == "location") {
			std::vector<std::string> locationBlock = extractLocationBlock(it, block.end());
			Location location = parseLocation(locationBlock);
			locations.push_back(location);
		} else if (directive == "error_page") {
			if (it == block.end())	
				throw CustomException("Missing error code && path after 'error_page'", 1);
			
		} else if (directive == "client_max_body_size") {
			if (it == block.end())	
				throw CustomException("Missing value after 'client_max_body_size'", 1);
			int tmp = atoi((*it).c_str());
			if (tmp < 0)
				throw CustomException("Invalid value after 'client_max_body_size'. Can't be < 0", 1);
			client_max_body_size = tmp;
		}
        // Ajouter d'autres directives ici
    }
	Server server(port, client_max_body_size, root, locations, errorPages, name);
    return server;
}

// Parse tous les blocs "server" et retourne un vecteur de serveurs
std::vector<Server> ConfigParser::parse() {
    std::vector<Server> servers;
    std::vector<std::vector<std::string> > serverBlocks = splitServerBlocks(_content);
	std::vector<std::vector<std::string> >::const_iterator it = serverBlocks.begin();

// try/catch logic to fallback on default settings if the .conf file is invalid

    for ( ;it != serverBlocks.end(); ++it) {
		Server tmp = parseServer(*it);
        //servers.push_back(parseServer(*it));
        servers.push_back(tmp);
    }
    return servers;
}
