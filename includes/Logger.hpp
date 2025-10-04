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
    static std::ofstream _file;
	static std::string _fileName;
    static std::map<LogPrio, std::pair<std::string, std::ostream*> > _levels;
    static std::map<LogPrio, std::string> _colors;
    
    static bool _initialized;

	Logger(const Logger& other);
	Logger& operator=(const Logger& other);

	public:

        Logger(const std::string& filename = "");

        ~Logger();

        static void         setFileName(std::string);
        static void         setState(L_State);
        static void         initLevels(void);

        static std::string  getCurrentTime();

		static void			initFile(const std::string& filename);
        static void         enableFileLog();

        static void         setPrio(LogPrio);
        static void         logMsg(LogPrio level, const std::string& message);
		static std::string	getColorCode(LogPrio level);
};
