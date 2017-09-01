#if 1

#include "CommonTest.h"
#include <stdlib.h>


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
		writeString(pf, "c:\\yxp\\闫西平\\xx.jpg");

		char atext[1024] = {0};
		writeArray(pf, atext, 1024);

		writeString(pf, "c:\\yxp\\闫西平\\xx1.jpg");
		writeArray(pf, atext, 1024);
		writeString(pf, "c:\\yxp\\闫西平\\xx2.jpg");
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

	TImage* ptsrc = Mat2TImage(src, I420);

	//cv::rectangle(src, rt, cv::Scalar(0,255,0), 2);
	// getRoiFromTImage1();

	TRect tRtRoi = Rect2TRect(rt);
	TImage tRoiImg;
	tRoiImg.pu8Data = (u8*)malloc(tRtRoi.l32Width * tRtRoi.l32Height * 3 / 2);
	getRoiFromTImage1(ptsrc, &tRoiImg, tRtRoi);

	cv::Mat roi0 = cv::Mat(tRtRoi.l32Height, tRtRoi.l32Width, CV_8UC1, tRoiImg.pu8Data);
	cv::Mat roi1 = cv::Mat(tRtRoi.l32Height / 2, tRtRoi.l32Width / 2, CV_8UC1, tRoiImg.pu8Data + tRtRoi.l32Width * tRtRoi.l32Height);
	cv::Mat roi2 = cv::Mat(tRtRoi.l32Height / 2, tRtRoi.l32Width / 2, CV_8UC1, tRoiImg.pu8Data + tRtRoi.l32Width * tRtRoi.l32Height * 5 / 4);

	cv::Mat roi = cv::Mat(tRtRoi.l32Height, tRtRoi.l32Width, CV_8UC3);
	FvevehI420BGR24(&tRoiImg, roi.cols, roi.rows, roi.data, roi.step, 0);
	
	cv::namedWindow("src", 1);
	cv::imshow("src", roi0);

	cv::namedWindow("roi", 1);
	cv::imshow("roi", roi);

	cv::waitKey(0);

	return 0;
}

int main(int argc, char** argv)
{
	return testGDB(argc, argv);
	
	//return testGetRoi();

	//string sss = "G:\\search_图像检索\\common_通用目标搜素\\通用目标测试集1\\lib.set";
	//std::vector<std::string> vecfn = readsample(sss);
 
	//CTestFeaReadWrite tfrw;
	//tfrw.test();

	//testMultiThrdPart();

	// std::string strSetFn = "D:\\svn_database\\trunk\\DiamondDetect_菱形椭圆检测v6.0\\图片\\圆形图片\\dir.set.ans";
	// std::vector<std::string> vecFn;
	// std::vector<std::vector<TRect> > vvPosRt;
	// parsePosAns(strSetFn, vecFn, vvPosRt);

	//testWriteLog();

	return 0;
}
#endif