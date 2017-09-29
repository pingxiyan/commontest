/*********************************************************
* Color space convert
* Sep. 20 2017
*********************************************************/

#include <stdio.h>
#include "typedef.h"
#include <opencv2/opencv.hpp>

#ifndef _COLOR_SPACE_CONVERT_H_
#define _COLOR_SPACE_CONVERT_H_

/**
* @brief Color space convert: BGR24_to_NV12
* @param pNV12: src nv12
* @param pBGR24: dst BGR24
* @param width: image width
* @param height: image height
* @param ystep:
*/
void bgr24_to_nv12(uint8_t *pNV12, uint8_t *pBGR24, int width, int height, int ystep);

/**
* @brief Color space convert: BGR24_to_YUV
* @param pNV12: src YUV, also called I420
* @param pBGR24: dst BGR24
* @param width: image width
* @param height: image height
* @param ystep:
*/
void bgr24_to_yuv(uint8_t *pYUV, uint8_t *pBGR24, int width, int height, int step);

/**
* @brief nv12_to_bgr24, color space convert, fix-point calculate, have accuracy loss.
* @param pNV12: src NV12 image
* @param width: NV12 width
* @param height: NV12 height
* @param pBGR24: Out BGR24 image, size = width * height * 3
*/
void nv12_to_bgr24_fixpoint(const uint8_t *pNV12, int width, int height, uint8_t *pBGR24);

/**
* @brief nv12_to_bgr24, color space convert
* @param pNV12: src NV12 image
* @param width: NV12 width
* @param height: NV12 height
* @param pBGR24: Out BGR24 image, size = width * height * 3
*/
void nv12_to_bgr24(const uint8_t *pNV12, int width, int height, uint8_t *pBGR24);

/**
* @brief YUV_to_BGR24, color space convert, fix-point calculate, have accuracy loss.
* @param pYUV: src YUV image
* @param width: YUV width
* @param height: YUV height
* @param pBGR24: Out BGR24 image, size = width * height * 3
*/
void yuv_to_bgr24_fixpoint(const uint8_t *pYUV, int width, int height, uint8_t *pBGR24);

/**
* @brief YUV_to_BGR24, color space convert, fix-point calculate.
* @param pYUV: src YUV image
* @param width: YUV width
* @param height: YUV height
* @param pBGR24: Out BGR24 image, size = width * height * 3
*/
void yuv_to_bgr24(const uint8_t *pYUV, int width, int height, uint8_t *pBGR24);

/**
* @brief Convert YUV to OpenCV Mat(BGR24).
* @param pYUV: src YUV image, also called YUV420
* @param width: YUV width
* @param height: YUV height
* @return cv::Mat
*/
cv::Mat yuv_to_cvmat(const uint8_t *pYUV, int width, int height);


#endif /* _TEST_LP_H_ */