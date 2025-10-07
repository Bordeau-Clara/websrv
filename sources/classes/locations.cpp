#include "webserv.hpp"

Location::Location() {
}

Location::Location(
		std::string& path,
		std::vector<std::string> methods) {
	_path = path;
	_methods = methods;
}

Location::~Location() {
}

Location::Location(const Location& src) {
	_path = src._path;
	_methods = src._methods;
}

Location& Location::operator=(const Location& src) {
	if (this == &src)
		return *this;
	_path = src._path;
	_methods = src._methods;
	return *this;
}

void Location::print() {

	std::cout << "PRINTING LOCATION CONFIG" << std::endl;
	std::cout << _path << std::endl;
	std::vector<std::string>::iterator it = _methods.begin();
	for ( ; it != _methods.end(); it++) {
		std::cout << *it << std::endl;
	}
}

std::string Location::getPath() const {
	return _path;
}

std::vector<std::string> Location::getMethod() const {
	return _methods;
}
