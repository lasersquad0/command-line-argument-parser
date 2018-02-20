#pragma once
/*
    Project includes
*/

/*
    Libs include
*/

/*
    STDLib/OS includes
*/
#include <string>
#include <vector>

class COption
{
    private:
        std::string m_Description;
        std::string m_ShortName;
        std::string m_LongName;

        bool m_IsRequired;
        size_t m_NumArgs;

        std::vector<std::string> m_Args;

    public:
        COption(const std::string &shortName, const std::string &longName, const std::string &description, size_t numArgs, bool isRequired = false);

        inline void AddArg(const std::string &arg) { m_Args.push_back(arg); }
        inline const std::vector<std::string> &GetArgs() { return m_Args; }

        inline void SetNumArgs(int NumArgs) { m_NumArgs = NumArgs; }
        inline size_t GetNumArgs() { return m_NumArgs; }

        inline const std::string &GetDescription() { return m_Description; }
        inline const std::string &GetShortName() { return m_ShortName; }
        inline const std::string &GetLongName() { return m_LongName; }

        inline bool HasShortName() { return m_ShortName.size() > 0; }
        inline bool HasLongName() { return m_LongName.size() > 0; }

        inline bool AcceptsArgs() { return m_Args.size() < m_NumArgs; }
        inline bool IsRequired() { return m_IsRequired; }
        inline bool HasArgs() { return m_NumArgs > 0; }
};