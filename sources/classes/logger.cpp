#include "Logger.hpp"

// Logger implementation matches includes/Logger.hpp where file_ is static std::ofstream
// The static members are defined elsewhere in the implementation unit that provides them.

LogPrio Logger::_prio = DEBUG;
L_State Logger::_state = ON;
std::ofstream Logger::_file;
std::string Logger::_fileName;
std::map<LogPrio, std::pair<std::string, std::ostream*> > Logger::_levels;
std::map<LogPrio, std::string> Logger::_colors;
bool Logger::_initialized = false;

Logger::Logger(const std::string& filename) {

    if (_initialized) {
		return ;
    }
    Logger::initLevels();
	Logger::initFile(filename);
    _initialized = true;
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

void Logger::initFile(const std::string& filename) {
    if (!_file.is_open() && !filename.empty()) {
        _fileName = filename;
        _file.open(_fileName.c_str(), std::ios::app);
        if (!_file.good()) {
			std::string error("Failed to open log file: " + _fileName);
			logMsg(ERROR, error);
            _file.clear();
        }
    }
}

void Logger::initLevels() {
    _levels[DEBUG]   = std::make_pair("DEBUG", &std::cout);
    _levels[INFO]    = std::make_pair("INFO",  &std::cout);
    _levels[WARNING] = std::make_pair("WARN",  &std::cout);
    _levels[ERROR]   = std::make_pair("ERROR", &std::cerr);
    _levels[FATAL]   = std::make_pair("FATAL", &std::cerr);

    _colors[DEBUG]   = WHITE;
    _colors[INFO]    = LIGHT_BLUE;
	_colors[WARNING] = YELLOW;
	_colors[ERROR]   = LIGHT_RED;
	_colors[FATAL]   = RED;
}

std::string Logger::getCurrentTime() {
    time_t now = time(NULL);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return std::string(buf);
}

std::string Logger::getColorCode(LogPrio level) {
	return _colors[level];
}

void Logger::logMsg(LogPrio level, const std::string& message) {

/*    if (level < DEBUG || level > FATAL || _state == OFF || level < _prio) {
        return;
    }
	*/

    std::string levelStr = _levels[level].first;
	std::string colorCode = getColorCode(level);
    std::ostream* levelStream = _levels[level].second;

    std::string logEntry = colorCode + getCurrentTime() + " [" + levelStr + "] " + message + RESET + "\n";

    if (levelStream) {
        (*levelStream) << logEntry;
    }
    
    if (_file.is_open() && _file.good()) {
        _file << logEntry;
        _file.flush();
    }
}
