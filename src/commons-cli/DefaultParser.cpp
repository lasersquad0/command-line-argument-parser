/*
    Project includes
*/
#include "DefaultParser.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/

CDeafultParser::CDeafultParser()
{
    m_Options = NULL;
    m_CommandLine = NULL;
    m_CurrentOption = NULL;
}

CDeafultParser::~CDeafultParser()
{
    m_Options = NULL;
    m_CommandLine = NULL;
    m_CurrentOption = NULL;
}

bool CDeafultParser::HandleToken(const std::string & token)
{
    if (token == "-" || token == "--")
    {
        m_LastError = "No name specified for the argument";
        return false;
    }
    else if (token.compare(0, 1, "-") == 0)
    {
        HandleShortOption(token);
    }
    else if (token.compare(0, 2, "--") == 0)
    {
        HandleLongOption(token);
    }
    else if (m_CurrentOption && m_CurrentOption->HasArgs())
    {
        if(m_CurrentOption->AcceptsArgs())
        {
            m_CurrentOption->AddArg(token);
            UpdateRequiredOptionsWithArguments(m_CurrentOption);
        }
        else
        {
            m_LastError = "Too many arguments for: " + m_CurrentOption->GetShortName();
            return false;
        }
    }
    else
    {
        // TODO(Andrei): Handle unknow token
    }

    return true;
}

void CDeafultParser::HandleLongOption(const std::string & token)
{
    std::string opt = token.substr(2, token.size());
    COption *longOption = m_Options->GetLongOption(opt);

    if (!longOption)
    {
        m_CurrentOption = NULL;
        return;
    }

    m_CurrentOption = longOption->HasArgs() ? longOption : NULL;
    m_CommandLine->AddOption(longOption);
    UpdateRequiredOptions(longOption);
}

void CDeafultParser::HandleShortOption(const std::string & token)
{
    std::string opt = token.substr(1, token.size());
    COption *shortOption = m_Options->GetShortOption(opt);

    if (!shortOption)
    {
        m_CurrentOption = NULL;
        return;
    }

    m_CurrentOption = shortOption->HasArgs() ? shortOption : NULL;
    m_CommandLine->AddOption(shortOption);
    UpdateRequiredOptions(shortOption);
}

void CDeafultParser::UpdateRequiredOptionsWithArguments(COption * option)
{
    size_t neededArgs = option->GetNumArgs();
    size_t availableArgs = option->GetArgs().size();

    bool argsAvailable = availableArgs >= neededArgs;
    std::string shortName = option->GetShortName();
    std::string longName = option->GetLongName();

    for (std::vector<COption>::iterator it = m_ExpectedOptionWithArguments.begin(); it != m_ExpectedOptionWithArguments.end(); ++it)
    {
        if ((it->GetShortName() == shortName || it->GetLongName() == longName) && argsAvailable)
        {
            m_ExpectedOptionWithArguments.erase(it);
            break;
        }
    }
}

void CDeafultParser::UpdateRequiredOptions(COption * option)
{
    std::string shortName = option->GetShortName();
    std::string longName = option->GetLongName();

    for (std::vector<COption>::iterator it = m_ExpectedOption.begin(); it != m_ExpectedOption.end(); ++it)
    {
        if (it->GetShortName() == shortName || it->GetLongName() == longName)
        {
            m_ExpectedOption.erase(it);
            break;
        }
    }
}

bool CDeafultParser::CheckMissingArguments()
{
	if (!m_CommandLine->CheckMissingArguments()) return false;

	std::vector<COption*> missing = m_CommandLine->GetOptionsWithMissingArguments();

	m_LastError = "Missing arguments for option";
	m_LastError.append(missing.size() == 1 ? "" : "s");
	m_LastError.append(": ");

	for (size_t i = 0; i < missing.size(); ++i)
	{
		std::string optName = missing[i]->GetShortName();
		if (optName.empty()) optName = missing[i]->GetLongName();

		m_LastError.append(optName);
		if (i + 1 < missing.size()) m_LastError.append(", ");
	}

	return true;
}

bool CDeafultParser::CheckMissingRequiredArguments()
{
    if (m_ExpectedOptionWithArguments.size() == 0) return false;
    m_LastError = "Missing arguments for option";

    m_LastError.append(m_ExpectedOptionWithArguments.size() == 1 ? "" : "s");
    m_LastError.append(": ");

    for (size_t i = 0; i < m_ExpectedOptionWithArguments.size(); ++i)
    {
        std::string optName = m_ExpectedOptionWithArguments[i].GetShortName();
        if (optName.empty()) optName = m_ExpectedOptionWithArguments[i].GetLongName();

        m_LastError.append(optName);
        if (i + 1 < m_ExpectedOptionWithArguments.size()) m_LastError.append(", ");
    }

    return true;
}

bool CDeafultParser::CheckMissingRequiredOptions()
{
    if (m_ExpectedOption.size() == 0) return false;
    m_LastError = "Missing required option";

    m_LastError.append(m_ExpectedOption.size() == 1 ? "" : "s");
    m_LastError.append(": ");

    for (size_t i = 0; i < m_ExpectedOption.size(); ++i)
    {
        std::string optName = m_ExpectedOption[i].GetShortName();
        if (optName.empty()) optName = m_ExpectedOption[i].GetLongName();

        m_LastError.append(optName);
        if (i + 1 < m_ExpectedOption.size()) m_LastError.append(", ");
    }

    return true;
}

bool CDeafultParser::Parse(COptions * options, CCommandLine * cmd, char * argv[], int argc)
{
    std::vector<std::string> args;
    args.resize(argc, "");

    for (int i = 0; i < argc; ++i)
    {
        args[i] = argv[i];
    }

    return Parse(options, cmd, args);
}

bool CDeafultParser::Parse(COptions * options, CCommandLine * cmd, const std::vector<std::string> & args)
{
    if (options == NULL)
    {
        m_LastError = "COptions can not be NULL";
        return false;
    }

    if (cmd == NULL)
    {
        m_LastError = "CCommandLine can not be NULL";
        return false;
    }

    m_Options = options;
    m_CommandLine = cmd;

    m_ExpectedOption = options->GetRequiredOption();
    m_ExpectedOptionWithArguments = options->GetRequiredOptionWithArguments();

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (!HandleToken(args[i]))
        {
            m_CommandLine->Clear();
            return false;
        }
    }

    if (CheckMissingRequiredArguments() || CheckMissingRequiredOptions() || CheckMissingArguments())
    {
        return false;
    }

    m_LastError.clear();
    return true;
}
