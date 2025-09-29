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
	WARN,
    ERROR
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

    static std::map<LogPrio, std::string> initMap();

	static std::string file_name;
    static LogPrio prio;
    static std::map<LogPrio, std::string> prio_str;
    static L_State state;
	
	public:

        static void         setFileName(std::string);
        static void         setState(L_State);

        static void         logMsg(const char *, Mode, const char*, ...);
        static void         setPrio(LogPrio);
        static void         enableFileLog();
        static std::string  getCurrTime();
};

