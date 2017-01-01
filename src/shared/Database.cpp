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

#include "Database.hpp"
#include "FileLog.hpp"

Database* Database::InitializeDatabaseConnection(std::string db_ip, std::string db_user, std::string db_password, std::string db_name, uint32 db_port)
{
    mysql_connection = mysql_init(NULL);
    mysql_real_connect(mysql_connection, db_ip.c_str(), db_user.c_str(), db_password.c_str(), db_name.c_str(), db_port, NULL, 0);

    mysql_query(mysql_connection, "SELECT 1");
    MYSQL_RES* result = mysql_store_result(mysql_connection);
    if (result != nullptr)
    {
        LogDefault("Succesful connected to realm database %s.", db_name.c_str());
        mysql_free_result(result);
        return this;
    }
    else
    {
        LogError("Not able to realm database %s!", db_name.c_str());
        mysql_free_result(result);
        return nullptr;
    }

    uint32 num_fields;
    uint32 num_rows;

    //retrieve and display data
    mysql_query(mysql_connection, "SELECT * FROM accounts");
    MYSQL_RES* result2 = mysql_store_result(mysql_connection);
    num_fields = mysql_num_fields(result2);
    num_rows = (uint32)mysql_num_rows(result2);

    LogInfo("Table `accounts` has %u fields and %u rows.", num_fields, num_rows);
}
