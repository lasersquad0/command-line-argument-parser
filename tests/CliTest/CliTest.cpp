//#include "pch.h"
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
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse0)
			TEST_GROUP_EMPTY()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse0)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt; 

			std::vector<std::string> args;

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because empty args list
			Assert::AreEqual("Empty list of options in command line", p.GetLastError().c_str());
		}

		// case when 'tt' is not an option. options should start from '-' or '--'
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse1)
			TEST_GROUP_EMPTY()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			std::vector<std::string> args;
			args.push_back("tt");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); 
			Assert::AreEqual("Unknown argument in command line: tt", p.GetLastError().c_str());
		}

		// case with unknown option in cmd
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse2)
			TEST_GROUP_EMPTY()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse2)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			std::vector<std::string> args;
			args.push_back("-t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' is unknown option.
			Assert::AreEqual("Unknown option has been met: -t", p.GetLastError().c_str());
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse2_1)
			TEST_GROUP_EMPTY()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse2_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			std::vector<std::string> args;
			args.push_back("--pp");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' is unknown option.
			Assert::AreEqual("Unknown option has been met: --pp", p.GetLastError().c_str());
		}

		// correct case with known option '-t'
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse3)
			TEST_GROUP_SIMPLE()
			END_TEST_METHOD_ATTRIBUTE()
			TEST_METHOD(TestParse3)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 0, false);

			std::vector<std::string> args;
			args.push_back("-t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args)); // should return true because '-t' is known option here.
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse3_1)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse3_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 0, false);

			std::vector<std::string> args;
			args.push_back("");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return true because '-t' is known option here.
			Assert::AreEqual("Unknown argument in command line: ", p.GetLastError().c_str());
		}

		// case when required option '-t' have not been found in empty cmd 
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse4)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse4)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 0, true);

			std::vector<std::string> args;
			args.push_back("p");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' is required but it absent in cmd line.
			Assert::AreEqual("Unknown argument in command line: p", p.GetLastError().c_str());
		}

		// case when required option '-t' have not been found in cmd while other options present in cmd
		// it detects unknown option '-p' before check for missing '-t'
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse4_1)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse4_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 0, true);

			std::vector<std::string> args;
			args.push_back("-p");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' is required but it absent in cmd line.
			Assert::AreEqual("Unknown option has been met: -p", p.GetLastError().c_str());
		}

		//case with missing argument for short option '-t'
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse5)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse5)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, true);

			std::vector<std::string> args;
			args.push_back("-t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' requires 1 argument that is not present in cmd.
			Assert::AreEqual("Missing arguments for option: -t", p.GetLastError().c_str());
		}

		//checks case with missing arguments for long option '--tt'
		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse6)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse6)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("", "tt", "descr", 1, false);

			std::vector<std::string> args;
			args.push_back("--tt");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '--tt' requires 1 argument that is not present in cmd.
			Assert::AreEqual("Missing arguments for option: --tt", p.GetLastError().c_str());
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(TestParse6_1)
			TEST_GROUP_SIMPLE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TestParse6_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("", "tt", "descr", 1, false);

			std::vector<std::string> args;
			args.push_back("--tt");
			args.push_back("arg_for_tt");

			Assert::IsTrue(p.Parse(&opt, &cmd, args)); // should return true because '--tt' requires 1 argument and 1 arg is present in cmd.
			//Assert::AreEqual("Missing arguments for option: tt", p.GetLastError().c_str());
		}

		//correct case with 1 option with 1 argument which is present in cmd
		TEST_METHOD(TestParse7)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);

			std::vector<std::string> args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args)); // should return true because '-t' requires 1 argument that is present in cmd.
		}

		// case with excessive arguments provided in cmd for option '-t' (requires 1 but provided 2)
		TEST_METHOD(TestParse8)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);

			std::vector<std::string> args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("arg_for_t2");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because '-t' requires 1 argument but 2 arguments actually present in cmd.
			Assert::AreEqual("Too many arguments for: t", p.GetLastError().c_str());
		}

		// case with excessive arguments provided in cmd for option '-t' (requires 1 but provided 10)
		TEST_METHOD(TestParse8_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);

			std::vector<std::string> args;
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
			Assert::AreEqual("Too many arguments for: t", p.GetLastError().c_str());
		}

		// case with two options with both empty long names 
		// test added for bug for incorrect comparison long option names in case both long names were empty strings 
		TEST_METHOD(TestParse9)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 1, true);
			
			std::vector<std::string> args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because required option '-d' is not present in cmd.
			Assert::AreEqual("Missing required option: -d", p.GetLastError().c_str());

		}

		
		TEST_METHOD(TestParse10)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "tt", "descr", 1, false);
			opt.AddOption("d", "dd", "descr", 0, true);

			std::vector<std::string> args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args)); // should return false because required option '-d' is not present in cmd.
			Assert::AreEqual("Missing required option: -d", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse11)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 0, true);

			std::vector<std::string> args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("-d");

			Assert::IsTrue(p.Parse(&opt, &cmd, args)); // should return true because required option '-d' is present in cmd.
			
		}

		// case when argument for required option '-d' is missing
		TEST_METHOD(TestParse12)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 1, true);

			std::vector<std::string> args;
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

			std::vector<std::string> args;
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

			std::vector<std::string> args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Missing arguments for option: t", p.GetLastError().c_str());
		}

		// case with two options with the sames name
		// second equal option overrides first one
		TEST_METHOD(TestParse14_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("d", "", "descr", 1, false);
			opt.AddOption("t", "", "descr", 1, true);
			opt.AddOption("d", "", "descr2", 0, true); // overwrites previous AddOption("d"...

			std::vector<std::string> args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing required option: -d", p.GetLastError().c_str());
		}

		// case with two options with the sames name
		// second equal option overrides first one
		TEST_METHOD(TestParse14_2)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("d", "", "descr", 1, false);
			opt.AddOption("t", "", "descr", 1, true);
			opt.AddOption("d", "", "descr2", 0, true);

			std::vector<std::string> args;
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

			std::vector<std::string> args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
			//Assert::AreEqual("Missing arguments for option: t", p.GetLastError().c_str());
		}

		// two option with the same names, secon option overrides the first one
		TEST_METHOD(TestParse16)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "tt", "descr", 1, false);
			opt.AddOption("t", "tt", "descr", 1, true);

			std::vector<std::string> args;
			args.push_back("-t");
			args.push_back("arg_for_t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
		}

		// two option with the same names, second option overrides the first one
		TEST_METHOD(TestParse16_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "tt", "descr", 1, false);
			opt.AddOption("t", "ttt", "descr", 1, true);

			std::vector<std::string> args;
			args.push_back("--ttt");
			args.push_back("arg_for_t");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));

			args.clear();
			args.push_back("--tt");
			args.push_back("arg_for_t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Unknown option has been met: --tt", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse17)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 2, false);

			std::vector<std::string> args;
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

			std::vector<std::string> args;
			args.push_back("-t");
			args.push_back("arg_for_t");
			args.push_back("-d");
			args.push_back("arg_for_d");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing arguments for option: -d", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse19)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("t", "", "descr", 1, false);
			opt.AddOption("d", "", "descr", 2, false);
			opt.AddOption("c", "", "descr", 1, false);

			std::vector<std::string> args;
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
		
			std::vector<std::string> args;
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

			std::vector<std::string> args;
			args.push_back("-a");
			args.push_back("arg_for_ab");
			args.push_back("-b");
			args.push_back("arg_for_b");

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
			
			std::vector<std::string> args;
			args.push_back("-a");
			args.push_back("arg_for_ab");
			args.push_back("-b");
			args.push_back("arg_for_b");

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
			
			std::vector<std::string> args;
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

			opt.AddOption("a", "b", "descr", -1);

			std::vector<std::string> args;
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

			opt.AddOption("", "", "descr", 1);

			std::vector<std::string> args;
			args.push_back("");
			
			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Unknown argument in command line: ", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse24_1)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			opt.AddOption("", "", "descr", 1);
			opt.AddOption("", "", "descr", 2);

			std::vector<std::string> args;
			args.push_back("-t");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Unknown option has been met: -t", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse25)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o; // std::string("a"), std::string(""), ;
			o.ShortName(std::string("a"));
			
			opt.AddOption(o);

			std::vector<std::string> args;
			args.push_back("-a");

			Assert::IsTrue(p.Parse(&opt, &cmd, args));
		//	Assert::AreEqual("Unknown option has been met: -t", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse26)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(2).requiredArgs(1).build();
			o.ShortName("a").NumArgs(2).RequiredArgs(1);
			opt.AddOption(o);

			std::vector<std::string> args;
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
			opt.AddOption(o);

			std::vector<std::string> args;
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
			opt.AddOption(o);

			std::vector<std::string> args;
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
			opt.AddOption(o);

			std::vector<std::string> args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");
			args.push_back("arg_for_a_3");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Too many arguments for: a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse30)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(2).requiredArgs(0).build();
			o.ShortName("a").NumArgs(2).RequiredArgs(0); 
			opt.AddOption(o);

			std::vector<std::string> args;
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
			opt.AddOption(o);

			std::vector<std::string> args;
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

			std::vector<std::string> args;
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

			std::vector<std::string> args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");
			args.push_back("arg_for_a_3");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Too many arguments for: a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse34)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(1).requiredArgs(2).build();
			o.ShortName("a").NumArgs(1).RequiredArgs(2); 
			opt.AddOption(o);

			std::vector<std::string> args;
			args.push_back("-a");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse35)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(1).requiredArgs(2).build();
			o.ShortName("a").NumArgs(1).RequiredArgs(2); 
			opt.AddOption(o);

			std::vector<std::string> args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Missing arguments for option: -a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse36)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").numOfArgs(1).requiredArgs(2).build();
			o.ShortName("a").NumArgs(1).RequiredArgs(2); 
			opt.AddOption(o);

			std::vector<std::string> args;
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

			std::vector<std::string> args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");
			args.push_back("arg_for_a_3");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Too many arguments for: a", p.GetLastError().c_str());
		}

		TEST_METHOD(TestParse38)
		{
			CDefaultParser p;
			CCommandLine cmd;
			COptionsList opt;

			COption o;// = COption::builder("a").requiredArgs(2).numOfArgs(1).build();
			o.ShortName("a").RequiredArgs(2).NumArgs(1); 
			opt.AddOption(o);

			std::vector<std::string> args;
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

			std::vector<std::string> args;
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

			COption o;// = COption::builder("a").requiredArgs(2).numOfArgs(1).build();
			o.ShortName("a").RequiredArgs(2).NumArgs(1); 
			opt.AddOption(o);

			std::vector<std::string> args;
			args.push_back("-a");
			args.push_back("arg_for_a_1");
			args.push_back("arg_for_a_2");

			Assert::IsFalse(p.Parse(&opt, &cmd, args));
			Assert::AreEqual("Too many arguments for: a", p.GetLastError().c_str());
		}


	};
}




