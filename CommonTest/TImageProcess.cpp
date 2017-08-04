/************************************************************************
/* 和TImage相关的操作，用于timage显示相关的调试
	2016-1-29 Sandy Yann												*/
/************************************************************************/
#include "CommonTest.h"
#include "ai_defs.h"
#include <opencv2/opencv.hpp>

#include <string>
using std::string;


void imshowTImage(const string& winname, TImage* ptImg, int flags)
{
	cv::Mat src;
	if (ptImg->u32Type == AI_RGB24)
	{
		src = cv::Mat(ptImg->atPlane->l32Height, ptImg->atPlane->l32Width, CV_8UC3, ptImg->atPlane->pvBuffer);
	}
	else if(ptImg->u32Type == AI_Y)
	{
		src = cv::Mat(ptImg->atPlane->l32Height, ptImg->atPlane->l32Width, CV_8UC1, ptImg->atPlane->pvBuffer);
	}
	else if(ptImg->u32Type == AI_I420)
	{

	}
	else if(ptImg->u32Type == AI_NV12)
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