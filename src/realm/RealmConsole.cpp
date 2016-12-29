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

#include "stdafx.hpp"

#include <algorithm>
#include <string>

RealmConsole::RealmConsole() : stop_realm_console(false) {}

void RealmConsole::StartConsoleListener()
{
    std::cout << "Type help to see all available commands. You can shut down this programm with command: end " << std::endl;

    ListenConsoleListener();
}

bool RealmConsole::IsRealmConsoleStopped()
{
    if (stop_realm_console)
    {
        std::cout << "RealmConsole is not stopped." << std::endl;
        return false;
    }
    else
    {
        std::cout << "RealmConsole is already stopped." << std::endl;
        return true;
    }
}

void RealmConsole::ListenConsoleListener()
{
    std::string option = "";
    while (!stop_realm_console)
    {
        std::getline(std::cin, option);
        HandleConsoleCommands(option);
    }
}

void RealmConsole::HandleConsoleCommands(std::string command)
{
    typedef void (RealmConsole::*HandleConsoleCommand_Function)();

    struct ConsoleCommandList
    {
        std::string command;
        HandleConsoleCommand_Function command_handler;
    };

    ConsoleCommandList ConsoleCommands[] =
    {
        { "help", &RealmConsole::ShowHelpRealmConsole },
        { "end", &RealmConsole::StopRealmConsole }
    };

    std::string processed_command = command;

    //prepare input for comparison
    std::transform(processed_command.begin(), processed_command.end(), processed_command.begin(), ::tolower);

    //compare input with available commands
    for (uint8 i = 0; i < sizeof(ConsoleCommands) / sizeof(ConsoleCommandList); ++i)
    {
        if (processed_command.compare(ConsoleCommands[i].command) == 0)
        {
            (this->*(ConsoleCommands[i].command_handler))();
            return;
        }
    }

    std::cout << "'" << command << "' is not a valid command! Type help to get a list with all available commands." << std::endl;
}

//command handlers
void RealmConsole::ShowHelpRealmConsole()
{
    std::cout << ":::::::::::::::::::::::::::::::::: H E L P :::::::::::::::::::::::::::::::::: " << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "    'help'       Will show available commands." << std::endl;
    std::cout << "    'end'        Will shutdown Cestra." << std::endl;
    std::cout << "::::::::::::::::::::::::::::::::::: E N D ::::::::::::::::::::::::::::::::::: " << std::endl;
}

void RealmConsole::StopRealmConsole()
{
    std::cout << "RealmConsole is now stopped." << std::endl;
    stop_realm_console = true;
}
