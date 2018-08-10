#include <Windows.h>
#include <windowsx.h>
#include <bwm801.h>
#include <bwm801_screenshot.h>
#include <bwm801_main_data.h>
#include <sstream>
#include <bwm801_glext.h>
#include <bwm801_top.h>
#include <string>
#include <iostream>
#include <map>
	
template<typename T> bool Load_From_DLL(const std::string & i_sName, const std::vector<std::string> & o_vsDLL_List, std::map<std::string, HINSTANCE> &io_mapDLLs, T * &o_tResult)
{
	std::string szDefault_Location;
	bool bFault = false;

	for (auto iterI = o_vsDLL_List.begin(); iterI != o_vsDLL_List.end(); iterI++)
	{
		std::ostringstream ossPath;
		ossPath << "./" << *iterI;
		HINSTANCE lpvDLL = nullptr;
		if (io_mapDLLs.count(*iterI) == 1)
			lpvDLL = io_mapDLLs[*iterI];
		else
		{

			lpvDLL = LoadLibrary(ossPath.str().c_str());
			io_mapDLLs[*iterI] = lpvDLL;
		}
		if (lpvDLL != nullptr)
		{
			T * lpInstance = (T *)GetProcAddress(lpvDLL,i_sName.c_str());
			if (lpInstance != nullptr)
			{
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

template<typename T> bool Load_Func_From_DLL(const std::string & i_sName, const std::vector<std::string> & o_vsDLL_List, std::map<std::string, HINSTANCE> &io_mapDLLs, T &o_tResult)
{
	std::string szDefault_Location;
	bool bFault = false;
	T tResult = nullptr;

	for (auto iterI = o_vsDLL_List.begin(); iterI != o_vsDLL_List.end(); iterI++)
	{
		std::ostringstream ossPath;
		ossPath << "./" << *iterI;
		HINSTANCE lpvDLL = nullptr;
		if (io_mapDLLs.count(*iterI) == 1)
			lpvDLL = io_mapDLLs[*iterI];
		else
		{

			lpvDLL = LoadLibrary(ossPath.str().c_str());
			io_mapDLLs[*iterI] = lpvDLL;
		}
		if (lpvDLL != nullptr)
		{
			T lpInstance = (T)GetProcAddress(lpvDLL,i_sName.c_str());
			if (lpInstance != nullptr)
			{
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

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT nCmdShow)
{
	std::cout << "starting..." << std::endl;
	int iRet = 1;
	bwm801::main * lpMain = nullptr;
	bwm801_top::fp_bwm801main bwm801main = nullptr;
	std::map<std::string, HINSTANCE> mapDLLs;

	std::cout << "bwm801-launcher v1.0.0" << std::endl;

	std::vector<std::string> sDLLs_Found;

	std::vector<std::string> szRet;
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::ostringstream szAll;
	hFind = FindFirstFile(".", &ffd);
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (strstr(ffd.cFileName, ".dll") != nullptr)
				sDLLs_Found.push_back(ffd.cFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);


	if (Load_From_DLL<bwm801::main>("g_cMain",sDLLs_Found,mapDLLs,lpMain))
	{
		if (Load_Func_From_DLL<bwm801_top::fp_bwm801main>("bwm801_main",sDLLs_Found,mapDLLs,bwm801main))
		{
			iRet = bwm801main(lpMain,hInstance, hPrevInstance, lpszCmdLine, nCmdShow);
		}
	}

	// close any DLLs that we've opened

	for (auto iterI = mapDLLs.begin(); iterI != mapDLLs.end(); iterI++)
	{
		if (iterI->second != nullptr)
		{
			FreeLibrary(iterI->second);
			iterI->second = nullptr;
		}
	}
	return iRet;
}


	
	
