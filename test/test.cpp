
#if 1

#include "common_test.h"
#include <stdlib.h>
#include "color_space_convert.h"
#include "args_parser.h"

#include <algorithm>
#include <iostream>
using namespace std;

class CTestFeaReadWrite
{
public:
	CTestFeaReadWrite(){};
	~CTestFeaReadWrite(){};
	void test()
	{
		FILE* pf = fopen("c:\\test.dat", "wb");
		writeHeader(pf, 0);
		writeString(pf, "c:\\yxp\\����ƽ\\xx.jpg");

		char atext[1024] = {0};
		writeArray(pf, atext, 1024);

		writeString(pf, "c:\\yxp\\����ƽ\\xx1.jpg");
		writeArray(pf, atext, 1024);
		writeString(pf, "c:\\yxp\\����ƽ\\xx2.jpg");
		writeArray(pf, atext, 1024);

		writeHeader(pf, 3);
		fclose(pf);

		FILE* pfOut = fopen("c:\\test.dat", "rb");
		int rownum = 0;
		readHeader(pfOut, rownum);

		string str = readString(pfOut);
		int fealen = 0;
		char* pfea = readArray(pfOut, fealen);
		delete[] pfea;

		rownum = 0;
		readHeader(pfOut, rownum);

		fealen = 0;
		str = readString(pfOut);
		pfea = readArray(pfOut, fealen);
		delete[] pfea;

		fealen = 0;
		str = readString(pfOut);
		pfea = readArray(pfOut, fealen);
		delete[] pfea;

		fclose(pfOut);
	}
};

void testWriteLog()
{
	//char atext[1024] = {0};		
	//sprintf(atext, "have parameter log %d\n", 10);	
	//printfLog(atext);

	{
/*		char atext[1024] = {0};		
		sprintf(atext, "have parameter log %d\n", 10);	
		printfLog(atext);	*/		
	}

	//printfLog("first test log\n");
	PrintfLogArg("have parameter log %d\n", 10);
}

void testMultiThrdPart()
{
	vector<string> vecStr;
	for (int i = 0; i < 109; i++)
	{
		char atxt[50] = { 0 };
		sprintf(atxt, "%d", i);
		vecStr.push_back(atxt);
	}

	vector<string> vecxx = vecStrSection(10, 5, vecStr);
}

int testGetRoi()
{
	cv::Mat src = cv::imread("d:\\000533.jpg", 1);
	cv::Rect rt = cv::Rect(100, src.rows-200, src.cols+100, 500);

	TImage* ptsrc = Mat2TImage(src, YUV);

	//cv::rectangle(src, rt, cv::Scalar(0,255,0), 2);
	// getRoiFromTImage1();

	TRect tRtRoi = Rect2TRect(rt);
	TImage tRoiImg;
	tRoiImg.pu8Data = (char*)malloc(tRtRoi.w * tRtRoi.h * 3 / 2);
	getRoiFromTImage1(ptsrc, &tRoiImg, tRtRoi);

	cv::Mat roi0 = cv::Mat(tRtRoi.h, tRtRoi.w, CV_8UC1, tRoiImg.pu8Data);
	cv::Mat roi1 = cv::Mat(tRtRoi.h / 2, tRtRoi.w / 2, CV_8UC1, tRoiImg.pu8Data + tRtRoi.w * tRtRoi.h);
	cv::Mat roi2 = cv::Mat(tRtRoi.h / 2, tRtRoi.w / 2, CV_8UC1, tRoiImg.pu8Data + tRtRoi.w * tRtRoi.h * 5 / 4);

	cv::Mat roi = cv::Mat(tRtRoi.h, tRtRoi.w, CV_8UC3);
	
	cv::namedWindow("src", 1);
	cv::imshow("src", roi0);

	cv::namedWindow("roi", 1);
	cv::imshow("roi", roi);

	cv::waitKey(0);

	return 0;
}

typedef int l32;
typedef unsigned char u8;
#define Clip(x) ((x) > 255 ? 255 : ((x) < 0 ? 0 : (x)))
void Yuv420ToBGR24(const char *pYuv, l32 l32Width, l32 l32Height, u8 *pu8RGB, l32 l32DstStride, bool bFlip)
{
	l32 l32w, l32h;
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
	for (l32h = 0; l32h < l32Height; l32h++)
	{
		pu8Y = (u8*)((u8 *)pYuv + l32h * l32Width);
		if (bFlip)
		{
			pu8RGBBuf = (u8*)(pu8RGB + (l32Height - l32h - 1) * l32DstStride);
		}
		else
		{
			pu8RGBBuf = (u8*)(pu8RGB + (l32h)* l32DstStride);
		}

		for (l32w = 0; l32w < l32Width; l32w++)
		{
			u8Y = pu8Y[0];
			u8U = pu8U[(l32h >> 1) * l32HalfWidth + (l32w >> 1)];
			u8V = pu8V[(l32h >> 1) * l32HalfWidth + (l32w >> 1)];

			l32C = u8Y - 16;
			l32D = u8U - 128;
			l32E = u8V - 128;

			pu8RGBBuf[0] = (u8)(Clip((298 * l32C + 516 * l32D + 128) >> 8));
			pu8RGBBuf[1] = (u8)(Clip((298 * l32C - 100 * l32D - 208 * l32E + 128) >> 8));
			pu8RGBBuf[2] = (u8)(Clip((298 * l32C + 409 * l32E + 128) >> 8));

			pu8Y++;
			pu8RGBBuf += 3;
		}
	}

	/*Mat bgr(l32Height, l32Width, CV_8UC3, pu8RGB);
	namedWindow("bgr", 0);
	imshow("bgr", bgr);
	waitKey();*/
}

int unit_test_color_space_convert()
{
	FILE* pf = fopen("..\\foreman.yuv", "rb");
	cv::Mat src = cv::Mat(288, 352, CV_8UC3);
	uint8_t *pNV12 = new uint8_t[352 * 288 * 3 / 2];

	fread(pNV12, 1, 352 * 288 * 3 / 2, pf);

	cv::Mat roiImg = getRoiFromNV12(pNV12, 352, 288, cv::Rect(50, 50, 100, 100));
	//nv12_to_bgr24(pNV12, 352, 288, src.data);
	//yuv_to_bgr24_fixpoint(pNV12, 352, 288, src.data);
	//yuv_to_bgr24(pNV12, 352, 288, src.data);
	//Yuv420ToBGR24((char*)pNV12, 352, 288, src.data, 352 * 3, false);

	cv::Mat gray = cv::Mat(288, 352, CV_8UC1, pNV12);
	cv::imshow("src", roiImg);
	cv::imshow("gray", gray);
	cv::waitKey(0);
	delete[] pNV12;
	fclose(pf);
	return 0;
}

int main(int argc, char** argv)
{
	return unit_test_args_parser(argc, argv);

	//return unit_test_color_space_convert();

	InitialLog("mylog", false, true, false);

	PERR("my error ");
	PWARNING("my WARNING ");
	//POUT("pout test %d ", 20);
	PLOG("this is a log");
	PLOG("test %d ", 20);
	return 0;
	return testGDB(argc, argv);
	
	//return testGetRoi();

	//string sss = "G:\\search_ͼ�����\\common_ͨ��Ŀ������\\ͨ��Ŀ����Լ�1\\lib.set";
	//std::vector<std::string> vecfn = readsample(sss);
 
	//CTestFeaReadWrite tfrw;
	//tfrw.test();

	//testMultiThrdPart();

	// std::string strSetFn = "D:\\svn_database\\trunk\\DiamondDetect_������Բ���v6.0\\ͼƬ\\Բ��ͼƬ\\dir.set.ans";
	// std::vector<std::string> vecFn;
	// std::vector<std::vector<TRect> > vvPosRt;
	// parsePosAns(strSetFn, vecFn, vvPosRt);

	//testWriteLog();

	return 0;
}
#endif
