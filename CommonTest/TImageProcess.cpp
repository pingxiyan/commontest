/************************************************************************
/* 和TImage相关的操作，用于timage显示相关的调试
	2016-1-29 Sandy Yann												*/
/************************************************************************/
#include "CommonTest.h"
#include "typedef.h"
#include <opencv2/opencv.hpp>

#include <string>
using std::string;


void imshowTImage(const string& winname, TImage* ptImg, int flags)
{
	cv::Mat src;
	if (ptImg->u32Type == BGR24)
	{
		src = cv::Mat(ptImg->l32Height, ptImg->l32Width, CV_8UC3, ptImg->pu8Data);
	}
	else if(ptImg->u32Type == GRAY)
	{
		src = cv::Mat(ptImg->l32Height, ptImg->l32Width, CV_8UC1, ptImg->pu8Data);
	}
	else if(ptImg->u32Type == I420)
	{

	}
	else if(ptImg->u32Type == NV12)
	{

	}
	else
	{
		std::cout << "Can't support TImage type\n";
	}

	cv::namedWindow(winname, flags);
	cv::imshow(winname, src);
};

void waitKeyTImage(int delay)
{
	cv::waitKey(delay);
};