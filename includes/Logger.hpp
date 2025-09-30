#pragma once

#include <map>
#include <ctime>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
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

    static LogPrio _prio;
    static L_State _state;
    static std::ofstream _file_;
	static std::string _file_name;
    static std::map<LogPrio, std::pair<std::string, std::ostream*> > _levels;
    
    static bool _initialized;
	
	public:

        Logger(const std::string& filename = "");
        Logger(const Logger& other);

        ~Logger();

        Logger& operator=(const Logger& other);

        static void         setFileName(std::string);
        static void         setState(L_State);
        static void         initLevels(void);

        static std::string  getCurrentTime();
        static void         enableFileLog();
        static void         setPrio(LogPrio);
        static void         logMsg(LogPrio level, const std::string& message);
};

