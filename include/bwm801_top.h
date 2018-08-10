#pragma once

#ifdef WIN32
#include <Windows.h>
#include <windowsx.h>
#endif

namespace bwm801_top
{
	extern "C"
	{
#ifndef WIN32
		typedef int(*fp_bwm801main)(bwm801::main * i_lpcMain, int i_iArg_Count, const char * i_lpszArg_Values[]);
		EXPORT int bwm801_main(bwm801::main * i_lpcMain, int i_iArg_Count, const char * i_lpszArg_Values[]);
#else
		typedef INT(*fp_bwm801main)(bwm801::main * i_lpcMain, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT nCmdShow);
		INT EXPORT  WINAPI bwm801_WinMain(bwm801::main * i_lpcMain, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT nCmdShow);
#endif

	}
};



