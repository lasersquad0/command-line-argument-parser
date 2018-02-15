/*
    Project includes
*/
#include "commons-cli\DefaultParser.h"
#include "commons-cli\HelpFormatter.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/
#include <stdio.h>

int main(int argc, char *argv[])
{
    CDeafultParser defaultParser;
    CCommandLine cmd;
    COptions options;

    options.AddOption
    (
        "i",
        "input",
        "input test",
        1,
        true
    );

    options.AddOption
    (
        "o",
        "output",
        "output test",
        1,
        true
    );

    options.AddOption
    (
        "a",
        "algorithm",
        "algorithm test",
        0,
        false
    );

    if (!defaultParser.Parse(&options, &cmd, argv, argc))
    {
        printf(defaultParser.GetLastError().c_str());
        printf("\n");

        printf(CHelpFormatter::Format("test", &options).c_str());
        printf("\n");

        return 0;
    }

    std::string input = cmd.GetOptionValue("input", "");
    printf("-i: %s\n", input.c_str());

    std::string output = cmd.GetOptionValue("output", "");
    printf("-0: %s\n", output.c_str());

    return 0;
}