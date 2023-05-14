/*
    Project includes
*/
#include "CommandLine.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/

COption * CCommandLine::ResolveOption(const std::string & opt)
{
    for (size_t i = 0; i < m_Options.size(); ++i)
    {
        if (m_Options[i]->GetShortName() == opt || m_Options[i]->GetLongName() == opt)
        {
            return m_Options[i];
        }
    }

    return NULL;
}

void CCommandLine::Clear()
{
    m_Options.clear();
}

bool CCommandLine::AddOption(COption * option)
{
    if (HasOption(option->GetShortName()) || HasOption(option->GetLongName()))
    {
        return false;
    }

    m_Options.push_back(option);
    return true;
}

bool CCommandLine::HasOption(const std::string & opt)
{
    COption * option = ResolveOption(opt);
    return option != NULL;
}

std::string CCommandLine::GetOptionValue(const std::string & opt, const std::string & defaultValue)
{
    COption * option = ResolveOption(opt);
    return option == NULL ? defaultValue : option->GetArgs()[0];
}

std::string CCommandLine::GetOptionValue(const std::string& opt, uint32_t valIndex, const std::string& defaultValue)
{
    COption* option = ResolveOption(opt);
    return option == NULL ? defaultValue : option->GetArgs()[valIndex];
}

bool CCommandLine::CheckMissingArguments()
{
    for (size_t i = 0; i < m_Options.size(); ++i)
    {
        if (m_Options[i]->AcceptsArgs())
            return true;
    }

    return false;
}

std::vector<COption*> CCommandLine::GetOptionsWithMissingArguments()
{
    std::vector<COption*> missingArgs;

    for (size_t i = 0; i < m_Options.size(); ++i)
    {
        if (m_Options[i]->AcceptsArgs())
        {
            missingArgs.push_back(m_Options[i]);
        }
    }

    return missingArgs;
}