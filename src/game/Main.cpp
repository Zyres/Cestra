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
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <ctime>


const std::string realm_hello_port = "13";
const std::string realm_host = "127.0.0.1";

using boost::asio::ip::tcp;

void StartConnectionSocket()
{
    try
    {
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(realm_host, realm_hello_port);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        tcp::socket socket(io_service);
        boost::system::error_code error = boost::asio::error::host_not_found;
        while (error && endpoint_iterator != end)
        {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }

        if (error)
            throw boost::system::system_error(error);
        else
        {
            //print connection information
            std::string s = socket.remote_endpoint().address().to_string();
            unsigned short remote_port = socket.remote_endpoint().port();
            std::cout << "RealmServer connection accepted for " << s << ":" << remote_port << std::endl;
        }

        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buf.data(), len);
        }
    } catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

int main()
{
    Master master;
    if (master.Initialize())
        std::cout << "GameServer successfully initialized!" << std::endl;
    else
        std::cout << "GameServer could not be initialized!" << std::endl;
    
    //socket
    boost::thread game_socket(&StartConnectionSocket);

    int wait = 0;
    std::cin >> wait;

    return 0;
}
