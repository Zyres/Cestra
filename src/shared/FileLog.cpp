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

#include "FileLog.hpp"
#include <ctime>


void FileLog::InitializeLogFile(std::string log_name, LogType logType)
{
#ifdef _WIN32
    stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

    time_t now;
    tm* time_info = nullptr;
    time(&now);
#pragma warning(push)
#pragma warning(disable : 4996)
    time_info = localtime(&now);

    console_log_file = std::fopen(log_name.c_str(), "a");
    if (console_log_file == nullptr)
        fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, log_name.c_str(), strerror(errno));
    else
    {
        InitLogFileHeader("::::::::::::: Date[%-4d-%02d-%02d] Time [%02d:%02d:%02d]:::::::::::::", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    }
#pragma warning(pop)
}

void FileLog::InitLogFileHeader(const char* message_string, ...)
{
    if (console_log_file != nullptr)
    {
        char message_buffer[32768];
        va_list ap;
        va_start(ap, message_string);
        vsnprintf(message_buffer, 32768, message_string, ap);
        va_end(ap);

        WriteToLogFile(console_log_file, message_buffer);
    }
    else
    {
        LogError("LogFile not found!");
        return;
    }
}

void FileLog::WriteToLogFile(FILE* log_file, char* message, const char* source)
{
    char time_buffer[100];
    time_t now;
    tm* time_info = nullptr;
    time(&now);
#pragma warning(push)
#pragma warning(disable : 4996)
    time_info = localtime(&now);
#pragma warning(pop)

    if (time_info != nullptr)
        strftime(time_buffer, 100, "[%H:%M:%S]", time_info);
    else
        time_buffer[0] = '\0';

    if (source != NULL)
        fprintf(log_file, "%s %s : %s\n", time_buffer, source, message);
    else
        fprintf(log_file, "%s %s\n", time_buffer, message);
}

#ifndef _WIN32
void FileLog::SetConsoleColor(const char* color)
{
    fputs(colorstrings[color], stdout);
}

#else
void FileLog::SetConsoleColor(int color)
{
    SetConsoleTextAttribute(stdout_handle, (WORD)color);
}
#endif


// Log types
void FileLog::ConsoleLogDefault(const char* format, ...)
{
    char message_buffer[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(message_buffer, 32768, format, ap);
    va_end(ap);

    if (console_log_file != nullptr)
        WriteToLogFile(console_log_file, message_buffer);

    std::cout << message_buffer << std::endl;
}

void FileLog::ConsoleLogInfo(const char* format, ...)
{
    char message_buffer[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(message_buffer, 32768, format, ap);
    va_end(ap);

    if (console_log_file != nullptr)
        WriteToLogFile(console_log_file, message_buffer);

    std::cout << message_buffer << std::endl;
}

void FileLog::ConsoleLogError(const char* source, const char* format, ...)
{
    char message_buffer[32768];
    va_list ap;

    const char* pName = source;
    std::string name("[ERROR] ");
    name += pName;

    const char* nName = name.c_str();

    va_start(ap, format);
    vsnprintf(message_buffer, 32768, format, ap);
    va_end(ap);

    if (console_log_file != nullptr)
        WriteToLogFile(console_log_file, message_buffer, nName);

    std::cerr << nName << " : " << message_buffer << std::endl;
}
