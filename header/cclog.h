/*********************************************************
* Module: Log 
*********************************************************/

#ifndef _COMMON_TEST_LOG_H_
#define _COMMON_TEST_LOG_H_


/**
* brief@ Initial log file name
* param@ pLogFn : log file name, NULL = default name[excutable.log]
*/
void InitialLog(const char* pLogFn);

/**
* brief@ Printf log to file txt
* param@ pLogText : log text
*/
void printfLog(const char* pLogText);

#define PrintfLogArg(_format, ...) {\
	char atext[1024] = { 0 };		\
	sprintf(atext, "%s_%d : ", __FUNCTION__, __LINE__);	\
	sprintf(atext + strlen(atext), _format, ##__VA_ARGS__);	\
	printfLog(atext);			\
}

#define PERR(_format, ...) {\
	char atext[1024] = { 0 };		\
	sprintf(atext, "fail: %s_%d : ", __FUNCTION__, __LINE__);	\
	sprintf(atext + strlen(atext), _format, ##__VA_ARGS__);	\
	printfLog(atext);			\
}

#define PWARNING(_format, ...) {\
	char atext[1024] = { 0 };		\
	sprintf(atext, "warning: %s_%d : ", __FUNCTION__, __LINE__);	\
	sprintf(atext + strlen(atext), _format, ##__VA_ARGS__);	\
	printfLog(atext);			\
}

#define POUT(_format, ...) {	\
	std::cout << __FUNCTION__ << " : " << __LINE__ << " << " ; \
	printf(_format, ##__VA_ARGS__);		\
}

#define POUT2 std::cout << __FUNCTION__ << " : " << __LINE__ << " << " ;

#define PLOG PrintfLogArg



#endif /* _COMMON_TEST_LOG_H_ */