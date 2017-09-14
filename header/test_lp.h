/*********************************************************
* Licence plate detect and recognize related test tool
* Sandy Yann. Sep. 11 2017
*********************************************************/

#ifndef _TEST_LP_H_
#define _TEST_LP_H_

#if 1	// 和opencv相关
#include <opencv2/opencv.hpp>
// 获取Mat的roi图像，超出边界部分补色为110
CTAPI cv::Mat getRoiFromMat1(cv::Mat src, cv::Rect roiRt);
// 根据车牌位置的中心，向车牌四周按宽度比例扩边，图像外，补值110
CTAPI cv::Mat getRoiFromMatByLpPos(cv::Mat src, cv::Rect tRtLp, float f32L, float f32R, float f32T, float f32B, int l32LpEx/*车牌扩边像素*/);

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

CTAPI void imshowTImage(const std::string& winname, TImage* ptImg, int flags = 1);
CTAPI void waitKeyTImage(int delay = 0);

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
	float f32L, float f32R, float f32T, float f32B, l32 l32LpEx);

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
CTAPI void parsePosAns(std::string strSetFn, std::vector<std::string>& vecFn, std::vector<std::vector<TRect> >& vvPosRt);
CTAPI void parsePosAns(std::string strSetFn, std::vector<std::string>& vecFn, std::vector<std::vector<cv::Rect> >& vvPosRt);

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


#endif /* _TEST_LP_H_ */