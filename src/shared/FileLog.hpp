/*
 * Copyright(C) 2016  Cestra
 *
 * This program is free software : you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FILE_LOG_HPP
#define FILE_LOG_HPP

#include "Common.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>

#ifdef _WIN32
    #define CONSOLE_COLOR_RED FOREGROUND_RED | FOREGROUND_INTENSITY
    #define CONSOLE_COLOR_GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
    #define CONSOLE_COLOR_YELLOW FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
    #define CONSOLE_COLOR_NORMAL FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
#else
    #define CONSOLE_COLOR_RED    "\033[22;31m"
    #define CONSOLE_COLOR_GREEN  "\033[22;32m"
    #define CONSOLE_COLOR_YELLOW "\033[01;33m"
    #define CONSOLE_COLOR_NORMAL "\033[0m"
#endif

enum LogType
{
    LOG_GAME,
    LOG_REALM
};

class FileLog
{
    public:
        
        void InitializeLogFile(std::string log_name, LogType logType);
        void InitLogFileHeader(const char* message_string, ...);

#ifndef _WIN32
        void SetConsoleColor(const char* color);
#else
        void SetConsoleColor(int color);
#endif
        void ConsoleLogDefault(const char* format, ...);
        void ConsoleLogInfo(const char* format, ...);
        void ConsoleLogError(const char* source, const char* format, ...);

        std::string realm_name;
        std::string game_name;

    private:

        std::FILE* console_log_file;
        void WriteToLogFile(std::FILE* log_file, char* message, const char* source = NULL);

#ifdef _WIN32
        HANDLE stdout_handle;
#endif
};

static FileLog Log;
#define LogDefault(msg, ...) Log.ConsoleLogDefault(msg, ##__VA_ARGS__)
#define LogInfo(msg, ...) Log.SetConsoleColor(CONSOLE_COLOR_GREEN); Log.ConsoleLogInfo(msg, ##__VA_ARGS__); Log.SetConsoleColor(CONSOLE_COLOR_NORMAL)
#define LogError(msg, ...) Log.SetConsoleColor(CONSOLE_COLOR_RED); Log.ConsoleLogError(__FUNCTION__, msg, ##__VA_ARGS__); Log.SetConsoleColor(CONSOLE_COLOR_NORMAL)

#endif // FILE_LOG_HPP
