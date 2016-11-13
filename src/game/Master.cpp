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

Master::Master() {}
Master::~Master() {}

bool Master::Initialize()
{
    PrintWelcomeMessage();

    LoadConfiguration();
    
    //check db connection
    //load other data
    //connect to realm

    return true;
}

void Master::PrintWelcomeMessage()
{
    //print some basic information
    //e.g. header/welcome message

    std::cout << "Welcome to Cestra!" << std::endl;
    std::cout << "This is the development version, do not expect anything ;)" << std::endl;
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
    std::string version, note;
    bool generic_boolean;
    float generic_float;
    uint32 generic_uint;
    int generic_int;

    //get value from config
    Config.GameConfig.GetString("Configs", "ConfigVersion", &version);
    Config.GameConfig.GetString("Configs", "ConfigNote", &note);
    Config.GameConfig.GetBool("Configs", "ConfigFalse", &generic_boolean);
    Config.GameConfig.GetFloat("Configs", "ConfigFloat", &generic_float);
    Config.GameConfig.GetUInt("Configs", "ConfigUInt", &generic_uint);
    Config.GameConfig.GetInt("Configs", "ConfigInt", &generic_int);

    //print value
    std::cout << "ConfigVersion:" << version << std::endl;
    std::cout << "ConfigNote:" << note << std::endl;
    std::cout << "ConfigFalse:" << generic_boolean << std::endl;
    std::cout << "ConfigFloat:" << generic_float << std::endl;
    std::cout << "ConfigUInt:" << generic_uint << std::endl;
    std::cout << "ConfigInt:" << generic_int << std::endl;

    std::cout << "game.conf successfully loaded" << std::endl;
}
