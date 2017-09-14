#include "CommonTest.h"

using std::vector;
using std::string;

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

static bool PairCompare(const std::pair<float, int>& lhs,
						const std::pair<float, int>& rhs) {
							return lhs.first > rhs.first;
}

static void resize_ratio_same(cv::Mat src, cv::Mat& dst, int sz)
{
	dst = cv::Mat::zeros(sz, sz, src.type());
	dst.setTo(60);

	float rr = MAX((float)src.rows / sz, (float)src.cols / sz);

	resize(src, src, cv::Size((int)(src.cols / rr), (int)(src.rows / rr)));

	int sx = (sz - src.cols) / 2;
	int sy = (sz - src.rows) / 2;

	cv::Mat dstroi = dst(cv::Rect(sx, sy, src.cols, src.rows));
	src.copyTo(dstroi);
	//cv::rectangle(dst, cv::Rect(0,0, dst.cols, dst.rows), cv::Scalar(0,0,255), 1);
}

/*=======================================================================
Function : Show retrieval result  
Param    : vecScore[in] The score of each image
		   vecFn[in] Image library.
		   strRefFn[in] Reference image
		   bSave[in] false: show result; true: not show, directly save result
Return   : void
=========================================================================*/
CTAPI void showRetrievalResut(vector<float> vecScore, std::vector<std::string> vecFn,
							  std::string strRefFn, bool bSave,string svpath)
{
#define IMGW 256
#define IMGH 256
#define NOTEH 30
#define NOTEW 4
#define show_cols_num 6
#define show_rows_num 3
#define SHOW_NUM (show_cols_num*show_rows_num)

	int nShowNum = MIN(vecScore.size(), SHOW_NUM);

	// ≈≈–Ú
	std::vector<std::pair<float, int> > pairs;
	for (size_t i = 0; i < vecScore.size(); ++i)
		pairs.push_back(std::make_pair(vecScore[i], i));

	//std::partial_sort(pairs.begin(), pairs.begin() + nShowNum, pairs.end(), PairCompare);
	std::sort(pairs.begin(), pairs.end(), PairCompare);

	// ππΩ®ÕºœÒœ‘ æ
	cv::Mat src = cv::Mat::zeros(show_rows_num*(IMGH + NOTEH), show_cols_num*(IMGW + NOTEW), CV_8UC3);
	int idx = 0;
	for(int h = 0; h < show_rows_num; h++)
	{
		for (int w = 1; w < show_cols_num; w++)
		{
			if (idx >= (int)pairs.size())
			{
				continue;
			}
			int curid = pairs[idx].second;
			cv::Mat curImg = cv::imread(vecFn[curid]);
			if (curImg.empty())
			{
				std::cout << "can't read image: " << vecFn[curid] << std::endl;
				continue;
			}
			cv::Mat rsz;
			//cv::resize(curImg, rsz, cv::Size(IMGW, IMGH));
			resize_ratio_same(curImg, rsz, IMGW);

			int curX = w*(IMGW + NOTEW); int curY = h*(IMGH+NOTEH);
			cv::Mat dstroi = src(cv::Rect(curX, curY, IMGW, IMGH));
			rsz.copyTo(dstroi);

			// –¥Œƒ◊÷
			char text[256] = {0};
			sprintf(text, "%d:%d:%.3f", idx, curid,pairs[idx].first);
			cv::putText(src, text, cv::Point(curX, curY + IMGH + NOTEH - 10), 1, 1.5, cv::Scalar(0,255, 0),2);
			idx ++;
		}
	}
	cv::Mat refImg = cv::imread(strRefFn);
	if (refImg.empty())
	{
		std::cout << "can't open ref image!\n";
	}
	cv::Mat rsz;
	//cv::resize(refImg, rsz, cv::Size(IMGW, IMGH));
	resize_ratio_same(refImg, rsz, IMGW);

	cv::Mat dstroi = src(cv::Rect(0, 0, IMGW, IMGH));
	rsz.copyTo(dstroi);
	cv::putText(src, "Ref Img", cv::Point(0, 0 + IMGH + NOTEH - 10), 1, 1.5, cv::Scalar(0,255, 0),2);
	cv::imshow("res", src);
	if (bSave)
	{
		cv::waitKey(1);
		int subp = strRefFn.rfind("\\");
		string s1 = strRefFn.substr(subp);
		std::string svname = svpath + s1;
		cv::imwrite(svname, src);
	}
	else
		cv::waitKey(0);
}