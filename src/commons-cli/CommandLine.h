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

class CCommandLine
{
    private:
        std::vector<COption *> m_Options;

    private:
        COption *ResolveOption(const std::string &opt);

    public:
        void Clear();
        bool AddOption(COption *option);
        inline std::vector<COption *> GetOptions() { return m_Options; }

        bool HasOption(const std::string &opt);
        std::string GetOptionValue(const std::string &opt, const std::string &defaultValue = "");
};