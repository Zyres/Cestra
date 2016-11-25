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

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

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

    char* game_conf = "configs/game.conf";
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
    std::string db_ip, db_port, db_user, db_password, db_name;

    //get value from config
    Config.GameConfig.GetString("GameDatabase", "DB_IP", &db_ip);
    Config.GameConfig.GetString("GameDatabase", "DB_Port", &db_port);
    Config.GameConfig.GetString("GameDatabase", "DB_User", &db_user);
    Config.GameConfig.GetString("GameDatabase", "DB_Password", &db_password);
    Config.GameConfig.GetString("GameDatabase", "DB_Name", &db_name);


    std::cout << std::endl;
    std::cout << "Try to connect to game database (" << db_name  << ")" << std::endl;

    try
    {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        std::string db_connection = "tcp://" + db_ip + ":" + db_port;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect(db_connection, db_user, db_password);
        /* Connect to the MySQL test database */
        con->setSchema(db_name);

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM `account_data`"); // replace with your statement
        while (res->next())
        {
            std::cout << "\t... MySQL replies: ";
            /* Access column data by alias or column name */
            std::cout << res->getString("_message") << std::endl;
            std::cout << "\t... MySQL says it again: ";
            /* Access column fata by numeric offset, 1 is the first column */
            std::cout << res->getString(1) << std::endl;
        }
        delete res;
        delete stmt;
        delete con;

    }

    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode() << " )" << std::endl;
        return false;
    }

    std::cout << std::endl;

    return true;
}
