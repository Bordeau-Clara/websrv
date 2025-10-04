#include "webserv.hpp"

int main(int argc, char **argv) {

	Logger	logger;

	if (argc > 2) {
		logger.logMsg(ERROR, USAGE);
		return USAGE_CODE;
	}

	// If no parameter was passed, we apply the default config && disable the fallback strategy
	std::string configFile = argc == 2 ? argv[1] : DEFAULT_CONFIG;
	
	bool allowFallback = (argc == 2);

//	std::vector<Server> servers;

	try {
		// parseConfig throws CustomException on fatal errors
		parseConfig(configFile, allowFallback);

		startServer();
/*
		runServer();
	
		exitServer();
*/

	} catch (const CustomException& e) {
		// Build a single-line message using e.what() and nested message if any
		std::string msg = std::string(e.what());
		if (!e.get_nested_message().empty()) {
			msg += " ";
			msg += e.get_nested_message();
		}
		return errorMessage(msg.c_str(), e.get_code());
	} catch (const std::exception& e) {
		// Temp catch
		return errorMessage(e.what(), OPEN_ERROR_CODE);
	}
	
	return SUCCESS;
}
