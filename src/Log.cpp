/**************************************************************
	Log part, but I recommond use common open source log lib
	Sandy Yann 
	2016-04-02
**************************************************************/

#include "CommonTest.h"
#include "cclog.h"

#ifdef _WIN32
#include <Windows.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <iostream>
#else
#include <sys/stat.h>
#endif


#define SHOW_LOG_CONSOLE 1	// 
#define LOG_NAME	"alg"	// 

#ifdef _WIN32
CTAPI std::string getExePath()
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; //   
	std::string path = szFilePath;
	return path;
}
#else
CTAPI std::string getExePath()
{
	char exec_name [1024] = {0};
	readlink ("/proc/self/exe", exec_name, 1024);
	return std::string(exec_name);
}
#endif

static void judgeTimeAndDel(std::string strFullFn)
{
	std::string fn = getFileNameFromFullName(strFullFn);
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

	if (oldY == 0 || oldM == 0 || oldD == 0)
	{
		return;
	}

	if (curY * 12 * 30 + curM * 30 + curD - oldY * 12 * 30 - oldM * 30 - oldD >= 28)
	{
		DelFile(strFullFn.c_str());
	}
}

// Delete log.txt more one month ago
static void delOneMonthAgoLog()
{
#ifdef _WIN32
	std::string strRootPath = getExePath();
	std::string strLogFullName = strRootPath + "\\" + "*.log";

	intptr_t Handle;
	struct _finddata_t FileInfo;
	if ((Handle = _findfirst(strLogFullName.c_str(), &FileInfo)) == -1L)
	{
		//printf("没有找到匹配的项目\n");
		return;
	}
	else
	{
		std::string curFullFn = strRootPath + "\\" + FileInfo.name;
		judgeTimeAndDel(curFullFn);
		while (_findnext(Handle, &FileInfo) == 0)
		{
			curFullFn = strRootPath + "\\" + FileInfo.name;
			judgeTimeAndDel(curFullFn);
			//printf("%s\n", FileInfo.name);
		}

		_findclose(Handle);
	}
#else

#endif
}

static std::string getLogFileName(std::string strLogFn)
{
	delOneMonthAgoLog();

	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H-%M-%S", localtime(&t));

	strLogFn = strLogFn.empty() ? "alg" : strLogFn;
	return strLogFn + "_" + std::string(tmp) + ".log";
}
static std::string g_LogFileName = getLogFileName(std::string());

CTAPI std::string getCurStrTime()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));

	return std::string(tmp);
}

CTAPI void printfLog(const char* pLogText)
{
	std::string strLogFullName = getExePath() + "\\" + g_LogFileName;

	static FILE* g_pFOut = NULL;
	if (g_pFOut == NULL)
	{
		g_pFOut = fopen(strLogFullName.c_str(), "a+");	// 如果文件不存在，则创建
		if (NULL == g_pFOut)
		{
			printf("can't write log\n");
			return;
		}
	}

	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));

	fprintf(g_pFOut, "%s : %s", tmp,  pLogText);
	fflush(g_pFOut);
	
	printf("%s : %s\n", tmp, pLogText);
}


CTAPI void InitialLog(const char* pLogFn)
{
	g_LogFileName = getLogFileName(pLogFn);
}


/*===========================================*/
void subFun(int arr[])
{
	arr[999999999] = 123;
}
/**
*brief@ Learn to how to use GDB tool
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