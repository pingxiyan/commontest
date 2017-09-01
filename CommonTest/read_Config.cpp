/***********************************************
* Read config file, myself format
* Aug. 30 2017 Sandy Yann
***********************************************/
#include "CommonTest.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

typedef enum
{
	eDigital = 0,
	eString = 1
}EVALType;

typedef struct
{
	string strLabel;
	string strVal;
}TCfgItem;


static char* readOneRowInFile(FILE* pF, int &len, bool &bEOF);
static bool parseOneRow(char* pbuf, int len, TCfgItem &tCfgItem);

void* readConfig(const char* ps8ConfigFn)
{
	char* pBufOneRow = NULL;
	FILE* pf = fopen(ps8ConfigFn, "rb");
	if (pf == NULL)
	{
		printf("can't open config file = %s\n", ps8ConfigFn);
		return NULL;
	}

	std::cout << "Start parsing config...\n" <<
		"===============================================\n";

	vector<TCfgItem> *pVecCfgItem = new vector<TCfgItem>();

	for(;;)
	{
		int len = 0;
		bool bEOF = false;
		pBufOneRow = readOneRowInFile(pf, len, bEOF);
		if (NULL == pBufOneRow)
		{
			break;
		}

		TCfgItem tCfgItem;
		if(parseOneRow(pBufOneRow, len, tCfgItem))
		{
			std::cout << tCfgItem.strLabel << "	= " << tCfgItem.strVal << std::endl;
			pVecCfgItem->push_back(tCfgItem);
		}

		free(pBufOneRow);
		if(bEOF)
			break;
	}

	fclose(pf);

	std::cout << "===============================================\n\n";
	return pVecCfgItem;
}

void releaseConfig(void**ppvHandle)
{
	if (*ppvHandle)
	{
		vector<TCfgItem> *pVecCfgItem = (vector<TCfgItem> *)*ppvHandle;

		delete pVecCfgItem;
	}
	*ppvHandle = NULL;
}

bool stringIsFloat(string str)
{
	const char* pbuf = str.c_str();
	for (size_t i = 0; i < str.length(); i++)
	{ 
		if ((pbuf[i] <= '9' && pbuf[i] >= '0') || pbuf[i] == '.')
		{
			continue;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool stringIsDigit(string str)
{
	const char* pbuf = str.c_str();
	for (size_t i = 0; i < str.length(); i++)
	{ 
		if (pbuf[i] > '9' || pbuf[i] < '0')
		{
			return false;
		}
	}

	return true;
}

bool parseConfig(void* pvHandle, char* ps8Label, int &val)
{
	vector<TCfgItem> *pVecCfgItem = (vector<TCfgItem> *)pvHandle;
	if (pVecCfgItem)
	{
		vector<TCfgItem> &vecCI = *pVecCfgItem;
		for (size_t i = 0; i < pVecCfgItem->size(); i++)
		{
			if(strcmp(vecCI[i].strLabel.c_str(), ps8Label) == 0)
			{
				if (stringIsDigit(vecCI[i].strVal))
				{
					val = atoi(vecCI[i].strVal.c_str());
					return true;
				}
				else
				{
					std::cout << "\nError: " << vecCI[i].strVal << " is not digital" << std::endl;
					return false;
				}
			}
		}
	}

	return false;
}

CTAPI bool parseConfig(void* pvHandle, char* ps8Label, bool &val)
{
	int v = 0;
	bool b = parseConfig(pvHandle, ps8Label, v);

	val = v == 0 ? false : true;
	return b;
}

bool parseConfig(void* pvHandle, char* ps8Label, float &val)
{
	vector<TCfgItem> *pVecCfgItem = (vector<TCfgItem> *)pvHandle;
	if (pVecCfgItem)
	{
		vector<TCfgItem> &vecCI = *pVecCfgItem;
		for (size_t i = 0; i < pVecCfgItem->size(); i++)
		{
			if(strcmp(vecCI[i].strLabel.c_str(), ps8Label) == 0)
			{
				if (stringIsFloat(vecCI[i].strVal))
				{
					val = (float)atof(vecCI[i].strVal.c_str());
					return true;
				}
				else
				{
					std::cout << "\nError: " << vecCI[i].strVal << " is not float" << std::endl;
					return false;
				}
			}
		}
	}

	return false;
}

bool parseConfig(void* pvHandle, char* ps8Label, string & strOut)
{
	vector<TCfgItem> *pVecCfgItem = (vector<TCfgItem> *)pvHandle;
	if (pVecCfgItem)
	{
		vector<TCfgItem> &vecCI = *pVecCfgItem;
		for (size_t i = 0; i < pVecCfgItem->size(); i++)
		{
			if(strcmp(vecCI[i].strLabel.c_str(), ps8Label) == 0)
			{
				strOut = vecCI[i].strVal;
				return true;
			}
		}
	}

	return false;
}

/**
*brief@ read one line from FILE handle
*/
static char* readOneRowInFile(FILE* pF, int &len, bool &bEOF)
{
	char curChar;
	char* pStr = NULL;
	int num = 0;
	#ifdef _WIN32
	fpos_t fpt1 = 0;
	#else
	fpos_t fpt1 = {0};
	#endif

	bEOF = false;
	// 获取当前位置
	int r = fgetpos(pF, &fpt1);
	if (r!=0)
	{
		return NULL;
	}

	// 获取一行结尾符号
	num = 0;
	for(;;)
	{
		curChar = fgetc(pF);
		// 文件尾
		if (curChar == EOF)
		{
			bEOF = true;
			break;
		}
		// 一行尾
		if (curChar == '\n')
		{
			break;
		}
		num++;
	}

	#ifdef _WIN32
	fseek(pF, (long)fpt1, SEEK_SET);
	#else
	fsetpos(pF, &fpt1);
	#endif // _WIN32

	pStr = (char*)malloc(num + 1);
	fread(pStr, sizeof(char), num + 1, pF);
	len = num + 1;

	return pStr;
}

// 去掉子字符串两端的空格
static string delSpaceChar(const char* ps8Res, int len)
{
	int startpos = 0;
	int endpos = len - 1;
	for (int i = 0; i < len; i++)
	{
		// 9表示tab键的填充字符
		if (ps8Res[i] == ' ' || ps8Res[i] == 9)
		{
			startpos ++ ;
		}
		else
		{
			break;
		}
	}

	for (int i = len - 1; i >= 0; i--)
	{
		if (ps8Res[i] == ' ' || ps8Res[i] == 9)
		{
			endpos -- ;
		}
		else
		{
			break;
		}
	}

	if (startpos > endpos - startpos + 1)
	{
		return std::string();
	}
	return string(ps8Res + startpos, endpos - startpos + 1);
}

// 找到label前面的字符串
static string findSubStrBeforeLabel(char* ps8Src, int len, char cLabel, int &offv)
{
	int findpos = 0;
	offv = 0;

	for (int i = 0; i < len; i ++)
	{
		if (ps8Src[i] == cLabel)
		{
			offv = findpos + 1;	// +1表示越过这个字符
			return delSpaceChar(ps8Src, findpos);
		}

		if (ps8Src[i] == '#')
		{
			return string();
		}

		if (ps8Src[i] == '\r' || ps8Src[i] == '\n' || ps8Src[i] == EOF)
		{
			offv = findpos + 1;	// +1表示越过这个字符
			return delSpaceChar(ps8Src, findpos);
		}

		// 一行的结束，也是文件的结束时，windows=-51,
		// 当路径中出现汉字时，也是有可能=-51,所以修改为最后一个字符
		if (i == len - 1)	
		{
			return delSpaceChar(ps8Src, findpos);
		}

		findpos++;
	}

	return string();
}

static bool parseOneRow(char* pbuf, int len, TCfgItem &tCfgItem)
{
	int offv = 0;
	string strLebal = findSubStrBeforeLabel(pbuf, len,  '=', offv);
	if (strLebal.empty())
	{
		return false;
	}

	string strVal = findSubStrBeforeLabel(pbuf + offv, len - offv, '#', offv);
	if (strVal.empty())
	{
		return false;
	}

	tCfgItem.strLabel = strLebal;
	tCfgItem.strVal = strVal;

	return true;
}