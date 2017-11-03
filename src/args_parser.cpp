/*
 * args_parser.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: xiping
 */

#include "args_parser.h"

#include <gflags/gflags.h>
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
#include "w_dirent.h"
#else
#include <dirent.h>
#endif

#include <cstddef>

#define DEFAULT_PATH_P "./lib"

/**
 * @brief Parses arguments from the command line
 * @param argc - number of parameters passed
 * @param argv - vector of parameters passed
 * @param config - config object
 * @return exit code
 */
int parseArgs(int argc, char *argv[], Configuration &config)
{

	return 1;
}

/// @brief message for help argument
static const char help_message[] = "Print a usage message.";
/// @brief message for images argument
static const char images_message[] = \
                "Required. Path to a folder with images or path to an image files: " \
                "a .ubyte file for LeNet and a .bmp file for the other networks.";
/// @brief message for model argument
static const char model_message[] = "Required. Path to an .xml file with a trained model.";

static const char bval_message[] = "test bool member";
static const char ival_message[] = "test int member";
static const char dval_message[] = "test double member";


/// @brief Define flag for showing help message <br>
DEFINE_bool(h, false, help_message);
/// @brief Define parameter for set image file <br>
/// It is a required parameter
DEFINE_string(i, "", images_message);
/// @brief Define parameter for set image file <br>
/// It is a required parameter
DEFINE_string(m, "", model_message);

DEFINE_bool(bval, false, bval_message);
DEFINE_int32(ival, 0, ival_message);
DEFINE_double(dval, 0, dval_message);


static void showUsage() {
    std::cout << std::endl;
    std::cout << __FUNCTION__ << " [OPTION]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << std::endl;
    std::cout << "    -h, --help" << std::endl;
    std::cout << "                              " << help_message << std::endl;
    std::cout << "    -i \"<path1>\" \"<path2>\" ..., --images \"<path1>\" \"<path2>\" ..." << std::endl;
    std::cout << "                              " << images_message << std::endl;
    std::cout << "    -m \"<path>\", --model \"<path>\"" << std::endl;
    std::cout << "                              " << model_message << std::endl;

    std::cout << "    -bval \"false, 0, true, 1\"" << std::endl;
    std::cout << "                              " << bval_message << std::endl;
    std::cout << "    -ival \"int \"" << std::endl;
    std::cout << "                 " << bval_message << std::endl;
    std::cout << "    -dval \"double \"" << std::endl;
    std::cout << "                 " << bval_message << std::endl;
}

static void readImageArgument(std::vector<std::string> &images, const std::string& arg) {
    struct stat sb;
    if (stat(arg.c_str(), &sb) != 0) {
        std::cout << "[WARNING] File " << arg << " cannot be opened!" << std::endl;
        return;
    }
    if (S_ISDIR(sb.st_mode)) {
        DIR *dp;
        dp = opendir(arg.c_str());
        if (dp == nullptr) {
            std::cout << "[WARNING] Directory " << arg << " cannot be opened!" << std::endl;
            return;
        }

        struct dirent *ep;
        while (nullptr != (ep = readdir(dp))) {
            std::string fileName = ep->d_name;
            if (fileName == "." || fileName == "..") continue;
            std::cout << "[INFO] Add file  " << ep->d_name << " from directory " << arg << "." << std::endl;
            images.push_back(arg + "/" + ep->d_name);
        }
    } else {
        images.push_back(arg);
    }
}

static void parseImagesArgs(std::vector<std::string> &images) {
    std::vector<std::string> args = gflags::GetArgvs();
    bool readArguments = false;
    for (size_t i = 0; i < args.size(); i++) {
        if (args.at(i) == "-i" || args.at(i) == "--images") {
            readArguments = true;
            continue;
        }
        if (!readArguments) {
            continue;
        }
        if (args.at(i).c_str()[0] == '-') {
            break;
        }
        readImageArgument(images, args.at(i));
    }
}

/**
 * @brief Parse command line parameter, like Linux.
 * All parameters will put into Configuration.
 */
int args_parser(int argc, char** argv, Configuration& config)
{
	//gflags::ParseCommandLineFlags(&argc, &argv, true);
    gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);
    gflags::CommandLineFlagInfo infoHelp;

    gflags::GetCommandLineFlagInfo("help", &infoHelp);
    if (FLAGS_h || (infoHelp.name == "help" && infoHelp.current_value == "true")) {
        showUsage();
        return 1;
    }

	config._bVal = FLAGS_bval;
	config._iVal = FLAGS_ival;
	config._dVal = FLAGS_dval;
	config._model = FLAGS_m;

    parseImagesArgs(config._vecImg);
    return 0;
}

/**
 * @brief args parser unit test
 */
int unit_test_args_parser(int argc, char** argv)
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

	args_parser(argc, argv, config);


#define POUT(N) std::cout << #N << " = " << N << std::endl;

	POUT(config._bVal);
	POUT(config._iVal);
	POUT(config._dVal);
	POUT(config._model);
	for(size_t i = 0; i < config._vecImg.size(); i++){
		POUT(config._vecImg[i]);
	}

	return 1;
}


