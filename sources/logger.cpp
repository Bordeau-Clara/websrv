#include "Logger.hpp"

Logger::Logger(const std::string& filename)
    : file_(filename.empty() ? nullptr : new std::ofstream(filename.c_str()))
{
    if (file_ && !file_->is_open()) {
        delete file_;
        file_ = nullptr;
    }
}

Logger::~Logger()
{
    if (file_) {
        file_->close();
        delete file_;
    }
}

Logger::Logger(const Logger& other)
    : file_(other.file_ ? new std::ofstream(other.file_->is_open() ? "copy_log.txt" : "") : nullptr)
{
    // Note: On ne copie pas le flux, mais on en crée un nouveau pour éviter les conflits.
    // En pratique, la copie d'un logger n'est pas toujours utile.
    // Ici, on ouvre un nouveau fichier "copy_log.txt" si l'original est ouvert.
}

Logger& Logger::operator=(const Logger& other)
{
    if (this != &other) {
        if (file_) {
            file_->close();
            delete file_;
        }
        file_ = other.file_ ? new std::ofstream(other.file_->is_open() ? "copy_log.txt" : "") : nullptr;
    }
    return *this;
}

std::string Logger::getCurrentTime() const
{
    time_t now = time(nullptr);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

void Logger::logMsg(LogLevel level, const std::string& message)
{
    const char* levelStr = "";
    
    switch (level) {
        case LOG_DEBUG:    levelStr = "DEBUG"; break;
        case LOG_INFO:     levelStr = "INFO";  break;
        case LOG_WARNING:  levelStr = "WARN";  break;
        case LOG_ERROR:    levelStr = "ERROR"; break;
        case LOG_FATAL:    levelStr = "FATAL"; break;
        default:           levelStr = "UNKNOWN";
    }

    std::string logEntry = getCurrentTime() + " [" + levelStr + "] " + message + "\n";

    // Sortie sur la console
    std::cout << logEntry;

    // Sortie dans le fichier si ouvert
    if (file_ && file_->is_open()) {
        *file_ << logEntry;
        file_->flush();
    }
}
