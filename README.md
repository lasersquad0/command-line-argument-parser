# Command line argument parser

Simple and convenient command line parser.

Command line for the parser looks like: "-i iarg1 iarg2 -o oarg1 oarg2 --algorithm".

Where '-i', '-o', '--algorithm' are options. Options must start from '-' (for short options) or from '--' for long options.

Short option name contains only one letter (e.g. '-i'). Log option name can contain any number of letters (e.g. '--input').

After the option there may follow several option arguments e.g. '-i iarg1 iarg2'. Option '-i' has two arguments.

There may be options without arguments at all e.g. '-i -o oarg'. Option '-i' here does not contain any arguments while option '-o' does contain one argument 'oarg'.

More examples can be found in section Usage below.

### Usage
```

#include <DefaultParser.h>
#include <HelpFormatter.h>

// command line: -i iarg1 iarg2 -o oarg1 oarg2 --algorithm 
int main(int argc, char *argv[])
{
    CDefaultParser defaultParser;
    COptionsList options;
    CCommandLine cmd;

    // define list of all options supported by application
    options.AddOption("i", "input", "input test", 2, true);
    options.AddOption("o", "output", "output test", 2, false);
    
    //another way to define option
    COption opt;
    opt.ShortName("a").LongName("algorithm").Descr("algorithm test").NumArgs(0).RequiredArgs(0)
                   .Required(false);
    options.AddOption(opt);

    // defining mutually exclusive options
    options["i"]->Excludes(options["o"]); // options i and o are mutually exclusive
    // another way to define mutually exclusive options
    options.MutuallyExclusive("output", "a"); // it's ok to mix short and long options here

    if (!defaultParser.Parse(&options, &cmd, argv, argc))
    {
        printf("%s\n", defaultParser.GetLastError().c_str());
        printf("%s\n", CHelpFormatter::Format("test", &options).c_str());
        return 0;
    }

    // work with options with 1 and more arguments. Argument indexes start from 0.
    std::string input = cmd.GetOptionValue("i", 1, "second arg"); // look for option by short name with index 1
    printf("-i <arg 2>: %s\n\n", input.c_str()); // iarg2, second argument has index 1

    input = cmd.GetOptionValue("input", 0, "first arg"); // look for option by long name wtih index 0
    printf("--input <arg 1>: %s\n", input.c_str()); // iarg1


    // option names are CASE SENSITIVE, the default value "second arg default" will be returned
    // because there is no 'I' option in the 'options' list
    input = cmd.GetOptionValue("I", 1, "second arg default"); 
    printf("-I <arg 2>: %s\n\n", input.c_str()); // "second arg default"

    std::string output = cmd.GetOptionValue("o", 0, "first arg");
    printf("-o <arg 1>: %s\n", output.c_str()); // oarg1

    output = cmd.GetOptionValue("output", 1, "second arg");
    printf("--output <arg 2>: %s\n\n", output.c_str()); // oarg2

    // work with options without arguments
    bool hasOption = cmd.HasOption("algorithm"); // look for option by long name
    printf("--algorithm: %s\n", hasOption ? "true" : "false"); // true

    hasOption = cmd.HasOption("a"); // look for option by short name
    printf("-a: %s\n", hasOption ? "true" : "false"); // true
    
    // all options arguments can be returned all together as vector.
    // it might be more convenient in some cases. 
    vector_string_t arguments = cmd.GetOptionValues("i");

    return 0;
}
```

### Output
```
-i <arg 2>: iarg2
--input <arg 1>: iarg1
-I <arg 2>: second arg default
-o <arg 1>: oarg1
--output <arg 2>: oarg2
--algorithm: true
-a: true
```
