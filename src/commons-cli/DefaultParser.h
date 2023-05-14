#pragma once
/*
    Project includes
*/
#include "Options.h"
#include "CommandLine.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/
#include <vector>
#include <string>

class CDeafultParser
{
    private:
        COptions *m_Options;
        CCommandLine *m_CommandLine;

        COption *m_CurrentOption;
        std::string m_LastError;

        std::vector<COption> m_ExpectedOption;
        std::vector<COption> m_ExpectedOptionWithArguments;

    private:
        bool HandleToken(const std::string &token);
        void HandleLongOption(const std::string &token);
        void HandleShortOption(const std::string &token);

        void UpdateRequiredOptionsWithArguments(COption *option);
        void UpdateRequiredOptions(COption *option);

        bool CheckMissingRequiredArguments();
        bool CheckMissingRequiredOptions();
        bool CheckMissingArguments();

    public:
        CDeafultParser();
        ~CDeafultParser();

        inline const std::string & GetLastError() { return m_LastError; };
        bool Parse(COptions *options, CCommandLine *cmd, char *argv[], int argc);
        bool Parse(COptions *options, CCommandLine *cmd, const std::vector<std::string> &args);
};