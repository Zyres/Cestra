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

#include <mysql.h>
#include <iostream>

#ifndef MASTER_HPP
#define MASTER_HPP

class Master
{
    public:

        Master();
        ~Master();

        bool Initialize();

        void PrintWelcomeMessage();
        void LoadConfiguration();
        bool ConnectToGameDB();

        std::string console_log;

    private:

        MYSQL* mysql_realm_connection;
};

#endif // MASTER_HPP