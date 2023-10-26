#pragma once
//
// Project includes
//

//
// Libs include
//

//
// STDLib/OS includes
//
#include <string>
#include <vector>
#include <stdexcept>

///////////////////////////////////////////////////////////////////////////////

/** @brief Vector that contains string. Commonly used type in many classes */
typedef std::vector<std::string> vector_string_t;

//class COption_basic
//{
//public:
//    COption_basic()
//    {
//        // default values for option
//        m_ShortName   = "";
//        m_LongName    = "";
//        m_Description = "";
//        m_IsRequired  = false;
//        m_NumArgs     = 0; // option without arguments by default 
//        m_NumRequiredArgs = 0;
//    }
//
//    /** @brief Option short name (ex: -v). */
//    std::string m_ShortName;
//
//    /** @brief Option long name (ex: --version). */
//    std::string m_LongName;
//
//    /** @brief Human reafable option description. */
//    std::string m_Description;
//
//    /** @brief Is the option required or optional. */
//    bool m_IsRequired;
//
//    /** @brief Number of arguments. */
//    uint16_t m_NumArgs;
//
//    /** @brief Number of required arguments in m_NumArgs. */
//    uint16_t m_NumRequiredArgs;
//
//};

class COption//: private COption_basic
{
public:
    /**
     * @brief Create an option instance with default values. This is used by the parser to check
     * if the program argument is an option name, an option argument or an unknown data.
     */
    COption():m_NumArgs(0),m_NumRequiredArgs(0),m_IsRequired(false) {}

    /**
     * @brief Create an option instance. This is used by the parser to check
     * if the program argument is an option name, an option argument or an
     * unknown data.
     *
     * @param shortName Short name of the option (ex: -v).
     * @param longName Long name of the option (ex: --version).
     * @param description Option description.
     * @param numArgs Number of arguments of the option.
     * @param isRequired True if is required, false otherwise.
     */
    COption(const std::string& shortName, const std::string& longName, const std::string& description, uint16_t numArgs, bool isRequired = false);

    /**
     * @brief Update option information.
     *
     * @param shortName Short name of the option (ex: -v).
     * @param longName Long name of the option (ex: --version).
     * @param description Option description.
     * @param numArgs Number of arguments of the option.
     * @param isRequired True if is required, false otherwise.
     */
    COption& Assign(const std::string& shortName, const std::string& longName, const std::string& description, uint16_t numArgs, bool isRequired = false);

    /**
     * @brief Update option information from another option.
     *
     * @param opt All fields of this Option are copied to the new option.
     */
    COption& Assign(const COption& opt);

    /**
     * @brief Update option information from another option.
     *
     * @param opt All fields of this Option are copied to the new option.
     */
   // void Assign(const COption_basic& opt);

    /**
     * @brief Store the option argument.
     * @param arg The option argument.
     */
    inline COption& AddArg(const std::string& arg) { m_Args.push_back(arg); return *this; }

    /**
     * @brief Retrieve the option arguments.
     * @return A reference to a vector with the option arguments.
     */
    inline const vector_string_t& GetArgs() { return m_Args; }

    /**
     * @brief Get the option number of possible arguments.
     * @return The number of possible arguments.
     */
    inline uint16_t GetNumArgs() { return m_NumArgs; }
    COption& NumArgs(uint16_t numArgs) 
    {
        m_NumArgs = numArgs;
        if(m_NumRequiredArgs > m_NumArgs)
           m_NumRequiredArgs = m_NumArgs;

        return *this; 
    }
    
    /**
     * @brief Get the option number of required arguments.
     * @return The number of required arguments.
     */
    inline uint16_t GetNumRequiredArgs() { return m_NumRequiredArgs; }
    COption& RequiredArgs(uint16_t numRequiredArgs) 
    { 
        m_NumRequiredArgs = numRequiredArgs;
        if (m_NumArgs < m_NumRequiredArgs) // m_NumRequiredArgs cannot be larger than m_NumArgs  
            m_NumArgs = m_NumRequiredArgs;

        return *this; 
    }
    
    /**
     * @brief Get the option description.
     * @return A string with the option description.
     */
    inline const std::string& GetDescription() { return m_Description; }
    COption& Descr(const std::string& descr) { m_Description = descr; return *this; }
    
    /**
     * @brief Get the option short name (does not contain the dash).
     * @return A string with the short name.
     */
    inline const std::string& GetShortName() { return m_ShortName; }
    COption& ShortName(const std::string& shortName) { m_ShortName = shortName; return *this; }
    
    /**
     * @brief Get the option long name (does not contain the dashes).
     * @return A string with the long name.
     */
    inline const std::string& GetLongName() { return m_LongName; }
    COption& LongName(const std::string& longName) { m_LongName = longName; return *this; }
    
    /**
     * @brief Check if the option has short nane.
     * @return true if has short name, false otherwise.
     */
    inline bool HasShortName() { return !m_ShortName.empty(); }

    /**
     * @brief Check if the option has long name.
     * @return true if has long name, false otherwise.
     */
    inline bool HasLongName() { return !m_LongName.empty(); }

    /**
     * @brief Check of the option accepts more arguments.
     * @return true if accepts more arguments, false, otherwise.
     */
    inline bool AcceptsArgs() { return m_Args.size() < m_NumArgs; }

    /**
   * @brief Check of the option needs more required arguments.
   * @return true if needs more arguments, false, otherwise.
   */
    inline bool NeedsArgs() { return m_Args.size() < m_NumRequiredArgs; }

    /**
     * @brief Check if the option is required or optional.
     * @return true if required, false otherwise.
     */
    inline bool IsRequired() { return m_IsRequired; }
    COption& Required(bool required) { m_IsRequired = required; return *this; }
    
    /**
     * @brief Check of the option has arguments.
     * @return True if has arguments, false otherwise.
     */
    inline bool HasArgs() { return m_NumArgs > 0 || m_NumRequiredArgs > 0; }


    //class Builder
    //{
    //private:
    //    COption_basic m_opt;
    //public:
    //    Builder() { }

    //    Builder(const std::string& shortName)
    //    {
    //        m_opt.m_ShortName = shortName;
    //    }

    //    Builder& longName(const std::string& longName)   { m_opt.m_LongName    = longName; return *this; }
    //    Builder& shortName(const std::string& shortName) { m_opt.m_ShortName   = shortName;return *this; }
    //    Builder& desc(const std::string& desc)           { m_opt.m_Description = desc;     return *this; }
    //    Builder& required(bool required)                 { m_opt.m_IsRequired  = required; return *this; }
    //    
    //    /**
    //    * @brief Specifies that option has exactly one argument.
    //     * @return Reference to itself (class COption::Builder).
    //    */
    //    Builder& hasArg() { m_opt.m_NumRequiredArgs = m_opt.m_NumArgs = 1; return *this; }

    //    /**
    //    * @brief Specifies that option has at least one argument.
    //    * Number of arguments may vary from 1 to 65535
    //     * @return Reference to itself (class COption::Builder).
    //    */
    //    Builder& hasArgs() { m_opt.m_NumRequiredArgs = 1; m_opt.m_NumArgs = UINT16_MAX; return *this; }

    //    /**
    //    * @brief Sets maximum number of allowed arguments for the option.
    //    * Use requiredArgs() method to specify number of required arguments. Should less or equal to numOfArgs() 
    //     * @return Reference to itself (class COption::Builder).
    //    */
    //    Builder& numOfArgs(uint16_t num)          
    //    { 
    //        m_opt.m_NumArgs  = num;
    //        if (m_opt.m_NumRequiredArgs > m_opt.m_NumArgs)
    //            m_opt.m_NumRequiredArgs = m_opt.m_NumArgs;

    //        return *this; 
    //    }

    //    /**
    //    * @brief Sets number of required arguments for the option.
    //    * You can use numOfArgs() method to specify max number of arguments allowed for the option. 
    //    * @return Reference to itself (class COption::Builder).
    //    */
    //    Builder& requiredArgs(uint16_t num)
    //    { 
    //        m_opt.m_NumRequiredArgs = num;
    //        if (m_opt.m_NumArgs < m_opt.m_NumRequiredArgs)
    //            m_opt.m_NumArgs = m_opt.m_NumRequiredArgs;

    //        return *this; 
    //    }
    //    
    //    /**
    //    * @brief Finished option parameters configuration and returns COption instance that can further be passed into COptionsList:: AddOption() method.
    //    * @return Reference to COption instance with all configured option parameters.
    //    */
    //    COption& build()
    //    {
    //        if (m_opt.m_ShortName.empty() && m_opt.m_LongName.empty())
    //            throw std::invalid_argument("It is not allowed to have both ShortName and LongName names empty for the option."); 
    //        else
    //            return *(new COption(m_opt));
    //    }

    //private:
    //    /* prevent copying and assignment */
    //    Builder(const Builder& other) = delete;
    //    Builder& operator=(const Builder& other) = delete;
    //    
    //}; // Builder

    
    /*COption(COption_basic& opt)
    {
        m_ShortName    = opt.m_ShortName;
        m_LongName     = opt.m_LongName;
        m_Description  = opt.m_Description;
        m_IsRequired   = opt.m_IsRequired;
        m_NumArgs      = opt.m_NumArgs;
        m_NumRequiredArgs = opt.m_NumRequiredArgs;
    }*/

    
    //static Builder& builder(const std::string& shortName) { return *(new Builder(shortName)); }
    //static Builder& builder() { return *(new Builder()); }

private:

    /** @brief Option short name (ex: -v). */
    std::string m_ShortName;

    /** @brief Option long name (ex: --version). */
    std::string m_LongName;

    /** @brief Human reafable option description. */
    std::string m_Description;

    /** @brief Is the option required or optional. */
    bool m_IsRequired;

    /** @brief Number of arguments. */
    uint16_t m_NumArgs;

    /** @brief Number of required arguments in m_NumArgs. */
    uint16_t m_NumRequiredArgs;

    /** @brief List where arguments are stored. */
    vector_string_t m_Args;

    /* prevent copying and assignment */
    //COption(const COption& other) = delete;
    COption& operator=(const COption& other) = delete;
 
};

/** @brief Vector that contains pointers to options. */
typedef std::vector<COption*> vector_option_pt;

/** @brief Vector that contains options. */
typedef std::vector<COption> vector_option_t;

