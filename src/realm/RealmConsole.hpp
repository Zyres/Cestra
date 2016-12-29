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

        RealmConsole() : stop_realm_console(false) {}

        void StartCosnoleListener()
        {
            std::string option = "";
            std::string end = "end";
            while (!stop_realm_console)
            {
                std::getline(std::cin, option);
                if (end.compare(option) == 0)
                {
                    std::cout << "command end was executed! Leaving the loop!" << std::endl;
                    break;
                }
                std::cout << "'" << option << "' is not a valid command! Type help to get a list with all available commands." << std::endl;
            }
        }

        bool IsRealmConsoleStopped()
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

        void StopRealmConsole()
        {
            std::cout << "RealmConsole is now stopped." << std::endl;
            stop_realm_console = true;
        }

    private:

        bool stop_realm_console;
    
};

#endif // REALM_CONSOLE_HPP