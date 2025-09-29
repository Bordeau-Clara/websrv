#pragma once

#include <map>
#include <ctime>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

#include "defines.hpp"

enum LogPrio{
    DEBUG,
    INFO,
	WARNING,
    ERROR,
    FATAL
};

enum L_State{
    ON,
    OFF
};


enum Mode{
    CONSOLE_OUTPUT,
    FILE_OUTPUT
};

class Logger{

    static LogPrio prio;
    static L_State state;
    static std::ofstream file_;
	static std::string file_name;
    static std::map<LogPrio, std::string> initMap();
    static std::map<LogPrio, std::string> prio_str;
	
	public:

        Logger(const std::string& filename = "");
        Logger(const Logger& other);

        ~Logger();

        Logger& operator=(const Logger& other);

        static void         setFileName(std::string);
        static void         setState(L_State);

        static std::string  getCurrentTime();
        static void         enableFileLog();
        static void         setPrio(LogPrio);
        static void         logMsg(LogPrio level, const std::string& message);
};

