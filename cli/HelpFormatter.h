#pragma once
//
// Project includes
//
#include "OptionsList.h"

//
// Libs include
//

//
// STDLib/OS includes
//
#include <string>

///////////////////////////////////////////////////////////////////////////////

struct CHelpFormatter
{
    /**
     * @brief Given a program name and the program options, generate a human
     * readable text of the program allowed arguments.
     *
     * @param appName Program name.
     * @param options List of option allowed by the program.
     * @return A string in human readable format.
     */
    static std::string Format(const std::string &appName, COptionsList *options);
};
