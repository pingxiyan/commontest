/************************************************************************/
/* 测试常用函数
	2015-11-24 Sandy Yann												*/
/************************************************************************/

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

#if 1	// 和opencv相关
#include <opencv2/opencv.hpp>
// 获取Mat的roi图像，超出边界部分补色为110
CTAPI cv::Mat getRoiFromMat1(cv::Mat src, cv::Rect roiRt);
// 根据车牌位置的中心，向车牌四周按宽度比例扩边，图像外，补值110
CTAPI cv::Mat getRoiFromMatByLpPos(cv::Mat src, cv::Rect tRtLp, float f32L,float f32R,float f32T,float f32B, int l32LpEx/*车牌扩边像素*/);

/* 针对车牌位置答案，保存和查看工具，对应的答案读写函数,
	针对位置类的测试，其实都可以用这个答案读写	*/
typedef struct
{
	std::vector<cv::Rect> vecRtPos;  // 一张图片中的车辆位置
	std::string fileName;       // 当前图片的名字
}TPosAns;  // 答案结构体

CTAPI std::vector<TPosAns> readPosAns(std::string strAnsFn);
CTAPI void writePosAns(std::string strAnsFn, std::vector<TPosAns> tPosAns);

#endif

// 和TImage相关调试，显示等功能
#if 1
#include "typedef.h"

CTAPI void imshowTImage(const std::string& winname, TImage* ptImg, int flags=1);
CTAPI void waitKeyTImage(int delay=0);

// 获取TImage的roi图像，超出边界部分补色为110
CTAPI void getRoiFromTImage1(const TImage *ptSrc, TImage *ptRoi, TRect tRtRoi);

// 根据车牌位置，获取扩充图片
CTAPI l32 GetRoi(const TRect tNumPlate, TImage* tSrc, TImage* tDst, const d64 d64Hor, const d64 d64Ver);

/*=======================================================================
Function : 根据车牌位置，获得扩边后的roi，如果扩充到图像外，补值110，
		   同时车牌四边扩充lpex像素后，车牌位置也设置成110   
Param    : ptSrc[in]	原始图像
		   ptRoi[in]	返回的roi图像，内存外层实现开辟，新的宽高，内部会赋值
		   f32L,f32R,f32T,f32B[in] 根据车牌位置，向左右上下各扩充车牌宽度的倍数
		   lpex[in]		车牌四周扩边像素数
Return   : void
=========================================================================*/
CTAPI void getRoiTImageByLpPos(const TImage *ptSrc, TImage *ptRoi, const TRect tRtLp, 
						 float f32L,float f32R,float f32T,float f32B, l32 l32LpEx);

inline TRect Rect2TRect(cv::Rect rt)
{
	TRect tRt;
	tRt.l32Left = rt.x;
	tRt.l32Top = rt.y;
	tRt.l32Width = rt.width;
	tRt.l32Height = rt.height;
	return tRt;
};
inline cv::Rect TRect2Rect(TRect tRt)
{
	cv::Rect rt;
	rt.x = tRt.l32Left;
	rt.y = tRt.l32Top;
	rt.width = tRt.l32Width;
	rt.height = tRt.l32Height;
	return rt;
};

// 解析车标文件名
CTAPI bool parseVehLogoFileName(std::string strfn, 
						  TRect& tLpPosition, 
						  char s8Plate[64], 
						  l32& l32LpHorizontalTheta);
// 解析位置答案文件
// 答案格式为：filename|x,y,w,h|x,y,w,h
CTAPI void parsePosAns(std::string strSetFn, std::vector<std::string>& vecFn, std::vector<std::vector<TRect>>& vvPosRt);
CTAPI void parsePosAns(std::string strSetFn, std::vector<std::string>& vecFn, std::vector<std::vector<cv::Rect>>& vvPosRt);

CTAPI void  ConvertRGB2NV12(u8 *puYUVSP, u8 *pu8RGB, l32 l32Width, l32 l32Height, l32 stride);
CTAPI void  ConvertRGB2I420(u8 *puYUV420, u8 *pu8RGB, l32 l32Width, l32 l32Height, l32 l32Stride);

CTAPI void FveCvtNV12BGR24(const u8 *puYUVSP, l32 l32Width, l32 l32Height, u8 *pu8RGB, l32 l32DstStride);
CTAPI void FvevehI420BGR24(TImage *tYuvImg, l32 l32Width, l32 l32Height, u8 *pu8RGB, l32 l32DstStride, BOOL bFlip);

// yuv 转换成 mat
CTAPI cv::Mat Yuv2Mat(const char *pYuv420, int width, int height);

CTAPI TImage* creatTImage(cv::Mat src, EImageType eImgType);
#define Mat2TImage creatTImage
CTAPI void releaesTImage(TImage** pptImg);

#endif


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


// 多线程测试
typedef void* (__stdcall*MultiOpen)(int idx/*线程序号*/);
typedef int (__stdcall*MultiProcess)(void* pvHandle, const char* fn, void *pvUser);
typedef int(__stdcall*MultiClose)(void*pvHandle);
CTAPI void multiThrdTest(std::string strSetFn,	// set文件名
						 int thrnum,	// 线程数目>=1
						 MultiOpen myOpen,
						 MultiProcess myProcess,
						 MultiClose myClose,
						 void *pvUser);
CTAPI void multiThrdTest(std::vector<std::string> vecFn,	// set文件名
	int thrnum,	// 线程数目>=1
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
		printf("Err: fun = ; lineNo = \n", __FUNCTION__, __LINE__);
		return FALSE;
	}

	fread(pBuf, 1, len, pfIn);
	pBuf[len] = 0;
	return TRUE;
}

// 数组读写
CTAPI void writeArray(FILE* pfOut, char* pf32Arr, int lenlen);
inline char* readArray(FILE* pfIn, int& len){
	readText(pfIn, len);
	//fscanf(pfIn, "%d,", &len);

	char* ptext = new char[len + 1];
	fread(ptext, 1, len, pfIn);
	ptext[len] = 0;

	return ptext;
}

// true：文件结尾，
CTAPI bool fileEof(FILE* pfIn);
#endif

// 解析配置文件部分
#if 1
// 打开配置文件，获取配置文件句柄
CTAPI void* readConfig(const char* ps8ConfigFn);
CTAPI void releaseConfig(void**ppvHandle);

// 根据配置文件句柄，获取某一类的值
CTAPI bool parseConfig(void* pvHandle, char* ps8Label, int &val);
CTAPI bool parseConfig(void* pvHandle, char* ps8Label, float &val);
CTAPI bool parseConfig(void* pvHandle, char* ps8Label, std::string & strOut);
CTAPI bool parseConfig(void* pvHandle, char* ps8Label, bool &val);
/* 测试用例
class CConfigParam
{
public:
	CConfigParam(std::string strConfigFn)
	{
		m_pvHandle = readConfig(strConfigFn.c_str());
		if (NULL == m_pvHandle)
		{
			std::cout << "can't open " << strConfigFn << " \n";
			exit(0);
		}

		parseConfig(m_pvHandle, "m_strAlgType",	m_strAlgType);
	}
	~CConfigParam()
	{
		releaseConfig(&m_pvHandle);
	}

	std::string m_strAlgType;
private:
	void *m_pvHandle;
	
};
*/
#endif

CTAPI std::string getExePath();

// 获得当前时间，精确到秒; 格式为：year-month-day h:m:s
CTAPI std::string getCurStrTime();
#if 1	// 日志管理
/* 初始化日志的名字，如果不初始化，使用默认名字log_时间.txt,
	会把一个月以前的日志文件删除；
	程序启动时，会初始化日志的文件名字	；	*/
CTAPI void InitialLog(const char* pLogFn);

// 所有的日志信息都会输出到log_time.log中
CTAPI void printfLog(const char* pLogText);
#define PrintfLogArg(_format, ...) {\
	char atext[1024] = { 0 };		\
	sprintf(atext, "%s_%d : ", __FUNCTION__, __LINE__);	\
	sprintf(atext + strlen(atext), _format, ##__VA_ARGS__);	\
	printfLog(atext);			\
}
#endif


#endif
