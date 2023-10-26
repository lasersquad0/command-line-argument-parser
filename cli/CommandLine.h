#pragma once
//
// Project includes
//
#include "Option.h"

//
// Libs include
//

//
// STDLib/OS includes
//
#include <vector>

///////////////////////////////////////////////////////////////////////////////

/**
 * @brief This contains a list of the parsed options. To check if a option
 * has been parsed use the "HasOption" method. To retrieve an option
 * argument use the "GetOptionValue".
 *
 * @usage A small example of the usage (there are no error checks).
 *
 *  CDefaultParser parser;  // parser used to parse the cli arguments
 *  COptionsList options;   // list of cli arguments accepted
 *  CCommandLine cmd;       // store the cli parsed arguments
 *
 *  parser.parse(options, cmd, argv, argc);
 *
 *  const char *present = cmd.HasOption("foo") ? "true" : "false";
 *  printf("Is 'foo' present: %s\n", preset");
 *
 *  std::string barArg0 = cmd.GetOptionValue("bar", 0, "default-value");
 *  printf("'bar' argument 0 is: %s\n", barArg0.c_str());
 */
class CCommandLine
{
public:
    /**
     * @brief Clean the stored option.
     */
    void Clear();

    /**
     * @brief Stores a parsed option. If the option is already stored the
     * function will do nothing and return false (it is stored as long as there is another
     * option with the same short or long name).
     *
     * @param option A valid pointer to the option to store.
     * @return true if an option is successfully added, false otherwise.
     */
    bool AddOption(COption *option);

    /**
     * @brief Retrieve all the stored option.
     * @return A const reference vector to the stored options.
     */
    inline const vector_option_pt &GetOptions() { return m_Options; }

    /**
     * @brief Check if a given option has been parsed.
     *
     * @param opt The option name (short or long).
     * @return true on success, false otherwise.
     */
    bool HasOption(const std::string &opt);

    /**
     * @brief Retrieves one of the option arguments. If the option is not found
     * or the index of the argument is out of range, the default value will be
     * returned.
     *
     * @param opt The option name (short or long).
     * @param index The option argument index (starts with zero).
     * @param defaultValue The default value.
     * @return The option argument value.
     */
    std::string GetOptionValue(const std::string &opt, size_t index = 0, const std::string &defaultValue = "");

    /**
     * @brief Retrieves all option arguments as std::vector<string>. If the option is not found
     * or the index of the argument is out of range, an empty vector will be
     * returned.
     *
     * @param opt The option name (short or long).
     * @return List of option arguments.
     */
    const vector_string_t& GetOptionValues(const std::string& opt);

    /**
     * @brief Check if a there are options which arguments have not been found in command line.
     *
     * @return true if there are such options, false otherwise.
     */
    bool CheckMissingArguments();


    /**
     * @brief Returns list of options which arguments have not been found in command line.
     *
     * @return std::vector with list of options which arguments have not been found in command line.
     */
    vector_option_pt GetOptionsWithMissingArguments();

private:
    /** @brief List of parsed options. */
    vector_option_pt m_Options;
};
