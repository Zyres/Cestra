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

#include <boost/asio.hpp>
#include "stdafx.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <thread>
#include <time.h>
#include <stdio.h>

#include <boost/thread.hpp>

#define realm_hello_port 13

using boost::asio::ip::tcp;

void StartListenSocket()
{
    try
    {
        boost::asio::io_service io_service;
        //create socket
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), realm_hello_port));

        //listen for connection
        for (;;)
        {
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            //write message
            std::string message = "Hello from RealmServer";

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message),
                               boost::asio::transfer_all(), ignored_error);

            //print connection information
            std::string s = socket.remote_endpoint().address().to_string();
            unsigned short remote_port = socket.remote_endpoint().port();
            std::cout << "GameServer connection accepted from " << s << ":" << remote_port << std::endl;
        }
    } catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

int main()
{
    bool shutdown = false;

    Master master1;
    if (master1.Initialize())
        std::cout << "RealmServer successfully initialized!" << std::endl;
    else
        std::cout << "RealmServer could not be initialized!" << std::endl;

    // socket
    boost::thread realm_socket(&StartListenSocket);


    //console
    RealmConsole console;
    boost::thread realm_console(&RealmConsole::StartConsoleListener, &console);

    //keep it alive
    while (true)
    {
        if (!console.IsRealmConsoleStopped())
            break;
    }

    // RealmConsole has left the loop, initiate shut down.
    for (int i = 5; i > 0; i--)
    {
        Sleep(1000);
        std::cout <<"Shutdown in " << i <<" seconds." << std::endl;
    }
    std::cout << "Process shutdown." << std::endl;

    return 0;
}
