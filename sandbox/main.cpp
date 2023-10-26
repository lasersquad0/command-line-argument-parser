
#include <cstdio>
#include <string>
#include <DefaultParser.h>
#include <HelpFormatter.h>

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

    //work with options with 1 and more arguments
    std::string input = cmd.GetOptionValue("input", 0, "first arg"); // looking for option by long name
    printf("-i <arg 1>: %s\n", input.c_str());

    input = cmd.GetOptionValue("i", 1, "second arg"); // looking for option by short name
    printf("-i <arg 2>: %s\n\n", input.c_str());
    
    //option names are CASE SENSITIVE, the default value "second arg" will be returned because there is no 'I' option in the 'options' list
    input = cmd.GetOptionValue("I", 1, "second arg"); 
    printf("-I <arg 2>: %s\n\n", input.c_str());


    std::string output = cmd.GetOptionValue("o", 0, "first arg");
    printf("-o <arg 1>: %s\n", output.c_str());

    output = cmd.GetOptionValue("output", 1, "second arg");
    printf("-o <arg 2>: %s\n\n", output.c_str());


    // work with options without arguments
    bool hasOption = cmd.HasOption("algorithm"); // look for option by long name
    printf("-a: %s\n", hasOption ? "true" : "false");

    bool hasOption = cmd.HasOption("a"); // loog for option by short name
    printf("-a: %s\n", hasOption ? "true" : "false");
    

    // all options argumants can be returned all together as vector. it might be more convenient in some cases. 
    vector_string_t& arguments = cmd.GetOptionValues("i");

    return 0;
}
