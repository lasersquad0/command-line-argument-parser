//
// Project includes
//
#include "DefaultParser.h"

//
// Libs include
//

//
// STDLib/OS includes
//

#define DASH  "-"
#define DDASH "--"

static std::string GetNonEmptyName(std::string shortName, std::string longName, bool addDash = false)
{
    if (addDash)
        if (shortName.empty())
            return DDASH + longName;
        else
            return DASH + shortName;
    else
        if (shortName.empty())
            return longName;
        else
            return shortName;
}


bool CDefaultParser::HandleToken(const std::string &token)
{
    if (token == DASH || token == DDASH)
    {
        m_LastError = "No name specified for the option";
        return false;
    }
    else if (token.compare(0, 2, DDASH) == 0)
    {
        if(m_CommandLine->HasOption(token.substr(2, token.size())))
        {
            m_LastError = "Duplicate option has been met: " + token;
            return false;
        }

        if (!HandleLongOption(token))
        {
            m_LastError = "Unknown option has been met: " + token;
            return false;
        }
    }
    else if (token.compare(0, 1, DASH) == 0)
    {
        if (m_CommandLine->HasOption(token.substr(1, token.size())))
        {
            m_LastError = "Duplicate option has been met: " + token;
            return false;
        }

        if (!HandleShortOption(token))
        {
            m_LastError = "Unknown option has been met: " + token;
            return false;
        }
    }
    else if (m_CurrentOption && m_CurrentOption->HasArgs())
    {
        if(m_CurrentOption->AcceptsArgs())
        {
            m_CurrentOption->AddArg(token);
            UpdateOptionsWithArguments(m_CurrentOption);
        }
        else
        {
            m_LastError = "Too many arguments for: " + m_CurrentOption->GetShortName();
            return false;
        }
    }
    else
    {
        m_LastError = "Unknown argument in command line: " + token;
        return false;
        // TODO(Andrei): Handle unknown token
    }

    return true;
}

bool CDefaultParser::HandleLongOption(const std::string &token)
{
    std::string opt = token.substr(2, token.size());
    COption* longOption = m_OptionsList->GetOptionByLongName(opt);

    if (!longOption)
    {
        m_CurrentOption = nullptr;
        return false;
    }

    m_CurrentOption = longOption->HasArgs() ? longOption : nullptr;
    m_CommandLine->AddOption(longOption);
    UpdateRequiredOptions(longOption);
    return true;
}

bool CDefaultParser::HandleShortOption(const std::string &token)
{
    std::string opt = token.substr(1, token.size());
    COption* shortOption = m_OptionsList->GetOptionByShortName(opt);

    if (!shortOption)
    {
        m_CurrentOption = nullptr;
        return false;
    }
    
    m_CurrentOption = shortOption->HasArgs() ? shortOption : nullptr;
    m_CommandLine->AddOption(shortOption);
    UpdateRequiredOptions(shortOption);
    return true;
}

void CDefaultParser::UpdateOptionsWithArguments(COption *option)
{
    size_t neededArgs = option->GetNumRequiredArgs();
    size_t availableArgs = option->GetArgs().size();

    //bool argsAvailable = availableArgs >= neededArgs;// TODO shall we exit immediately if argsAvailable==false? no need doing 'for' in this case.
    
    if (availableArgs < neededArgs) return;

    std::string shortName = option->GetShortName();
    std::string longName = option->GetLongName();

    for (auto it = m_ExpectedOptionWithArguments.begin(); it != m_ExpectedOptionWithArguments.end(); ++it)
    {
        if( ((*it)->GetShortName() == shortName || (!((*it)->GetLongName().empty()) && (*it)->GetLongName() == longName)) /* && argsAvailable */ )
        {
            m_ExpectedOptionWithArguments.erase(it);
            break;
        }
    }
}

void CDefaultParser::UpdateRequiredOptions(COption *option)
{
    std::string shortName = option->GetShortName();
    std::string longName = option->GetLongName();

    for (auto it = m_ExpectedOption.begin(); it != m_ExpectedOption.end(); ++it)
    {
        if ((*it)->GetShortName() == shortName || (!((*it)->GetLongName().empty()) && (*it)->GetLongName() == longName))
        {
            m_ExpectedOption.erase(it);
            break;
        }
    }
}

bool CDefaultParser::CheckMissingRequiredArguments()
{
    for (auto it = m_ExpectedOptionWithArguments.begin(); it != m_ExpectedOptionWithArguments.end();)
    {
        if( ((*it)->GetNumRequiredArgs() == 0) || (/*(*it)->GetArgs().empty() /* && !(*it)->IsRequired() &&*/ !m_CommandLine->HasOption((*it)->GetShortName())) )
            it = m_ExpectedOptionWithArguments.erase(it);
        else
            ++it;
    }

    if (m_ExpectedOptionWithArguments.empty())
    {
        return false;
    }

    m_LastError = "Missing arguments for option";

    m_LastError.append(m_ExpectedOptionWithArguments.size() == 1 ? "" : "s");
    m_LastError.append(": ");

    for (size_t i = 0; i < m_ExpectedOptionWithArguments.size(); ++i)
    {
        //std::string optName = m_ExpectedOptionWithArguments[i]->GetShortName();
        //if (optName.empty())
        //    optName = m_ExpectedOptionWithArguments[i]->GetLongName();

        std::string optName = GetNonEmptyName(m_ExpectedOptionWithArguments[i]->GetShortName(), m_ExpectedOptionWithArguments[i]->GetLongName(), true);

        m_LastError.append(optName);
        if (i + 1 < m_ExpectedOptionWithArguments.size()) m_LastError.append(", ");
    }

    return true;
}

bool CDefaultParser::CheckMissingArguments()
{
    //if (!m_CommandLine->CheckMissingArguments()) return false;

    vector_option_pt missing = m_CommandLine->GetOptionsWithMissingArguments();
    
    if (missing.size() == 0) return false;

    m_LastError = "Missing arguments for option";
    m_LastError.append(missing.size() == 1 ? "" : "s");
    m_LastError.append(": ");

    for (size_t i = 0; i < missing.size(); ++i)
    {
        std::string optName = GetNonEmptyName(missing[i]->GetShortName(), missing[i]->GetLongName(), true);

        m_LastError.append(optName);
        if (i + 1 < missing.size()) m_LastError.append(", ");
    }

    return true;
}

bool CDefaultParser::CheckMissingRequiredOptions()
{
    if (m_ExpectedOption.empty())
    {
        return false;
    }

    m_LastError = "Missing required option";

    m_LastError.append(m_ExpectedOption.size() == 1 ? "" : "s");
    m_LastError.append(": ");

    for (size_t i = 0; i < m_ExpectedOption.size(); ++i)
    {
        //std::string optName = m_ExpectedOption[i]->GetShortName();
        //if (optName.empty()) optName = m_ExpectedOption[i]->GetLongName();

        std::string optName = GetNonEmptyName(m_ExpectedOption[i]->GetShortName(), m_ExpectedOption[i]->GetLongName(), true);

        m_LastError.append(optName);
        if (i + 1 < m_ExpectedOption.size()) m_LastError.append(", ");
    }

    return true;
}

bool CDefaultParser::Parse(COptionsList *options, CCommandLine *cmd, char *argv[], int argc)
{
    vector_string_t args;
    args.resize(argc-1, "");

    for (int i = 1; i < argc; ++i) // i=1: bypass application name
    {
        args[i-1] = argv[i];
    }

    return Parse(options, cmd, args);
}

bool CDefaultParser::Parse(COptionsList *options, CCommandLine *cmd, const vector_string_t& args)
{
    if (args.size() == 0)
    {
        m_LastError = "Empty list of options in command line";
        return false;
    }

    if (options == nullptr)
    {
        m_LastError = "COptionsList can not be nullptr";
        return false;
    }

    if (cmd == nullptr)
    {
        m_LastError = "CCommandLine can not be nullptr";
        return false;
    }

    m_OptionsList = options;
    m_CommandLine = cmd;

    m_ExpectedOption = options->GetRequiredOptions();
    m_ExpectedOptionWithArguments = options->GetOptionsWithRequiredArguments();

    for (const auto &arg : args)
    {
        if (!HandleToken(arg))
        {
            m_CommandLine->Clear();
            return false;
        }
    }

    if (CheckMissingRequiredOptions() || CheckMissingRequiredArguments() || CheckMissingArguments())
    {
        return false;
    }

    m_LastError.clear();
    return true;
}
