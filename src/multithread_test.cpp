/************************************************************************
/* 2016-5-30 Sandy Yann 通过回调函数实现多线程测试，			      	*/
/************************************************************************/
#ifdef _WIN32
#include <Windows.h>
#endif

#include "common_test.h"

typedef struct tagThrdParam
{
	std::vector<std::string> vecSet;
	int id;				// my thread id

	MultiOpen myOpen;
	MultiProcess myProcess;
	MultiClose myClose;
	void *pvUser;
}TThrdParam;

#ifdef _WIN32
DWORD WINAPI MyThrdProcFun( LPVOID lpParam ) 
#else
uint32_t MyThrdProcFun( void* lpParam ) 
#endif /*_WIN32*/
{
	TThrdParam *pThrdParam = (TThrdParam*)lpParam;

	void* pvCBHandle = NULL;
	if(pThrdParam->myOpen != NULL)
	{
		pvCBHandle = pThrdParam->myOpen(pThrdParam->id);
	}

	std::vector<std::string> &vecFn = pThrdParam->vecSet;
	for (int i = 0; i < vecFn.size(); i++)
	{
		std::cout << "thr " << pThrdParam->id 
			<< " progress = " << double(i*100) / vecFn.size() << std::endl;

		pThrdParam->myProcess(pvCBHandle, vecFn[i].c_str(), pThrdParam->pvUser);
	}

	if(pThrdParam->myClose != NULL)
	{
		pThrdParam->myClose(pvCBHandle);
		pvCBHandle = NULL;
	}

	return 1;
}

CTAPI void multiThrdTest(std::vector<std::string> vecFn,	// set文件名
						 int thrnum,	// 线程数目>=1
						 MultiOpen myOpen,
						 MultiProcess myProcess,
						 MultiClose myClose,
						 void *pvUser)
{	
	std::cout << "\n=====================================\n";
	std::cout << "file count = " << vecFn.size() << "\n" ;
	std::cout << "multi-thread num = " << thrnum << std::endl;
	// divide 8 thread processing.
	int thrdnum = thrnum;
	int onepart = vecFn.size() / thrdnum;

#ifdef _WIN32
	DWORD   *dwThreadIdArray = new DWORD[thrdnum];
	HANDLE  *hThreadArray = new HANDLE[thrdnum]; 
	TThrdParam  *pvecThrdPram = new TThrdParam[thrdnum]; 

	std::cout << "start multi-thread test\n";
	for (int i = 0; i < thrdnum; i++)
	{
		std::cout << "start thrd " << i << std::endl;
		pvecThrdPram[i].vecSet = vecStrSection(thrdnum, i, vecFn);
		pvecThrdPram[i].id = i;
		pvecThrdPram[i].pvUser = pvUser;
		pvecThrdPram[i].myOpen = myOpen;
		pvecThrdPram[i].myProcess = myProcess;
		pvecThrdPram[i].myClose = myClose;

		hThreadArray[i] = CreateThread( 
			NULL,                   // default security attributes
			0,                      // use default stack size  
			MyThrdProcFun,			// thread function name
			&(pvecThrdPram[i]),		// argument to thread function 
			0,                      // use default creation flags 
			&dwThreadIdArray[i]);
	}

	WaitForMultipleObjects(thrdnum, hThreadArray, TRUE, INFINITE);

	// release buffer
	delete[] dwThreadIdArray;
	delete[] hThreadArray;
	delete[] pvecThrdPram;
#else

#endif

	std::cout << "finish\n";
}


CTAPI void multiThrdTest(std::string strSetFn,	// set file name
						 int thrnum,			// my thread id
						 MultiOpen myOpen,
						 MultiProcess myProcess,
						 MultiClose myClose,
						 void *pvUser)
{
	std::vector<std::string> vevFn = readsample(strSetFn);

	multiThrdTest(vevFn, thrnum, myOpen, myProcess, myClose, pvUser);
}