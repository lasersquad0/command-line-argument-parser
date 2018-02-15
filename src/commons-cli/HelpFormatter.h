#pragma once
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
#include <string>

class CHelpFormatter
{
    private:
        static std::vector<std::string> SplitString(const std::string &text, const std::string &delimiter);

    public:
        static std::string Format(const std::string &appName, COptions *options);
};