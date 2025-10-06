#pragma once

// Using strongly typed definition to avoid conflicts with other projects/libs && silent errors

static const char INVALID_FILE_MESSAGE[] = ": invalid file.";
static const int INVALID_FILE_CODE = 2;

static const char USAGE[] = "Usage: ./webserv <configuration file>";
static const int USAGE_CODE = 64;

static const char OPEN_ERROR[] = ": unable to open file.";
static const int OPEN_ERROR_CODE = 3;

static const char INVALID_CONFIG_EXTENSION[] = ": invalid config file extension (must be .conf).";
static const int INVALID_CONFIG_EXTENSION_CODE = 4;
 
static const char READ_ERROR[] = ": error while reading file.";
static const int READ_ERROR_CODE = 5;

static const char IO_CONFIG_ERROR[] = "I/O error while reading config";

static const char INVALID_CONFIG_FILE[] = "Invalid config file";
static const char INVALID_CONFIG_FORMAT[] = "Invalid config format";
static const int PARSING_ERROR = 6;

static const char INVALID_PORT[] = "Invalid port value. Must be between 0 && 65535 inclusive";

// Prototype for error reporting function implemented in sources/utilities/errors.cpp
int errorMessage(const char *message, int ecode);
