/**************************************************************
	Log part, but I recommond use common open source log lib
	Sandy Yann 
	2016-04-02
**************************************************************/

#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <mutex>

#ifdef _WIN32
#include <Windows.h>
#include <time.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#endif


#define LOG_NAME	"mylog"	// 

static bool g_bWriteFile = false;				// default not to write log file.
static std::string g_LogFileName = LOG_NAME;	// default log name.
static std::mutex g_output_mutex;
/**
 * @brief Get executable path.
 */
#ifdef _WIN32
std::string getExePath()
{
	char szFilePath[MAX_PATH + 1] = {0};
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; //   
	std::string path = szFilePath;
	return path;
}
#else
std::string getExePath() {
	char exec_name[1024] = { 0 };
	readlink("/proc/self/exe", exec_name, 1024);
	return std::string(exec_name);
}
#endif

static std::string get_fn_from_fullfn(std::string fullfn) {
#ifdef _WIN32
	int pos = fullfn.rfind("\\");
#else
	int pos = fullfn.rfind("/");
#endif
	std::string fn = fullfn.substr(pos + 1, fullfn.length());
	return fn;
}

/**
 * @brief Based on time in the log file name, delete log files one month ago.
 */
static void judgeTimeAndDel(std::string strFullFn) {
	std::string fn = get_fn_from_fullfn(strFullFn);
	int curY = 0;
	int curM = 0;
	int curD = 0;
	int oldY = 0;
	int oldM = 0;
	int oldD = 0;
	std::string strTemp = fn.substr(fn.find("_") + 1, fn.length() - fn.find("_") - 1);
	sscanf(strTemp.c_str(), "%d-%d-%d", &oldY, &oldM, &oldD);

	time_t curTm = time(0);
	char tmp[64] = { 0 };
	strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H-%M-%S", localtime(&curTm));
	sscanf(tmp, "%d-%d-%d", &curY, &curM, &curD);

	std::cout << strFullFn << std::endl;

	if (oldY == 0 || oldM == 0 || oldD == 0) {
		return;
	}

	if (curY * 12 * 30 + curM * 30 + curD - oldY * 12 * 30 - oldM * 30 - oldD >= 28) {
		remove(strFullFn.c_str());
		std::cout << " had deleted! " << std::endl;
	}
}

/**
 * @brief Delete log.txt before more one month, or all '*.log' files
 * @param bOneMonth:
 *	true: delete *.log created before one month.
 *	false: delete all *.log
 */
static void delOneMonthAgoLog(bool bOneMonth) {
	std::string strRootPath = getExePath();

#ifdef _WIN32
	std::string strLogFullName = strRootPath + "\\" + "*.log";

	intptr_t Handle;
	struct _finddata_t FileInfo;
	if ((Handle = _findfirst(strLogFullName.c_str(), &FileInfo)) == -1L) {
		// printf("Can' find '*.log' file\n");
		return;
	}
	else {
		std::string curFullFn = strRootPath + "\\" + FileInfo.name;
		if (bOneMonth) {
			judgeTimeAndDel(curFullFn);
		}
		else {	// Directly delete.
			std::cout << curFullFn;
			remove(curFullFn.c_str());
			std::cout << " had deleted" << std::endl;
		}

		while (_findnext(Handle, &FileInfo) == 0) {
			curFullFn = strRootPath + "\\" + FileInfo.name;
			if (bOneMonth) {
				judgeTimeAndDel(curFullFn);
			}
			else { // Directly delete.
				std::cout << curFullFn;
				remove(curFullFn.c_str());
				std::cout << " had deleted" << std::endl;
			}
		}

		_findclose(Handle);
	}
#else
	unsigned int count = 0;		//
	char txtname[128];			//
	DIR *dp;
	struct dirent *dirp;

	// opendir, readdir, closedir
	dp = opendir(strRootPath.c_str());
	if (dp == NULL) {
		perror("opendir");
	}

	// loop all files.  
	while ((dirp = readdir(dp)) != NULL) {
		std::string strDName = std::string(dirp->d_name);
		if (strDName == std::string(".") || strDName == std::string(".."))
			continue;

		size_t sz = strDName.length();
		// file length < 5, can't is *.log file.  
		if (sz < 5)
			continue;

		// find all *.log file
		if (strDName.substr(sz - 4, 4) != std::string(".log"))
			continue;

		std::cout << strDName << std::endl;
	}

	closedir(dp);
#endif
}

/**
 * @brief Convert time into particular format.
 */
static std::string getCurStrTime() {
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));
	return std::string(tmp);
}

/**
 * @brief Log file name added time.
 */
static std::string getLogFileName(std::string strLogFn) {
	std::string strTime = getCurStrTime();
	strLogFn = strLogFn.empty() ? LOG_NAME : strLogFn;
	strLogFn = strLogFn + "_" + strTime + ".log";
	return strLogFn;
}

/**
 * @brief Printf log to screen and file.
 * @param pLogText: log text.
 */
void printfLog(const char* pLogText) {
	g_output_mutex.lock();
	std::string strLogFullName = getExePath() + "\\" + g_LogFileName;

	static FILE* g_pFOut = NULL;
	if (g_bWriteFile) {
		if (g_pFOut == NULL) {
			/**
			 * Save log to file.
			 */
			g_pFOut = fopen(strLogFullName.c_str(), "a+");
			if (NULL == g_pFOut) {
				printf("can't write log\n");
				g_output_mutex.unlock();
				return;
			}
		}
	}

	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));

	// Write log to file.
	if (g_bWriteFile) {
		fprintf(g_pFOut, "%s %s", tmp, pLogText);
		fflush(g_pFOut);
	}
	printf("%s %s\n", tmp, pLogText);

	g_output_mutex.unlock();
}

void InitialLog(const char* pLogFn, bool bLogNameAddTime, bool bDelOldLog, bool bWriteFile)
{
	std::cout << "Start init log ..." << std::endl;
	
	g_bWriteFile = bWriteFile;
	delOneMonthAgoLog(bDelOldLog ? false : true);

	g_LogFileName = bLogNameAddTime ? getLogFileName(pLogFn) : (std::string(pLogFn) + ".log");

	std::cout << "Init log finish, log name = " << g_LogFileName << std::endl 
		<< "*********************************************************"
		<< std::endl << std::endl;
}


/*===========================================*/
void subFun(int arr[])
{
	arr[999999999] = 123;
}
/**
*@brief Learn to how to use GDB tool
*/
int testGDB(int argc, char** argv)
{
	std::cout << "Enter " << __FUNCTION__ << std::endl;

	int arr[20] = {0};

	arr[20] = 9;

	std::cout << "err index value = " << arr[20] << std::endl;

	subFun(arr);

	std::cout << "err index value = " << arr[99999] << std::endl;

	return 1;
}
/*===========================================*/
