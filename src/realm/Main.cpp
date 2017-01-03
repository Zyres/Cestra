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

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <thread>
#include <time.h>
#include <stdio.h>


int main()
{
    bool shutdown = false;

    Master master1;
    if (master1.Initialize())
        std::cout << "RealmServer successfully initialized!" << std::endl;
    else
        std::cout << "RealmServer could not be initialized!" << std::endl;

    // socket

    //console
    RealmConsole console;
    std::thread t(&RealmConsole::StartConsoleListener, &console);

    //keep it alive
    while (true)
    {
        if (!console.IsRealmConsoleStopped())
        {
            break;
        }
    }

    t.join();

    // RealmConsole has left the loop, initiate shut down.
    for (int i = 5; i > 0; i--)
    {
        sleep(1000);
        std::cout <<"Shutdown in " << i <<" seconds." << std::endl;
    }
    std::cout << "Process shutdown." << std::endl;

    return 0;
}
