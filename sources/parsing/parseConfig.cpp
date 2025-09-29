#include "webserv.hpp"
#include "customException.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <ios>

// Try to open 'path' into 'out'. If the stream throws, translate to CustomException.
// C++98: enable stream exceptions temporarily so open() throws std::ios::failure.
static void tryOpenOrThrow(const std::string& path, std::ifstream& out)
{

    out.exceptions(std::ios::failbit | std::ios::badbit);
    try {
        // check extension
        if (path.substr(path.find_last_of(".") + 1) != "conf") {
            throw CustomException(INVALID_CONFIG_EXTENSION, INVALID_CONFIG_EXTENSION_CODE);
        }
        out.open(path.c_str());
    } catch (const std::ios::failure& e) {
        out.clear();
        out.exceptions(std::ios::goodbit);
        throw CustomException(std::string(e.what()), OPEN_ERROR_CODE);
    }
 
    out.exceptions(std::ios::goodbit);
}

// Build a nested CustomException whose message is 'inner' and whose nested_message
// contains the outer failure context (possibly already containing a nested_message).
static inline CustomException make_nested(const CustomException& outer, const CustomException& inner)
{
    std::string nested = outer.what();

    if (!outer.get_nested_message().empty()) {
        nested += " ";
        nested += outer.get_nested_message();
    }

    return CustomException(std::string(inner.what()), inner.get_code(), std::string(), nested);
}

// Try opening primary; if it fails and allowFallback is true, try fallback.
// If both fail, throw a CustomException that contains both messages (nested).
static void openWithOptionalFallback(const std::string& primary,
                                     const std::string& fallback,
                                     std::ifstream& out,
                                     bool allowFallback)
{
    try {
        tryOpenOrThrow(primary, out);
        return;
    } catch (const CustomException& primaryEx) {
        if (!allowFallback) {
            throw;
        }
        
		try {
            tryOpenOrThrow(fallback, out);
            std::cerr << "Using default config: " << fallback << std::endl;
            return;
        } catch (const CustomException& fallbackEx) {
            throw make_nested(primaryEx, fallbackEx);
        }
    }
}

// Parse the already-open stream. Convert stream errors to CustomException.
static void parseStream(std::ifstream& configFile)
{
    std::string line;

    try {
        while (std::getline(configFile, line)) {
			std::cout << line << std::endl;
			// parseLine(line);	
			// (void)line;
        }
        if (configFile.bad()) {
            throw CustomException(std::string("I/O error while reading config"), READ_ERROR_CODE);
        }
    } catch (const std::ios::failure& e) {
        throw CustomException(std::string(e.what()), READ_ERROR_CODE);
    }
}

/*
 * parseConfig :
 * - ouvre le fichier de configuration (essaie le fallback si autorisé),
 * - lance CustomException en cas d'erreur (avec messages imbriqués),
 * - retourne SUCCESS quand tout est OK.
 *
 * Signature: int parseConfig(const std::string& configFilePath, bool allowFallback)
 * Note: erreurs graves sont propagées via exceptions pour être traitées dans main.
 */
int parseConfig(const std::string& configFilePath, bool allowFallback)
{
    std::ifstream configFile;

    // Try to open primary or fallback (may throw CustomException)
	try {	
    	openWithOptionalFallback(configFilePath, DEFAULT_CONFIG, configFile, allowFallback);
	} catch (const CustomException& e) {
		//Logger::logMsg("Failed to open config file: %s", CONSOLE_OUTPUT, e.what());
		throw;
	}

    // Parse the open stream (may throw CustomException)
    parseStream(configFile);

    return SUCCESS;
}