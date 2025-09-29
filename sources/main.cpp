#include "webserv.hpp"

int main(int argc, char **argv) {

	if (argc > 2) {
		return errorMessage(USAGE, 1);
	}

	std::string configFile = argc == 2 ? argv[1] : DEFAULT_CONFIG;
	bool allowFallback = (argc == 2);

	try {
		// parseConfig throws CustomException on fatal errors
		parseConfig(configFile, allowFallback);
	} catch (const CustomException& e) {
		// Build a single-line message using e.what() and nested message if any
		std::string msg = std::string(e.what());
		if (!e.get_nested_message().empty()) {
			msg += " ";
			msg += e.get_nested_message();
		}
		return errorMessage(msg.c_str(), e.get_code());
	} catch (const std::exception& e) {
		return errorMessage(e.what(), OPEN_ERROR_CODE);
	}
	
	startServer();
/*
	runServer();
	exitServer();
*/
	return 0;
}
