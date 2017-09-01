/******************************************************************************
* Same to cc_types.h
* Some common date type define. Linux Windows...
* Sandy Yann. Aug. 4 2017
******************************************************************************/
#ifndef __COMMON_TEST_TYPE_DEF_H__
#define __COMMON_TEST_TYPE_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif


// linux window difference part
#ifdef WIN32
#include <Windows.h>
typedef char				s8;
typedef unsigned char		u8;
typedef short				s16;
typedef unsigned short		u16;
typedef float				f32;
typedef double				d64;

typedef __int64				s64;
typedef unsigned __int64	u64;
typedef int					l32;
typedef unsigned int		u32;
typedef unsigned __int64	ptrv;

#else	/* LINUX DATA TYPE*/
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdbool.h>
typedef char				s8;
typedef uint8_t				u8;
typedef short				s16;
typedef uint16_t			u16;
typedef float				f32;
typedef double				d64;

typedef int64_t				s64;
typedef uint64_t			u64;
typedef int					l32;
typedef uint32_t			u32;
typedef uint64_t			ptrv;
#ifndef BOOL
#define BOOL int
#endif	/* BOOL */

#endif


	typedef enum emEImageType
	{
		GRAY = 0,
		I420 = 1,	// Y,U,V, planer storage.
		NV12 = 2,	// Y single planer, uv mix storage.
		BGR24 = 3,
		BGRP = 4,	// Plane, split storage.
	}EImageType;

	/* Image type of KEDACOM, NO use in the futhure */
	typedef struct tagImage
	{
		u8* pu8Data;
		l32 l32Width;
		l32 l32Height;
		l32 l32Channel;
		l32 l32Stride;
		u32 u32Type;
	}TImage;
	
	typedef struct TRect
	{
		l32  l32Left;
		l32  l32Top;
		l32  l32Width;
		l32  l32Height;
	}TRect;

#ifndef NULL
  #ifdef  __cplusplus
    #define NULL    0
  #else
    #define NULL    ((void *)0)
  #endif
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef MIN
#define MIN(a, b)   ((a) > (b) ? (b) : (a))
#endif
#ifndef MAX
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#endif
#ifndef ABS
#define ABS(x) ((x) >= 0 ? (x) : (-(x)))
#endif


#ifdef __cplusplus
}
#endif

#endif	/* __COMMON_TEST_TYPE_DEF_H__ */
