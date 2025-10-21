#pragma once

// Return codes
static const int	SUCCESS = 0;
static const int	ERROR = 1;

// Recv buffersize
static const unsigned int	BUFFER_SIZE = 4096;

// Color flags
static const char RESET[] = "\x1B[0m";
static const char RED[] = "\x1B[31m";
static const char LIGHT_RED[] = "\x1B[91m";
static const char WHITE[] = "\x1B[37m";
static const char BLINK[] = "\x1b[5m";
static const char YELLOW[] = "\x1B[33m";
static const char LIGHT_BLUE[] = "\x1B[94m";
static const char CYAN[] = "\x1B[36m";
static const char DARK_GREY[] = "\x1B[90m";
static const char LIGHTMAGENTA[] = "\x1B[95m";
static const int GST = 4;

// Configuration file
static const char CONF_EXTENSION[] = "conf";
static const char DEFAULT_CONFIG[] = "configs/default.conf";
static const char LOADING_DEFAULT[] = "Loading default config file";
static const char DEFAULT_CONFIG_MESSAGE[] = "Using default config: ";

// Parsing keywords
static const char SERVER_KEYWORD[] = "server";
static const char PORT_KEYWORD[] = "port";
static const char METHODS_KEYWORD[] = "methods";

// Grammar tokens
static const char GRAMMAR_OPEN = '{';
static const char GRAMMAR_CLOSE = '}';
static const char GRAMMAR_STOP = ';';

// Misc infos messages
static const char SERVER_START[] = "Starting server";
static const char SERVER_START_SUCESS[] = "Server started sucessfully";
static const char SERVER_STOP[] = "Stoping server";
static const char SERVER_LISTENING[] = "Server listening on port ";
