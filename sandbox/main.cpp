//
// Project includes
//
#include <DefaultParser.h>
#include <HelpFormatter.h>

//
// Libs include
//

//
// STDLib/OS includes
//
#include <cstdio>

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    CDefaultParser defaultParser;
    COptionsList options;
    CCommandLine cmd;

    options.AddOption("i", "input", "input test", 2, true);
    options.AddOption("o", "output", "output test", 2, false);
    options.AddOption("a", "algorithm", "algorithm test", 0, false);

    if (!defaultParser.Parse(&options, &cmd, argv, argc))
    {
        printf("%s\n", defaultParser.GetLastError().c_str());
        printf("%s\n", CHelpFormatter::Format("test", &options).c_str());
        return 0;
    }

    {
        std::string input = cmd.GetOptionValue("input", 0, "first arg");
        printf("-i <arg 1>: %s\n", input.c_str());

        input = cmd.GetOptionValue("input", 1, "second arg");
        printf("-i <arg 2>: %s\n\n", input.c_str());
    }

    {
        std::string output = cmd.GetOptionValue("output", 0, "first arg");
        printf("-O <arg 1>: %s\n", output.c_str());

        output = cmd.GetOptionValue("output", 1, "second arg");
        printf("-O <arg 2>: %s\n\n", output.c_str());
    }

    {
        bool hasOption = cmd.HasOption("algorithm");
        printf("-a: %s\n", hasOption ? "true" : "false");
    }

    return 0;
}
