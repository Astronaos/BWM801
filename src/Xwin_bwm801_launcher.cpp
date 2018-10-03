#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <GL/glx.h>
#include <bwm801.h>
#include <FTGL/ftgl.h>
#include <iostream>
#include <sys/types.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#define NAMLEN(dirent) strlen ((dirent)->d_name)
#else
#define dirent direct
#define NAMLEN(dirent) ((dirent)->d_namlen)
#ifdef HAVE_SYS_NDIR_H
#include <sys/ndir.h>
#endif
#ifdef HAVE_SYS_DIR_H
#include <sys/dir.h>
#endif
#ifdef HAVE_NDIR_H
#include <ndir.h>
#endif
#endif
#include <thread>
#include <GL/glext.h>
#include <string>
#include <bwm801_screenshot.h>
#include <bwm801_main_data.h>
#include <sstream>
#include <bwm801_glext.h>
#include <deque>
#include <map>
#include <bwm801_top.h>
#include <dlfcn.h>
#include <cstring>
	
typedef void (*fp_v_v)(void);

template<typename T> bool Load_From_DLL(const std::string & i_sName, const std::vector<std::string> & o_vsDLL_List, std::map<std::string, void *> &io_mapDLLs, T * &o_tResult)
{
	std::string szDefault_Location;
	bool bFault = false;

	for (auto iterI = o_vsDLL_List.begin(); iterI != o_vsDLL_List.end(); iterI++)
	{
		std::ostringstream ossPath;
		ossPath << "./" << *iterI;
		void * lpvDLL = nullptr;
		if (io_mapDLLs.count(*iterI) == 1 && io_mapDLLs[*iterI] != nullptr)
			lpvDLL = io_mapDLLs[*iterI];
		else
		{
			//std::cout << "Trying to open " << *iterI << std::endl;
			lpvDLL = dlopen(ossPath.str().c_str(),RTLD_LAZY);
			if (lpvDLL != nullptr)
			{
				//std::cout << "Successfully opened " << *iterI << std::endl;
				io_mapDLLs[*iterI] = lpvDLL;
			}
		}
		if (lpvDLL != nullptr)
		{
			//std::cout << "Checking " << *iterI << std::endl;
			T * lpInstance = (T *)dlsym(lpvDLL,i_sName.c_str());
			if (lpInstance != nullptr)
			{
				//std::cout << i_sName << " instance found" << std::endl;
				if (o_tResult == nullptr)
				{
					szDefault_Location = ossPath.str();
					o_tResult = lpInstance;
				}
				else
				{
					bFault = true;
					std::cerr << "Extra instance of " << i_sName << " found in " << ossPath.str() << std::endl;
				}
			}
		}
	}
	if (bFault)
	{
		std::cerr << "Extra instance of " << i_sName << " found in " << szDefault_Location << std::endl;
	}
	if (o_tResult == nullptr)
	{
		bFault = true;
		std::cerr << "Unable to find " << i_sName << std::endl;
	}
	else
	{
		std::cout << "Loaded " << i_sName << " from " << szDefault_Location << std::endl;
	}

	return !bFault;
}

template<typename T> bool Load_Func_From_DLL(const std::string & i_sName, const std::vector<std::string> & o_vsDLL_List, std::map<std::string, void *> &io_mapDLLs, T &o_tResult)
{
	std::string szDefault_Location;
	bool bFault = false;
	T tResult = nullptr;

	for (auto iterI = o_vsDLL_List.begin(); iterI != o_vsDLL_List.end(); iterI++)
	{
		std::ostringstream ossPath;
		ossPath << "./" << *iterI;
		void * lpvDLL = nullptr;
		if (io_mapDLLs.count(*iterI) == 1 && io_mapDLLs[*iterI] != nullptr)
			lpvDLL = io_mapDLLs[*iterI];
		else
		{

			lpvDLL = dlopen(ossPath.str().c_str(),RTLD_LAZY);
			if (lpvDLL != nullptr)
				io_mapDLLs[*iterI] = lpvDLL;
		}
		if (lpvDLL != nullptr)
		{
			T lpInstance;
			*(void **) (&lpInstance) = dlsym(lpvDLL,i_sName.c_str());
			if (lpInstance != nullptr)
			{
				//std::cout << i_sName << " instance found" << std::endl;
				if (tResult == nullptr)
				{
					szDefault_Location = ossPath.str();
					tResult = lpInstance;
				}
				else
				{
					bFault = true;
					std::cerr << "Extra instance of " << i_sName << " found in " << ossPath.str() << std::endl;
				}
			}
		}
	}
	if (bFault)
	{
		std::cerr << "Extra instance of " << i_sName << " found in " << szDefault_Location << std::endl;
	}
	if (tResult == nullptr)
	{
		bFault = true;
		std::cerr << "Unable to find " << i_sName << std::endl;
	}
	else
	{
		std::cout << "Loaded " << i_sName << " from " << szDefault_Location << std::endl;
	}
	o_tResult = tResult;

	return !bFault;
}

typedef int (*fp_i_bwm801main)(bwm801::main * i_lpcMain, int i_iArg_Count, const char * i_lpszArg_Values[]);

int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	int iRet = 1;
	bwm801::main * lpMain = nullptr;
	fp_i_bwm801main bwm801main = nullptr;
	std::map<std::string, void *> mapDLLs;

	std::cout << "bwm801-launcher v1.0.0" << std::endl;

	std::vector<std::string> sDLLs_Found;
	DIR * dirList = opendir (".");
	if (dirList)
	{
		dirent64 * direntInfo = readdir64(dirList);
		while (direntInfo)
		{
			if (direntInfo->d_name[0] != '.') // don't include . and ..; these can be assumed
			{
				std::cout << "file/dir " << direntInfo->d_name;
				if (strstr(direntInfo->d_name,".so") != nullptr ||
					strstr(direntInfo->d_name,".la") != nullptr)
				{
					std::cout << " added to list.";
					sDLLs_Found.push_back(direntInfo->d_name);
				}
				std::cout << std::endl;
			}
			direntInfo = readdir64(dirList);
		}
	}

	if (Load_From_DLL<bwm801::main>("g_cMain",sDLLs_Found,mapDLLs,lpMain))
	{
		if (Load_Func_From_DLL<fp_i_bwm801main>("bwm801_main",sDLLs_Found,mapDLLs,bwm801main))
		{
			iRet = bwm801main(lpMain,i_iArg_Count,i_lpszArg_Values);
		}
	}

	// close any DLLs that we've opened

	for (auto iterI = mapDLLs.begin(); iterI != mapDLLs.end(); iterI++)
	{
		if (iterI->second != nullptr)
		{
			dlclose(iterI->second);
			iterI->second = nullptr;
		}
	}
	return iRet;
}


	
	
