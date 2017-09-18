/************************************************************************
* Parse config file, format as follow:
* test_file_name = /root/myexe/mytest # comment
* test_param2 = param # commont2
* Sandy Yann, Sep. 18 2017
*************************************************************************/

#include <string>

#ifndef _CONFIG_H_
#define _CONFIG_H_

void* readConfig(const char* ps8ConfigFn);
void releaseConfig(void**ppvHandle);

bool parseConfig(void* pvHandle, char* ps8Label, int &val);
bool parseConfig(void* pvHandle, char* ps8Label, float &val);
bool parseConfig(void* pvHandle, char* ps8Label, std::string & strOut);
bool parseConfig(void* pvHandle, char* ps8Label, bool &val);
/**************************************
* Sample ******************************
class CConfigParam
{
public:
	CConfigParam(std::string strConfigFn)
	{
		m_pvHandle = readConfig(strConfigFn.c_str());
		if (NULL == m_pvHandle)
		{
			std::cout << "can't open " << strConfigFn << " \n";
			exit(0);
		}

		parseConfig(m_pvHandle, "m_strAlgType",	m_strAlgType);
	}
	~CConfigParam()
	{
		releaseConfig(&m_pvHandle);
	}

	std::string m_strAlgType;
private:
	void *m_pvHandle;
};
*/

#endif /* _CONFIG_H_ */