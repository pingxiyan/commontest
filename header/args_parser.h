/*
 * args_parser.h
 * Same to config.h, argv_parse based on 'gflags' to parse command line parameter.
 * And config.h just parse 'config.txt'
 *
 *  Created on: Nov 2, 2017
 *      Author: xiping
 */

#ifndef HEADER_ARGS_PARSER_H_
#define HEADER_ARGS_PARSER_H_

#include <iostream>
#include <vector>


class Configuration
{
public:
	Configuration(){};
	~Configuration(){};

	// member
	bool _bVal = true;
	int _iVal = 0;
	double _dVal = 0;
	std::string _model;
	std::vector<std::string> _vecImg;
};

/**
 * @brief Parse command line parameter, like Linux.
 * All parameters will put into Configuration.
 */
int args_parser_gflags(int argc, char** argv, Configuration& config);

/**
 * @brief argv_parse sample
 */
int unit_test_args_parser_gflags(int argc, char** argv);


/**
 * @brief Parse command line parameter, like Linux.
 * All parameters will put into Configuration.
 */
int args_parser_myself(int argc, char** argv, Configuration& config);

/**
 * @brief argv_parse sample
 */
int unit_test_args_parser_myself(int argc, char** argv);

#endif /* HEADER_ARGS_PARSER_H_ */
