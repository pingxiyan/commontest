/*
 * args_parser_myself.cpp
 * Because legal reasons, sometimes we need to write myself command line parsing code.
 *
 *  Created on: Dec 4, 2017
 *      Author: xiping
 */

#include "args_parser.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <cstdlib>

static int _argc;
static char** _argv;
static std::vector<std::tuple<std::string, std::string, std::string> > _tupOption;

/**
 * Parsing Command Line Option.
 */
class CPCLOption
{
public:
	CPCLOption(int argc, char* argv[]){
		_argc = argc;
		_argv = argv;
		if (argc > 1) {
			if (argv[1] == "-h" || argv[1] == "--help") {
				showUsage();
				std::exit(EXIT_SUCCESS);
			}
		}
		else if (argc == 1) {
			showUsage();
			std::exit(EXIT_SUCCESS);	
		}
	};
	~CPCLOption(){};

	static void addOption(std::string opt, std::string val, std::string descrition)
	{
		std::tuple<std::string, std::string, std::string> oneOpt =
				std::make_tuple(opt, val, descrition);
		_tupOption.push_back(oneOpt);
	}
	static bool initAllOption()
	{
		CPCLOption::addOption("-b", "0,1 or false, true", "Bool option.");
		CPCLOption::addOption("-i", "int", "Int option.");
		CPCLOption::addOption("-d", "double", "Double option.");
		CPCLOption::addOption("-m", "string", "String option.");
		return true;
	}

	void getOption(const std::string& option, std::string& val)
	{
		try {
			val = getOpt(_argc, _argv, option);
		} catch (std::bad_exception& e) {
			std::cerr << "Caught " << e.what() << '\n';
			std::exit(EXIT_FAILURE);
		}
	}
	void getOption(const std::string& option, int& val)
	{
		try {
			val = std::stoi(getOpt(_argc, _argv, option));
		} catch (std::bad_exception& e) {
			std::cerr << "Caught " << e.what() << '\n';
			std::exit(EXIT_FAILURE);
		}
	}
	void getOption(const std::string& option, float& val)
	{
		try {
			val = std::stof(getOpt(_argc, _argv, option));
		} catch (std::bad_exception& e) {
			std::cerr << "Caught " << e.what() << '\n';
			std::exit(EXIT_FAILURE);
		}
	}
	void getOption(const std::string& option, double& val)
	{
		try {
			val = std::stod(getOpt(_argc, _argv, option));
		} catch (std::bad_exception& e) {
			std::cerr << "Caught " << e.what() << '\n';
			std::exit(EXIT_FAILURE);
		}
	}
	void getOption(const std::string& option, bool& val)
	{
		int ival = 0;
		try {
			ival = std::stoi(getOpt(_argc, _argv, option));
			val = (bool)ival;
		} catch (std::bad_exception& e) {
			std::cerr << "Caught " << e.what() << '\n';
			std::exit(EXIT_FAILURE);
		}
	}

	static void showUsage()
	{
		std::cout << std::endl;
		std::cout << __FUNCTION__ << " [OPTION]" << std::endl;
		std::cout << "Options:" << std::endl;
		std::cout << std::endl;
		std::cout << "    -h" << std::endl;
		std::cout << "            " << "Parsing Command Line Parameter." << std::endl;

		for (size_t i = 0; i < _tupOption.size(); i++) {
			const auto& curOpt = _tupOption[i];//(i);
			std::cout << "    " << std::get<0>(curOpt) << " : " << std::get<1>(curOpt) << std::endl;
			std::cout << "            " << std::get<2>(curOpt) << std::endl;
		}
	}

private:
	std::string getOpt(int argc, char* argv[], const std::string& option)
	{
		std::string cmd;
		for (int i = 0; i < argc; ++i) {
			std::string arg = argv[i];
			/**
			 * Only show usage.
			 */
			if (arg == "-h" || arg == "--help") {
				showUsage();
				std::exit(EXIT_SUCCESS);
			}

			if (0 == arg.find(option) && i < (argc - 1)) {
				cmd = argv[i + 1];
				return cmd;
			}
			else
			{
				std::cout << "Can't find option [ " << option << " ], " << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}
		return cmd;
	}
};

bool bInitAllOption = CPCLOption::initAllOption();

/**
 * @brief Parse command line parameter, like Linux.
 * All parameters will put into Configuration.
 */
int args_parser_myself(int argc, char** argv, Configuration& config)
{
	CPCLOption opt(argc, argv);

	opt.getOption("-b", config._bVal);
	opt.getOption("-i", config._iVal);
	opt.getOption("-d", config._dVal);
	opt.getOption("-m", config._model);

    return 1;
}

/**
 * @brief args parser unit test
 */
int unit_test_args_parser_myself(int argc, char** argv)
{
	std::cout << __FUNCTION__ << " had entered!" << std::endl;

	/**
	 * @brief Add new variant step:
	 * 1: add member variant for Configuration
	 * 2: add DEFINE_bool(bval, false, bval_message);
	 * 3: define variant brief 'bval_message';
	 * 4: add readme for 'showUsage'
	 */

	Configuration config;

	args_parser_myself(argc, argv, config);

#define POUT(N) std::cout << #N << " = " << N << std::endl;

	POUT(config._bVal);
	POUT(config._iVal);
	POUT(config._dVal);
	POUT(config._model);

	return 1;
}


