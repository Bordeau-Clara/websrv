#include "Logger.hpp"

// Definitions for static members
LogPrio Logger::prio = INFO;
L_State Logger::state = ON;
std::ofstream Logger::file_;
std::string Logger::file_name = "webserv.log";
std::map<LogPrio, std::string> Logger::prio_str = Logger::initMap();

std::map<LogPrio, std::string> Logger::initMap() {
    std::map<LogPrio, std::string> m;
    m[DEBUG] = "DEBUG";
    m[INFO] = "INFO";
    m[WARNING] = "WARN";
    m[ERROR] = "ERROR";
    m[FATAL] = "FATAL";
    return m;
}

void Logger::setFileName(std::string name) {
    file_name = name;
    if (file_.is_open()) file_.close();
    file_.open(file_name.c_str(), std::ios::app);
}

void Logger::setState(L_State s) { state = s; }

void Logger::enableFileLog() {
    if (!file_.is_open()) file_.open(file_name.c_str(), std::ios::app);
}

void Logger::setPrio(LogPrio p) { prio = p; }
