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

typedef std::basic_string<TCHAR> winstring;
#ifdef UNICODE
static std::string utf16ToUTF8(const winstring &s)
{
	const int size = ::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL);

	std::vector<char> buf(size);
	::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL);

	return std::string(&buf[0]);
}
#else
static std::string utf16ToUTF8(const winstring &s)
{
	return s;
}
#endif
template<typename T> bool Load_From_DLL(const winstring & i_sName, const std::vector<winstring> & o_vsDLL_List, std::map<winstring, HINSTANCE> &io_mapDLLs, T * &o_tResult)
{
	winstring szDefault_Location;
	bool bFault = false;
	LPTSTR lpszCurr_Dir = nullptr;

	for (auto iterI = o_vsDLL_List.begin(); iterI != o_vsDLL_List.end(); iterI++)
	{
		HINSTANCE lpvDLL = nullptr;
		if (io_mapDLLs.count(*iterI) == 1 && io_mapDLLs[*iterI] != nullptr)
			lpvDLL = io_mapDLLs[*iterI];
		else
		{
			if (lpszCurr_Dir == nullptr)
			{
				lpszCurr_Dir = new TCHAR[256];
				GetCurrentDirectory(
					256,
					lpszCurr_Dir
				);
			}
			std::basic_ostringstream<TCHAR> ossPath;
			ossPath << lpszCurr_Dir << "\\" << iterI->c_str();
			lpvDLL = LoadLibrary(ossPath.str().c_str());
			io_mapDLLs[*iterI] = lpvDLL;
		}
		if (lpvDLL != nullptr)
		{
			std::string sName = utf16ToUTF8(i_sName);
			T * lpInstance = (T *)GetProcAddress(lpvDLL, sName.c_str());
			if (lpInstance != nullptr)
			{
				if (o_tResult == nullptr)
				{
					szDefault_Location = *iterI;
					o_tResult = lpInstance;
				}
				else
				{
					bFault = true;
					std::ostringstream ossWarning; 
					ossWarning << "Extra instance of " << i_sName.c_str() << " found in " << iterI->c_str() << std::endl;
					_RPT1(_CRT_ERROR, "%s", ossWarning.str().c_str());
				}
			}
		}
		else
		{
			DWORD wError_Code = GetLastError();
			LPVOID lpMsgBuf;
			DWORD dw = GetLastError();

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);

			// Display the error message and exit the process
			_RPT1(_CRT_ERROR, "%s", lpMsgBuf);

			LocalFree(lpMsgBuf);
		}
	}
	if (bFault)
	{
		std::ostringstream ossWarning;
		ossWarning << "Extra instance of " << i_sName.c_str() << " found in " << szDefault_Location.c_str() << std::endl;
		_RPT1(_CRT_ERROR, "%s", ossWarning.str().c_str());
	}
	if (o_tResult == nullptr)
	{
		bFault = true;
		std::ostringstream ossWarning;
		ossWarning << "Unable to find " << i_sName.c_str() << std::endl;
		_RPT1(_CRT_ERROR, "%s", ossWarning.str().c_str());
	}
	else
	{
		std::ostringstream ossWarning;
		ossWarning << "Loaded " << i_sName.c_str() << " from " << szDefault_Location.c_str() << std::endl;
		_RPT1(_CRT_WARN, "%s", ossWarning.str().c_str());
	}
	if (lpszCurr_Dir != nullptr)
		delete[] lpszCurr_Dir;

	return !bFault;
}

template<typename T> bool Load_Func_From_DLL(const winstring & i_sName, const std::vector<winstring> & o_vsDLL_List, std::map<winstring, HINSTANCE> &io_mapDLLs, T &o_tResult)
{
	winstring szDefault_Location;
	bool bFault = false;
	LPTSTR lpszCurr_Dir = nullptr;

	for (auto iterI = o_vsDLL_List.begin(); iterI != o_vsDLL_List.end(); iterI++)
	{
		HINSTANCE lpvDLL = nullptr;
		if (io_mapDLLs.count(*iterI) == 1 && io_mapDLLs[*iterI] != nullptr)
			lpvDLL = io_mapDLLs[*iterI];
		else
		{
			if (lpszCurr_Dir == nullptr)
			{
				lpszCurr_Dir = new TCHAR[256];
				GetCurrentDirectory(
					256,
					lpszCurr_Dir
				);
			}
			std::basic_ostringstream<TCHAR> ossPath;
			ossPath << lpszCurr_Dir << "\\" << iterI->c_str();
			lpvDLL = LoadLibrary(ossPath.str().c_str());
			io_mapDLLs[*iterI] = lpvDLL;
		}
		if (lpvDLL != nullptr)
		{
			std::string sName = utf16ToUTF8(i_sName);
			T lpInstance = (T)GetProcAddress(lpvDLL, sName.c_str());
			if (lpInstance != nullptr)
			{
				if (o_tResult == nullptr)
				{
					szDefault_Location = *iterI;
					o_tResult = lpInstance;
				}
				else
				{
					bFault = true;
					std::ostringstream ossWarning;
					ossWarning << "Extra instance of " << i_sName.c_str() << " found in " << iterI->c_str() << std::endl;
					_RPT1(_CRT_ERROR, "%s", ossWarning.str().c_str());
				}
			}
		}
		else
		{
			DWORD wError_Code = GetLastError();
			LPVOID lpMsgBuf;
			DWORD dw = GetLastError();

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);

			// Display the error message and exit the process
			_RPT1(_CRT_WARN, "%s", lpMsgBuf);

			LocalFree(lpMsgBuf);
		}
	}
	if (bFault)
	{
		std::ostringstream ossWarning;
		ossWarning << "Extra instance of " << i_sName.c_str() << " found in " << szDefault_Location.c_str() << std::endl;
		_RPT1(_CRT_ERROR, "%s", ossWarning.str().c_str());
	}
	if (o_tResult == nullptr)
	{
		bFault = true;
		std::ostringstream ossWarning;
		ossWarning << "Unable to find " << i_sName.c_str() << std::endl;
		_RPT1(_CRT_ERROR, "%s", ossWarning.str().c_str());
	}
	else
	{
		std::ostringstream ossWarning;
		ossWarning << "Loaded " << i_sName.c_str() << " from " << szDefault_Location.c_str() << std::endl;
		_RPT1(_CRT_WARN, "%s", ossWarning.str().c_str());
	}
	if (lpszCurr_Dir != nullptr)
		delete[] lpszCurr_Dir;

	return !bFault;
}
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT nCmdShow)
{
	std::cout << "starting..." << std::endl;
	int iRet = 1;
	bwm801::main * lpMain = nullptr;
	bwm801_top::fp_bwm801main bwm801_WinMain = nullptr;
	std::map<winstring, HINSTANCE> mapDLLs;

	winstring  sVer = "bwm801-launcher v1.0.0";
	_RPT1(_CRT_WARN, "%s\n", sVer.c_str());

	std::vector<winstring> sDLLs_Found;

	std::vector<winstring> szRet;
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::ostringstream szAll;
	hFind = FindFirstFile("*.dll", &ffd);
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			sDLLs_Found.push_back(ffd.cFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);


	if (Load_From_DLL<bwm801::main>("g_cMain",sDLLs_Found,mapDLLs,lpMain))
	{
		if (Load_Func_From_DLL<bwm801_top::fp_bwm801main>("bwm801_WinMain",sDLLs_Found,mapDLLs, bwm801_WinMain))
		{
			//lpMain->Initializer();
			iRet = bwm801_WinMain(lpMain,hInstance, hPrevInstance, lpszCmdLine, nCmdShow);
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


	
	
