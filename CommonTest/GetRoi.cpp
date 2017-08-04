/******************************************************************************
根据一张图像的rect，获取对应的roi图像
2016-04-03
Sandy Yann
******************************************************************************/

#include <string.h>
#include "CommonTest.h"
#include <opencv2/opencv.hpp>
#include "ai_defs.h"


// 获得roi yxp版本
#if 1

// 超出边界部分补色为110
cv::Mat getRoiFromMat1(cv::Mat src, cv::Rect roiRt)
{
	if (roiRt.width <=0 || roiRt.height <= 0)
	{
		PrintfLogArg("paramter err\n");
	}

	//cv::rectangle(src, roiRt, cv::Scalar(0,255,0), 2);
	//cv::imshow("src", src);
	//cv::waitKey(0);

	cv::Mat roi = cv::Mat(roiRt.height, roiRt.width, src.type());
	roi.setTo(110);

	// 在原始图像中的真实位置
	int x1,x2,y1,y2;
	x1 = MAX(0, roiRt.x);
	y1 = MAX(0, roiRt.y);
	x2 = MIN(src.cols - 1, roiRt.width + roiRt.x - 1);
	y2 = MIN(src.rows - 1, roiRt.height + roiRt.y - 1);

	int rw = x2 - x1 + 1;
	int rh = y2 - y1 + 1;

	// 对应到roi图像中的开始位置
	int roix1 = MAX(0, -roiRt.x);
	int roiy1 = MAX(0, -roiRt.y);

	uchar* psrcbuf = src.data + y1 * src.step + src.channels()*x1;
	uchar* proibuf = roi.data + roiy1 * roi.step + roi.channels()*roix1;
	for (int h = 0; h < rh; h++)
	{
		memcpy(proibuf, psrcbuf, rw * src.channels());
		psrcbuf += src.step;
		proibuf += roi.step;
	}

	return roi;
}

// 根据车牌位置的中心，向车牌四周按宽度比例扩边，图像外，补值110
cv::Mat getRoiFromMatByLpPos(cv::Mat src, cv::Rect tRtLp, float f32L,float f32R,float f32T,float f32B, int l32LpEx)
{
	// 车牌的中心
	int cx = tRtLp.x + tRtLp.width/2;
	int cy = tRtLp.y + tRtLp.height/2;

	// 根据中心，向4周扩边
	int x1 = (int)(cx - f32L * tRtLp.width);
	int x2 = (int)(cx + f32R * tRtLp.width);
	int y1 = (int)(cy - f32T * tRtLp.width);
	int y2 = (int)(cy + f32B * tRtLp.width);

	cv::Rect rtRoi;
	rtRoi.x=x1;
	rtRoi.y=y1;
	rtRoi.width = x2 - x1 + 1;
	rtRoi.height = y2 - y1 + 1;

	cv::Mat roiMat = getRoiFromMat1(src, rtRoi);

	// 抹黑roi中的车牌
	// 对应到roi图像中的开始位置
	// 扩边后的新车牌位置，用于抹黑roi中的车牌
	cv::Rect rtLp2 = tRtLp;
	rtLp2.x -= l32LpEx;
	rtLp2.y -= l32LpEx;
	rtLp2.width += 2*l32LpEx;
	rtLp2.height += 2*l32LpEx;

	l32 roiStartPosX = rtLp2.x - x1;
	l32 roiStartPosY = rtLp2.y - y1;

	// 检查车牌位置，在roi图像中是否越界
	l32 roiX1,roiX2,roiY1,roiY2;
	roiX1 = MAX(0, roiStartPosX);
	roiX2 = MIN(roiMat.cols - 1, roiStartPosX + rtLp2.width - 1);
	roiY1 = MAX(0, roiStartPosY);
	roiY2 = MIN(roiMat.rows - 1, roiStartPosY + rtLp2.height - 1);

	cv::Rect rtLpInRoi = cv::Rect(roiX1, roiY1, roiX2 - roiX1 + 1, roiY2 - roiY1 + 1);
	cv::Mat roiLp = roiMat(rtLpInRoi);
	roiLp.setTo(110);

	return roiMat;
}

/*=======================================================================
Function : 获取TImage的roi图像，超出边界部分补色为110  
Param    : ptSrc[in]	原始图像
		   ptRoi[in]	返回的roi图像，内存外层实现开辟，新的宽高，内部会赋值
		   tRtRoi[in]	roi位置
Return   : void
=========================================================================*/
void getRoiFromTImage1(const TImage *ptSrc, TImage *ptRoi, TRect tRtRoi)
{
	l32 l32Channel;
	u8 *pu8SrcBuf;
	u8 *pu8DstBuf;
	l32 l32SrcStride;
	l32 l32DstStride;
	l32 x1,x2,y1,y2;
	
	if(ptRoi == NULL || ptSrc == NULL)
	{
		return ;
	}

	// 设置结果图像width，height，内存外层已经开辟
	ptRoi->atPlane->l32Width = tRtRoi.l32Width;
	ptRoi->atPlane->l32Height = tRtRoi.l32Height;
	l32Channel = ptSrc->atPlane->l32Stride / ptSrc->atPlane->l32Width;
	ptRoi->atPlane->l32Stride = tRtRoi.l32Width * l32Channel;

	pu8SrcBuf = (u8 *)ptSrc->atPlane->pvBuffer;
	pu8DstBuf = (u8 *)ptRoi->atPlane->pvBuffer;
	l32SrcStride = ptSrc->atPlane->l32Stride;
	l32DstStride = ptRoi->atPlane->l32Stride;

	// 设置边界值为110,全部赋值
	memset(pu8DstBuf, 110, ptRoi->atPlane->l32Stride*ptRoi->atPlane->l32Height);
	memset(pu8DstBuf + ptRoi->atPlane->l32Stride*ptRoi->atPlane->l32Height, 128, ptRoi->atPlane->l32Width*ptRoi->atPlane->l32Height/2);

	// 在原始图像中的真实位置
	x1 = MAX(0, tRtRoi.l32Left);
	y1 = MAX(0, tRtRoi.l32Top);
	x2 = MIN(ptSrc->atPlane->l32Width - 1, tRtRoi.l32Width + tRtRoi.l32Left - 1);
	y2 = MIN(ptSrc->atPlane->l32Height - 1, tRtRoi.l32Height + tRtRoi.l32Top - 1);

	int rw = x2 - x1 + 1;
	int rh = y2 - y1 + 1;

	// 对应到roi图像中的开始位置
	int roix1 = MAX(0, -tRtRoi.l32Left);
	int roiy1 = MAX(0, -tRtRoi.l32Top);

	uchar* psrcbuf = pu8SrcBuf + y1 * l32SrcStride + l32Channel*x1;
	uchar* proibuf = pu8DstBuf + roiy1 * l32DstStride + l32Channel*roix1;
	for (int h = 0; h < rh; h++)
	{
		memcpy(proibuf, psrcbuf, rw * l32Channel);
		psrcbuf += l32SrcStride;
		proibuf += l32DstStride;
	}


	if (ptSrc->u32Type == AI_NV12)
	{
		ptRoi->atPlane[1].pvBuffer = (u8*)ptRoi->atPlane->pvBuffer + tRtRoi.l32Width * tRtRoi.l32Height;
		psrcbuf = pu8SrcBuf + ptSrc->atPlane->l32Stride * ptSrc->atPlane->l32Height + y1 / 2 * l32SrcStride + l32Channel*x1;
		proibuf = (u8*)ptRoi->atPlane[1].pvBuffer + roiy1 /2 * l32DstStride + l32Channel*roix1;
		for (int h = 0; h < rh/2; h++)
		{
			memcpy(proibuf, psrcbuf, rw * l32Channel);
			psrcbuf += l32SrcStride;
			proibuf += l32DstStride;
		}
	}
	else if(ptSrc->u32Type == AI_I420)
	{
		// u
		ptRoi->atPlane[1].pvBuffer = (u8*)ptRoi->atPlane->pvBuffer + tRtRoi.l32Width * tRtRoi.l32Height;
		psrcbuf = pu8SrcBuf + ptSrc->atPlane->l32Stride * ptSrc->atPlane->l32Height + (y1 * l32SrcStride / 2 + l32Channel*x1)/2;
		proibuf = (u8*)ptRoi->atPlane[1].pvBuffer + (roiy1 * l32DstStride / 2 + l32Channel*roix1)/2;
		for (int h = 0; h < rh/2; h++)
		{
			memcpy(proibuf, psrcbuf, rw * l32Channel / 2);
			psrcbuf += l32SrcStride/2;
			proibuf += l32DstStride/2;
		}

		// v
		ptRoi->atPlane[2].pvBuffer = (u8*)ptRoi->atPlane->pvBuffer + tRtRoi.l32Width * tRtRoi.l32Height * 5 / 4;
		psrcbuf = pu8SrcBuf + ptSrc->atPlane->l32Stride * ptSrc->atPlane->l32Height * 5 / 4 + (y1 * l32SrcStride / 2 + l32Channel*x1)/2;
		proibuf = (u8*)ptRoi->atPlane[2].pvBuffer + (roiy1 * l32DstStride / 2 + l32Channel*roix1)/2;
		for (int h = 0; h < rh/2; h++)
		{
			memcpy(proibuf, psrcbuf, rw * l32Channel / 2);
			psrcbuf += l32SrcStride/2;
			proibuf += l32DstStride/2;
		}
	}
	else
	{
		PrintfLogArg("only support yuv420 or nv12\n");
	}
}

/*=======================================================================
Function : 根据车牌位置，获得扩边后的roi，如果扩充到图像外，补值110，
		   同时车牌四边扩充lpex像素后，车牌位置也设置成110   
Param    : ptSrc[in]	原始图像
		   ptRoi[in]	返回的roi图像，内存外层实现开辟，新的宽高，内部会赋值
		   f32L,f32R,f32T,f32B[in] 根据车牌位置，向左右上下各扩充车牌宽度的倍数
		   lpex[in]		车牌四周扩边像素数
Return   : void
=========================================================================*/
void getRoiTImageByLpPos(const TImage *ptSrc, TImage *ptRoi, const TRect tRtLp, 
						 float f32L,float f32R,float f32T,float f32B, l32 l32LpEx)
{
	// 新车牌位置，用于抹黑roi中的车牌
	TRect rtLp2 = tRtLp;
	rtLp2.l32Left -= l32LpEx;
	rtLp2.l32Top -= l32LpEx;
	rtLp2.l32Width += 2*l32LpEx;
	rtLp2.l32Height += 2*l32LpEx;

	int cx = tRtLp.l32Left + tRtLp.l32Width/2;
	int cy = tRtLp.l32Top + tRtLp.l32Height/2;

	int x1 = (int)(cx - f32L * tRtLp.l32Width);
	int x2 = (int)(cx + f32R * tRtLp.l32Width);
	int y1 = (int)(cy - f32T * tRtLp.l32Width);
	int y2 = (int)(cy + f32B * tRtLp.l32Width);

	TRect tRtRoi;
	tRtRoi.l32Left=x1;
	tRtRoi.l32Top=y1;
	tRtRoi.l32Width = x2 - x1 + 1;
	tRtRoi.l32Height = y2 - y1 + 1;

	getRoiFromTImage1(ptSrc, ptRoi, tRtRoi);

	// 将车牌设置为110，有个假设，车牌在原始图像一定不是越界的。
	// 对应到roi图像中的开始位置
	l32 roiStartPosX = rtLp2.l32Left - x1;
	l32 roiStartPosY = rtLp2.l32Top - y1;

	l32 l32Channel = ptSrc->atPlane->l32Stride / ptSrc->atPlane->l32Width;

	// 检查车牌位置，在roi图像中是否越界
	l32 roiX1,roiX2,roiY1,roiY2;
	roiX1 = MAX(0, roiStartPosX);
	roiX2 = MIN(ptRoi->atPlane->l32Width - 1, roiStartPosX + rtLp2.l32Width - 1);
	roiY1 = MAX(0, roiStartPosY);
	roiY2 = MIN(ptRoi->atPlane->l32Height - 1, roiStartPosY + rtLp2.l32Height - 1);

	u8* pu8LpBuf = (u8*)ptRoi->atPlane->pvBuffer + roiY1 * ptRoi->atPlane->l32Stride + roiX1 * l32Channel;
	l32 l32CopySize = MAX(0, (roiX2 - roiX1)*l32Channel);
	for (int h = 0; h < (roiY2 - roiY1); h++)
	{
		memset(pu8LpBuf, 110, l32CopySize);
		pu8LpBuf += ptRoi->atPlane->l32Stride;
	}
}

#endif