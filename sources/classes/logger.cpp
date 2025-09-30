#include "Logger.hpp"

// Logger implementation matches includes/Logger.hpp where file_ is static std::ofstream
// The static members are defined elsewhere in the implementation unit that provides them.

LogPrio Logger::_prio = DEBUG;
L_State Logger::_state = ON;
std::ofstream Logger::_file_;
std::string Logger::_file_name;
std::map<LogPrio, std::pair<std::string, std::ostream*> > Logger::_levels;
bool Logger::_initialized = false;

Logger::Logger(const std::string& filename) {

    if (!_initialized) {
        Logger::initLevels();
    }

    if (!filename.empty()) {
        _file_name = filename;
        if (!_file_.is_open()) {
            _file_.open(_file_name.c_str(), std::ios::app);
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

void Logger::initLevels() {
    _levels.clear();
    _levels.insert(std::make_pair(DEBUG,   std::make_pair(std::string("DEBUG"), &std::cout)));
    _levels.insert(std::make_pair(INFO,    std::make_pair(std::string("INFO"),  &std::cout)));
    _levels.insert(std::make_pair(WARNING, std::make_pair(std::string("WARN"),  &std::cout)));
    _levels.insert(std::make_pair(ERROR,   std::make_pair(std::string("ERROR"), &std::cerr)));
    _levels.insert(std::make_pair(FATAL,   std::make_pair(std::string("FATAL"), &std::cerr)));
    _initialized = true;
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

    if (level < DEBUG || level > FATAL || _state == OFF || level < _prio) {
        return;
    }

    std::string levelStr = _levels[level].first;
    std::ostream* levelStream = _levels[level].second;

    std::string logEntry = getCurrentTime() + " [" + levelStr + "] " + message + "\n";
    if (levelStream) {
        (*levelStream) << logEntry;
    }
    
    if (_file_.is_open()) {
        _file_ << logEntry;
        _file_.flush();
    }
}