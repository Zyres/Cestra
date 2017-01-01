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


Master::Master() : console_log("")
{}

Master::~Master() {}

bool Master::Initialize()
{
    // log file
    console_log = "realm-console.log";
    Log.InitializeLogFile(console_log, LOG_REALM);

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
    std::cout << "       `/oooo:`./o/`         Welcome to Cestra RealmServer!                                         " << std::endl;
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
    LogInfo("Start loading realm.conf");

    const char* realm_conf = "configs/realm.conf";
    if (!Config.GameConfig.SetSource(realm_conf))
    {
        LogError("realm.conf not found in configs/ folder!");
        return;
    }

    LogDefault("realm.conf successfully loaded");
}

bool Master::ConnectToGameDB()
{
    //define var
    std::string db_ip, db_user, db_password, db_name;
    uint32 db_port;

    //get value from config
    Config.GameConfig.GetString("RealmDatabase", "DB_IP", &db_ip);
    Config.GameConfig.GetUInt("RealmDatabase", "DB_Port", &db_port);
    Config.GameConfig.GetString("RealmDatabase", "DB_User", &db_user);
    Config.GameConfig.GetString("RealmDatabase", "DB_Password", &db_password);
    Config.GameConfig.GetString("RealmDatabase", "DB_Name", &db_name);

    LogInfo("Try to connect to realm database (%s)", db_name.c_str());

    Database db_realm;
    realm_db_connection = db_realm.InitializeDatabaseConnection(db_ip, db_user, db_password, db_name, db_port);
    if (realm_db_connection == nullptr)
        return false;

    QueryResult* query_result = realm_db_connection->Query("SELECT * FROM accounts");
    if (query_result != nullptr)
    {
        uint32 count = 0;
        do
        {
            QueryField* fields = query_result->Fetch();

            uint32 guid = fields[0].GetUInt32();
            std::string account = fields[1].GetString();
            std::string e_mail = fields[5].GetString();

            LogInfo("Account %s (%u) with E-Mail: %s loaded from table 'accounts'.", account.c_str(), guid, e_mail.c_str());

            ++count;

        }
        while (query_result->NextRow());

        delete query_result;

        LogInfo("%u accounts loaded from  table 'accounts'", count);
    }
    else
    {
        LogError("Failed to load data from table 'accounts'!");
        return false;
    }

    return true;
}
