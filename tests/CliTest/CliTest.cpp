
#include "CppUnitTest.h"
#include "CommandLine.h"
#include "OptionsList.h"
#include "DefaultParser.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CliTest
{
#define TEST_GROUP_EMPTY() TEST_METHOD_ATTRIBUTE(L"Group", L"empty input")
#define TEST_GROUP_SIMPLE() TEST_METHOD_ATTRIBUTE(L"Group", L"simple")

	TEST_CLASS(CliTest)
	{
	public:
		// case with empty command line (no any option is the command line)
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_EmptyCmd)
			TEST_GROUP_EMPTY()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse_EmptyCmd)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt; 

			vector_string_t args;

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because empty args list
			Assert::AreEqual(_T("Empty list of options in command line"), p.GetLastError().c_str());
		}

		// case when 'tt' is not an option. options should start from '-' or '--'
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_NotAnOption)
			TEST_GROUP_EMPTY()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse_NotAnOption)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			vector_string_t args;
			args.push_back(_T("tt"));

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); 
			Assert::AreEqual(_T("Unknown argument in command line: tt"), p.GetLastError().c_str());
		}

		// case with unknown option in cmd
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_UnknownShortOption)
			TEST_GROUP_EMPTY()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse_UnknownShortOption)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			vector_string_t args;
			args.push_back(_T("-t"));

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' is unknown option.
			Assert::AreEqual(_T("Unknown option has been met: -t"), p.GetLastError().c_str());
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_UnknownLongOption)
			TEST_GROUP_EMPTY()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse_UnknownLongOption)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			vector_string_t args;
			args.push_back("--pp");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' is unknown option.
			Assert::AreEqual("Unknown option has been met: --pp", p.GetLastError().c_str());
		}

		// correct case with known option '-t'
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_EmptyLongOption)
			TEST_GROUP_SIMPLE()
			END_TEST_METHOD_ATTRIBUTE()
			TEST_METHOD(TestParse_EmptyLongOption)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 0, false);

			vector_string_t args;
			args.push_back("-t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args)); // should return true because '-t' is known option here.
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_EmptyOptionInCmd)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse_EmptyOptionInCmd)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 0, false);

			vector_string_t args;
			args.push_back("");

			Assert::IsTrue(p.Parse(&opt, &cmd, args)); 
			//Assert::AreEqual("Unknown argument in command line: ", p.GetLastError().c_str());
		}

		// case when required option '-t' have not been found in empty cmd 
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_RequiredOption)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse_RequiredOption)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 0, true);

			vector_string_t args;
			args.push_back("p");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' is required but it absent in cmd line.
			Assert::AreEqual("Unknown argument in command line: p", p.GetLastError().c_str());
		}

		// case when required option '-t' have not been found in cmd while other registered options are present in cmd
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_RequiredOption2)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse_RequiredOption2)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 0, true);
			opt.AddOption("p", "", "descr", 0);

			vector_string_t args;
			args.push_back("-p");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' is required but it absent in cmd line.
			Assert::AreEqual("Missing required option: -t", p.GetLastError().c_str());
		}

		//case with missing argument for short option '-t'
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_MissingArgument)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse_MissingArgument)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, true);

			vector_string_t args;
			args.push_back("-t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' requires 1 argument that is not present in cmd.
			Assert::AreEqual("Missing arguments for option: -t", p.GetLastError().c_str());
		}

		//checks case with missing arguments for long option '--tt'
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_MissingArgument2)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse_MissingArgument2)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("", "tt", "descr", 1, false);

			vector_string_t args;
			args.push_back("--tt");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '--tt' requires 1 argument that is not present in cmd.
			Assert::AreEqual("Missing arguments for option: --tt", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse_EmptyShortName)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("", "tt", "descr", 1, false);

			vector_string_t args;
			args.push_back(""); // empty aguments are ignored
			args.push_back("");

			Assert::IsTrue(p.Parse(&opt, &cmd, args)); 
		//	Assert::AreEqual("Unknown argument in command line: ", p.GetLastError().c_str());
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse_HasArgument)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse_HasArgument)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("", "tt", "descr", 1, false);

			vector_string_t args;
			args.push_back("--tt");
			args.push_back("arg_for_tt");

			Assert::IsTrue(p.Parse(&opt, &cmd, args)); // should return true because '--tt' requires 1 argument and 1 arg is present in cmd.
			//Assert::AreEqual("Missing arguments for option: tt", p.GetLastError().c_str());
		}

		//correct case with 1 option with 1 argument which is present in cmd
		TEST_METHOD(TestParse_HasArgument2)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args)); // should return true because '-t' requires 1 argument that is present in cmd.
		}

		// case with excessive arguments provided in cmd for option '-t' (requires 1 but provided 2)
		TEST_METHOD(TestParse_TooManyArguments)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("arg_for_t2");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' requires 1 argument but 2 arguments actually present in cmd.
			Assert::AreEqual("Too many arguments for option: -t", p.GetLastError().c_str());
		}

		// case with excessive arguments provided in cmd for option '-t' (requires 1 but provided 10)
		TEST_METHOD(TestParse_TooManyArguments2)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("arg_for_t");
			args.push_back("arg_for_t3");
			args.push_back("arg_for_t4");
			args.push_back("arg_for_t5");
			args.push_back("arg_for_t6");
			args.push_back("arg_for_t7");
			args.push_back("arg_for_t8");
			args.push_back("arg_for_t9");
			args.push_back("arg_for_t10");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' requires 1 argument but 10 arguments actually present in cmd.
			Assert::AreEqual("Too many arguments for option: -t", p.GetLastError().c_str());
		}

		// case with two options with both empty long names 
		// test added for bug for incorrect comparison long option names in case both long names were empty strings 
		TEST_METHOD(TestParse_EmptyLongNames)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 1, true);
			
			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because required option '-d' is not present in cmd.
			Assert::AreEqual("Missing required option: -d", p.GetLastError().c_str());
		}

		
		TEST_METHOD(TestParse_MissingReqOpt)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "tt", "descrT", 1, false);
			opt.AddOption("d", "dd", "descrD", 0, true);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because required option '-d' is not present in cmd.
			Assert::AreEqual("Missing required option: -d", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse_HasReqOpt)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descrT", 1, false);
			opt.AddOption("d", "", "descrD", 0, true);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("-d");

			Assert::IsTrue(p.Parse(&opt, &cmd, args)); // should return true because required option '-d' is present in cmd.
			
		}

		// case when argument for required option '-d' is missing
		TEST_METHOD(TestParse_MissingArgument3)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 1, true);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("-d");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because no argument for option '-d' is not present in cmd.
			Assert::AreEqual("Missing arguments for option: -d", p.GetLastError().c_str());
		}

		// correct case with two options (one of them is required) and with 1 argument for each option as defined.
		TEST_METHOD(TestParse13)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 1, true);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("-d");
			args.push_back("arg_for_d");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
		}

		// case with two options with both empty long names 
		// test added for bug for incorrect comparison long option names in case both long names were empty strings 
		TEST_METHOD(TestParse14)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("d", "", "descr", 1, false);
			opt.AddOption("t", "", "descr", 1, true);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Missing arguments for option: t", p.GetLastError().c_str());
		}

		// case with two options with the sames name
		// second equal option overrides first one
		TEST_METHOD(TestParse_OverwriteOption)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("d", "Duuo", "descr", 1, false);
			Assert::AreEqual(cli_string("Duuo"), opt["d"]->GetLongName());
			Assert::AreEqual(cli_string("descr"), opt["d"]->GetDescription());
			Assert::AreEqual((uint16_t)1, opt["d"]->GetNumArgs());
			Assert::AreEqual(false, opt["d"]->IsRequired());

			opt.AddOption("t", "", "descr", 1, true);

			opt.AddOption("d", "", "descr2", 0, true); // overwrites previous AddOption("d"...
			Assert::AreEqual(cli_string(""), opt["d"]->GetLongName());
			Assert::AreEqual(cli_string("descr2"), opt["d"]->GetDescription());
			Assert::AreEqual((uint16_t)0, opt["d"]->GetNumArgs());
			Assert::AreEqual(true, opt["d"]->IsRequired());

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing required option: -d", p.GetLastError().c_str());
		}

		// case with two options with the sames name
		// second equal option overrides first one
		TEST_METHOD(TestParse_OverwriteOption2)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("d", "", "descr", 1, false);
			Assert::AreEqual(cli_string("d"), opt["d"]->GetShortName());
			Assert::AreEqual(cli_string(""), opt["d"]->GetLongName());
			Assert::AreEqual(cli_string("descr"), opt["d"]->GetDescription());
			Assert::AreEqual((uint16_t)1, opt["d"]->GetNumArgs());
			Assert::AreEqual(false, opt["d"]->IsRequired());

			opt.AddOption("t", "", "text", 1, true);
			
			opt.AddOption("d", "Dub", "descr2", 0, true);
			Assert::AreEqual(cli_string("d"), opt["d"]->GetShortName());
			Assert::AreEqual(cli_string("Dub"), opt["d"]->GetLongName());
			Assert::AreEqual(cli_string("descr2"), opt["d"]->GetDescription());
			Assert::AreEqual((uint16_t)0, opt["d"]->GetNumArgs());
			Assert::AreEqual(true, opt["d"]->IsRequired());

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("-d");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
		}

		// case with two options with both non-empty long names 
		// test added for bug for incorrect comparison long option names in case both long names were empty strings 
		TEST_METHOD(TestParse15)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("d", "dd", "descr", 1, false);
			opt.AddOption("t", "tt", "descr", 1, true);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
		}

		// two option with the same Long names, but different short ones
		TEST_METHOD(TestParse_EqualLongNames)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "tt", "descr1", 0, false);
			opt.AddOption("f", "tt", "descr2", 1, true);

			vector_string_t args;
			args.push_back("--tt");
			args.push_back("arg_for_t"); // it finds first registered option by --tt that has no arguments

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Unknown argument in command line: arg_for_t", p.GetLastError().c_str());
		}

		// Short option withg length >1
		TEST_METHOD(TestParse_LongShortOpt)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "total", "descr1", 1, false);
			opt.AddOption("todo", "tutu", "descr2", 0);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
		}

		// short option passed in cmd like a long one
		TEST_METHOD(TestParse16)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "total", "descr1", 1, false);
			
			vector_string_t args;
			args.push_back("--t");
			args.push_back("arg_for_t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Unknown option has been met: --t", p.GetLastError().c_str());
		}

		// two option with the same names, second option overrides the first one
		TEST_METHOD(TestParse16_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "tt", "descr", 1, false);
			opt.AddOption("t", "ttt", "descr", 1, true);

			vector_string_t args;
			args.push_back("--ttt");
			args.push_back("arg_for_t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));

			args.clear();
			args.push_back("--tt");
			args.push_back("arg_for_t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Unknown option has been met: --tt", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse_NotEnoughArgs)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 2, false);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("-d");
			args.push_back("arg_for_d");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing arguments for option: -d", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse18)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 2, true);

			vector_string_t args;
			args.push_back("");
			args.push_back("-d");
			args.push_back("arg_for_d");
			args.push_back("");
			args.push_back("-t");
			args.push_back("arg_for_t_1");
			args.push_back("arg_for_t_2");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Too many arguments for option: -t", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse19)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 2, false);
			opt.AddOption("c", "", "descr", 1, false);

			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("-d");
			args.push_back("arg_for_d");
			args.push_back("-c");
			args.push_back("arg_for_c");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing arguments for option: -d", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse20)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
		
			vector_string_t args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("-t");
			args.push_back("arg_for_t2");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Duplicate option has been met: -t", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse21)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			// that's really two duplicate options. option2 shortname 'b' is the same as option1 long name 'b'
			opt.AddOption("a", "b", "descr", 1, false);
			opt.AddOption("b", "", "descr", 1, false);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_ab");
			args.push_back("-b");
			args.push_back("arg_for_b");
			//TODO think of this case whether opt a equals b here?
			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Duplicate option has been met: -b", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse21_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			// that's really two duplicate options. option2 shortname 'b' is the same as option1 long name 'b'
			opt.AddOption("b", "", "descr", 1, false);
			opt.AddOption("a", "b", "descr", 1, false);
			
			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_ab");
			args.push_back("-b");
			args.push_back("arg_for_b");
			//TODO think of this case whether opt a equals b here?
			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Duplicate option has been met: -b", p.GetLastError().c_str());
		}

		// incorrect parameter numArgs passed to AddOption() method
		TEST_METHOD(TestParse22)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("a", "b", "descr", -1);
			
			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_ab");
			args.push_back("arg_for_ab");
			args.push_back("arg_for_ab");
			args.push_back("arg_for_ab");
			args.push_back("arg_for_ab");
			args.push_back("arg_for_ab");
		
			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse23)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			Assert::IsTrue(opt.AddOption("a", "b", "descr", -1));

			vector_string_t args;
			args.push_back("-a");
			for (size_t i = 0; i < 65535; i++)
			{
				args.push_back("arg_for_ab");
			}

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Duplicate option has been met: -b", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse24)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			Assert::IsFalse(opt.AddOption("", "", "descr", 1)); // option is NOT added to the list
			Assert::AreEqual(0ull, opt.Count());

			vector_string_t args;
			args.push_back(""); // empty arguments are ignored during parsing
			
			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Unknown argument in command line: ", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse24_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			Assert::IsFalse(opt.AddOption("", "", "descr", 1));
			Assert::IsFalse(opt.AddOption("", "", "descr", 2));
			Assert::AreEqual(0ull, opt.Count());

			vector_string_t args;
			args.push_back("-t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Unknown option has been met: -t", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse25)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;
			o.ShortName("a");
			
			Assert::IsTrue(opt.AddOption(o));

			vector_string_t args;
			args.push_back("-a");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
		//	Assert::AreEqual("Unknown option has been met: -t", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse26)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;
			o.ShortName("a").NumArgs(2).RequiredArgs(1);
			Assert::AreEqual(cli_string("a"), o.GetShortName());
			Assert::AreEqual((uint16_t)2, o.GetNumArgs());
			Assert::AreEqual((uint16_t)1, o.GetNumRequiredArgs());

			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse27)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(2).requiredArgs(1).build();
			o.ShortName("a").NumArgs(2).RequiredArgs(1);
			Assert::AreEqual(cli_string("a"), o.GetShortName());
			Assert::IsTrue(o.GetLongName().empty());
			Assert::AreEqual((uint16_t)2, o.GetNumArgs());
			Assert::AreEqual((uint16_t)1, o.GetNumRequiredArgs());
			Assert::IsFalse(o.IsRequired());

			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//	Assert::AreEqual("Unknown option has been met: -t", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse28)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(2).requiredArgs(1).build();
			o.ShortName("a").NumArgs(2).RequiredArgs(1); 
			Assert::AreEqual(cli_string("a"), o.GetShortName());
			Assert::IsTrue(o.GetLongName().empty());
			Assert::AreEqual((uint16_t)2, o.GetNumArgs());
			Assert::AreEqual((uint16_t)1, o.GetNumRequiredArgs());
			Assert::IsFalse(o.IsRequired());

			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//	Assert::AreEqual("Unknown option has been met: -t", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse29)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(2).requiredArgs(1).build();
			o.ShortName("a").NumArgs(2).RequiredArgs(1);
			Assert::AreEqual(cli_string("a"), o.GetShortName());
			Assert::IsTrue(o.GetLongName().empty());
			Assert::AreEqual((uint16_t)2, o.GetNumArgs());
			Assert::AreEqual((uint16_t)1, o.GetNumRequiredArgs());
			Assert::IsFalse(o.IsRequired());

			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");
			args.push_back("arg_for_a_3");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Too many arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse30)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(2).requiredArgs(0).build();
			o.ShortName("a").NumArgs(2).RequiredArgs(0); 
			Assert::AreEqual(cli_string("a"), o.GetShortName());
			Assert::IsTrue(o.GetLongName().empty());
			Assert::AreEqual((uint16_t)2, o.GetNumArgs());
			Assert::AreEqual((uint16_t)0, o.GetNumRequiredArgs());
			Assert::IsFalse(o.IsRequired());
			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			
			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Too many arguments for: a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse31)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(2).requiredArgs(0).build();
			o.ShortName("a").NumArgs(2).RequiredArgs(0); 
			Assert::AreEqual(cli_string("a"), o.GetShortName());
			Assert::IsTrue(o.GetLongName().empty());
			Assert::AreEqual((uint16_t)2, o.GetNumArgs());
			Assert::AreEqual((uint16_t)0, o.GetNumRequiredArgs());
			Assert::IsFalse(o.IsRequired());

			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Too many arguments for: a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse32)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(2).requiredArgs(0).build();
			o.ShortName("a").NumArgs(2).RequiredArgs(0); 
			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Too many arguments for: a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse33)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(2).requiredArgs(0).build();
			o.ShortName("a").NumArgs(2).RequiredArgs(0); 
			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");
			args.push_back("arg_for_a_3");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Too many arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse34)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(1).requiredArgs(2).build();
			o.LongName("ab").NumArgs(1).RequiredArgs(2); 
			Assert::AreEqual(cli_string("ab"), o.GetLongName());
			Assert::IsTrue(o.GetShortName().empty());
			Assert::AreEqual((uint16_t)2, o.GetNumArgs());
			Assert::AreEqual((uint16_t)2, o.GetNumRequiredArgs());
			Assert::IsFalse(o.IsRequired());
			opt.AddOption(o);

			vector_string_t args;
			args.push_back("--ab");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing arguments for option: --ab", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse35)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(1).requiredArgs(2).build();
			o.LongName("automotovelofototeleradio").NumArgs(1).RequiredArgs(2); 
			Assert::AreEqual(cli_string("automotovelofototeleradio"), o.GetLongName());
			Assert::IsTrue(o.GetShortName().empty());
			Assert::AreEqual((uint16_t)2, o.GetNumArgs());
			Assert::AreEqual((uint16_t)2, o.GetNumRequiredArgs());
			Assert::IsFalse(o.IsRequired());
			opt.AddOption(o);

			vector_string_t args;
			args.push_back("--automotovelofototeleradio");
			args.push_back("arg_for_a_1");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing arguments for option: --automotovelofototeleradio", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse36)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(1).requiredArgs(2).build();
			o.ShortName("a").NumArgs(1).RequiredArgs(2); 
			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Too many arguments for: a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse37)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(1).requiredArgs(2).build();
			o.ShortName("a").NumArgs(1).RequiredArgs(2); 
			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");
			args.push_back("arg_for_a_3");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Too many arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse38)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").requiredArgs(2).numOfArgs(1).build();
			o.ShortName("a").RequiredArgs(2).NumArgs(1); 
			Assert::AreEqual(cli_string("a"), o.GetShortName());
			Assert::IsTrue(o.GetLongName().empty());
			Assert::AreEqual((uint16_t)1, o.GetNumArgs());
			Assert::AreEqual((uint16_t)1, o.GetNumRequiredArgs());
			Assert::IsFalse(o.IsRequired());

			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse39)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").requiredArgs(2).numOfArgs(1).build();
			o.ShortName("a").RequiredArgs(2).NumArgs(1);
			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Missing arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse40)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;
			o.ShortName("a").RequiredArgs(2).NumArgs(1); 
			opt.AddOption(o);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Too many arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse_Excludes1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption a;
			a.ShortName("a").NumArgs(1);
			opt.AddOption(a);

			COption b;
			b.ShortName("b");
			opt.AddOption(b);

			opt.MutuallyExclusive("a", "b");
			opt.MutuallyExclusive("a", "b");

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
		
			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Too many arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse_Excludes2)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption a;
			a.ShortName("a").NumArgs(1);
			opt.AddOption(a);

			COption b;
			b.ShortName("b").Excludes(opt["a"]); // important to use opt["a"] instead of a because opt.AddOption(a) creates a copy of a
			opt.AddOption(b);
			
			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a");
			args.push_back("-b");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("There are mutually exclusive options found: (a and b) (b and a) ", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse_Excludes3)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("a", "aa", "descr A", 1);
			opt.AddOption("b", "bb", "descr B", 0);
			opt.AddOption("c", "cc", "descr C", 0);

			opt["a"]->Excludes(opt["b"]);
			opt["c"]->Excludes(opt["a"], opt["b"]);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a");
			args.push_back("-b");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("There are mutually exclusive options found: (a and b) (b and a) ", p.GetLastError().c_str());

			vector_string_t args2;
			args2.push_back("-c");
			args2.push_back("-b");
			
			cmd.Clear(); opt.ResetOptions();
			Assert::IsFalse(p.Parse(&opt, &cmd, args2));
			Assert::AreEqual("There are mutually exclusive options found: (c and b) (b and c) ", p.GetLastError().c_str());

			vector_string_t args3;
			args3.push_back("-a");
			args3.push_back("arg_for_a");
			args3.push_back("-c");

			cmd.Clear(); opt.ResetOptions();
			Assert::IsFalse(p.Parse(&opt, &cmd, args3));
			Assert::AreEqual("There are mutually exclusive options found: (a and c) (c and a) ", p.GetLastError().c_str());

		}

		TEST_METHOD(TestParse_Excludes4)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("a", "aa", "descr A", 1);
			opt.AddOption("b", "bb", "descr B", 0);
			opt.AddOption("c", "cc", "descr C", 0);
			opt.AddOption("d", "dd", "descr D", 0);

			opt["a"]->Excludes(opt["b"], opt["b"], opt["a"], opt["c"], opt["d"]);
			opt["b"]->Excludes(opt["b"], opt["c"], opt["d"]);
			opt["c"]->Excludes(opt["d"]);

			vector_string_t args;
			args.push_back("-a");
			args.push_back("arg_for_a");
			args.push_back("-b");
			
			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("There are mutually exclusive options found: (a and b) (b and a) ", p.GetLastError().c_str());

			vector_string_t args1;
			args1.push_back("-b");
			args1.push_back("-c");

			cmd.Clear(); opt.ResetOptions();
			Assert::IsFalse(p.Parse(&opt, &cmd, args1));
			Assert::AreEqual("There are mutually exclusive options found: (b and c) (c and b) ", p.GetLastError().c_str());

			vector_string_t args2;
			args2.push_back("-c");
			args2.push_back("-d");

			cmd.Clear(); opt.ResetOptions();
			Assert::IsFalse(p.Parse(&opt, &cmd, args2));
			Assert::AreEqual("There are mutually exclusive options found: (c and d) (d and c) ", p.GetLastError().c_str());

			vector_string_t args3;
			args3.push_back("-a");
			args3.push_back("arg_for_a");
			args3.push_back("-b");
			args3.push_back("-c");

			cmd.Clear(); opt.ResetOptions();
			Assert::IsFalse(p.Parse(&opt, &cmd, args3));
			Assert::AreEqual("There are mutually exclusive options found: (a and b,c) (b and a,c) (c and a,b) ", p.GetLastError().c_str());

			vector_string_t args4;
			args4.push_back("-b");
			args4.push_back("-c");
			args4.push_back("-d");

			cmd.Clear(); opt.ResetOptions();
			Assert::IsFalse(p.Parse(&opt, &cmd, args4));
			Assert::AreEqual("There are mutually exclusive options found: (b and c,d) (c and b,d) (d and b,c) ", p.GetLastError().c_str());

			vector_string_t args5;
			args5.push_back("-a");
			args5.push_back("arg_for_a");
			args5.push_back("-b");
			args5.push_back("-c");
			args5.push_back("-d");

			cmd.Clear(); opt.ResetOptions();
			Assert::IsFalse(p.Parse(&opt, &cmd, args5));
			Assert::AreEqual("There are mutually exclusive options found: (a and b,c,d) (b and a,c,d) (c and a,b,d) (d and a,b,c) ", p.GetLastError().c_str());

		}
	};
}




