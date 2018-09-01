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

template<typename T> bool Load_From_DLL(const std::string & i_sName, const std::vector<std::string> & o_vsDLL_List, std::map<std::string, void *> &io_mapDLLs, T * &o_tResult, bool i_bDebug)
{
	std::string szDefault_Location;
	bool bFault = false;

	if (i_bDebug)
		std::cout << "Looking for " << i_sName << std::endl;

	for (auto iterI = o_vsDLL_List.begin(); iterI != o_vsDLL_List.end(); iterI++)
	{
		std::ostringstream ossPath;
		//ossPath << "./" << *iterI;
		ossPath << *iterI;
		void * lpvDLL = nullptr;
		if (io_mapDLLs.count(*iterI) == 1 && io_mapDLLs[*iterI] != nullptr)
			lpvDLL = io_mapDLLs[*iterI];
		else
		{
			if (i_bDebug)
				std::cout << "Trying to open " << *iterI << std::endl;
			lpvDLL = dlopen(ossPath.str().c_str(),RTLD_LAZY);
			if (lpvDLL != nullptr)
			{
				if (i_bDebug)
					std::cout << "Successfully opened " << *iterI << std::endl;
				io_mapDLLs[*iterI] = lpvDLL;
			}
			else
				std::cout << "Failed to open " << ossPath.str() << ". (" << dlerror() << ")" << std::endl;
		}
		if (lpvDLL != nullptr)
		{
			if (i_bDebug)
				std::cout << "Checking " << *iterI << std::endl;
			T * lpInstance = (T *)dlsym(lpvDLL,i_sName.c_str());
			if (lpInstance != nullptr)
			{
				if (i_bDebug)
					std::cout << i_sName << " instance found" << std::endl;
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

template<typename T> bool Load_Func_From_DLL(const std::string & i_sName, const std::vector<std::string> & o_vsDLL_List, std::map<std::string, void *> &io_mapDLLs, T &o_tResult, bool i_bDebug)
{
	std::string szDefault_Location;
	bool bFault = false;
	T tResult = nullptr;
	if (i_bDebug)
		std::cout << "Looking for " << i_sName << std::endl;

	for (auto iterI = o_vsDLL_List.begin(); iterI != o_vsDLL_List.end(); iterI++)
	{
		std::ostringstream ossPath;
		//ossPath << "./" << *iterI;
		ossPath << *iterI;
		void * lpvDLL = nullptr;
		if (io_mapDLLs.count(*iterI) == 1 && io_mapDLLs[*iterI] != nullptr)
			lpvDLL = io_mapDLLs[*iterI];
		else
		{

			if (i_bDebug)
				std::cout << "Trying to open " << *iterI << std::endl;
			lpvDLL = dlopen(ossPath.str().c_str(),RTLD_LAZY);
			if (lpvDLL != nullptr)
			{
				if (i_bDebug)
					std::cout << "Successfully opened " << *iterI << std::endl;
				io_mapDLLs[*iterI] = lpvDLL;
			}
			else
				std::cout << "Failed to open " << ossPath.str() << ". (" << dlerror() << ")" << std::endl;
		}
		if (lpvDLL != nullptr)
		{
			T lpInstance = (T)dlsym(lpvDLL,i_sName.c_str());
			if (lpInstance != nullptr)
			{
				if (i_bDebug)
					std::cout << i_sName << " instance found" << std::endl;
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

std::vector<std::string> & operator += (std::vector<std::string> & i_cLHO, const std::vector<std::string> & i_cRHO)
{
	for (auto iterI = i_cRHO.begin(); iterI != i_cRHO.end(); iterI++)
	{
		i_cLHO.push_back(*iterI);
	}
	return i_cLHO;
}

std::vector<std::string> Find_DLLs_In_Directory(const std::string & i_sDirectory, bool i_bDebug)
{
	std::vector<std::string> sDLLs_Found;
	DIR * dirList = opendir (i_sDirectory.c_str());
	if (i_bDebug)
		std::cout << "Searchig directory " << i_sDirectory << std::endl;
	if (dirList)
	{
		dirent64 * direntInfo = readdir64(dirList);
		while (direntInfo)
		{
			if (direntInfo->d_name[0] != '.') // don't include . and ..; these can be assumed
			{
				if (i_bDebug)
					std::cout << "file/dir " << direntInfo->d_name;
//				if (strstr(direntInfo->d_name,".so") != nullptr ||
//					strstr(direntInfo->d_name,".la") != nullptr)
				if (strstr(direntInfo->d_name,".so") != nullptr)
				{
					std::ostringstream ossPath;
					ossPath << i_sDirectory;
					ossPath << "/";
					ossPath << direntInfo->d_name;

					if (i_bDebug)
						std::cout << " added to list.";
					sDLLs_Found.push_back(ossPath.str());
				}
				if (i_bDebug)
					std::cout << std::endl;
			}
			direntInfo = readdir64(dirList);
		}
	}
	return sDLLs_Found;
}

int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	int iRet = 1;
	bwm801::main * lpMain = nullptr;
	fp_i_bwm801main bwm801main = nullptr;
	std::map<std::string, void *> mapDLLs;
	size_t tArg_Count = (size_t)i_iArg_Count;

	std::cout << "bwm801-launcher v1.1.0" << std::endl;
	std::string szMain_Name = "g_cMain";
	bool bLaunch_Debug = false;

	for (size_t tI = 1; tI < tArg_Count; tI++)
	{
		if (std::strcmp(i_lpszArg_Values[tI],"--launch-debug") == 0)
		{
			bLaunch_Debug = true;
		}
	}

	std::vector<std::string> sDLLs_Found = Find_DLLs_In_Directory(".",bLaunch_Debug);

	for (size_t tI = 1; tI < tArg_Count; tI++)
	{
		if (i_lpszArg_Values[tI][0] != '-') // not an option
		{
			if (tI == 1 || i_lpszArg_Values[tI - 1][std::strlen(i_lpszArg_Values[tI]) - 1] != '=') // make sure this isn't a value for an option that had a space between the option and the value
			{
				size_t tStrlen = std::strlen(i_lpszArg_Values[tI]);
				if (i_lpszArg_Values[tI][tStrlen - 3] == '.' && i_lpszArg_Values[tI][tStrlen - 2] == 's' && i_lpszArg_Values[tI][tStrlen - 1] == 'o')
					sDLLs_Found.push_back(i_lpszArg_Values[tI]);
				else
					sDLLs_Found += Find_DLLs_In_Directory(i_lpszArg_Values[tI],bLaunch_Debug);
			}
		}
		else if (std::strncmp(i_lpszArg_Values[tI],"--main=",7) == 0)
		{
			if (i_lpszArg_Values[tI][7] == 0)
			{
				tI++;
				if (tI < tArg_Count)
				{
					szMain_Name = i_lpszArg_Values[tI];
					std::cout << "Using " << szMain_Name << " for main class." << std::endl;
				}
			}
			else
			{
				szMain_Name = &i_lpszArg_Values[tI][7];
				std::cout << "Using " << szMain_Name << " for main class." << std::endl;
			}
		}
			
	}

	if (Load_From_DLL<bwm801::main>(szMain_Name,sDLLs_Found,mapDLLs,lpMain,bLaunch_Debug))
	{
		if (Load_Func_From_DLL<fp_i_bwm801main>("bwm801_main",sDLLs_Found,mapDLLs,bwm801main,bLaunch_Debug))
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


	
	
