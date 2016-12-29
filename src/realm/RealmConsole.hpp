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


#ifndef REALM_CONSOLE_HPP
#define REALM_CONSOLE_HPP

#include <iostream>
#include <string>
#include <sstream>

class RealmConsole
{
    public:

        RealmConsole();

        void StartConsoleListener();
        bool IsRealmConsoleStopped();
        void ListenConsoleListener();

        void HandleConsoleCommands(std::string command);

        //commands
        void StopRealmConsole();

    private:

        bool stop_realm_console;
    
};

#endif // REALM_CONSOLE_HPP