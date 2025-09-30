#pragma once

#include <string>
#include <exception>

// Custom exception class allowing chained exceptions with error codes
class CustomException : public std::exception {
private:
    int code;
    std::string cause;
    std::string message;
    std::string nested_message;
    CustomException& operator=(const CustomException&);

public:

    CustomException(const std::string& msg, int c, const std::string& cause_msg = "");
    CustomException(const std::string& msg, int c, const std::string& cause_msg, const std::string& nested_msg);

    virtual ~CustomException() throw();

    int get_code() const;
    const char* what() const throw();
    const std::string& get_cause() const;
    const std::string& get_nested_message() const;
};
