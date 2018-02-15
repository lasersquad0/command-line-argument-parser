# Command line argument parser

A basic implementation in C++ of the apache-commons-cli

### Usage
```
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
printf("-o: %s\n", output.c_str());
```

### Output
```
Missing arguments for options: i, o
test usage:
-i, --input     <arg> input test
-o, --output    <arg> output test
-a, --algorithm       algorithm test
```
