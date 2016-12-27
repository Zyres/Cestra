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
#include <iostream>


Master::Master() {}
Master::~Master() {}

bool Master::Initialize()
{
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
    std::cout << "       `/oooo:`./o/`         Welcome to Cestra!                                                     " << std::endl;
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
    std::cout << "Start loading game.conf..." << std::endl;

    const char* game_conf = "configs/game.conf";
    if (!Config.GameConfig.SetSource(game_conf))
    {
        std::cout << "game.conf not found in configs/ folder!" << std::endl;
        return;
    }

    //define var
    //std::string version, note;
    //bool generic_boolean;
    //float generic_float;
    //uint32 generic_uint;
    //int generic_int;

    ////get value from config
    //Config.GameConfig.GetString("Configs", "ConfigVersion", &version);
    //Config.GameConfig.GetString("Configs", "ConfigNote", &note);
    //Config.GameConfig.GetBool("Configs", "ConfigFalse", &generic_boolean);
    //Config.GameConfig.GetFloat("Configs", "ConfigFloat", &generic_float);
    //Config.GameConfig.GetUInt("Configs", "ConfigUInt", &generic_uint);
    //Config.GameConfig.GetInt("Configs", "ConfigInt", &generic_int);

    ////print value
    //std::cout << "ConfigVersion:" << version << std::endl;
    //std::cout << "ConfigNote:" << note << std::endl;
    //std::cout << "ConfigFalse:" << generic_boolean << std::endl;
    //std::cout << "ConfigFloat:" << generic_float << std::endl;
    //std::cout << "ConfigUInt:" << generic_uint << std::endl;
    //std::cout << "ConfigInt:" << generic_int << std::endl;

    std::cout << "game.conf successfully loaded" << std::endl;
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
    std::cout << "Try to connect to game database (" << db_name  << ")" << std::endl;

    mysql_game_connection = mysql_init(NULL);
    mysql_real_connect(mysql_game_connection, db_ip.c_str(), db_user.c_str(), db_password.c_str(), db_name.c_str(), db_port, NULL, 0);


    //display data using mysql_query() method
    MYSQL_RES* result;
    mysql_query(mysql_game_connection, "SELECT 1");
    result = mysql_store_result(mysql_game_connection);
    if (result != nullptr)
    {
        std::cout << "Succesful connected to game database " << db_name << "." << std::endl;
        mysql_free_result(result);
    }
    else
    {
        std::cout << "Not able to game database " << db_name << "!" << std::endl;
        mysql_free_result(result);
        return false;
    }

    //display data using mysql_query() method
    MYSQL_ROW row;
    uint32 num_fields;
    uint32 num_rows;
    int i;

    //retrieve and display data
    mysql_query(mysql_game_connection, "SELECT * FROM account_data");
    result = mysql_store_result(mysql_game_connection);
    num_fields = mysql_num_fields(result);
    num_rows = mysql_num_rows(result);

    /* display table content
    while ((row = mysql_fetch_row(result)))
    {
        for (i = 0; i < num_fields; ++i)
        {
            std::cout << row[i] << "\t";

        }
        std::cout << "\n";
    }
    mysql_free_result(result);*/

    std::cout << "MySQL : Table `account_data` has "<< num_fields <<" fields and " << num_rows << " rows."  << std::endl;

    return true;
}
