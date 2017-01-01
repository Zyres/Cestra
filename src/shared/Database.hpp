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

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <mysql.h>
#include <string>

#include "Common.hpp"

class QueryResult
{

};

class Database
{
    public:

        Database* InitializeDatabaseConnection(std::string db_ip, std::string db_user, std::string db_password, std::string db_name, uint32 db_port);
        QueryResult* Query(const char* QueryString, ...);

    private:

        MYSQL* mysql_connection;

};

#endif // DATABASE_HPP
