/************************************************************************
* Sample file process, related with 'copy'
*	Sandy Yann
*	2016-1-4															
/************************************************************************/
#include "common_test.h"

#ifdef _WIN32
#include <Windows.h>
#endif

/**
*@brief Create path based file name, eg. /home/xx/xx.bmp
*@param strFn : File name string
*@return TRUE or FALSE
*/
BOOL createPath(std::string strFn)
{
#ifdef _WIN32
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
#else	/* LINXU */

#endif
	return TRUE;
}

/**
*@brief Fast copy one file to destination file 
*@param strSrcFn : Copyed file
*@param strDstFn : Destination file
*/
void copyFile2DstFn(std::string strSrcFn, std::string strDstFn)
{
	FILE* pfSrc = fopen(strSrcFn.c_str(), "rb");

	// 
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