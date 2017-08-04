#include "CommonTest.h"
#include "ai_defs.h"


#define Clip(x) ((x) > 255 ? 255 : ((x) < 0 ? 0 : (x)))
/*=============================================================================
函数名: ConvertRGB2NV12
功   能: RGB转YUV
算法实现: 
全局变量: 无
参   数: pu8RGB			RGB指针
		  puYUV420   		YUV指针
		  l32Width			图像宽度
		  l32Height			图像高度
返回值: 无
=============================================================================*/
void  ConvertRGB2NV12(u8 *puYUVSP, u8 *pu8RGB, l32 l32Width, l32 l32Height, l32 stride)
{
    l32 l32w,l32h;
    u8* pu8Y, *pu8UV;
    u8* pu8RGBBuf;
    l32 YSize;
	l32 l32HalfWidth = l32Width >> 1;
	l32 l32HalfHeight = l32Height >> 1;

    // Y
    for(l32h = 0; l32h < l32Height; l32h++)
    {
        pu8Y = puYUVSP + l32h * l32Width;
        pu8RGBBuf = pu8RGB + l32h * stride;

        for(l32w = 0; l32w < l32Width; l32w++)
        {
            pu8Y[0] = Clip(((66 * pu8RGBBuf[2] + 129 * pu8RGBBuf[1] + 25 * pu8RGBBuf[0] + 128) >> 8) + 16);
            pu8Y++;
            pu8RGBBuf += 3;
        }
    }

    // uv
    YSize = l32Width * l32Height;
    for(l32h = 0; l32h < l32HalfHeight; l32h++)
    {
        pu8UV = puYUVSP + l32h * l32Width + YSize;
        //pu8RGBBuf = pu8RGB + l32h * 6 * l32Width + stride;
		pu8RGBBuf = pu8RGB + l32h * 2 * stride + stride;

        for(l32w = 0; l32w < l32HalfWidth; l32w++)
        {
            pu8UV[0] = Clip(((-38 * pu8RGBBuf[2] - 74 * pu8RGBBuf[1] + 112 * pu8RGBBuf[0] + 128) >> 8) + 128);
            pu8UV[1] = Clip(((112 * pu8RGBBuf[5] - 94 * pu8RGBBuf[4] - 18 * pu8RGBBuf[3] + 128) >> 8) + 128);
            pu8UV += 2;
            pu8RGBBuf += 6;
        }
    }
}

/*=============================================================================
函数名: ConvertRGB2I420
功   能: 
算法实现: 无
全局变量: 无
参   数: 
返回值: 无
=============================================================================*/
void  ConvertRGB2I420(u8 *puYUV420, u8 *pu8RGB, l32 l32Width, l32 l32Height, l32 l32Stride)
{
	l32 l32w,l32h;
	u8* pu8Y, *pu8U, *pu8V;
	u8* pu8RGBBuf;
	l32 l32YSize;
	l32 l32FiveQuarterYSize;
	l32 l32HalfWidth = l32Width >> 1;
	l32 l32HalfHeight = l32Height >> 1;

	// Y
	for(l32h = l32Height - 1; l32h >= 0; l32h--)
	{
		pu8Y = puYUV420 + l32h * l32Width;
		pu8RGBBuf = pu8RGB + l32h * l32Stride;

		for(l32w = 0; l32w < l32Width; l32w++)
		{
			pu8Y[0] = Clip(((66 * pu8RGBBuf[2] + 129 * pu8RGBBuf[1] + 25 * pu8RGBBuf[0] + 128) >> 8) + 16);
			pu8Y++;
			pu8RGBBuf += 3;
		}
	}

	// u
	l32YSize = l32Width * l32Height;
	for(l32h = 0; l32h < l32HalfHeight; l32h++)
	{
		pu8U = puYUV420 + l32h * l32HalfWidth + l32YSize;
		//pu8RGBBuf = pu8RGB + l32h * 6 * l32Width + l32Stride;
		pu8RGBBuf = pu8RGB + l32h * 2 * l32Stride + l32Stride;

		for(l32w = 0; l32w < l32HalfWidth; l32w++)
		{
			pu8U[0] = Clip(((-38 * pu8RGBBuf[2] - 74 * pu8RGBBuf[1] + 112 * pu8RGBBuf[0] + 128) >> 8) + 128);
			pu8U++;
			pu8RGBBuf += 6;
		}
	}

	// v
	l32FiveQuarterYSize = l32YSize + (l32YSize >> 2);
	for(l32h = 0; l32h < l32HalfHeight; l32h++)
	{
		pu8V = puYUV420 + l32h * l32HalfWidth + l32FiveQuarterYSize;
		//pu8RGBBuf = pu8RGB + l32h * 6 * l32Width + l32Stride + 3;
		pu8RGBBuf = pu8RGB + l32h * 2 * l32Stride + l32Stride + 3;

		for(l32w = 0; l32w < l32HalfWidth; l32w++)
		{
			pu8V[0] = Clip(((112 * pu8RGBBuf[2] - 94 * pu8RGBBuf[1] - 18 * pu8RGBBuf[0] + 128) >> 8) + 128);
			pu8V++;
			pu8RGBBuf += 6;
		}
	}
}


#define Clip(x) ((x) > 255 ? 255 : ((x) < 0 ? 0 : (x)))

void FveCvtNV12RGB24(const u8 *puYUVSP, l32 l32Width, l32 l32Height, u8 *pu8RGB, l32 l32DstStride)
{
	l32 l32w,l32h;
	u8* pu8Y, *pu8UV;
	u8* pu8RGBBuf;
	u8 u8Y, u8U, u8V;
	l32 l32YSize;
	l32 l32HalfWidth;
	l32 l32Stride;

	l32 l32C, l32D, l32E;

	l32YSize = l32Width * l32Height;
	l32HalfWidth = (l32Width >> 1);
	l32Stride = l32Width * 3;

	if (l32Height%2 == 1)// 最后一行，rgb=y
	{
		l32h = l32Height - 1;
		pu8Y = (u8*)(puYUVSP + l32h * l32Width);
		pu8RGBBuf = pu8RGB + l32h * l32Stride;
		for(l32w = 0; l32w < l32Width; l32w++)
		{
			pu8RGBBuf[0] = pu8RGBBuf[1] = pu8RGBBuf[2] = pu8Y[0];
			pu8RGBBuf+=3;
			pu8Y ++;
		}
		l32Height--;
	}

	pu8UV = (u8*)(puYUVSP + l32YSize);
	for(l32h = 0; l32h < l32Height; l32h++)
	{
		pu8Y = (u8*)(puYUVSP + l32h * l32Width);
		pu8RGBBuf = pu8RGB + l32h * l32Stride;

		for(l32w = 0; l32w < l32Width; l32w++)
		{
			u8Y = pu8Y[0]; 
			u8U = pu8UV[(l32h >> 1) * l32Width + (l32w&0XFFFE)];
			u8V = pu8UV[(l32h >> 1) * l32Width + (l32w&0XFFFE) + 1];

			l32C = u8Y - 16;
			l32D = u8U - 128;
			l32E = u8V - 128;

			pu8RGBBuf[0] = (u8)(Clip(( 298 * l32C + 516 * l32D + 128) >> 8));
			pu8RGBBuf[1] = (u8)(Clip(( 298 * l32C - 100 * l32D - 208 * l32E + 128) >> 8));
			pu8RGBBuf[2] = (u8)(Clip(( 298 * l32C  + 409 * l32E + 128) >> 8));

			pu8Y++;
			pu8RGBBuf += 3;
		}
	}
}

void FvevehI420RGB24(TImage *tYuvImg, l32 l32Width, l32 l32Height, u8 *pu8RGB, l32 l32DstStride, BOOL bFlip)
{
    l32 l32w,l32h;
    u8 *pu8Y, *pu8U, *pu8V;
    u8 *pu8RGBBuf;
    u8 u8Y, u8U, u8V;
    l32 l32HalfWidth;
    //l32 l32Stride;
    l32 l32C, l32D, l32E;
	int nImgSize = l32Width * l32Height;

    l32HalfWidth = (l32Width >> 1);
    //l32Stride = l32Width * 3;

    // Y
    pu8Y = (u8 *)tYuvImg->atPlane[0].pvBuffer;
    pu8U = (u8 *)tYuvImg->atPlane[0].pvBuffer + nImgSize;
    pu8V = (u8 *)tYuvImg->atPlane[0].pvBuffer + nImgSize * 5 / 4;
    for(l32h = 0; l32h < l32Height; l32h++)
    {
        pu8Y = (u8*)((u8 *)tYuvImg->atPlane[0].pvBuffer + l32h * l32Width);
        if(bFlip)
        {
            pu8RGBBuf = (u8*)(pu8RGB + (l32Height - l32h - 1) * l32DstStride);
        }
        else
        {
            pu8RGBBuf = (u8*)(pu8RGB + (l32h) * l32DstStride);
        }

        for(l32w = 0; l32w < l32Width; l32w++)
        {
            u8Y = pu8Y[0]; 
            u8U = pu8U[(l32h >> 1) * l32HalfWidth + (l32w >> 1)];
            u8V = pu8V[(l32h >> 1) * l32HalfWidth + (l32w >> 1)];

            l32C = u8Y - 16;
            l32D = u8U - 128;
            l32E = u8V - 128;

            pu8RGBBuf[0] = (u8)(Clip(( 298 * l32C + 516 * l32D + 128) >> 8));
            pu8RGBBuf[1] = (u8)(Clip(( 298 * l32C - 100 * l32D - 208 * l32E + 128) >> 8));
            pu8RGBBuf[2] = (u8)(Clip(( 298 * l32C  + 409 * l32E + 128) >> 8));

            pu8Y++;
            pu8RGBBuf += 3;
        }
    }

	/*Mat bgr(l32Height, l32Width, CV_8UC3, pu8RGB);
	namedWindow("bgr", 0);
	imshow("bgr", bgr);
	waitKey();*/
}

void Yuv420ToRGB24(const char *pYuv, l32 l32Width, l32 l32Height, u8 *pu8RGB, l32 l32DstStride, bool bFlip)
{
    l32 l32w,l32h;
    u8 *pu8Y, *pu8U, *pu8V;
    u8 *pu8RGBBuf;
    u8 u8Y, u8U, u8V;
    l32 l32HalfWidth;
    l32 l32C, l32D, l32E;

    l32HalfWidth = (l32Width >> 1);

    // Y
    pu8Y = (u8 *)pYuv;
    pu8U = (u8 *)pYuv + l32Width * l32Height;
    pu8V = (u8 *)pu8U + l32Width * l32Height / 4;
    for(l32h = 0; l32h < l32Height; l32h++)
    {
        pu8Y = (u8*)((u8 *)pYuv + l32h * l32Width);
        if(bFlip)
        {
            pu8RGBBuf = (u8*)(pu8RGB + (l32Height - l32h - 1) * l32DstStride);
        }
        else
        {
            pu8RGBBuf = (u8*)(pu8RGB + (l32h) * l32DstStride);
        }

        for(l32w = 0; l32w < l32Width; l32w++)
        {
            u8Y = pu8Y[0]; 
            u8U = pu8U[(l32h >> 1) * l32HalfWidth + (l32w >> 1)];
            u8V = pu8V[(l32h >> 1) * l32HalfWidth + (l32w >> 1)];

            l32C = u8Y - 16;
            l32D = u8U - 128;
            l32E = u8V - 128;

            pu8RGBBuf[0] = (u8)(Clip(( 298 * l32C + 516 * l32D + 128) >> 8));
            pu8RGBBuf[1] = (u8)(Clip(( 298 * l32C - 100 * l32D - 208 * l32E + 128) >> 8));
            pu8RGBBuf[2] = (u8)(Clip(( 298 * l32C  + 409 * l32E + 128) >> 8));

            pu8Y++;
            pu8RGBBuf += 3;
        }
    }

	/*Mat bgr(l32Height, l32Width, CV_8UC3, pu8RGB);
	namedWindow("bgr", 0);
	imshow("bgr", bgr);
	waitKey();*/
}

TImage* creatTImage(cv::Mat src, EImageType eImgType)
{
	TImage* ptImg = new TImage();
	int nWidth = src.cols;
	int nHeight = src.rows;
	u8 * pu8YUV420 = new u8[src.step * src.rows];

	ptImg->u32Type = eImgType; //AI_NV12; //AI_I420;
	ptImg->atPlane[0].l32Width = nWidth;
	ptImg->atPlane[0].l32Height = nHeight;

	if (AI_I420 == eImgType || AI_NV12 == eImgType)
	{
		ptImg->atPlane[0].l32Stride = nWidth;
		ptImg->atPlane[1].l32Width = nWidth >> 1;
		ptImg->atPlane[1].l32Height = nHeight >> 1;
		ptImg->atPlane[1].l32Stride = nWidth >> 1;
		ptImg->atPlane[2].l32Width = nWidth >> 1;
		ptImg->atPlane[2].l32Height = nHeight >> 1;
		ptImg->atPlane[2].l32Stride = nWidth >> 1;

		ptImg->atPlane[0].pvBuffer = pu8YUV420;
		ptImg->atPlane[1].pvBuffer = pu8YUV420 + nWidth * nHeight;
		ptImg->atPlane[2].pvBuffer = pu8YUV420 + nWidth * nHeight * 5 / 4;

		if(AI_I420 == eImgType)
		{
			ConvertRGB2I420(pu8YUV420, src.data, nWidth, nHeight, src.step);
		}
		else
		{
			ConvertRGB2NV12(pu8YUV420, src.data, nWidth, nHeight, src.step);
		}
	}
	else if(AI_RGB24 == eImgType)
	{
		ptImg->atPlane[0].l32Stride = src.step;
		ptImg->atPlane[0].pvBuffer = pu8YUV420;

		memcpy(pu8YUV420, src.data, src.step * src.rows);
	}
	else if(AI_Y == eImgType)
	{
		if(src.channels() > 1)
		{
			cv::cvtColor(src,src, 6/*CV_BGR2GRAY*/);
		}
		ptImg->atPlane[0].l32Stride = src.step;
		ptImg->atPlane[0].pvBuffer = pu8YUV420;
		memcpy(pu8YUV420, src.data, src.step * src.rows);
	}
	else
	{
		printf("Err: 目前不支持的类型");
		delete[] pu8YUV420;
		delete ptImg;
		return NULL;
	}

	//cv::Mat maty = cv::Mat(ptImg->atPlane->l32Height, ptImg->atPlane->l32Width, CV_8UC1, ptImg->atPlane->pvBuffer);
	//cv::imshow("y", maty);
	//cv::Mat matu = cv::Mat(ptImg->atPlane[1].l32Height, ptImg->atPlane[1].l32Width, CV_8UC1, ptImg->atPlane[1].pvBuffer);
	//cv::imshow("matu", matu);
	//cv::Mat matv = cv::Mat(ptImg->atPlane[2].l32Height, ptImg->atPlane[2].l32Width, CV_8UC1, ptImg->atPlane[2].pvBuffer);
	//cv::imshow("matv", matv);
	//cv::waitKey();

	return ptImg;
}

void releaesTImage(TImage** pptImg)
{
	TImage* ptImg = (TImage*)*pptImg;
	if (ptImg)
	{
		if(ptImg->atPlane[0].pvBuffer) 
		{
			delete[] ptImg->atPlane[0].pvBuffer;
		}
		delete ptImg;
		*pptImg = NULL;
	}
}

// yuv 转换成 mat
CTAPI cv::Mat Yuv2Mat(const char *pYuv420, int width, int height)
{
	cv::Mat src = cv::Mat(height, width, CV_8UC3);

	Yuv420ToRGB24(pYuv420, width, height, src.data, src.step, false);

	return src;
}