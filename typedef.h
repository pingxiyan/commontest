/******************************************************************************
* Initial version
* Some common date type define. Linux Windows...
* Sandy Yann. Aug. 4 2017
******************************************************************************/
#ifndef __COMMON_TEST_TYPE_DEF_H__
#define __COMMON_TEST_TYPE_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifdef WIN32
	typedef char        s8;
	typedef short           s16;
	typedef unsigned char   u8;
	typedef unsigned short  u16;
	typedef float           f32;
	typedef double          d64;

    typedef unsigned __int64   x64;
    typedef __int64            s64;
    typedef unsigned __int64   u64;
    typedef int                l32;
    typedef unsigned int       u32;
    typedef unsigned __int64   ptrv;   
#else

#endif



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
