#include "CustomException.hpp"

// Constructeur de base
CustomException::CustomException(const std::string& msg, int c, const std::string& cause_msg)
    : code(c), cause(cause_msg), message(msg) {}

// Constructeur avec message imbriqué
CustomException::CustomException(const std::string& msg, int c, const std::string& cause_msg, const std::string& nested_msg)
    : code(c), cause(cause_msg), message(msg), nested_message(nested_msg) {}

// Destructeur
CustomException::~CustomException() throw() {}

// Redéfinition de what() pour inclure les messages imbriqués
const char* CustomException::what() const throw() {
    static std::string full_msg;
    full_msg = message;
    if (!cause.empty()) {
        full_msg += " (cause: " + cause + ")";
    }
    if (!nested_message.empty() && nested_message != "std::exception") {
        full_msg += " [nested: " + nested_message + "]";
    }
    return full_msg.c_str();
}

// Getters
int CustomException::get_code() const {
    return code;
}

const std::string& CustomException::get_cause() const {
    return cause;
}

const std::string& CustomException::get_nested_message() const {
    return nested_message;
}

CustomException CustomException::make_nested(
		const CustomException& outer, 
		const CustomException& inner)
{
    std::string nested = outer.what();

    if (!outer.get_nested_message().empty()) {
        nested += " ";
        nested += outer.get_nested_message();
    }

    return CustomException(std::string(inner.what()), inner.get_code(), std::string(), nested);
}
