/***************************************************
* Multi-thread need mutex, support windows and linux.
* Not need cpp file.
* Sandy Yann.
* Oct. 8 2017
****************************************************/

#ifndef _COMMON_TEST_THREAD_H_
#define _COMMON_TEST_THREAD_H_

#include <string>

#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

class CThreadLock
{
public:
	CThreadLock(std::string strLockName)
	{
		_strLockName = strLockName;
#ifdef _WIN32
		_hMutex = CreateMutexA(NULL, FALSE, strLockName.c_str());
#else
		pthread_mutex_init(&_hMutex, NULL);
#endif
	};

	~CThreadLock() 
	{
#ifdef _WIN32
		CloseHandle(_hMutex);
		_hMutex = NULL;
#else

#endif
	};

	void lock() 
	{
#ifdef _WIN32
		WaitForSingleObject(_hMutex, INFINITE);
#else
		pthread_mutex_lock(&_hMutex);
#endif
	};
	void unlock() 
	{
#ifdef _WIN32
		ReleaseMutex(_hMutex);
#else
		pthread_mutex_unlock(&_hMutex);
#endif
	};
private:
	std::string _strLockName;

#ifdef _WIN32
	HANDLE _hMutex;
#else
	pthread_mutex_t _hMutex;
#endif
};



#endif /* _COMMON_TEST_THREAD_H_ */