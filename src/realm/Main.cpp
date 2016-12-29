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

    std::cout << "Type help to see all available commands. You can shut down this programm with command: end " << std::endl;


    ////socket
    SocketMgr* socket = new SocketMgr;

    std::thread t(&SocketMgr::InitializeSocketListener, socket);

    std::cout << "thread " << t.get_id() << " created!" << std::endl;

    std::string option = "";
    std::string end = "end";
    while (true)
    {
        std::getline(std::cin, option);
        if (end.compare(option) == 0)
        {
            std::cout << "command end was executed! Leaving the loop!" << std::endl;
            break;
        }
        std::cout << "'" << option << "' is not a valid command! Type help to get a list with all available commands." << std::endl;
    }

    if (socket->ShutdownSocketMgr())
        std::cout << "Shutdown SocketMgr is set to true." << std::endl;


    // uhh... call it for death.
    //socket->InitializeSocketListener();

    ////// socket end

    for (int i = 5; i > 0; i--)
    {
        Sleep(1000);
        std::cout <<"Shutdown in " << i <<" seconds." << std::endl;
    }
    std::cout << "Process shutdown." << std::endl;

    ///////////////////////////////////////////
    // Shutdown here
    ///////////////////////////////////////////

    socket->~SocketMgr();

    return 0;
}
