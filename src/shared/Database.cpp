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
        LogError("Not able to connect to realm database %s!", db_name.c_str());
        mysql_free_result(result);
        return nullptr;
    }
}

QueryResult* Database::Query(const char* query_string, ...)
{
    char query_buffer[32768];

    va_list ap;
    va_start(ap, query_string);
    vsnprintf(query_buffer, 32768, query_string, ap);
    va_end(ap);

    QueryResult* query_result = nullptr;

    if (IsValidQuery(query_buffer, false))
        query_result = GetQueryResult();

    return query_result;
}

QueryResult* Database::GetQueryResult()
{
    MYSQL_RES* mysql_res = mysql_store_result(mysql_connection);

    uint32 num_fields = (uint32)mysql_field_count(mysql_connection);
    uint32 num_rows = (uint32)mysql_affected_rows(mysql_connection);

    if (num_rows == 0 || num_fields == 0 || mysql_res == nullptr)
    {
        if (mysql_res != nullptr)
            mysql_free_result(mysql_res);

        return nullptr;
    }

    QueryResult* query_result = new QueryResult(mysql_res, num_fields, num_rows);
    query_result->NextRow();

    return query_result;
}

bool Database::IsValidQuery(const char* query_string, bool retry)
{
    mysql_query(mysql_connection, query_string); //connect to db and run query
    
    uint32 handle_number = HandleMySQLError(mysql_errno(mysql_connection));

    if (retry == false && handle_number == 1)
    {
        IsValidQuery(query_string, true);
    }
    else if (handle_number != 0)
    {
        LogError("MySQL error code %u query failed due to [%s]", mysql_errno(mysql_connection), mysql_error(mysql_connection));
        return false;
    }

    return true;
}

uint32 Database::HandleMySQLError(uint32 mysql_error_number)
{
    if (mysql_error_number == 0)
        return 0;

    uint32 error_number = mysql_error_number;

    switch (mysql_error_number)
    {
        case 2006:
            return 1;   // retry (reconnect to mysql)
            break;
        default:
            return error_number;    // return error number
    }
}

// QueryResult
