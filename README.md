# Command line argument parser

A basic implementation in C++ of the apache-commons-cli

### Usage
```
CDefaultParser defaultParser;
COptionsList options;
CCommandLine cmd;

options.AddOption("i", "input", "input test", 2, true);
options.AddOption("o", "output", "output test", 2, false);

COption opt;
opt.ShortName("a").LongName("algorithm").Descr("algorithm test").NumArgs(0).RequiredArgs(0).Required(false);
options.AddOption(opt);


if (!defaultParser.Parse(&options, &cmd, argv, argc))
{
    printf("%s\n", defaultParser.GetLastError().c_str());
    printf("%s\n", CHelpFormatter::Format("test", &options).c_str());
    return 0;
}

//work with options with 1 and more arguments
std::string input = cmd.GetOptionValue("i", 1, "second arg"); // looking for option by short name and index 1
printf("-i <arg 2>: %s\n\n", input.c_str());

input = cmd.GetOptionValue("input", 0, "first arg"); // looking for option by long name and index 0
printf("--input <arg 1>: %s\n", input.c_str());


//option names are CASE SENSITIVE, the default value "second arg" will be returned because there is no 'I' option in the 'options' list
input = cmd.GetOptionValue("I", 1, "second arg default"); 
printf("-I <arg 2>: %s\n\n", input.c_str());


std::string output = cmd.GetOptionValue("o", 0, "first arg");
printf("-o <arg 1>: %s\n", output.c_str());

output = cmd.GetOptionValue("output", 1, "second arg");
printf("--output <arg 2>: %s\n\n", output.c_str());


// work with options without arguments
bool hasOption = cmd.HasOption("algorithm"); // look for option by long name
printf("--algorithm: %s\n", hasOption ? "true" : "false");

hasOption = cmd.HasOption("a"); // loog for option by short name
printf("-a: %s\n", hasOption ? "true" : "false");


// all options arguments can be returned all together as vector. it might be more convenient in some cases. 
vector_string_t arguments = cmd.GetOptionValues("i");

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
