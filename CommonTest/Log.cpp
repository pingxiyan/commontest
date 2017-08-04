/**/

/**************************************************************
	和日志相关的代码
	Sandy Yann 
	2016-04-02
**************************************************************/

/************************************************************************
/* 日志管理：为了每次日志不被覆盖，所以名字中加入时间，这样就必须删除一段时间
	之前的日志文件 	

	2017-2-14 修改为：程序启动初始化一次日志的名字，没有日志产生时，没有日志文
				件产生，日志文件名字可以初始化（没有初始化，采用默认名字），
				名字中依然有时间信息*/
/************************************************************************/
#include "CommonTest.h"
#include "opencv2/opencv.hpp"

#include <Windows.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include<io.h>

#define SHOW_LOG_CONSOLE 1	// 是否显示日志到命令行
#define LOG_NAME	"alg"	// 初始化日志的名字（如果没有调用InitialLog）	

CTAPI std::string getExePath()
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
	std::string path = szFilePath;
	return path;
}

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

	// 如果log格式有误
	if (oldY == 0 || oldM == 0 || oldD == 0)
	{
		return;
	}

	if (curY * 12 * 30 + curM * 30 + curD - oldY * 12 * 30 - oldM * 30 - oldD >= 28)
	{
		DelFile(strFullFn.c_str());
	}
}

// 删除一个月以前的日志文件,避免太多
static void delOneMonthAgoLog()
{
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
}

static std::string getLogFileName(std::string strLogFn)
{
	// 删除一个月以前的日志文件
	delOneMonthAgoLog();

	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H-%M-%S", localtime(&t));

	strLogFn = strLogFn.empty() ? "alg" : strLogFn;
	return strLogFn + "_" + std::string(tmp) + ".log";
}
static std::string g_LogFileName = getLogFileName(std::string());

// 获得当前时间，精确到秒; 格式为：year-month-day h:m:s
CTAPI std::string getCurStrTime()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));

	return std::string(tmp);
}

// 所有的日志信息都会输出到log.txt
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

/* 初始化日志的名字，如果不初始化，使用默认名字log_时间.txt,
会把一个月以前的日志文件删除	*/
CTAPI void InitialLog(const char* pLogFn)
{
	// 初始化日志名字,默认为"alg_time.log"
	g_LogFileName = getLogFileName(pLogFn);

}