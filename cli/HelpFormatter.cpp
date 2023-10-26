//
// Project includes
//
#include "HelpFormatter.h"

//
// Libs include
//

//
// STDLib/OS includes
//

///////////////////////////////////////////////////////////////////////////////

static vector_string_t SplitString(const std::string &text, const std::string &delimiter)
{
    vector_string_t retData;
    size_t start = 0, end = text.find_first_of(delimiter);
    size_t delimiterSize = 1; // NOTE(Andrei): Because we use find_first_of

    while(end != std::string::npos)
    {
        if(end - start)
        {
            retData.push_back(text.substr(start, end - start));
        }

        start = end + delimiterSize;
        end = text.find_first_of(delimiter, start);
    }

    if(start != end)
    {
        retData.push_back(text.substr(start, end));
    }

    return retData;
}

std::string CHelpFormatter::Format(const std::string &appName, COptionsList *options)
{
    std::string helpBuffer(appName + " usage:\n");
    vector_option_t& opt = options->GetAllOptions();

    vector_string_t argsNames;
    vector_string_t argsParams;

    size_t largestArgsName = 0;
    size_t largestParamsName = 0;

    for (auto &it : opt)
    {
        std::string shortName = it.GetShortName(), longName = it.GetLongName();
        std::string auxBuffer;

        auxBuffer.append("-");
        auxBuffer.append(shortName);

        if (!longName.empty())
        {
            auxBuffer.append(", --");
            auxBuffer.append(longName);
        }

        largestArgsName = auxBuffer.size() > largestArgsName ? auxBuffer.size() : largestArgsName;
        argsNames.push_back(auxBuffer);
        auxBuffer.clear();

        const uint16_t ARGS_THRESHOLD = 4;
        uint16_t rargs = it.GetNumRequiredArgs();
        uint16_t oargs = it.GetNumArgs();
        if (rargs > ARGS_THRESHOLD || oargs > ARGS_THRESHOLD)
        {
            auxBuffer.append(" <req args>(" + std::to_string(rargs) + ")");
            auxBuffer.append("...<total args>(up to " + std::to_string(oargs) + ")");
        }
        else
        {
            std::string s;
            if (oargs - rargs > 0)
                s = " <req arg>";
            else
                s = " <arg>";

            for (size_t n = 0; n < rargs; ++n)
            {
                auxBuffer.append(s);
            }
            for (size_t n = 0; n < oargs-rargs; ++n)
            {
                auxBuffer.append(" <arg>");
            }
        }

        largestParamsName = auxBuffer.size() > largestParamsName ? auxBuffer.size() : largestParamsName;
        argsParams.push_back(auxBuffer);
        auxBuffer.clear();
    }

    for (size_t i = 0; i < opt.size(); ++i)
    {
        std::string auxBuffer(argsNames[i]);
        auxBuffer.append(std::string(largestArgsName - argsNames[i].size(), ' ') + argsParams[i]);

        vector_string_t descriptionLines = SplitString(opt[i].GetDescription(), "\n");
        if (!descriptionLines.empty()) auxBuffer.append(std::string(largestArgsName + largestParamsName + 1 - auxBuffer.size(), ' ') + descriptionLines[0]);

        for (size_t l = 1; l < descriptionLines.size(); ++l)
        {
            auxBuffer.append("\n");
            auxBuffer.append(std::string(largestArgsName + largestParamsName + 1, ' ') + descriptionLines[l]);
        }

        if (i + 1 < opt.size())
        {
            auxBuffer.append("\n");
        }

        helpBuffer.append(auxBuffer);
    }

    return helpBuffer;
}
