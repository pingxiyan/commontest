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
#define LOG_NAME	"mylog"	// 

static std::string g_LogFileName = LOG_NAME; //getLogFileName(std::string());

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
		std::cout << " had deleted! "<< std::endl;
	}
}

/**
* brief@ Delete log.txt before more one month, or all '*.log' files
* param@ bOneMonth: 
*	true: delete *.log created before one month.
*	false: delete all *.log 
*/
static void delOneMonthAgoLog(bool bOneMonth)
{
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
			DelFile(curFullFn.c_str());
			std::cout << " had deleted" << std::endl;
		}

		while (_findnext(Handle, &FileInfo) == 0) {
			curFullFn = strRootPath + "\\" + FileInfo.name;
			if (bOneMonth) {
				judgeTimeAndDel(curFullFn);
			}
			else { // Directly delete.
				std::cout << curFullFn;
				DelFile(curFullFn.c_str());
				std::cout << " had deleted" << std::endl;
			}
		}

		_findclose(Handle);
	}
#else
	unsigned int count = 0;		//临时计数，[0，SINGLENUM]  
	char txtname[128];			//存放文本文件名  
	DIR *dp;
	struct dirent *dirp;

	//打开指定目录  
	if ((dp = opendir(strRootPath.c_str()) == NULL)
	{
		perror("opendir");
	}

	//开始遍历目录  
	while ((dirp = readdir(dp)) != NULL)
	{
		//跳过'.'和'..'两个目录  
		if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
			continue;

		int size = strlen(dirp->d_name);

		//如果是.wav文件，长度至少是5  
		if (size<5)
			continue;

		//只存取.mp3扩展名的文件名  
		if (strcmp((dirp->d_name + (size - 4)), ".mp3") != 0)
			continue;

		/*把文件名d_name 每SINGLENUM个写入一个文件，
		**用一个变量count记录遍历到的文件的数量，
		**每SINGLENUM个打开一个新文件
		*/

		if ((++count) > SINGLENUM)
		{
			fclose(fp);
			times++;
			sprintf(txtname, "%02d.txt", times);           //自动命名生成.txt文件                                        
			if ((fp = fopen(txtname, "w+")) == NULL)
			{
				perror("fopen");
				exit(EXIT_FAILURE);
			}

			count = 1;
		}

		fputs(dirp->d_name, fp);
		fputs(" ", fp);                                  // 在一个filename结束之后，写入空格，方便脚本读取，进程转换操作  

	}

	fclose(fp);
	closedir(dp);
#endif
}

static std::string getLogFileName(std::string strLogFn)
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H-%M-%S", localtime(&t));

	strLogFn = strLogFn.empty() ? LOG_NAME : strLogFn;

	strLogFn = strLogFn + "_" + std::string(tmp) + ".log";

	return strLogFn;
}

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

CTAPI void InitialLog(const char* pLogFn, bool bLogNameAddTime, bool bDelOldLog)
{
	std::cout << "Start init log ..." << std::endl;

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