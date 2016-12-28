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

#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <mysql.h>


Master::Master() : console_log("")
{}

Master::~Master() {}

bool Master::Initialize()
{
    // log file
    console_log = "game-console.log";
    Log.InitializeLogFile(console_log, LOG_GAME);

    PrintWelcomeMessage();

    LoadConfiguration();
    
    //check db connection
    if (!ConnectToGameDB())
        return false;

    //load other data
    //packet handler
    //connect to realm
    //listen to socket


    return true;
}

void Master::PrintWelcomeMessage()
{
    //print some basic information
    //e.g. header/welcome message
    std::cout << "                                                                                                    " << std::endl;
    std::cout << "           `.-.`                                                                                    " << std::endl;
    std::cout << "         -+oooo: `                                                                                  " << std::endl;
    std::cout << "       `/oooo:`./o/`         Welcome to Cestra GameServer!                                          " << std::endl;
    std::cout << "      ..`.:-`./oooo+.                                              -++                              " << std::endl;
    std::cout << "     -ooo`   .ooooooo.          `.--.`      `.--.`      `.--..`  `./oo...  `..  `.. ``..-..`        " << std::endl;
    std::cout << "    .ooo/``.-`.+ooooo+`      `/oo+///+:   :oo///+o+.  .+oo+++++  .+ooo++/` -oo/+++- /+////+o+.      " << std::endl;
    std::cout << "    +oo/ -oooo:`.+oooo/     `oo/`        /o+     .oo` :oo-`        /oo     -oo-`           /o+      " << std::endl;
    std::cout << "   `+/: -oooooo+- :oooo`    -oo.         +o+//////oo.  -/oo+/:.    /oo     -oo.    ./+ooooooo+      " << std::endl;
    std::cout << "   ``   `::///+++.  `--`    .oo:         +o+````````      `.:oo:   /oo     -oo.   `oo:     /o+      " << std::endl;
    std::cout << "   :+  .:::----.-.   ./-     :oo/-...--  .+o/-....-.  .:----:oo/   -oo:..` -oo.   `+o/...-:oo+      " << std::endl;
    std::cout << "   -: :ooooooooo+``/.`+.      `-:////:.    .::////:.  `://///:.     .:///. .::`    `-:///:.-/:      " << std::endl;
    std::cout << "    ``ooooooooo+`.oo+``                                                                             " << std::endl;
    std::cout << "     .ooooooooo``+ooo-                                                                              " << std::endl;
    std::cout << "      `:+ooooo. +o+:`                                                                               " << std::endl;
    std::cout << "         `.-:- `.`                  This is the development version, do not expect anything ;)      " << std::endl;
    std::cout << "                                                                                                    " << std::endl;
}

void Master::LoadConfiguration()
{
    LogInfo("Start loading game.conf");

    const char* game_conf = "configs/game.conf";
    if (!Config.GameConfig.SetSource(game_conf))
    {
        LogError("game.conf not found in configs/ folder!");
        return;
    }

    LogDefault("game.conf successfully loaded");
}

bool Master::ConnectToGameDB()
{
    //define var
    std::string db_ip, db_user, db_password, db_name;
    uint32 db_port;

    //get value from config
    Config.GameConfig.GetString("GameDatabase", "DB_IP", &db_ip);
    Config.GameConfig.GetUInt("GameDatabase", "DB_Port", &db_port);
    Config.GameConfig.GetString("GameDatabase", "DB_User", &db_user);
    Config.GameConfig.GetString("GameDatabase", "DB_Password", &db_password);
    Config.GameConfig.GetString("GameDatabase", "DB_Name", &db_name);


    std::cout << std::endl;
    LogInfo("Try to connect to game database (%s)", db_name.c_str());

    mysql_game_connection = mysql_init(NULL);
    mysql_real_connect(mysql_game_connection, db_ip.c_str(), db_user.c_str(), db_password.c_str(), db_name.c_str(), db_port, NULL, 0);


    //display data using mysql_query() method
    MYSQL_RES* result;
    mysql_query(mysql_game_connection, "SELECT 1");
    result = mysql_store_result(mysql_game_connection);
    if (result != nullptr)
    {
        LogDefault("Succesful connected to game database %s.", db_name.c_str());
        mysql_free_result(result);
    }
    else
    {
        LogError("Not able to game database %s!", db_name.c_str());
        mysql_free_result(result);
        return false;
    }

    uint32 num_fields;
    uint32 num_rows;

    //retrieve and display data
    mysql_query(mysql_game_connection, "SELECT * FROM account_data");
    result = mysql_store_result(mysql_game_connection);
    num_fields = mysql_num_fields(result);
    num_rows = (uint32)mysql_num_rows(result);

    LogInfo("Table `account_data` has %u fields and %u rows.", num_fields, num_rows);

    return true;
}
