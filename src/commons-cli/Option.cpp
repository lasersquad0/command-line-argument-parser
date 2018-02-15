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

COption::COption(const std::string & shortName, const std::string & longName, const std::string & description, size_t numArgs, bool isRequired)
{
    m_ShortName.assign(shortName);
    m_LongName.assign(longName);

    m_Description.assign(description);
    m_IsRequired = isRequired;
    m_NumArgs = numArgs;

}
