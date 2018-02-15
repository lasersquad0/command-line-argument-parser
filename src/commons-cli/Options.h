#pragma once
/*
    Project includes
*/
#include "Option.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/
#include <vector>
#include <string>
#include <map>

class COptions
{
    private:
        typedef std::map<std::string, COption> map_option_t;
        typedef std::vector<COption> vector_option_t;

        vector_option_t m_RequiredOptionsWithArguments;
        vector_option_t m_RequiredOptions;
        vector_option_t m_AllOptions;

        map_option_t m_ShortOptions;
        map_option_t m_LongOptions;

    public:
        void AddOption(COption option);
        void AddOption(const std::string &shortName, const std::string &longName, const std::string &description, size_t numArgs, bool isRequired = false);

        COption *GetShortOption(const std::string shortName);
        COption *GetLongOption(const std::string longName);

        inline const vector_option_t &GetRequiredOptionWithArguments() { return m_RequiredOptionsWithArguments; }
        inline const vector_option_t &GetRequiredOption() { return m_RequiredOptions; }
        inline const vector_option_t &GetAllOptions() { return m_AllOptions; }
};