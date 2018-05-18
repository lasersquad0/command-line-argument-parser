/*
    Project includes
*/
#include "HelpFormatter.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/

std::vector<std::string> CHelpFormatter::SplitString(const std::string & text, const std::string &delimiter, bool allowEmptyText/* = false */)
{
    std::vector<std::string> retData;
    size_t start = 0, end = text.find_first_of(delimiter);
    size_t delimiterSize = 1; // NOTE(Andrei): Because we use find_first_of

    while(end != std::string::npos)
    {
        if(end - start || allowEmptyText)
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

std::string CHelpFormatter::Format(const std::string & appName, COptions * options)
{
    std::string helpBuffer(appName + " usage:\n");
    std::vector<COption> opt = options->GetAllOptions();

    std::vector<std::string> argsNames;
    std::vector<std::string> argsParams;

    size_t largestArgsName = 0;
    size_t largestParamsName = 0;

    for (size_t i = 0; i < opt.size(); ++i)
    {
        std::string shortName = opt[i].GetShortName(), longName = opt[i].GetLongName();
        std::string auxBuffer;

        auxBuffer.append("-");
        auxBuffer.append(shortName);

        if (longName.size())
        {
            auxBuffer.append(", --");
            auxBuffer.append(longName);
        }

        largestArgsName = auxBuffer.size() > largestArgsName ? auxBuffer.size() : largestArgsName;
        argsNames.push_back(auxBuffer);
        auxBuffer.clear();

        for (size_t n = 0; n < opt[i].GetNumArgs(); ++n)
        {
            auxBuffer.append(" <arg>");
        }

        largestParamsName = auxBuffer.size() > largestParamsName ? auxBuffer.size() : largestParamsName;
        argsParams.push_back(auxBuffer);
        auxBuffer.clear();
    }

    for (size_t i = 0; i < opt.size(); ++i)
    {
        std::string auxBuffer(argsNames[i]);
        auxBuffer.append(std::string(largestArgsName - argsNames[i].size(), ' ') + argsParams[i]);

        std::vector<std::string> descriptionLines = SplitString(opt[i].GetDescription(), "\n");
        if (descriptionLines.size()) auxBuffer.append(std::string(largestArgsName + largestParamsName + 1 - auxBuffer.size(), ' ') + descriptionLines[0]);

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
