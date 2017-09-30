/************************************************************************
* Module: Log : example as follow
int main(int argc, char** argv)
{
	InitialLog("mylog", true, true);
	PERR("my error \n");
	PWARNING("my WARNING \n");
	POUT("pout test %d \n", 20);
	return 0;
}
************************************************************************/

#ifndef _COMMON_TEST_LOG_H_
#define _COMMON_TEST_LOG_H_


/**
* @brief Initial log file name, it will delete '*.log' file before one month.
* @param pLogFn : log file name, NULL = default name[excutable.log]
* @param bLogNameAddTime: default false
*		ture: logfilename = 'mylog.log_2017-09-14-21-26-35.log'
*		false: logfilename = 'mylog.log'
* @param bWriteFile: log write to file ?
* @param bDelOldLog: ture delete old *.log file, false: not delete.
* @sample InitialLog(fn, true, true, false);
*/
void InitialLog(const char* pLogFn, bool bLogNameAddTime, bool bDelOldLog, bool bWriteFile);

/**
* @brief Printf log to file txt, multi-thread safe.
* @param pLogText : log text
*/
void printfLog(const char* pLogText);

/**
* @brief Printf log and add function name and line number.
*/
#define PrintfLogArg(_format, ...) {\
	char atext[1024] = { 0 };		\
	sprintf(atext, "[%s_%d] ", __FUNCTION__, __LINE__);	\
	sprintf(atext + strlen(atext), _format, ##__VA_ARGS__);	\
	printfLog(atext);			\
}
// Normalize printf.
#define PLOG PrintfLogArg


#define PERR(_format, ...) {\
	char atext[1024] = { 0 };		\
	sprintf(atext, "[FAIL:%s_%d] ", __FUNCTION__, __LINE__);	\
	sprintf(atext + strlen(atext), _format, ##__VA_ARGS__);	\
	printfLog(atext);			\
}

#define PWARNING(_format, ...) {\
	char atext[1024] = { 0 };		\
	sprintf(atext, "[WARNING:%s_%d] ", __FUNCTION__, __LINE__);	\
	sprintf(atext + strlen(atext), _format, ##__VA_ARGS__);	\
	printfLog(atext);			\
}

//#define POUT(_format, ...) {	\
//	std::cout << __FUNCTION__ << "_" << __LINE__ << " :: " ; \
//	printf(_format, ##__VA_ARGS__);	std::cout << "\n";	\
//}

#endif /* _COMMON_TEST_LOG_H_ */