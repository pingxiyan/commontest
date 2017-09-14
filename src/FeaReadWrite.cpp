/******************************************************************************
模块名      ： GetRoi
文件实现功能： 通用特征库文件读写
版本        ： 1.0
-------------------------------------------------------------------------------
修改记录:
日  期        版本        修改人        走读人    修改内容
2015-11-24    1.0         闫西平                    创建
******************************************************************************/

#include "CommonTest.h"

#include <vector>
#include <string>
using std::vector;
using std::string;

#include <stdio.h>
#include <stdlib.h>


// 数组读写
void writeArray(FILE* pfOut, char* pf32Arr, int len)
{
	// fprintf(pfOut, "%d,", len);
	writeText(pfOut, len);
	fwrite(pf32Arr, sizeof(char), len, pfOut);
}

// 变量读写
void writeString(FILE* pfOut, std::string str)
{
	writeText(pfOut, (int)(str.length()));
	fwrite(str.c_str(), 1, str.length(), pfOut);
}

void writeText(FILE* pfOut, int val)
{
	fwrite(&val, sizeof(int), 1, pfOut);
}

void writeText(FILE* pfOut, float val)
{
	fwrite(&val, sizeof(float), 1, pfOut);
}
void writeText(FILE* pfOut, char val)
{
	fwrite(&val, sizeof(char), 1, pfOut);
}

void writeVector(FILE* pfOut, std::vector<float> val)
{
	int total = val.size();
	writeText(pfOut, total);
	for (int i = 0; i < total; i++)
	{
		writeText(pfOut, val[i]);
	}
}

void readText(FILE* pfIn, int& val)
{
	fread(&val, sizeof(int), 1, pfIn);
}

void readText(FILE* pfIn, float& val)
{
	fread(&val, sizeof(float), 1, pfIn);
}

void readText(FILE* pfIn, char& val)
{
	fread(&val, sizeof(char), 1, pfIn);
}

void readVector(FILE* pfIn, std::vector<float>& vecVal)
{
	vecVal.clear();

	int total = 0;
	readText(pfIn, total);

	if (vecVal.size() > 0)
	{
		for (int i = 0; i < total; i++)
		{
			float fval = 0;
			readText(pfIn, fval);
			vecVal[i] = (fval);
		}
	}
	else
	{
		for (int i = 0; i < total; i++)
		{
			float fval = 0;
			readText(pfIn, fval);
			vecVal.push_back(fval);

			//std::cout << fval << std::endl;
		}
	}
}
void readVector2Buf(FILE* pfIn, float * pArr)
{
	int total = 0;
	readText(pfIn, total);

	fread(pArr, sizeof(float), total, pfIn);
	//for (int i = 0; i < total; i++)
	//{
	//	readText(pfIn, pArr[i]);
	//}
}
// true：文件结尾，
CTAPI bool fileEof(FILE* pfIn)
{
	return feof(pfIn) == EOF;
}

// 文件头，记录文件中有多少个条特征
void writeHeader(FILE* pfOut, int num)
{
	fseek (pfOut, 0, SEEK_SET);
	writeText(pfOut, num);
	fseek(pfOut, 0, SEEK_END);
}

void writeHeader3P(FILE* pfOut, int num, int dim)
{
	fseek(pfOut, 0, SEEK_SET);
	writeText(pfOut, num);
	writeText(pfOut, dim);
	fseek(pfOut, 0, SEEK_END);
}

// 读文件头
void readHeader(FILE* pfOut, int& num)
{
	int curpos = ftell(pfOut);
	if (curpos == 0)
	{
		// 当前位置在文件开头
		readText(pfOut, num);
	}
	else
	{
		fseek (pfOut, 0, SEEK_SET);
		readText(pfOut, num);
		fseek (pfOut, curpos, SEEK_SET);
	}
}

void readHeader3P(FILE* pfOut, int& num, int &dim)
{
	int curpos = ftell(pfOut);
	if (curpos == 0)
	{
		// 当前位置在文件开头
		readText(pfOut, num);
		readText(pfOut, dim);
	}
	else
	{
		fseek(pfOut, 0, SEEK_SET);
		readText(pfOut, num);
		readText(pfOut, dim);
		fseek(pfOut, curpos, SEEK_SET);
	}
}

// 多线程测试时，vector分段(平均)
std::vector<std::string> vecStrSection(int totalSection, int curId/*从0开始*/,
									   std::vector<std::string> vecAll)
{
	std::vector<std::string> vecRes;
	int onePartLen = vecAll.size() / totalSection;

	// 最后一段
	if (curId == totalSection - 1)
	{
		vecRes.assign(vecAll.begin() + curId*onePartLen, vecAll.end()); 
	}
	else
	{
		vecRes.assign(vecAll.begin() + curId*onePartLen, vecAll.begin() + (curId+1)*onePartLen); 
	}

	return vecRes;
}

// 多线程测试时，vector分段(平均)
CTAPI std::vector<TImgSet> vecStrSection(int totalSection, int curId/*从0开始*/,
										 std::vector<TImgSet> vecAll)
{
	std::vector<TImgSet> vecRes;
	int onePartLen = vecAll.size() / totalSection;

	// 最后一段
	if (curId == totalSection - 1)
	{
		vecRes.assign(vecAll.begin() + curId*onePartLen, vecAll.end()); 
	}
	else
	{
		vecRes.assign(vecAll.begin() + curId*onePartLen, vecAll.begin() + (curId+1)*onePartLen); 
	}

	return vecRes;
}