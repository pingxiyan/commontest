/************************************************************************
/* 2016-5-30 Sandy Yann 通过回调函数实现多线程测试，			      	*/
/************************************************************************/

#include <Windows.h>
#include "commontest.h"

typedef struct tagThrdParam
{
	std::vector<std::string> vecSet;
	int id;				// 当前线程,for循环用的序号

	MultiOpen myOpen;
	MultiProcess myProcess;
	MultiClose myClose;
	void *pvUser;
}TThrdParam;

DWORD WINAPI MyThrdProcFun( LPVOID lpParam ) 
{
	TThrdParam  *pThrdParam = (TThrdParam*)lpParam;

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
	// 开辟线程
	std::cout << "\n=====================================\n";
	std::cout << "file count = " << vecFn.size() << "\n" ;
	std::cout << "multi-thread num = " << thrnum << std::endl;
	// divide 8 thread processing.
	int thrdnum = thrnum;
	int onepart = vecFn.size() / thrdnum;

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

		// 创建线程
		hThreadArray[i] = CreateThread( 
			NULL,                   // default security attributes
			0,                      // use default stack size  
			MyThrdProcFun,       // thread function name
			&(pvecThrdPram[i]),          // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadIdArray[i]);
	}

	WaitForMultipleObjects(thrdnum, hThreadArray, TRUE, INFINITE);

	// 释放资源
	delete[] dwThreadIdArray;
	delete[] hThreadArray;
	delete[] pvecThrdPram;

	std::cout << "finish\n";
}

// 多线程测试
CTAPI void multiThrdTest(std::string strSetFn,	// set文件名
						 int thrnum,	// 线程数目>=1
						 MultiOpen myOpen,
						 MultiProcess myProcess,
						 MultiClose myClose,
						 void *pvUser)
{
	std::vector<std::string> vevFn = readsample(strSetFn);

	multiThrdTest(vevFn, thrnum, myOpen, myProcess, myClose, pvUser);
}