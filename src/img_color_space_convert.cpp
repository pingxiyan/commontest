#include "typedef.h"
#include "color_space_convert.h"

#define RANGE_UCHAR(x) ((x) > 255 ? 255 : ((x) < 0 ? 0 : (x)))

/**
* @brief Color space convert: BGR24_to_NV12
* @param pNV12: src nv12
* @param pBGR24: dst BGR24
* @param width: image width
* @param height: image height
* @param ystep: 
*/
void bgr24_to_nv12(uint8_t *pNV12, uint8_t *pBGR24, int width, int height, int ystep)
{
    int w, h, sizeY;
	uint8_t* pY, *pUV, *pTmpBufBGR;
	int halfW = width >> 1;
	int halfH = height >> 1;

    // Y
    for(h = 0; h < height; h++)
    {
        pY = pNV12 + h * width;
        pTmpBufBGR = pBGR24 + h * ystep;

        for(w = 0; w < width; w++)
        {
            pY[0] = RANGE_UCHAR(((66 * pTmpBufBGR[2] + 129 * pTmpBufBGR[1] + 25 * pTmpBufBGR[0] + 128) >> 8) + 16);
            pY++;
            pTmpBufBGR += 3;
        }
    }

    // uv
    sizeY = width * height;
    for(h = 0; h < halfH; h++)
    {
        pUV = pNV12 + h * width + sizeY;
		pTmpBufBGR = pBGR24 + h * 2 * ystep + ystep;

        for(w = 0; w < halfW; w++)
        {
            pUV[0] = RANGE_UCHAR(((-38 * pTmpBufBGR[2] - 74 * pTmpBufBGR[1] + 112 * pTmpBufBGR[0] + 128) >> 8) + 128);
            pUV[1] = RANGE_UCHAR(((112 * pTmpBufBGR[5] - 94 * pTmpBufBGR[4] - 18 * pTmpBufBGR[3] + 128) >> 8) + 128);
            pUV += 2;
            pTmpBufBGR += 6;
        }
    }
}

/**
* @brief Color space convert: BGR24_to_YUV
* @param pNV12: src YUV, also called I420
* @param pBGR24: dst BGR24
* @param width: image width
* @param height: image height
* @param ystep:
*/
void bgr24_to_yuv(uint8_t *pYUV, uint8_t *pBGR24, int width, int height, int step)
{
	int w, h, sizeY, sizeOff;
	uint8_t *pY, *pU, *pV, *pTmpBufBGR;
	int halfW = width >> 1, halfH = height >> 1;

	// Y
	for(h = height - 1; h >= 0; h--) {
		pY = pYUV + h * width;
		pTmpBufBGR = pBGR24 + h * step;

		for(w = 0; w < width; w++) {
			pY[0] = RANGE_UCHAR(((66 * pTmpBufBGR[2] + 129 * pTmpBufBGR[1] + 25 * pTmpBufBGR[0] + 128) >> 8) + 16);
			pY++;
			pTmpBufBGR += 3;
		}
	}

	// u
	sizeY = width * height;
	for(h = 0; h < halfH; h++) {
		pU = pYUV + h * halfW + sizeY;
		pTmpBufBGR = pBGR24 + h * 2 * step + step;

		for(w = 0; w < halfW; w++) {
			pU[0] = RANGE_UCHAR(((-38 * pTmpBufBGR[2] - 74 * pTmpBufBGR[1] + 112 * pTmpBufBGR[0] + 128) >> 8) + 128);
			pU++;
			pTmpBufBGR += 6;
		}
	}

	// v
	sizeOff = sizeY + (sizeY >> 2);
	for(h = 0; h < halfH; h++) {
		pV = pYUV + h * halfW + sizeOff;
		pTmpBufBGR = pBGR24 + h * 2 * step + step + 3;

		for(w = 0; w < halfW; w++) {
			pV[0] = RANGE_UCHAR(((112 * pTmpBufBGR[2] - 94 * pTmpBufBGR[1] - 18 * pTmpBufBGR[0] + 128) >> 8) + 128);
			pV++;
			pTmpBufBGR += 6;
		}
	}
}

/**
* @brief nv12_to_bgr24, color space convert, fix-point calculate.
* @param pNV12: src NV12 image
* @param width: NV12 width
* @param height: NV12 height
* @param pBGR24: Out BGR24 image, size = width * height * 3
*/
void nv12_to_bgr24(const uint8_t *pNV12, int width, int height, uint8_t *pBGR24)
{
	int w, h, tmp1, tmp2, tmp3;
	const uint8_t* pY, *pUV;
	uint8_t* pTmpBGRBuf;
	int halfWidth = width >> 1;
	
	pTmpBGRBuf = pBGR24;
	pY = pNV12;
	pUV = pNV12 + width * height;
	for(h = 0; h < height; h++) {
		for(w = 0; w < width; w++) {
			uint8_t curY = pY[0];
			uint8_t curU = pUV[(h >> 1) * width + (w&0XFFFE)];
			uint8_t curV = pUV[(h >> 1) * width + (w&0XFFFE) + 1];

			tmp1 = curY - 16;
			tmp2 = curU - 128;
			tmp3 = curV - 128;
			
			pTmpBGRBuf[0] = (uint8_t)(RANGE_UCHAR(1.164f * tmp1 + 2.018f * tmp2));
			pTmpBGRBuf[1] = (uint8_t)(RANGE_UCHAR(1.164f * tmp1 - 0.813f * tmp3 + 0.391f * tmp2));
			pTmpBGRBuf[2] = (uint8_t)(RANGE_UCHAR(1.164f * tmp1 + 1.596f * tmp3));

			pY++;
			pTmpBGRBuf += 3;
		}
	}

	/* If height is not even number, need to process last line data */
	if (height % 2 == 1) {
		h = height - 1;
		pY = (pNV12 + h * width);
		pTmpBGRBuf = pBGR24 + h * width * 3;
		for (w = 0; w < width; w++) {
			pTmpBGRBuf[0] = pTmpBGRBuf[1] = pTmpBGRBuf[2] = pY[0];
			pTmpBGRBuf += 3;
			pY++;
		}
		height--;
	}
}

void nv12_to_bgr24_fixpoint(const uint8_t *pNV12, int width, int height, uint8_t *pBGR24)
{
	int w, h, tmp1, tmp2, tmp3;
	const uint8_t* pY, *pUV;
	uint8_t* pTmpBGRBuf;
	int halfWidth = width >> 1;

	pTmpBGRBuf = pBGR24;
	pY = pNV12;
	pUV = pNV12 + width * height;
	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			uint8_t curY = pY[0];
			uint8_t curU = pUV[(h >> 1) * width + (w & 0XFFFE)];
			uint8_t curV = pUV[(h >> 1) * width + (w & 0XFFFE) + 1];

			tmp1 = curY - 16;
			tmp2 = curU - 128;
			tmp3 = curV - 128;

			pTmpBGRBuf[0] = (uint8_t)(RANGE_UCHAR((298 * tmp1 + 516 * tmp2 + 128) >> 8));
			pTmpBGRBuf[1] = (uint8_t)(RANGE_UCHAR((298 * tmp1 - 100 * tmp2 - 208 * tmp3 + 128) >> 8));
			pTmpBGRBuf[2] = (uint8_t)(RANGE_UCHAR((298 * tmp1 + 409 * tmp3 + 128) >> 8));

			pY++;
			pTmpBGRBuf += 3;
		}
	}

	/* If height is not even number, need to process last line data */
	if (height % 2 == 1) {
		h = height - 1;
		pY = (pNV12 + h * width);
		pTmpBGRBuf = pBGR24 + h * width * 3;
		for (w = 0; w < width; w++) {
			pTmpBGRBuf[0] = pTmpBGRBuf[1] = pTmpBGRBuf[2] = pY[0];
			pTmpBGRBuf += 3;
			pY++;
		}
		height--;
	}
}

/**
* @brief YUV_to_BGR24, color space convert, fix-point calculate.
* @param pYUV: src YUV image
* @param width: YUV width
* @param height: YUV height
* @param pBGR24: Out BGR24 image, size = width * height * 3
*/
void yuv_to_bgr24_fixpoint(const uint8_t *pYUV, int width, int height, uint8_t *pBGR24)
{
	int w, h, tmp1, tmp2, tmp3;
	const uint8_t *pY, *pU, *pV;
	uint8_t *pBufBGR;
	int l32HalfWidth = (width >> 1);

	pY = pYUV;
	pU = pY + width * height;
	pV = pU + ((width * height) >> 2);
	pBufBGR = pBGR24;

	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			uint8_t curY = pY[0];
			int idxUV = (h >> 1) * l32HalfWidth + (w >> 1);
			uint8_t curU = pU[idxUV];
			uint8_t curV = pV[idxUV];

			tmp1 = (int)curY - 16;
			tmp2 = (int)curU - 128;
			tmp3 = (int)curV - 128;

			pBufBGR[0] = (uint8_t)(RANGE_UCHAR((298 * tmp1 + 516 * tmp2 + 128) >> 8));
			pBufBGR[1] = (uint8_t)(RANGE_UCHAR((298 * tmp1 - 100 * tmp2 - 208 * tmp3 + 128) >> 8));
			pBufBGR[2] = (uint8_t)(RANGE_UCHAR((298 * tmp1 + 409 * tmp3 + 128) >> 8));

			pY++;
			pBufBGR += 3;
		}
	}
}
// same to yuv_to_bgr24_fixpoint
void yuv_to_bgr24(const uint8_t *pYUV, int width, int height, uint8_t *pBGR24)
{
	int w, h, tmp1, tmp2, tmp3;
	const uint8_t *pY, *pU, *pV;
	uint8_t *pBufBGR;
	int l32HalfWidth = (width >> 1);

	pY = pYUV;
	pU = pY + width * height;
	pV = pU + ((width * height) >> 2);
	pBufBGR = pBGR24;

	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			uint8_t curY = pY[0];
			int idxUV = (h >> 1) * l32HalfWidth + (w >> 1);
			uint8_t curU = pU[idxUV];
			uint8_t curV = pV[idxUV];

			tmp1 = (int)curY - 16;
			tmp2 = (int)curU - 128;
			tmp3 = (int)curV - 128;

			pBufBGR[0] = (uint8_t)(RANGE_UCHAR(1.164f * tmp1 + 2.018f * tmp2));
			pBufBGR[1] = (uint8_t)(RANGE_UCHAR(1.164f * tmp1 - 0.813f * tmp3 + 0.391f * tmp2));
			pBufBGR[2] = (uint8_t)(RANGE_UCHAR(1.164f * tmp1 + 1.596f * tmp3));

			pY++;
			pBufBGR += 3;
		}
	}
}

/**
* @brief Convert YUV to OpenCV Mat(BGR24).
* @param pYUV: src YUV image, also called YUV420
* @param width: YUV width
* @param height: YUV height
* @return cv::Mat
*/
cv::Mat yuv_to_cvmat(const uint8_t *pYUV, int width, int height)
{
	cv::Mat src = cv::Mat(height, width, CV_8UC3);

	yuv_to_bgr24(pYUV, width, height, src.data);

	return src;
}