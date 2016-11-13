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

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "Common.hpp"
#include <string>
#include <map>
#include <cstdint>

class Configuration
{
    public:

        Configuration();
        ~Configuration();

        struct ConfigSetting
        {
            std::string stringValue;
            bool boolValue;
            int intValue;
            uint32 uIntValue;
            float floatValue;
        };

        typedef std::map<uint32, ConfigSetting> ConfigSettingsMap;

        bool SetSource(const char* file, bool ignorecase = true);
        ConfigSetting* GetSetting(const char* conf_block, const char* conf_name);

        bool GetString(const char* conf_block, const char* conf_name, std::string* conf_value);
        bool GetBool(const char* conf_block, const char* conf_name, bool* conf_value);
        bool GetUInt(const char* conf_block, const char* conf_name, uint32* conf_value);
        bool GetInt(const char* conf_block, const char* conf_name, int* conf_value);
        bool GetFloat(const char* conf_block, const char* conf_name, float* conf_value);

    private:

        std::map<uint32, ConfigSettingsMap> m_settings;

        // helpers
        void RemoveSpaces(std::string& str);
        void RemoveAllSpaces(std::string& str);
        bool IsComment(std::string& str, bool* in_multiline_quote);
        void ApplySetting(std::string& str, ConfigSetting& setting);
        uint32 ahash(const char* str);
        uint32 ahash(std::string& str);
};


class ConfigurationManager
{
    public:

        Configuration GameConfig;
        Configuration RealmConfig;
};

extern ConfigurationManager Config;

#endif // CONFIGURATION_HPP
