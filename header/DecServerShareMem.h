/************************************************************************/
/* 2017-03-19 SandyYann.
	通过共享内存传递解码数据，注意事项如下：
	1.共享内存的大小固定，解码视频如果过大，需要缩放图像再传递；	*/
/************************************************************************/

#ifndef _DEC_SERVER_SHARE_MEM_H_
#define _DEC_SERVER_SHARE_MEM_H_

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TIP_ERR_DSSM() printf("ERR:%s, %d\n", __FUNCTION__, __LINE__);

// 固定大小，不能改变
#define SHARE_MEM_SIZE (1920*2560*3/2)  

typedef enum emShareMemState
{
	emSMS_NULL = 0,		// initialize null state
	emSMS_PUTED = 1,	// had putted, waited for be getted;
	emSMS_GETED = 2,	// had got,
}EMShareMemState;

typedef enum emDecResState
{
	emDRS_Fail = 0,		// fail, need to exit.
	emDMS_Success = 1,	// Decode finish, and then exit.
	emDMS_Decing = 2,	// Decoding.
}EMDecResState;

typedef struct tagShareMemHeader
{
	EMShareMemState emState;	// 共享内存状态
	int width;
	int height;
	int imgSize;		// 图像size
	__int64 pts;		// 时间戳
	__int64 frameId;	// 帧ID
	EMDecResState emDRState;	// 是否解码完成
}TShareMemHeader;

class CShareMemDecServer
{
public:
	// 获取
	CShareMemDecServer(const char* pShareMemName)
	{
		memcpy(m_aShareMemName, pShareMemName, strlen(pShareMemName));

		createShareMem(SHARE_MEM_SIZE, m_aShareMemName);

		// 创建锁
		m_stream1Mutex = CreateMutex(NULL, false, (LPCWSTR)"streamMutex");

		m_pBufCacheImg = new char[SHARE_MEM_SIZE];
		if (NULL == m_pBufCacheImg)
		{
			TIP_ERR_DSSM();
			return;
		}
	}

	~CShareMemDecServer()
	{
		releaseShareMem();

		if (m_pBufCacheImg) delete[] m_pBufCacheImg;
	}

	// 放入一帧图像,无法放入时，会阻塞。
	bool putOneFrame(char* pYuv, int w, int h, __int64 frameId, __int64 pts)
	{
		m_getCount = 0;

		// 如果图像过大，图像需要缩放，
		checkShareMemSize(w, h);

	AGAIN:
		WaitForSingleObject(m_stream1Mutex, INFINITE);
		// 获取共享内存的头信息
		TShareMemHeader tSMH = getSMH(m_pShareMem);

		if (tSMH.emState == emSMS_GETED || tSMH.emState == emSMS_NULL)
		{
			// could put image data
			tSMH.emState = emSMS_PUTED;
			tSMH.height = h;
			tSMH.width = w;
			tSMH.imgSize = min(w * h * 3 / 2, SHARE_MEM_SIZE);
			tSMH.frameId = frameId;
			tSMH.pts = pts;
			tSMH.emDRState = emDMS_Decing;

			memcpy(m_pShareMem + getShareMemHeaderSize(), pYuv, tSMH.imgSize);

			// set state to got, and then DecServer could put new image data.
			setSMH(m_pShareMem, tSMH);
			ReleaseMutex(m_stream1Mutex);
			return true;
		}
		else
		{
			ReleaseMutex(m_stream1Mutex);

			// If we continue to fail to put it, it will block.
			goto AGAIN;
		}

		return true;
	}

	bool putLastResult(EMDecResState emState)
	{

	AGAIN:
		WaitForSingleObject(m_stream1Mutex, INFINITE);
		// 获取共享内存的头信息
		TShareMemHeader tSMH = getSMH(m_pShareMem);

		if (tSMH.emState == emSMS_GETED || tSMH.emState == emSMS_NULL)
		{
			tSMH.emState = emSMS_PUTED;
			tSMH.emDRState = emState;

			setSMH(m_pShareMem, tSMH);
			ReleaseMutex(m_stream1Mutex);
			return true;
		}
		else
		{
			ReleaseMutex(m_stream1Mutex);

			// If we continue to fail to put it, it will block.
			goto AGAIN;
		}

		return true;
	}

	// 获取一帧图像, 返回解码状态，长时间获取不到解码结果，也会返回失败
	EMDecResState getOneFrame(char** ppYuv, int& w, int& h, __int64& frameId, __int64& pts)
	{
		m_getCount = 0;

	AGAIN:
		WaitForSingleObject(m_stream1Mutex, INFINITE);
		// 获取共享内存的头信息
		TShareMemHeader tSMH = getSMH(m_pShareMem);

		if (tSMH.emState == emSMS_PUTED)
		{
			// could get image data
			memcpy(m_pBufCacheImg, m_pShareMem + getShareMemHeaderSize(), tSMH.width * tSMH.height * 3 / 2);
			*ppYuv = m_pBufCacheImg;
			w = tSMH.width;
			h = tSMH.height;
			frameId = tSMH.frameId;
			pts = tSMH.pts;

			// set state to got, and then DecServer could put new image data.
			tSMH.emState = emSMS_GETED;
			setSMH(m_pShareMem, tSMH);
			ReleaseMutex(m_stream1Mutex);

			return tSMH.emDRState;
		}
		else
		{
			ReleaseMutex(m_stream1Mutex);
			m_getCount++;
			// If we continue to fail to get it, it returns false;
			if (m_getCount > 999999999)
			{
				// 长时间不能获取解码结果，则返回失败，退出
				return emDRS_Fail;
			}
			goto AGAIN;
		}
	}
private:
	inline void createShareMem(int shareMemSize, const char* pShareMemName)
	{
		// 创建共享文件句柄  
		m_hMapFile = CreateFileMappingA(
			INVALID_HANDLE_VALUE,    // 物理文件句柄  
			NULL,                    // 默认安全级别  
			PAGE_READWRITE,          // 可读可写  
			0,                       // 高位文件大小  
			SHARE_MEM_SIZE + sizeof(TShareMemHeader),          // 地位文件大小  
			pShareMemName                   // 共享内存名称  
			);

		m_pShareMem = (char *)MapViewOfFile(
			m_hMapFile,            // 共享内存的句柄  
			FILE_MAP_ALL_ACCESS, // 可读写许可  
			0,
			0,
			SHARE_MEM_SIZE + sizeof(TShareMemHeader)
			);
	}
	inline void releaseShareMem()
	{
		if (m_pShareMem)
		{
			UnmapViewOfFile(m_pShareMem);
			m_pShareMem = NULL;
		}
		if (m_hMapFile)
		{
			CloseHandle(m_hMapFile);
			m_hMapFile = NULL;
		}
		if (m_stream1Mutex)
		{
			CloseHandle(m_stream1Mutex);
			m_stream1Mutex = NULL;
		}
	}

	inline int getShareMemHeaderSize()
	{
		return sizeof(TShareMemHeader);
	}

	inline TShareMemHeader getSMH(char* pbuf)
	{
		TShareMemHeader tSMH;
		memcpy(&tSMH, pbuf, sizeof(TShareMemHeader));
		return tSMH;
	}
	inline void setSMH(char* pbuf, TShareMemHeader tSMH)
	{
		memcpy(pbuf, &tSMH, sizeof(TShareMemHeader));
	}

	// putOneFrame共享内存端，需要设置这个参数，
	// getOneFrame时，不需要设置这个参数
	bool checkShareMemSize(int imgWidth, int imgHeight)
	{
		if (imgWidth * imgHeight * 3 / 2 > SHARE_MEM_SIZE)
		{
			// 需要缩放图像...

		}
		return TRUE;
	}
private:
	char m_aShareMemName[256];
	HANDLE m_hMapFile;
	char *m_pShareMem;
	HANDLE m_stream1Mutex;

	int m_getCount;

	char* m_pBufCacheImg;
};


#endif