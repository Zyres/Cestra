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
    std::cout << "Welcome to Cestra!" << std::endl;
    std::cout << "This is the development version, do not expect anything ;)" << std::endl;

    //\todo print some basic information
    // e.g. header/welcome message

    //\todo load configs

    //check db connection
    //load other data
    //connect to realm

    return true;
}
