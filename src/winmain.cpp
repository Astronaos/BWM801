#include <windows.h>
#include <windowsx.h>
#include <cmath>
#include <cstdio>
#include <GL/gl.h>
#include <process.h>    /* _beginthread, _endthread */
#include <bwm801.h>
#include <ctime>
#include <sstream>
#include <thread>
#include <CRTDBG.H>
#include <core_screenshot.h>
// MS Visual c++ doesn't have nan("") and isnan defined
#ifndef __MINGW32__//_WIN32
    const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
//	double nan(const char * i_lpszFlags){float fNan = (*(const float *)__nan); return (double)fNan;}
//	bool isnan(const double & i_dValue){return _isnan(i_dValue) != 0;}
#endif
#include <bwm801_glext.h>

using namespace bwm801;


HINSTANCE		g_hInst = 0;
HGLRC			g_hRC = 0;
HWND			g_hWnd = 0;
bool			g_bRC_Created = false;
HDC				g_hDC = 0;
GLuint			g_uiFont_Glyph_Lists_Poly[11][4] = {{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1}};
GLuint			g_uiFont_Glyph_Lists_Lines[11][4] = {{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1}};
GLuint *		g_lpuiCurrent_Font_Poly =  NULL;
GLuint *		g_lpuiCurrent_Font_Lines =  NULL;
GLYPHMETRICSFLOAT g_gmfGlyph_Metrics[11][4][256]; 
GLYPHMETRICSFLOAT * g_lpgmfCurrent_Font_Glyphs = NULL;
std::string		g_cszFont_Paths[8] = {"","","","","","","",""};
pair<unsigned int>	Fix_Coordinate_Global(const pair<unsigned int> &i_tPosition)
{
	pair<unsigned int> tRet;
	RECT  rectClient;
	GetClientRect(NULL, &rectClient); 
//	printf("Global geo: %i %i\n",uiWidth,uiHeight);
	tRet.m_tX = i_tPosition.m_tX;
	tRet.m_tY = (rectClient.bottom - i_tPosition.m_tY);
	return tRet;
}
pair<unsigned int>	Fix_Coordinate_Window(const pair<unsigned int> &i_tPosition)
{
	pair<unsigned int> tRet;
	RECT  rectClient;
	GetClientRect(g_hWnd, &rectClient); 
//	printf("window geo: %i %i\n",uiWidth,uiHeight);
	tRet.m_tX = i_tPosition.m_tX;
	tRet.m_tY = (rectClient.bottom - i_tPosition.m_tY);
	return tRet;
}


void Load_OGL_Extensions(void)
{
	g_lpf_glGenerateMipmap = reinterpret_cast<oglve>(wglGetProcAddress("glGenerateMipmap"));
	g_lpf_glGenerateTextureMipmap = reinterpret_cast<oglvui>(wglGetProcAddress("glGenerateTextureMipmap"));
}


void Gfx_Loop(void)
{
	LARGE_INTEGER	cLast_Counter, cFrequency;
	QueryPerformanceCounter(&cLast_Counter);
	QueryPerformanceFrequency(&cFrequency);
	double	dFreq = (double)(cFrequency.QuadPart);
	bool	bFirst_Draw = true;

	double	dInv_Freq = 1.0 / dFreq;

	while (g_hWnd == 0) // wait until main thread has created a window
		Sleep(20);
	int				iPixelFormat;
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,0,
		0,0,0,0,0,
		32,
		8,
		0,
		PFD_main_PLANE,
		0,
		0,0,0
	};
	g_hDC = GetDC(g_hWnd);

	iPixelFormat = ChoosePixelFormat(g_hDC, &pfd);
	SetPixelFormat(g_hDC, iPixelFormat, &pfd);

	g_hRC = wglCreateContext(g_hDC);
	wglMakeCurrent(g_hDC, g_hRC);
#ifdef _DEBUG
	std::string szVendor = (char *)glGetString(GL_VENDOR);
	std::string szRenderer = (char *)glGetString(GL_RENDERER);
	std::string szVersion = (char *)glGetString(GL_VERSION);
	_RPT1(_CRT_WARN, "Vendor: %s\n",szVendor.c_str());
	_RPT1(_CRT_WARN, "Renderer: %s\n", szRenderer.c_str());
	_RPT1(_CRT_WARN, "Version: %s\n", szVersion.c_str());
#endif
	std::map<std::string, int> vstrGL_Extensions_list;

	std::string szExtensions = (char *)glGetString(GL_EXTENSIONS);
	if (!szExtensions.empty())
	{
		size_t sEnd = szExtensions.find_first_of(' ');
		size_t sLast = 0;
		while (sEnd != std::string::npos)
		{
			vstrGL_Extensions_list[szExtensions.substr(sLast, (sEnd - sLast))] = 1;
			sLast = sEnd + 1;
			sEnd = szExtensions.find_first_of(' ', sLast);
		}
		vstrGL_Extensions_list[szExtensions.substr(sLast, (szExtensions.size() - sLast))] = 1;
	}
	//	for (std::vector<std::string>::iterator cI = vstrGL_Extensions_list.begin(); cI != vstrGL_Extensions_list.end(); cI++)
	//	{
	//		printf("%s\n",cI->c_str());
	//	}
	if (vstrGL_Extensions_list.count("WGL_EXT_swap_control") > 0)
	{
		typedef BOOL (WINAPI * PFNGLXSWAPINTERVALEXTPROC)(int);
		PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = NULL;
		glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (glXSwapIntervalEXT)
			glXSwapIntervalEXT(0);
	}


	g_bRC_Created = true;

	g_lpMain->gfx_init();
	g_lpMain->gfx_reshape(g_lpMain->Get_Window_Size());

	std::deque<double> vFrame_Rate;
	while (!g_lpMain->Pending_Quit())
	{
		if (g_lpMain->Pending_Draw() && g_lpMain->Get_Window_Size().m_tX > 0 && g_lpMain->Get_Window_Size().m_tY > 0)
		{
			LARGE_INTEGER	cCounter;
			QueryPerformanceCounter(&cCounter);
			double	dTimestep = (cCounter.QuadPart - cLast_Counter.QuadPart) * dInv_Freq;
			cLast_Counter = cCounter;
			vFrame_Rate.push_back(dTimestep);
			double dDraw_Time = 0.0;
			for (unsigned int uiI = 0; uiI < vFrame_Rate.size(); uiI++)
				dDraw_Time += vFrame_Rate[uiI];
			g_lpMain->Set_Frame_Rate(vFrame_Rate.size() / dDraw_Time);
			if (vFrame_Rate.size() > 10)
				vFrame_Rate.pop_front();
			if (g_bRC_Created && g_lpMain->Pending_Draw())
			{
				wglMakeCurrent(g_hDC, g_hRC); // just in case someone did something funny
				g_lpMain->Draw();
				glFlush();
				SwapBuffers(wglGetCurrentDC());
				bFirst_Draw = false;
			}
		}
		Sleep(0);
	}
	g_lpMain->gfx_close();
	g_bRC_Created = false;
	for (unsigned int uiI = 0; uiI < 11; uiI++)
	{
		for (unsigned int uiJ = 0; uiJ < 4; uiJ++)
		{
			if (g_uiFont_Glyph_Lists_Poly[uiI][uiJ] != -1)
				glDeleteLists(g_uiFont_Glyph_Lists_Poly[uiI][uiJ], 256);
			if (g_uiFont_Glyph_Lists_Lines[uiI][uiJ] != -1)
				glDeleteLists(g_uiFont_Glyph_Lists_Lines[uiI][uiJ], 256);
		}
	}
	for (unsigned int uiI = 0; uiI < 8; uiI++)
	{
		if (g_cszFont_Paths[uiI][0] != 0)
			RemoveFontResourceEx(g_cszFont_Paths[uiI].c_str(), FR_PRIVATE, 0);
	}

	wglMakeCurrent(g_hDC, NULL);
	ReleaseDC(g_hWnd, g_hDC);
	wglDeleteContext(g_hRC);
}

void Main_Timer_Loop(void)
{
	LARGE_INTEGER	cLast_Counter, cFrequency;
	QueryPerformanceCounter(&cLast_Counter);
	QueryPerformanceFrequency(&cFrequency);
	double	dFreq = (double)(cFrequency.QuadPart);
	double	dInv_Freq = 1.0 / dFreq;
	while (g_hWnd == 0) // wait until main thread has created a window
		Sleep(20);
	while (!g_lpMain->Pending_Quit())
	{
		LARGE_INTEGER	cCounter;
		QueryPerformanceCounter(&cCounter);
		double	dTimestep = (cCounter.QuadPart - cLast_Counter.QuadPart) * dInv_Freq;
		cLast_Counter = cCounter;
		g_lpMain->On_Timer(0, dTimestep);
		Sleep(66); // pass back to Windows
	}
}

/* COMPILER(MINGW) - MinGW GCC */
//#if defined(MINGW) || defined(__MINGW32__)
//long FAR PASCAL WndProc(HWND hwnd,UINT msg,UINT wParam,LONG lParam);
/* COMPILER(MINGW64) - mingw-w64 GCC */
//#elif defined(__MINGW64__)
//long long FAR PASCAL WndProc(HWND hwnd,UINT msg,UINT wParam,LONG lParam);
//#else
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
//#endif

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT nCmdShow)
{
	MSG msg;
    WNDCLASS wndclass;
	char lpszWindow[] = {"Window"};
	char lpszClass[] = {"Class"};
	int iReturn;
	if (g_lpMain)
	{
		std::vector<std::thread> vThread_List;

		Initialize_Circle_Vectors();
		g_lpMain->init();

		g_bRC_Created = false;
		g_hInst = hInstance;

		 // load opengl extensions
		Load_OGL_Extensions();

		if(!hPrevInstance)
   		{
			wndclass.style = 0;
    		wndclass.lpfnWndProc = WndProc;
    		wndclass.cbClsExtra = 0;
    		wndclass.cbWndExtra = 0;
			wndclass.hInstance = hInstance;
			wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    		wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    		wndclass.hbrBackground = NULL;//(HBRUSH)GetStockObject(GRAY_BRUSH);
    		wndclass.lpszMenuName = NULL;
    		wndclass.lpszClassName = lpszClass;

			RegisterClass(&wndclass);

		}	

		g_hWnd = CreateWindow(lpszClass,
    						lpszWindow,
							WS_OVERLAPPEDWINDOW,
							//WS_POPUP,
							0,//CW_USEDEFAULT,
    						0,//CW_USEDEFAULT,
							1280,
							600,
							NULL,
							NULL,
							hInstance,
							NULL);

		ShowWindow(g_hWnd,nCmdShow);
		UpdateWindow(g_hWnd);

		std::string szScreenshot_Default_Path = "C:\\"; //@@TODO: make this the user's Pictures folder
		g_lpMain->Set_Screenshot_Save_Path(szScreenshot_Default_Path);

		vThread_List.push_back(std::thread(Main_Timer_Loop));
		vThread_List.push_back(std::thread(Gfx_Loop));
		vThread_List.push_back(std::thread(Screenshot_Loop));


		while (iReturn = GetMessage(&msg,NULL,0,0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		for (std::vector<std::thread>::iterator cI = vThread_List.begin(); cI != vThread_List.end(); cI++)
			cI->join();
		g_lpMain->close();

		for (std::vector<std::thread>::iterator cI = vThread_List.begin(); cI != vThread_List.end(); cI++)
		{
			cI->join();
		}
	}
	return iReturn;
}


uintptr_t g_hMainThread = 0;

main::KEYID Key_Map(UINT wParam)
{
	main::KEYID		eKey = main::KEY_NOOP;
	switch (wParam)
	{
	case VK_LBUTTON: eKey = main::KEY_MB_LEFT; break;
	case VK_RBUTTON: eKey = main::KEY_MB_RGT; break;
	case VK_CANCEL: eKey = main::KEY_CANCEL; break;
	case VK_MBUTTON: eKey = main::KEY_MB_CTR; break;

#if(_WIN32_WINNT >= 0x0500)
	case VK_XBUTTON1: eKey = main::KEY_MB_X1; break;
	case VK_XBUTTON2: eKey = main::KEY_MB_X2; break;
#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0x07 : unassigned
 */

	case VK_BACK: eKey = main::KEY_BACKSPACE; break;
	case VK_TAB: eKey = main::KEY_TAB; break;

/*
 * 0x0A - 0x0B : reserved
 */

	case VK_CLEAR: eKey = main::KEY_CLEAR; break;
	case VK_RETURN: eKey = main::KEY_ENTER; break;

	case VK_SHIFT: eKey = main::KEY_SHIFT; break;
	case VK_CONTROL: eKey = main::KEY_CONTROL; break;
	case VK_MENU: eKey = main::KEY_ALT; break;
	case VK_PAUSE: eKey = main::KEY_PAUSE; break;
	case VK_CAPITAL: eKey = main::KEY_CAPSLOCK; break;

//	case VK_KANA: eKey = main::KEY_KANA; break;
	case VK_HANGUL: eKey = main::KEY_HANGUL; break;
	case VK_JUNJA: eKey = main::KEY_JUNJA; break;
	case VK_FINAL: eKey = main::KEY_FINAL; break;
//	case VK_HANJA: eKey = main::KEY_HANJA; break;
	case VK_KANJI: eKey = main::KEY_KANJI; break;

	case VK_ESCAPE: eKey = main::KEY_ESCAPE; break;

	case VK_CONVERT: eKey = main::KEY_CONVERT; break;
	case VK_NONCONVERT: eKey = main::KEY_NONCONVERT; break;
	case VK_ACCEPT: eKey = main::KEY_ACCEPT; break;
	case VK_MODECHANGE: eKey = main::KEY_MODECHANGE; break;

	case VK_SPACE: eKey = main::KEY_SPACE; break;
	case VK_PRIOR: eKey = main::KEY_PGUP; break;
	case VK_NEXT: eKey = main::KEY_PGDN; break;
	case VK_END: eKey = main::KEY_END; break;
	case VK_HOME: eKey = main::KEY_HOME; break;
	case VK_LEFT: eKey = main::KEY_LEFT; break;
	case VK_UP: eKey = main::KEY_UP; break;
	case VK_RIGHT: eKey = main::KEY_RIGHT; break;
	case VK_DOWN: eKey = main::KEY_DOWN; break;
	case VK_SELECT: eKey = main::KEY_SELECT; break;
	case VK_PRINT: eKey = main::KEY_PRINT; break;
	case VK_EXECUTE: eKey = main::KEY_EXEC; break;
	case VK_SNAPSHOT: eKey = main::KEY_PRSCRN; break;
	case VK_INSERT: eKey = main::KEY_INSERT; break;
	case VK_DELETE: eKey = main::KEY_DELETE; break;
	case VK_HELP: eKey = main::KEY_HELP; break;

/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */
	case 0x30:	eKey = main::KEY_0; break;
	case 0x31:	eKey = main::KEY_1; break;
	case 0x32:	eKey = main::KEY_2; break;
	case 0x33:	eKey = main::KEY_3; break;
	case 0x34:	eKey = main::KEY_4; break;
	case 0x35:	eKey = main::KEY_5; break;
	case 0x36:	eKey = main::KEY_6; break;
	case 0x37:	eKey = main::KEY_7; break;
	case 0x38:	eKey = main::KEY_8; break;
	case 0x39:	eKey = main::KEY_9; break;

	case 0x41:	eKey = main::KEY_A; break;
	case 0x42:	eKey = main::KEY_B; break;
	case 0x43:	eKey = main::KEY_C; break;
	case 0x44:	eKey = main::KEY_D; break;
	case 0x45:	eKey = main::KEY_E; break;
	case 0x46:	eKey = main::KEY_F; break;
	case 0x47:	eKey = main::KEY_G; break;
	case 0x48:	eKey = main::KEY_H; break;
	case 0x49:	eKey = main::KEY_I; break;
	case 0x4a:	eKey = main::KEY_J; break;
	case 0x4b:	eKey = main::KEY_K; break;
	case 0x4c:	eKey = main::KEY_L; break;
	case 0x4d:	eKey = main::KEY_M; break;
	case 0x4e:	eKey = main::KEY_N; break;
	case 0x4f:	eKey = main::KEY_O; break;
	case 0x50:	eKey = main::KEY_P; break;
	case 0x51:	eKey = main::KEY_Q; break;
	case 0x52:	eKey = main::KEY_R; break;
	case 0x53:	eKey = main::KEY_S; break;
	case 0x54:	eKey = main::KEY_T; break;
	case 0x55:	eKey = main::KEY_U; break;
	case 0x56:	eKey = main::KEY_V; break;
	case 0x57:	eKey = main::KEY_W; break;
	case 0x58:	eKey = main::KEY_X; break;
	case 0x59:	eKey = main::KEY_Y; break;
	case 0x5a:	eKey = main::KEY_Z; break;

	case VK_LWIN: eKey = main::KEY_LWIN; break;
	case VK_RWIN: eKey = main::KEY_RWIN; break;
	case VK_APPS: eKey = main::KEY_APPS; break;

/*
 * 0x5E : reserved
 */

	case VK_SLEEP: eKey = main::KEY_SLEEP; break;

	case VK_NUMPAD0: eKey = main::KEY_NUMPD_0; break;
	case VK_NUMPAD1: eKey = main::KEY_NUMPD_1; break;
	case VK_NUMPAD2: eKey = main::KEY_NUMPD_2; break;
	case VK_NUMPAD3: eKey = main::KEY_NUMPD_3; break;
	case VK_NUMPAD4: eKey = main::KEY_NUMPD_4; break;
	case VK_NUMPAD5: eKey = main::KEY_NUMPD_5; break;
	case VK_NUMPAD6: eKey = main::KEY_NUMPD_6; break;
	case VK_NUMPAD7: eKey = main::KEY_NUMPD_7; break;
	case VK_NUMPAD8: eKey = main::KEY_NUMPD_8; break;
	case VK_NUMPAD9: eKey = main::KEY_NUMPD_9; break;
	case VK_MULTIPLY: eKey = main::KEY_NUMPD_TIMES; break;
	case VK_ADD: eKey = main::KEY_NUMPD_PLUS; break;
	case VK_SEPARATOR: eKey = main::KEY_NUMPD_ENTER; break;
	case VK_SUBTRACT: eKey = main::KEY_NUMPD_MINUS; break;
	case VK_DECIMAL: eKey = main::KEY_NUMPD_DOT; break;
	case VK_DIVIDE: eKey = main::KEY_NUMPD_DIV; break;
	case VK_F1: eKey = main::KEY_F1; break;
	case VK_F2: eKey = main::KEY_F2; break;
	case VK_F3: eKey = main::KEY_F3; break;
	case VK_F4: eKey = main::KEY_F4; break;
	case VK_F5: eKey = main::KEY_F5; break;
	case VK_F6: eKey = main::KEY_F6; break;
	case VK_F7: eKey = main::KEY_F7; break;
	case VK_F8: eKey = main::KEY_F8; break;
	case VK_F9: eKey = main::KEY_F9; break;
	case VK_F10: eKey = main::KEY_F10; break;
	case VK_F11: eKey = main::KEY_F11; break;
	case VK_F12: eKey = main::KEY_F12; break;
	case VK_F13: eKey = main::KEY_F13; break;
	case VK_F14: eKey = main::KEY_F14; break;
	case VK_F15: eKey = main::KEY_F15; break;
	case VK_F16: eKey = main::KEY_F16; break;
	case VK_F17: eKey = main::KEY_F17; break;
	case VK_F18: eKey = main::KEY_F18; break;
	case VK_F19: eKey = main::KEY_F19; break;
	case VK_F20: eKey = main::KEY_F20; break;
	case VK_F21: eKey = main::KEY_F21; break;
	case VK_F22: eKey = main::KEY_F22; break;
	case VK_F23: eKey = main::KEY_F23; break;
	case VK_F24: eKey = main::KEY_F24; break;

/*
 * 0x88 - 0x8F : unassigned
 */

	case VK_NUMLOCK: eKey = main::KEY_NMLCK; break;
	case VK_SCROLL: eKey = main::KEY_SCROLLLOCK; break;
		// ----------------------------
		// OEM specific keys.
		// Not mapped
		// ----------------------------
//
// NEC PC-9800 kbd definitions
//
	case VK_OEM_NEC_EQUAL: eKey = main::KEY_OEM; break;

//
// Fujitsu/OASYS kbd definitions
//
//	case VK_OEM_FJ_JISHO: eKey = main::KEY_OEM; break;
	case VK_OEM_FJ_MASSHOU: eKey = main::KEY_OEM; break;
	case VK_OEM_FJ_TOUROKU: eKey = main::KEY_OEM; break;
	case VK_OEM_FJ_LOYA: eKey = main::KEY_OEM; break;
	case VK_OEM_FJ_ROYA: eKey = main::KEY_OEM; break;

/*
 * 0x97 - 0x9F : unassigned
 */

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
	case VK_LSHIFT: eKey = main::KEY_LSHIFT; break;
	case VK_RSHIFT: eKey = main::KEY_RSHIFT; break;
	case VK_LCONTROL: eKey = main::KEY_LCTRL; break;
	case VK_RCONTROL: eKey = main::KEY_RCTRL; break;
	case VK_LMENU: eKey = main::KEY_LALT; break;
	case VK_RMENU: eKey = main::KEY_RALT; break;

#if(_WIN32_WINNT >= 0x0500)
	case VK_BROWSER_BACK: eKey = main::KEY_BRSR_BACK; break;
	case VK_BROWSER_FORWARD: eKey = main::KEY_BRSR_FWD; break;
	case VK_BROWSER_REFRESH: eKey = main::KEY_BRSR_REFRESH; break;
	case VK_BROWSER_STOP: eKey = main::KEY_BRSR_STOP; break;
	case VK_BROWSER_SEARCH: eKey = main::KEY_BRSR_SEARCH; break;
	case VK_BROWSER_FAVORITES: eKey = main::KEY_BRSR_FAV; break;
	case VK_BROWSER_HOME: eKey = main::KEY_BRSR_HOME; break;

	case VK_VOLUME_MUTE: eKey = main::KEY_VOLMUTE; break;
	case VK_VOLUME_DOWN: eKey = main::KEY_VOLDN; break;
	case VK_VOLUME_UP: eKey = main::KEY_VOLUP; break;
	case VK_MEDIA_NEXT_TRACK: eKey = main::KEY_MEDIA_NEXT; break;
	case VK_MEDIA_PREV_TRACK: eKey = main::KEY_MEDIA_PREV; break;
	case VK_MEDIA_STOP: eKey = main::KEY_MEDIA_STOP; break;
	case VK_MEDIA_PLAY_PAUSE: eKey = main::KEY_MEDIA_PLAY; break;
	case VK_LAUNCH_MAIL: eKey = main::KEY_EMAIL; break;
	case VK_LAUNCH_MEDIA_SELECT: eKey = main::KEY_MEDIA_SELECT; break;
	case VK_LAUNCH_APP1: eKey = main::KEY_APP_1; break;
	case VK_LAUNCH_APP2: eKey = main::KEY_APP_2; break;

#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0xB8 - 0xB9 : reserved
 */

	case VK_OEM_1: eKey = main::KEY_SEMICOLON; break; //warning: should do this as region / keyboard specific
	case VK_OEM_PLUS: eKey = main::KEY_EQUALS; break;
	case VK_OEM_COMMA: eKey = main::KEY_COMMA; break;
	case VK_OEM_MINUS: eKey = main::KEY_MINUS; break;
	case VK_OEM_PERIOD: eKey = main::KEY_PERIOD; break;
	case VK_OEM_2: eKey = main::KEY_SLASH; break; // warning: depends on region 
	case VK_OEM_3: eKey = main::KEY_TILDE; break; // warning: depends on region 

/*
 * 0xC1 - 0xD7 : reserved
 */

/*
 * 0xD8 - 0xDA : unassigned
 */

	case VK_OEM_4: eKey = main::KEY_LSQBRKT; break; // warning: depends on region 
	case VK_OEM_5: eKey = main::KEY_BACKSLASH; break; // warning: depends on region 
	case VK_OEM_6: eKey = main::KEY_RSQBRKT; break; // warning: depends on region 
	case VK_OEM_7: eKey = main::KEY_SQUOTE; break; // warning: depends on region 
	case VK_OEM_8: eKey = main::KEY_OEM; break;

/*
 * 0xE0 : reserved
 */

/*
 * Various extended or enhanced keyboards
 */
	case VK_OEM_AX: eKey = main::KEY_OEM; break;
	case VK_OEM_102: eKey = main::KEY_OEM; break;
	case VK_ICO_HELP: eKey = main::KEY_OEM; break;
	case VK_ICO_00: eKey = main::KEY_OEM; break;

#if(WINVER >= 0x0400)
	case VK_PROCESSKEY: eKey = main::KEY_OEM; break;
#endif /* WINVER >= 0x0400 */

case VK_ICO_CLEAR: eKey = main::KEY_OEM; break;


#if(_WIN32_WINNT >= 0x0500)
	case VK_PACKET: eKey = main::KEY_OEM; break;
#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0xE8 : unassigned
 */

/*
 * Nokia/Ericsson definitions
 */
	case VK_OEM_RESET: eKey = main::KEY_OEM; break;
	case VK_OEM_JUMP: eKey = main::KEY_OEM; break;
	case VK_OEM_PA1: eKey = main::KEY_OEM; break;
	case VK_OEM_PA2: eKey = main::KEY_OEM; break;
	case VK_OEM_PA3: eKey = main::KEY_OEM; break;
	case VK_OEM_WSCTRL: eKey = main::KEY_OEM; break;
	case VK_OEM_CUSEL: eKey = main::KEY_OEM; break;
	case VK_OEM_ATTN: eKey = main::KEY_OEM; break;
	case VK_OEM_FINISH: eKey = main::KEY_OEM; break;
	case VK_OEM_COPY: eKey = main::KEY_OEM; break;
	case VK_OEM_AUTO: eKey = main::KEY_OEM; break;
	case VK_OEM_ENLW: eKey = main::KEY_OEM; break;
	case VK_OEM_BACKTAB: eKey = main::KEY_OEM; break;

	case VK_ATTN: eKey = main::KEY_ATTN; break;
	case VK_CRSEL: eKey = main::KEY_CRSEL; break;
	case VK_EXSEL: eKey = main::KEY_EXSEL; break;
	case VK_EREOF: eKey = main::KEY_EREOF; break;
	case VK_PLAY: eKey = main::KEY_PLAY; break;
	case VK_ZOOM: eKey = main::KEY_ZOOM; break;
	case VK_NONAME: eKey = main::KEY_NONAME; break;
	case VK_PA1: eKey = main::KEY_PA1; break;
	case VK_OEM_CLEAR: eKey = main::KEY_OEM; break;
	}
	return eKey;
}


LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch (msg)
   	{
    case WM_CREATE:
		// no break - set window size
	case WM_SIZE:
		{
			RECT rectClient;
			GetClientRect(hwnd, &rectClient); 
			g_hWnd = hwnd;
			g_lpMain->On_Window_Move(Fix_Coordinate_Global(pair<unsigned int>(0,0)));
			g_lpMain->On_Window_Resize(pair<unsigned int>(rectClient.right,rectClient.bottom));
		}
		break;
// Keyboard operations
	case WM_KEYDOWN:
		g_lpMain->On_Key_Down(Key_Map(wParam),(lParam & 0x0ff00) >> 8, lParam & 0xffff, (lParam & 0x01000000) != 0, (lParam & 0x040000000) != 0);
			
		break;
	case WM_KEYUP:
		g_lpMain->On_Key_Up(Key_Map(wParam),(lParam & 0x0ff00) >> 8,lParam & 0xffff, (lParam & 0x01000000) != 0, (lParam & 0x040000000) != 0);
		break;
// Mouse operations
	case WM_MBUTTONDBLCLK:
		g_lpMain->On_Mouse_Button_Double_Click(main::MB_CTR, Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_MBUTTONDOWN:
		g_lpMain->On_Mouse_Button_Down(main::MB_CTR, Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_MBUTTONUP:
		g_lpMain->On_Mouse_Button_Up(main::MB_CTR, Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_RBUTTONDBLCLK:
		g_lpMain->On_Mouse_Button_Double_Click(main::MB_RGT, Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_RBUTTONDOWN:
		g_lpMain->On_Mouse_Button_Down(main::MB_RGT, Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_RBUTTONUP:
		g_lpMain->On_Mouse_Button_Up(main::MB_RGT,  Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_LBUTTONDBLCLK:
		g_lpMain->On_Mouse_Button_Double_Click(main::MB_LEFT, Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_LBUTTONDOWN:
		g_lpMain->On_Mouse_Button_Down(main::MB_LEFT, Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_LBUTTONUP:
		g_lpMain->On_Mouse_Button_Up(main::MB_LEFT, Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_MOUSEMOVE:
		g_lpMain->On_Mousemove(Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_MOUSEWHEEL:
		g_lpMain->On_Mouse_Wheel(main::MB_SCROLL_V, GET_WHEEL_DELTA_WPARAM(wParam), Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
#if !defined(__MINGW64__) && !defined(__MINGW32__)
	case WM_MOUSEHWHEEL:
		g_lpMain->On_Mouse_Wheel(main::MB_SCROLL_V, GET_WHEEL_DELTA_WPARAM(wParam), Fix_Coordinate_Window(pair<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
#endif
	case WM_TIMER:
//		on_timer(wParam);
		break;

	case WM_PAINT:
		g_lpMain->Request_Refresh();
		break;

    case WM_DESTROY:
		g_lpMain->Request_Quit(); // in case user closed window, make sure to exit gracefully
		Sleep(100); // give the timer loop time to shut down
		PostQuitMessage(0);
		break;
	}
    return DefWindowProc(hwnd,msg,wParam,lParam);
}

std::string Read_TTF_Font_Name(const char * i_lpszPath_To_Font);
void LoadFontFace(FONT i_eFont, const char * i_lpszPath_To_Font, bool i_bBold, bool i_bItalics)
{
	if (i_eFont >= USER_1 && i_eFont <= USER_8)
	{
		unsigned int uiFont = (i_eFont - SANS);
		unsigned int uiFace = (i_bBold ? 1 : 0) + (i_bItalics ? 2 : 0);
		std::string csFont_Name;
		AddFontResourceEx(i_lpszPath_To_Font,FR_PRIVATE,0);
		g_cszFont_Paths[i_eFont - USER_1] = i_lpszPath_To_Font;
		csFont_Name = Read_TTF_Font_Name(i_lpszPath_To_Font);
		g_uiFont_Glyph_Lists_Poly[uiFont][uiFace] = glGenLists(256);
		g_uiFont_Glyph_Lists_Lines[uiFont][uiFace] = glGenLists(256);
		HFONT hFont = CreateFont( -24,0,0,0,i_bBold ? FW_BOLD : FW_NORMAL, i_bItalics, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, 
			ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, csFont_Name.c_str());
		HFONT hOldFont = (HFONT)SelectObject(g_hDC, hFont);       // Selects The Font We Want
		wglUseFontOutlines( g_hDC, 0, 255, g_uiFont_Glyph_Lists_Poly[uiFont][uiFace], 0.0f, 0.2f, WGL_FONT_POLYGONS, g_gmfGlyph_Metrics[uiFont][uiFace]);
		wglUseFontOutlines( g_hDC, 0, 255, g_uiFont_Glyph_Lists_Lines[uiFont][uiFace], 0.0f, 0.2f, WGL_FONT_LINES, g_gmfGlyph_Metrics[uiFont][uiFace]);
		SelectObject(g_hDC, hOldFont);             // Set system back to default font
		DeleteObject(hFont);                 // Delete The Font

	}
}

void SelectFontFace(FONT i_eFont, bool i_bBold, bool i_bItalics)
{
	unsigned int uiFont = (i_eFont - SANS);
	unsigned int uiFace = (i_bBold ? 1 : 0) + (i_bItalics ? 2 : 0);
	if (g_uiFont_Glyph_Lists_Poly[uiFont][uiFace] == -1)
	{
		std::string csFont_Name;

		switch (i_eFont)
		{
		case SANS:
			csFont_Name ="Arial";
			break;
		case SERIF:
			csFont_Name ="Times New Roman";
			break;
		case MONO:
			csFont_Name ="Courier New";
			break;
		}
		g_uiFont_Glyph_Lists_Poly[uiFont][uiFace] = glGenLists(256);
		g_uiFont_Glyph_Lists_Lines[uiFont][uiFace] = glGenLists(256);
		HFONT hFont = CreateFont( -24,0,0,0,i_bBold ? FW_BOLD : FW_NORMAL, i_bItalics, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, 
			ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, csFont_Name.c_str());
		HFONT hOldFont = (HFONT)SelectObject(g_hDC, hFont);       // Selects The Font We Want
		wglUseFontOutlines( g_hDC, 0, 255, g_uiFont_Glyph_Lists_Poly[uiFont][uiFace], 0.0f, 0.2f, WGL_FONT_POLYGONS, g_gmfGlyph_Metrics[uiFont][uiFace]);
		wglUseFontOutlines( g_hDC, 0, 255, g_uiFont_Glyph_Lists_Lines[uiFont][uiFace], 0.0f, 0.2f, WGL_FONT_LINES, g_gmfGlyph_Metrics[uiFont][uiFace]);
		SelectObject(g_hDC, hOldFont);             // Set system back to default font
		DeleteObject(hFont);                 // Delete The Font
	}
	g_lpuiCurrent_Font_Poly = &g_uiFont_Glyph_Lists_Poly[uiFont][uiFace];
	g_lpuiCurrent_Font_Lines = &g_uiFont_Glyph_Lists_Lines[uiFont][uiFace];
	g_lpgmfCurrent_Font_Glyphs = g_gmfGlyph_Metrics[uiFont][uiFace];
}

void glPrint(const double & i_dSize, const double & i_dX, const double & i_dY, const double & i_dZ, const char * i_lpszString)
{
	if (g_lpuiCurrent_Font_Poly[0] != -1)
	{
		glPushMatrix();
			glTranslated(i_dX,i_dY,i_dZ);
			glScaled(i_dSize,i_dSize,1.0);
			glPushAttrib(GL_LIST_BIT);              // Pushes The Display List Bits     ( NEW )
			glListBase(g_lpuiCurrent_Font_Poly[0]);  
			glCallLists(strlen(i_lpszString), GL_UNSIGNED_BYTE, i_lpszString);  // Draws The Display List Text  ( NEW )
			glPopAttrib(); // Pops The Display List Bits   ( NEW )
		glPopMatrix();
	}
	if (g_lpuiCurrent_Font_Lines[0] != -1)
	{
		glPushMatrix();
			glTranslated(i_dX,i_dY,i_dZ);
			glScaled(i_dSize,i_dSize,1.0);
			glPushAttrib(GL_LIST_BIT | GL_LINE_BIT);              // Pushes The Display List Bits     ( NEW )
			glEnable(GL_LINE_SMOOTH);
			glLineWidth(0.7f);
			glListBase(g_lpuiCurrent_Font_Lines[0]);  
			glCallLists(strlen(i_lpszString), GL_UNSIGNED_BYTE, i_lpszString);  // Draws The Display List Text  ( NEW )
			glPopAttrib(); // Pops The Display List Bits   ( NEW )
		glPopMatrix();
	}
}

void TextBBox(const double & i_dSize, const char * i_lpszString, pair<double> & o_cBot_Left, pair<double> & o_cTop_Right)
{
	o_cBot_Left.m_tX = o_cBot_Left.m_tY = o_cTop_Right.m_tX = o_cTop_Right.m_tY = 0.0;
	const char * lpszCursor = i_lpszString;
	if (lpszCursor)
	{ // this actually gets us the top left - fix after processing
		o_cBot_Left.m_tX = g_lpgmfCurrent_Font_Glyphs[lpszCursor[0]].gmfptGlyphOrigin.x;
		o_cBot_Left.m_tY = g_lpgmfCurrent_Font_Glyphs[lpszCursor[0]].gmfptGlyphOrigin.y; 
	}
	while (lpszCursor && lpszCursor[0] != 0)
	{
		o_cTop_Right.m_tX += g_lpgmfCurrent_Font_Glyphs[lpszCursor[0]].gmfBlackBoxX;
		if (o_cTop_Right.m_tY < g_lpgmfCurrent_Font_Glyphs[lpszCursor[0]].gmfBlackBoxY)
			o_cTop_Right.m_tY = g_lpgmfCurrent_Font_Glyphs[lpszCursor[0]].gmfBlackBoxY;
		lpszCursor++;
	}
	double dTemp = o_cBot_Left.m_tY;
	//o_cTop_Right.m_tX -= o_cBot_Left.m_tX
	o_cBot_Left.m_tY = dTemp - o_cTop_Right.m_tY;
	o_cTop_Right.m_tY = dTemp;

	o_cBot_Left *= i_dSize;
	o_cTop_Right *= i_dSize;
}

//All following code from http://www.codeproject.com/Articles/2293/Retrieving-font-name-from-TTF-file
//This is TTF file header
typedef struct _tagTT_OFFSET_TABLE{
    USHORT uMajorVersion;
    USHORT uMinorVersion;
    USHORT uNumOfTables;
    USHORT uSearchRange;
    USHORT uEntrySelector;
    USHORT uRangeShift;
}TT_OFFSET_TABLE;

//Tables in TTF file and there placement and name (tag)
typedef struct _tagTT_TABLE_DIRECTORY{
    char szTag[4]; //table name
    ULONG uCheckSum; //Check sum
    ULONG uOffset; //Offset from beginning of file
    ULONG uLength; //length of the table in bytes
}TT_TABLE_DIRECTORY;

//Header of names table
typedef struct _tagTT_NAME_TABLE_HEADER{
    USHORT uFSelector; //format selector. Always 0
    USHORT uNRCount; //Name Records count
    USHORT uStorageOffset; //Offset for strings storage, 
                           //from start of the table
}TT_NAME_TABLE_HEADER;

//Record in names table
typedef struct _tagTT_NAME_RECORD{
    USHORT uPlatformID;
    USHORT uEncodingID;
    USHORT uLanguageID;
    USHORT uNameID;
    USHORT uStringLength;
    USHORT uStringOffset; //from start of storage area
}TT_NAME_RECORD;

#define SWAPWORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
#define SWAPLONG(x) MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))

std::string Read_TTF_Font_Name(const char * i_lpszPath_To_Font)
{
	std::string cszFontName = "";
	FILE * fileFont = fopen(i_lpszPath_To_Font,"rb");
	if (fileFont)
	{
		//define and read file header
		TT_OFFSET_TABLE ttOffsetTable;
		fread(&ttOffsetTable, sizeof(TT_OFFSET_TABLE),1,fileFont);

		//remember to rearrange bytes in the field you gonna use
		ttOffsetTable.uNumOfTables = SWAPWORD(ttOffsetTable.uNumOfTables);
		ttOffsetTable.uMajorVersion = SWAPWORD(ttOffsetTable.uMajorVersion);
		ttOffsetTable.uMinorVersion = SWAPWORD(ttOffsetTable.uMinorVersion);

		//check is this is a true type font and the version is 1.0
		if(ttOffsetTable.uMajorVersion == 1 || ttOffsetTable.uMinorVersion == 0)
		{
			TT_TABLE_DIRECTORY tblDir;
			BOOL bFound = FALSE;
			std::string cszTemp = "";

			for(int i=0; i< ttOffsetTable.uNumOfTables && !bFound; i++)
			{
				fread(&tblDir, sizeof(TT_TABLE_DIRECTORY),1,fileFont);
				cszTemp.empty();
				if (tblDir.szTag[0] == 'n' &&
					tblDir.szTag[1] == 'a' &&
					tblDir.szTag[2] == 'm' &&
					tblDir.szTag[3] == 'e')
				{
					//we found our table. Rearrange order and quit the loop
					bFound = TRUE;
					tblDir.uLength = SWAPLONG(tblDir.uLength);
					tblDir.uOffset = SWAPLONG(tblDir.uOffset);
				}
			}
			if(bFound)
			{
				//move to offset we got from Offsets Table
				fseek(fileFont,tblDir.uOffset,SEEK_SET);
				TT_NAME_TABLE_HEADER ttNTHeader;
				fread(&ttNTHeader, sizeof(TT_NAME_TABLE_HEADER),1,fileFont);

				//again, don't forget to swap bytes!
				ttNTHeader.uNRCount = SWAPWORD(ttNTHeader.uNRCount);
				ttNTHeader.uStorageOffset = SWAPWORD(ttNTHeader.uStorageOffset);
				TT_NAME_RECORD ttRecord;
				bFound = FALSE;
				for(int i=0; i<ttNTHeader.uNRCount && !bFound; i++)
				{
					fread(&ttRecord, sizeof(TT_NAME_RECORD),1,fileFont);
					ttRecord.uNameID = SWAPWORD(ttRecord.uNameID);

			//1 says that this is font name. 0 for example determines copyright info
					if(ttRecord.uNameID == 1)
					{
						ttRecord.uStringLength = SWAPWORD(ttRecord.uStringLength);
						ttRecord.uStringOffset = SWAPWORD(ttRecord.uStringOffset);
						fpos_t fpPos;
						//save file position, so we can return to continue with search
						fgetpos(fileFont,&fpPos);
						fseek(fileFont,tblDir.uOffset + ttRecord.uStringOffset + 
									ttNTHeader.uStorageOffset, SEEK_SET);
						if (ttRecord.uStringLength > 0)
						{
							char * lpszStr = new char[ttRecord.uStringLength + 1];
							fread(lpszStr,sizeof(char),ttRecord.uStringLength,fileFont);
							lpszStr[ttRecord.uStringLength] = 0;
							if (lpszStr[0] != 0)
							{
								cszFontName = lpszStr;
								bFound = true;
							}
							delete [] lpszStr;
						}
						fsetpos(fileFont,&fpPos);
					}
				}
			}
		}
		fclose(fileFont);
	}
	return cszFontName;
}

std::vector<std::string> main::Get_Directory_File_List(const std::string &i_szDirectory) /// get list of all files in the given direcctory
{
	std::vector<std::string> szRet;
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::ostringstream szAll;
	szAll << i_szDirectory;
	szAll << "/*.luftwaffe";
	hFind = FindFirstFile(szAll .str().c_str(), &ffd);
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			szRet.push_back(ffd.cFileName);
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
	return szRet;
}



