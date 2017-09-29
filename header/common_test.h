/************************************************************************
* Common test pulbic header
*	2015-11-24 Sandy Yann
*	9/11/2017  Sandy Yann, Start support Windows and Linux
*	9/14/2017  Sparating license plate test algorithm to 'test_lp.h'.
*************************************************************************/

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#ifndef _COMMON_TEST_H_
#define _COMMON_TEST_H_

#ifdef COMMONTEST_EXPORTS
#define CTAPI __declspec(dllexport)
#else
#define CTAPI /*__declspec(dllimport)*/
#endif

#define CC_CHECK(condition) if(!(condition)){printf("FAIL:%s, %d\n", __FUNCTION__, __LINE__);}

#include "test_lp.h"
#include "cclog.h"
#include "config.h"

// 从文件全路径名中，获取文件名字(\后面的部分)
CTAPI std::string getFileNameFromFullName(std::string strFullName);

// 从文件全路径名中，获取倒数第n级目录的名字，0=文件名，找不到返回空
CTAPI std::string getDirNameFromFullName(std::string strFullName, int n/*倒数n级名字*/);

/*=======================================================================
Function : Read file name list  
Param    : strFN[in] dir.set
Return   : vector<string>
=========================================================================*/
CTAPI std::vector<std::string> readsample(std::string strFN);
CTAPI std::vector<std::string> readsample_from_dir(std::string rootdir);	// 读取文件夹下的所有文件
CTAPI std::vector<std::string> readsample_from_dir_suffix(std::string rootdir, std::string suffix/*.c*/);	// 读取文件夹下的所有文件(某种后缀）
CTAPI std::vector<std::string> readsample_del_mask(std::string strFN);
// 1个文件夹下，选择n个文件（总数<=n，全选)
CTAPI std::vector<std::string> sampleChoose1DirNFile(std::vector<std::string> vecFn, int n);
// 1个文件夹下，返回所有的文件名的vector
CTAPI std::vector<std::string> sampleChooseOneDir(std::vector<std::string> vecFn, int startId);

// 文件copy，源文件名到目的文件名
CTAPI void copyFile2DstFn(std::string strSrcFn, std::string strDstFn);

typedef struct
{
	std::string strFn;		// 原始图像文件名
	std::string strFnMask;	// 原始图像对应的mask文件名
}TImgSet;
// 读set，获取成对的文件名字，一个是原始图像，一个是mask
CTAPI std::vector<TImgSet> readsample_with_mask(std::string strFN);

/*=======================================================================
Function : Create folder by path  
Param    : 
Return   : void
=========================================================================*/
CTAPI int MKDirTest(const char *filepath);
CTAPI int RMDirTest(const char *filepath);	// 清空文件夹
#define DeleteDirTest RMDirTest
#define DelDir RMDirTest
// 删除全名的文件
CTAPI void DelFile(const char* fn);


// Fast multi-thread test interface
#ifdef _WIN32
typedef void* (__stdcall*MultiOpen)(int idx/*thread id, */);
typedef int (__stdcall*MultiProcess)(void* pvHandle, const char* fn, void *pvUser);
typedef int(__stdcall*MultiClose)(void*pvHandle);
#else
typedef void* (*MultiOpen)(int idx/*thread id, */);
typedef int (*MultiProcess)(void* pvHandle, const char* fn, void *pvUser);
typedef int(*MultiClose)(void*pvHandle);
#endif
CTAPI void multiThrdTest(std::string strSetFn,	// multi-thread test function
						 int thrnum,	// Start thread count >=1
						 MultiOpen myOpen,
						 MultiProcess myProcess,
						 MultiClose myClose,
						 void *pvUser);

CTAPI void multiThrdTest(std::vector<std::string> vecFn,	// Directly process every file based on multi-thread.
	int thrnum,	// Start thread count >=1
	MultiOpen myOpen,
	MultiProcess myProcess,
	MultiClose myClose,
	void *pvUser);

/*=======================================================================
Function : Show retrieval result  
Param    : vecScore[in] The score of each image
		   vecFn[in] Image library.
		   strRefFn[in] Reference image
		   bSave[in] false: show result; true: not show, directly save result
Return   : void
=========================================================================*/
CTAPI void showRetrievalResut(std::vector<float> vecScore, std::vector<std::string> vecFn,
							  std::string strRefFn, bool bSave, std::string svpath);
							  


// 通用特征库管理
#if 1
// 多线程测试时，vector分段(平均)
CTAPI std::vector<std::string> vecStrSection(int totalSection, int curId/*从0开始*/,
									   std::vector<std::string> vecAll);
CTAPI std::vector<TImgSet> vecStrSection(int totalSection, int curId/*从0开始*/,
										 std::vector<TImgSet> vecAll);

// 文件头，记录文件中有多少个条特征
CTAPI void writeHeader(FILE* pfOut, int num);
CTAPI void writeHeader3P(FILE* pfOut, int num, int dim);

// 读文件头,会改变当前位置到文件头
CTAPI void readHeader(FILE* pfOut, int& num);
CTAPI void readHeader3P(FILE* pfOut, int& num, int &dim);

CTAPI void writeText(FILE* pfOut, int val);
CTAPI void writeText(FILE* pfOut, size_t val);
CTAPI void writeText(FILE* pfOut, float val);
CTAPI void writeText(FILE* pfOut, char val);
CTAPI void writeVector(FILE* pfOut, std::vector<float> val);

CTAPI void readText(FILE* pfIn, int& val);
CTAPI void readText(FILE* pfIn, float& val);
CTAPI void readText(FILE* pfIn, char& val);
CTAPI void readVector(FILE* pfIn, std::vector<float>& vecVal);
CTAPI void readVector2Buf(FILE* pfIn, float * pArr);

// sting 读写
CTAPI void writeString(FILE* pfOut, std::string str);
inline std::string readString(FILE* pfIn)
{
	std::string str;
	int len = 0;
	readText(pfIn, len);
	if (len < 0)// 文件结尾
	{
		return str;
	}

	char* ptext = new char[len + 1];
	memset(ptext, 0, len + 1);
	fread(ptext, 1, len, pfIn);
	ptext[len] = 0;
	str = std::string(ptext);
	delete[] ptext;

	return str;
}
inline BOOL readString2Buf(FILE* pfIn, char* pBuf, int bufSZ)
{
	std::string str;
	int len = -1;
	readText(pfIn, len);
	if (len > bufSZ || len < 0)// 文件结尾
	{
		printf("Err: fun = %s; lineNo = %d\n", __FUNCTION__, __LINE__);
		return FALSE;
	}

	fread(pBuf, 1, len, pfIn);
	pBuf[len] = 0;
	return TRUE;
}

// Read array based byte
CTAPI void writeArray(FILE* pfOut, char* pf32Arr, int lenlen);
inline char* readArray(FILE* pfIn, int& len){
	readText(pfIn, len);
	//fscanf(pfIn, "%d,", &len);

	char* ptext = new char[len + 1];
	fread(ptext, 1, len, pfIn);
	ptext[len] = 0;

	return ptext;
}

// true: file exist
CTAPI bool fileEof(FILE* pfIn);
#endif

/**
* brief@ Get executable program folder.
*/
CTAPI std::string getExePath();

/**
* brief@ Return string, format = year-month-day h:m:s
*/
CTAPI std::string getCurStrTime();

/**
*brief@ Learn to how to use GDB tool
*/
int testGDB(int argc, char** argv);

#endif