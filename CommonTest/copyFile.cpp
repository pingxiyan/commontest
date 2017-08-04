/************************************************************************
/* 文件的copy相关操作
	Sandy Yann
	2016-1-4															*/
/************************************************************************/
#include "CommonTest.h"


#include <Windows.h>
// 根据文件名，如果路径不存在，则创建路径
BOOL createPath(std::string strFn)
{
	std::string strCurPath = strFn.substr(0, strFn.find('\\'));

	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA( strCurPath.c_str(), &fd );
	while( hFind != INVALID_HANDLE_VALUE )
	{
		if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			return TRUE;
	}

	if ( !::CreateDirectoryA( strCurPath.c_str(), NULL ) )
	{
		return FALSE;
	}

	return TRUE;
}

/*=======================================================================
Function : 文件copy，源文件名到目的文件名,实测发现这样读写要比调用dos命令要快
Param    : 
Return   : void
=========================================================================*/
void copyFile2DstFn(std::string strSrcFn, std::string strDstFn)
{
	FILE* pfSrc = fopen(strSrcFn.c_str(), "rb");

	// 检查文件夹是否存在，如果不存在，则创建
	createPath(strDstFn);
	FILE* pfDst = fopen(strDstFn.c_str(), "wb");

	if (pfDst && pfSrc)
	{
		char curChr;
		for(;;)
		{
			int r = fread(&curChr, 1,1, pfSrc);
			if (r)
			{
				fwrite(&curChr, 1,1, pfDst);
			}
			else
			{
				break;
			}
		}
	}

	if(pfSrc) fclose(pfSrc);
	if(pfDst) fclose(pfDst);
}