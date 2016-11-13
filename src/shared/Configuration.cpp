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

#include "Configuration.hpp"
#include <cstdint>

#pragma warning (disable : 4996)

ConfigurationManager Config;

Configuration::Configuration()
{ }


Configuration::~Configuration()
{ }

bool Configuration::SetSource(const char* file, bool ignorecase)
{
    m_settings.clear();

    if (file != 0)
    {
#ifdef WIN32
        FILE* configFile = fopen(file, "rb");
#else
        FILE* configFile = fopen(file, "r");
#endif
        char* fileBuffer;
        uint32 fileLength;

        if (!configFile)
        {
            //sLog.outError("Could not open %s.", file);
            return false;
        }

        // get the length of the file
        fseek(configFile, 0, SEEK_END);

        if (ftell(configFile) <= 0)
        {
            fclose(configFile);
            return false;
        }
        else
        {
            fileLength = ftell(configFile);
        }

        fileBuffer = new char[fileLength + 1];
        fseek(configFile, 0, SEEK_SET);

        if (fread(fileBuffer, fileLength, 1, configFile) != 1)
        {
            // file not readable
            delete[] fileBuffer;
            fclose(configFile);
            return false;
        }

        // we read the file, close it and free memory
        fileBuffer[fileLength] = '\0';
        std::string buffer = std::string(fileBuffer);
        delete[] fileBuffer;
        fclose(configFile);

        // parse the file content
        std::string fileLine;
        std::string::size_type fileEnd;
        std::string::size_type fileOffset;
        bool isMultilineComment = false;
        bool isInBlock = false;
        std::string currentSetting = "";
        std::string currentVariable = "";
        std::string currentBlock = "";
        ConfigSettingsMap currentBlockMap;
        ConfigSetting currentSettingStructure;

        try
        {
            for (;;)
            {
                // get the line
                fileEnd = buffer.find(EOL);
                if (fileEnd == std::string::npos)
                {
                    if (buffer.size() == 0)
                        break;
                    fileLine = buffer;
                    buffer.clear();
                    goto parse;
                }

                fileLine = buffer.substr(0, fileEnd);
                buffer.erase(0, fileEnd + EOL_SIZE);
                goto parse;

            parse:
                if (!fileLine.size())
                    continue;

                // check comment
                if (!isMultilineComment && IsComment(fileLine, &isMultilineComment))
                {
                    if (!isMultilineComment)
                    {
                        // it is a single line comment, continue with next line
                        continue;
                    }
                }

                if (isMultilineComment)
                {
                    // find end of multiline comment
                    fileOffset = fileLine.find("*/", 0);

                    // skip multiline comment to from begin to end
                    if (fileOffset == std::string::npos)
                        continue;

                    // erase multiline comment and continue with next line
                    fileLine.erase(0, fileOffset + 2);
                    isMultilineComment = false;
                }

                if (isInBlock)
                {
                    RemoveSpaces(fileLine);

                    if (!fileLine.size())
                        continue;

                    // look for "="
                    fileOffset = fileLine.find("=");
                    if (fileOffset != std::string::npos)
                    {
                        currentVariable = fileLine.substr(0, fileOffset);
                        RemoveAllSpaces(currentVariable);
                        fileLine.erase(0, fileOffset + 1);
                    }

                    // look for opening quote
                    fileOffset = fileLine.find("\"");
                    if (fileOffset != std::string::npos)
                    {
                        // find closing quote
                        fileEnd = fileLine.find("\"", fileOffset + 1);
                        if (fileEnd != std::string::npos)
                        {
                            // on same line
                            currentSetting = fileLine.substr(fileOffset + 1, fileEnd - fileOffset - 1);

                            // erase to the end
                            fileLine.erase(0, fileEnd + 1);

                            // apply setting
                            ApplySetting(currentSetting, currentSettingStructure);

                            // append setting to the current block
                            currentBlockMap[ahash(currentVariable)] = currentSettingStructure;

                            // done with the setting, set var and setting to empty string
                            currentSetting = "";
                            currentVariable = "";

                            // grab next setting from the same line
                            goto parse;
                        }
                        else
                        {
                            // closing quote not on the same line. Try to find it oin the next line
                            currentSetting.append(fileLine.c_str(), fileOffset);
                            continue;
                        }
                    }

                    // check for the end of the block
                    fileOffset = fileLine.find(">");
                    if (fileOffset != std::string::npos)
                    {
                        fileLine.erase(0, fileOffset + 1);
                        isInBlock = false;

                        // assign block to map
                        m_settings[ahash(currentBlock)] = currentBlockMap;

                        // erase all values
                        currentBlockMap.clear();
                        currentSetting = "";
                        currentVariable = "";
                        currentBlock = "";
                    }
                }
                else
                {
                    // not in a block. Look for next block
                    fileOffset = fileLine.find("<");
                    if (fileOffset != std::string::npos)
                    {
                        isInBlock = true;
                        // erase all data before this blcok
                        fileLine.erase(0, fileOffset + 1);

                        // get the block name
                        fileOffset = fileLine.find(" ");
                        if (fileOffset != std::string::npos)
                        {
                            currentBlock = fileLine.substr(0, fileOffset);
                            fileLine.erase(0, fileOffset + 1);
                        }
                        else
                        {
                            // block has no name, invalid config file
                            return false;
                        }

                        // grab next block
                        goto parse;
                    }
                }
            }

        }
        catch (...)
        {
            return false;
        }

        if (isInBlock)
            return false;

        if (isMultilineComment)
            return false;

        // config is loaded, no errors, return true (loaded)
        return true;
    }

    return false;
}

Configuration::ConfigSetting* Configuration::GetSetting(const char* conf_block, const char* conf_name)
{
    uint32 block_hash = ahash(conf_block);
    uint32 setting_hash = ahash(conf_name);

    // block value in our map
    std::map<uint32, ConfigSettingsMap>::iterator itr = m_settings.find(block_hash);
    if (itr != m_settings.end())
    {
        ConfigSettingsMap::iterator it2 = itr->second.find(setting_hash);
        if (it2 != itr->second.end())
            return &(it2->second);

        return nullptr;
    }

    return nullptr;
}

bool Configuration::GetString(const char* conf_block, const char* conf_name, std::string* conf_value)
{
    ConfigSetting* configSetting = GetSetting(conf_block, conf_name);
    if (configSetting == nullptr)
        return false;

    *conf_value = configSetting->stringValue;
    return true;
}


bool Configuration::GetBool(const char* conf_block, const char* conf_name, bool* conf_value)
{
    ConfigSetting* configSetting = GetSetting(conf_block, conf_name);
    if (configSetting == nullptr)
        return false;

    *conf_value = configSetting->boolValue;
    return true;
}

bool Configuration::GetUInt(const char* conf_block, const char* conf_name, uint32* conf_value)
{
    ConfigSetting* configSetting = GetSetting(conf_block, conf_name);
    if (configSetting == nullptr)
        return false;

    *conf_value = configSetting->uIntValue;
    return true;
}

bool Configuration::GetInt(const char* conf_block, const char* conf_name, int* conf_value)
{
    ConfigSetting* configSetting = GetSetting(conf_block, conf_name);
    if (configSetting == nullptr)
        return false;

    *conf_value = configSetting->intValue;
    return true;
}

bool Configuration::GetFloat(const char* conf_block, const char* conf_name, float* conf_value)
{
    ConfigSetting* configSetting = GetSetting(conf_block, conf_name);
    if (configSetting == nullptr)
        return false;

    *conf_value = configSetting->floatValue;
    return true;
}

void Configuration::RemoveSpaces(std::string& str)
{
    while (str.size() && (*str.begin() == ' ' || *str.begin() == '\t'))
        str.erase(str.begin());
}

void Configuration::RemoveAllSpaces(std::string& str)
{
    std::string::size_type off = str.find(" ");
    while (off != std::string::npos)
    {
        str.erase(off, 1);
        off = str.find(" ");
    }

    off = str.find("\t");
    while (off != std::string::npos)
    {
        str.erase(off, 1);
        off = str.find("\t");
    }
}

bool Configuration::IsComment(std::string& str, bool* in_multiline_quote)
{
    std::string stemp = str;
    RemoveSpaces(stemp);
    if (stemp.length() == 0)
        return false;

    if (stemp[0] == '/')
    {
        if (stemp.length() < 2)
            return false;

        if (stemp[1] == '*')
        {
            *in_multiline_quote = true;
            return true;
        }
        else if (stemp[2] == '/')
        {
            return true;
        }
    }

    if (stemp[0] == '#')
        return true;

    return false;
}

void Configuration::ApplySetting(std::string& str, ConfigSetting& setting)
{
    setting.stringValue = str;
    setting.intValue = atoi(str.c_str());
    setting.uIntValue = atoi(str.c_str());
    setting.boolValue = (setting.intValue > 0);
    setting.floatValue = (float)atof(str.c_str());

    if (str.length() > 1)
    {
        if (str.size() >= 3 && !_strnicmp("yes", str.c_str(), 3))
        {
            setting.boolValue = true;
            setting.intValue = 1;
        }
        else if (str.size() >= 2 && !_strnicmp("no", str.c_str(), 2))
        {
            setting.boolValue = false;
            setting.intValue = 0;
        }
    }
}

uint32 Configuration::ahash(const char* str)
{
    register size_t len = strlen(str);
    register uint32 ret = 0;
    register size_t i = 0;

    for (; i < len; ++i)
        ret += 5 * ret + (tolower(str[i]));

    return ret;
}

uint32 Configuration::ahash(std::string& str)
{
    return ahash(str.c_str());
}
