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


#include <stdint.h>

// linux window difference part
#ifdef WIN32
#include <Windows.h>
#else	/* LINUX DATA TYPE*/
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#ifndef BOOL
#define BOOL int
#endif	/* BOOL */

#endif
	
	typedef struct TRect
	{
		int  x;
		int  y;
		int  w;
		int  h;
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
