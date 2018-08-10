#pragma once

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace bwm801_top
{
	extern "C"
	{
		EXPORT int bwm801_main(bwm801::main * i_lpcMain, int i_iArg_Count, const char * i_lpszArg_Values[]);
	}
};



#undef EXPORT

