/*
    Project includes
*/
#include "Options.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/


void COptions::AddOption(COption option)
{
    if (option.HasLongName())
    {
        m_LongOptions.insert(std::pair<std::string, COption>(option.GetLongName(), option));
    }

    m_ShortOptions.insert(std::pair<std::string, COption>(option.GetShortName(), option));
    m_AllOptions.push_back(option);

    if (option.HasArgs())
    {
        m_RequiredOptionsWithArguments.push_back(option);
    }

    if (option.IsRequired())
    {
        m_RequiredOptions.push_back(option);
    }
}

void COptions::AddOption(const std::string & shortName, const std::string & longName, const std::string & description, size_t numArgs, bool isRequired)
{
    AddOption(COption(shortName, longName, description, numArgs, isRequired));
}

COption * COptions::GetShortOption(const std::string &shortName)
{
    map_option_t::iterator it = m_ShortOptions.find(shortName);
    return it == m_ShortOptions.end() ? NULL : &it->second;
}

COption * COptions::GetLongOption(const std::string &longName)
{
    map_option_t::iterator it = m_LongOptions.find(longName);
    return it == m_LongOptions.end() ? NULL : &it->second;
}
