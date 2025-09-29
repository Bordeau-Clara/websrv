#include "Logger.hpp"

// Logger implementation matches includes/Logger.hpp where file_ is static std::ofstream
// The static members are defined elsewhere in the implementation unit that provides them.

Logger::Logger(const std::string& filename) {
    if (!filename.empty()) {
        file_name = filename;
        // open the static file_ stream if not already open
        if (!file_.is_open()) {
            file_.open(file_name.c_str(), std::ios::app);
        }
    }
}

Logger::~Logger() {
    // Do not close static file_ here; leave it open for program lifetime.
}

Logger::Logger(const Logger& other) {
    // No per-instance state to copy; file_ is static
    (void)other;
}

Logger& Logger::operator=(const Logger& other) {
    (void)other;
    return *this;
}

std::string Logger::getCurrentTime() {
    time_t now = time(NULL);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return std::string(buf);
}

void Logger::logMsg(LogPrio level, const std::string& message) {
    const char* levelStr = "";
    switch (level) {
        case DEBUG:    levelStr = "DEBUG"; break;
        case INFO:     levelStr = "INFO";  break;
        case WARNING:  levelStr = "WARN";  break;
        case ERROR:    levelStr = "ERROR"; break;
        case FATAL:    levelStr = "FATAL"; break;
        default:       levelStr = "UNKNOWN";
    }

    std::string logEntry = getCurrentTime() + " [" + levelStr + "] " + message;
    std::cout << logEntry << std::endl;
    if (file_.is_open()) {
        file_ << logEntry << std::endl;
        file_.flush();
    }
}
