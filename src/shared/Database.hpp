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


class QueryField
{
    public:

        void SetFieldValue(char* value) { m_field_value = value; }

        const char* GetString() { return m_field_value ? m_field_value : ""; }

        float GetFloat() { return m_field_value ? static_cast<float>(atof(m_field_value)) : 0; }
        bool GetBool() { return m_field_value ? atoi(m_field_value) > 0 : false; }

        uint8 GetUInt8() { return m_field_value ? static_cast<uint8>(atol(m_field_value)) : 0; }
        int8 GetInt8() { return m_field_value ? static_cast<int8>(atol(m_field_value)) : 0; }

        uint16 GetUInt16() { return m_field_value ? static_cast<uint16>(atol(m_field_value)) : 0; }
        int16 GetInt16() { return m_field_value ? static_cast<int16>(atol(m_field_value)) : 0; }

        uint32 GetUInt32() { return m_field_value ? static_cast<uint32>(atol(m_field_value)) : 0; }
        int32 GetInt32() { return m_field_value ? static_cast<int32>(atol(m_field_value)) : 0; }

    private:

        char* m_field_value;
};


class QueryResult
{
    public:

        QueryResult(MYSQL_RES* mysql_res, uint32 num_fields, uint32 num_rows) : m_num_field(num_fields), m_num_row(num_rows), m_current_field(nullptr), m_mysql_result(mysql_res)
        {
            m_current_field = new QueryField[num_fields];
        }

        void Delete() { delete this; }

        QueryField* Fetch() { return m_current_field; }
        uint32 GetFieldCount() const { return m_num_field; }
        uint32 GetRowCount() const { return m_num_row; }

        bool NextRow()
        {
            MYSQL_ROW mysql_row = mysql_fetch_row(m_mysql_result);
            if (mysql_row == NULL)
                return false;

            for (uint32 i = 0; i < m_num_field; ++i)
                m_current_field[i].SetFieldValue(mysql_row[i]);

            return true;
        }

    protected:

        uint32 m_num_field;
        uint32 m_num_row;
        QueryField* m_current_field;

        MYSQL_RES* m_mysql_result;
};


class Database
{
    public:

        Database* InitializeDatabaseConnection(std::string db_ip, std::string db_user, std::string db_password, std::string db_name, uint32 db_port);
        QueryResult* Query(const char* query_string, ...);

        bool IsValidQuery(const char* query_string, bool retry);
        uint32 HandleMySQLError(uint32 mysql_error_number);
        QueryResult* GetQueryResult();

    private:

        MYSQL* mysql_connection;

};

#endif // DATABASE_HPP
