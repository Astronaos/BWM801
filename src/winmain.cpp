#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include <math.h>
#include <process.h>    /* _beginthread, _endthread */
#include <core.hpp>
#include <ctime>
#include <sstream>

// MS Visual c++ doesn't have nan("") and isnan defined
#ifndef __MINGW32__//_WIN32
    const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
	double nan(const char * i_lpszFlags){float fNan = (*(const float *)__nan); return (double)fNan;}
	bool isnan(const double & i_dValue){return _isnan(i_dValue) != 0;}
#endif

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
PAIR<unsigned int>	Fix_Coordinate_Global(const PAIR<unsigned int> &i_tPosition)
{
	PAIR<unsigned int> tRet;
	RECT  rectClient;
	GetClientRect(NULL, &rectClient); 
//	printf("Global geo: %i %i\n",uiWidth,uiHeight);
	tRet.m_tX = i_tPosition.m_tX;
	tRet.m_tY = (rectClient.bottom - i_tPosition.m_tY);
	return tRet;
}
PAIR<unsigned int>	Fix_Coordinate_Window(const PAIR<unsigned int> &i_tPosition)
{
	PAIR<unsigned int> tRet;
	RECT  rectClient;
	GetClientRect(g_hWnd, &rectClient); 
//	printf("window geo: %i %i\n",uiWidth,uiHeight);
	tRet.m_tX = i_tPosition.m_tX;
	tRet.m_tY = (rectClient.bottom - i_tPosition.m_tY);
	return tRet;
}

void MainThread(void *pData)
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
			PFD_MAIN_PLANE,
			0,
			0,0,0
	};
	g_hDC = GetDC(g_hWnd);

	iPixelFormat = ChoosePixelFormat(g_hDC,&pfd);
	SetPixelFormat(g_hDC,iPixelFormat,&pfd);

	g_hRC = wglCreateContext(g_hDC);
	wglMakeCurrent(g_hDC,g_hRC);

	g_bRC_Created = true;
//	g_lpMain->On_Window_Move(PAIR<unsigned int>(0,0));
//	g_lpMain->On_Window_Resize(PAIR<unsigned int>(1680 - 6,1050 - 6 - 30));

	g_lpMain->gfx_init();
	g_lpMain->gfx_reshape(g_lpMain->Get_Window_Size());
	while (!g_lpMain->Pending_Quit())
	{
		LARGE_INTEGER	cCounter;
		QueryPerformanceCounter(&cCounter);
		double	dTimestep = (cCounter.QuadPart - cLast_Counter.QuadPart) * dInv_Freq;
		cLast_Counter = cCounter;
		if (!bFirst_Draw)
			g_lpMain->On_Timer(0,dTimestep);
		if (g_bRC_Created && g_lpMain->Pending_Draw())
		{
			wglMakeCurrent(g_hDC,g_hRC); // just in case someone did something funny
			g_lpMain->Draw();
			glFlush();
			SwapBuffers(wglGetCurrentDC());
			bFirst_Draw = false;
		}
		Sleep(0); // pass back to Windows
	}
	g_lpMain->gfx_close();
	g_bRC_Created = false;
	for (unsigned int uiI = 0; uiI < 11; uiI++)
	{
		for (unsigned int uiJ =0; uiJ < 4; uiJ++)
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
			RemoveFontResourceEx(g_cszFont_Paths[uiI].c_str(),FR_PRIVATE,0);
	}

	wglMakeCurrent(g_hDC,NULL);
 	ReleaseDC(g_hWnd,g_hDC);
	wglDeleteContext(g_hRC);
	SendMessage(g_hWnd,WM_DESTROY,0,0);
	_endthread();
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
		Initialize_Circle_Vectors();
		g_lpMain->init();

		g_bRC_Created = false;
		g_hInst = hInstance;

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

		while (iReturn = GetMessage(&msg,NULL,0,0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		g_lpMain->close();
	}
	return iReturn;
}


uintptr_t g_hMainThread = 0;

MAIN::KEYID Key_Map(UINT wParam)
{
	MAIN::KEYID		eKey = MAIN::KEY_NOOP;
	switch (wParam)
	{
	case VK_LBUTTON: eKey = MAIN::KEY_MB_LEFT; break;
	case VK_RBUTTON: eKey = MAIN::KEY_MB_RGT; break;
	case VK_CANCEL: eKey = MAIN::KEY_CANCEL; break;
	case VK_MBUTTON: eKey = MAIN::KEY_MB_CTR; break;

#if(_WIN32_WINNT >= 0x0500)
	case VK_XBUTTON1: eKey = MAIN::KEY_MB_X1; break;
	case VK_XBUTTON2: eKey = MAIN::KEY_MB_X2; break;
#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0x07 : unassigned
 */

	case VK_BACK: eKey = MAIN::KEY_BACKSPACE; break;
	case VK_TAB: eKey = MAIN::KEY_TAB; break;

/*
 * 0x0A - 0x0B : reserved
 */

	case VK_CLEAR: eKey = MAIN::KEY_CLEAR; break;
	case VK_RETURN: eKey = MAIN::KEY_ENTER; break;

	case VK_SHIFT: eKey = MAIN::KEY_SHIFT; break;
	case VK_CONTROL: eKey = MAIN::KEY_CONTROL; break;
	case VK_MENU: eKey = MAIN::KEY_ALT; break;
	case VK_PAUSE: eKey = MAIN::KEY_PAUSE; break;
	case VK_CAPITAL: eKey = MAIN::KEY_CAPSLOCK; break;

//	case VK_KANA: eKey = MAIN::KEY_KANA; break;
	case VK_HANGUL: eKey = MAIN::KEY_HANGUL; break;
	case VK_JUNJA: eKey = MAIN::KEY_JUNJA; break;
	case VK_FINAL: eKey = MAIN::KEY_FINAL; break;
//	case VK_HANJA: eKey = MAIN::KEY_HANJA; break;
	case VK_KANJI: eKey = MAIN::KEY_KANJI; break;

	case VK_ESCAPE: eKey = MAIN::KEY_ESCAPE; break;

	case VK_CONVERT: eKey = MAIN::KEY_CONVERT; break;
	case VK_NONCONVERT: eKey = MAIN::KEY_NONCONVERT; break;
	case VK_ACCEPT: eKey = MAIN::KEY_ACCEPT; break;
	case VK_MODECHANGE: eKey = MAIN::KEY_MODECHANGE; break;

	case VK_SPACE: eKey = MAIN::KEY_SPACE; break;
	case VK_PRIOR: eKey = MAIN::KEY_PGUP; break;
	case VK_NEXT: eKey = MAIN::KEY_PGDN; break;
	case VK_END: eKey = MAIN::KEY_END; break;
	case VK_HOME: eKey = MAIN::KEY_HOME; break;
	case VK_LEFT: eKey = MAIN::KEY_LEFT; break;
	case VK_UP: eKey = MAIN::KEY_UP; break;
	case VK_RIGHT: eKey = MAIN::KEY_RIGHT; break;
	case VK_DOWN: eKey = MAIN::KEY_DOWN; break;
	case VK_SELECT: eKey = MAIN::KEY_SELECT; break;
	case VK_PRINT: eKey = MAIN::KEY_PRINT; break;
	case VK_EXECUTE: eKey = MAIN::KEY_EXEC; break;
	case VK_SNAPSHOT: eKey = MAIN::KEY_PRSCRN; break;
	case VK_INSERT: eKey = MAIN::KEY_INSERT; break;
	case VK_DELETE: eKey = MAIN::KEY_DELETE; break;
	case VK_HELP: eKey = MAIN::KEY_HELP; break;

/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */
	case 0x30:	eKey = MAIN::KEY_0; break;
	case 0x31:	eKey = MAIN::KEY_1; break;
	case 0x32:	eKey = MAIN::KEY_2; break;
	case 0x33:	eKey = MAIN::KEY_3; break;
	case 0x34:	eKey = MAIN::KEY_4; break;
	case 0x35:	eKey = MAIN::KEY_5; break;
	case 0x36:	eKey = MAIN::KEY_6; break;
	case 0x37:	eKey = MAIN::KEY_7; break;
	case 0x38:	eKey = MAIN::KEY_8; break;
	case 0x39:	eKey = MAIN::KEY_9; break;

	case 0x41:	eKey = MAIN::KEY_A; break;
	case 0x42:	eKey = MAIN::KEY_B; break;
	case 0x43:	eKey = MAIN::KEY_C; break;
	case 0x44:	eKey = MAIN::KEY_D; break;
	case 0x45:	eKey = MAIN::KEY_E; break;
	case 0x46:	eKey = MAIN::KEY_F; break;
	case 0x47:	eKey = MAIN::KEY_G; break;
	case 0x48:	eKey = MAIN::KEY_H; break;
	case 0x49:	eKey = MAIN::KEY_I; break;
	case 0x4a:	eKey = MAIN::KEY_J; break;
	case 0x4b:	eKey = MAIN::KEY_K; break;
	case 0x4c:	eKey = MAIN::KEY_L; break;
	case 0x4d:	eKey = MAIN::KEY_M; break;
	case 0x4e:	eKey = MAIN::KEY_N; break;
	case 0x4f:	eKey = MAIN::KEY_O; break;
	case 0x50:	eKey = MAIN::KEY_P; break;
	case 0x51:	eKey = MAIN::KEY_Q; break;
	case 0x52:	eKey = MAIN::KEY_R; break;
	case 0x53:	eKey = MAIN::KEY_S; break;
	case 0x54:	eKey = MAIN::KEY_T; break;
	case 0x55:	eKey = MAIN::KEY_U; break;
	case 0x56:	eKey = MAIN::KEY_V; break;
	case 0x57:	eKey = MAIN::KEY_W; break;
	case 0x58:	eKey = MAIN::KEY_X; break;
	case 0x59:	eKey = MAIN::KEY_Y; break;
	case 0x5a:	eKey = MAIN::KEY_Z; break;

	case VK_LWIN: eKey = MAIN::KEY_LWIN; break;
	case VK_RWIN: eKey = MAIN::KEY_RWIN; break;
	case VK_APPS: eKey = MAIN::KEY_APPS; break;

/*
 * 0x5E : reserved
 */

	case VK_SLEEP: eKey = MAIN::KEY_SLEEP; break;

	case VK_NUMPAD0: eKey = MAIN::KEY_NUMPD_0; break;
	case VK_NUMPAD1: eKey = MAIN::KEY_NUMPD_1; break;
	case VK_NUMPAD2: eKey = MAIN::KEY_NUMPD_2; break;
	case VK_NUMPAD3: eKey = MAIN::KEY_NUMPD_3; break;
	case VK_NUMPAD4: eKey = MAIN::KEY_NUMPD_4; break;
	case VK_NUMPAD5: eKey = MAIN::KEY_NUMPD_5; break;
	case VK_NUMPAD6: eKey = MAIN::KEY_NUMPD_6; break;
	case VK_NUMPAD7: eKey = MAIN::KEY_NUMPD_7; break;
	case VK_NUMPAD8: eKey = MAIN::KEY_NUMPD_8; break;
	case VK_NUMPAD9: eKey = MAIN::KEY_NUMPD_9; break;
	case VK_MULTIPLY: eKey = MAIN::KEY_NUMPD_TIMES; break;
	case VK_ADD: eKey = MAIN::KEY_NUMPD_PLUS; break;
	case VK_SEPARATOR: eKey = MAIN::KEY_NUMPD_ENTER; break;
	case VK_SUBTRACT: eKey = MAIN::KEY_NUMPD_MINUS; break;
	case VK_DECIMAL: eKey = MAIN::KEY_NUMPD_DOT; break;
	case VK_DIVIDE: eKey = MAIN::KEY_NUMPD_DIV; break;
	case VK_F1: eKey = MAIN::KEY_F1; break;
	case VK_F2: eKey = MAIN::KEY_F2; break;
	case VK_F3: eKey = MAIN::KEY_F3; break;
	case VK_F4: eKey = MAIN::KEY_F4; break;
	case VK_F5: eKey = MAIN::KEY_F5; break;
	case VK_F6: eKey = MAIN::KEY_F6; break;
	case VK_F7: eKey = MAIN::KEY_F7; break;
	case VK_F8: eKey = MAIN::KEY_F8; break;
	case VK_F9: eKey = MAIN::KEY_F9; break;
	case VK_F10: eKey = MAIN::KEY_F10; break;
	case VK_F11: eKey = MAIN::KEY_F11; break;
	case VK_F12: eKey = MAIN::KEY_F12; break;
	case VK_F13: eKey = MAIN::KEY_F13; break;
	case VK_F14: eKey = MAIN::KEY_F14; break;
	case VK_F15: eKey = MAIN::KEY_F15; break;
	case VK_F16: eKey = MAIN::KEY_F16; break;
	case VK_F17: eKey = MAIN::KEY_F17; break;
	case VK_F18: eKey = MAIN::KEY_F18; break;
	case VK_F19: eKey = MAIN::KEY_F19; break;
	case VK_F20: eKey = MAIN::KEY_F20; break;
	case VK_F21: eKey = MAIN::KEY_F21; break;
	case VK_F22: eKey = MAIN::KEY_F22; break;
	case VK_F23: eKey = MAIN::KEY_F23; break;
	case VK_F24: eKey = MAIN::KEY_F24; break;

/*
 * 0x88 - 0x8F : unassigned
 */

	case VK_NUMLOCK: eKey = MAIN::KEY_NMLCK; break;
	case VK_SCROLL: eKey = MAIN::KEY_SCROLLLOCK; break;
		// ----------------------------
		// OEM specific keys.
		// Not mapped
		// ----------------------------
//
// NEC PC-9800 kbd definitions
//
	case VK_OEM_NEC_EQUAL: eKey = MAIN::KEY_OEM; break;

//
// Fujitsu/OASYS kbd definitions
//
//	case VK_OEM_FJ_JISHO: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_FJ_MASSHOU: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_FJ_TOUROKU: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_FJ_LOYA: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_FJ_ROYA: eKey = MAIN::KEY_OEM; break;

/*
 * 0x97 - 0x9F : unassigned
 */

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
	case VK_LSHIFT: eKey = MAIN::KEY_LSHIFT; break;
	case VK_RSHIFT: eKey = MAIN::KEY_RSHIFT; break;
	case VK_LCONTROL: eKey = MAIN::KEY_LCTRL; break;
	case VK_RCONTROL: eKey = MAIN::KEY_RCTRL; break;
	case VK_LMENU: eKey = MAIN::KEY_LALT; break;
	case VK_RMENU: eKey = MAIN::KEY_RALT; break;

#if(_WIN32_WINNT >= 0x0500)
	case VK_BROWSER_BACK: eKey = MAIN::KEY_BRSR_BACK; break;
	case VK_BROWSER_FORWARD: eKey = MAIN::KEY_BRSR_FWD; break;
	case VK_BROWSER_REFRESH: eKey = MAIN::KEY_BRSR_REFRESH; break;
	case VK_BROWSER_STOP: eKey = MAIN::KEY_BRSR_STOP; break;
	case VK_BROWSER_SEARCH: eKey = MAIN::KEY_BRSR_SEARCH; break;
	case VK_BROWSER_FAVORITES: eKey = MAIN::KEY_BRSR_FAV; break;
	case VK_BROWSER_HOME: eKey = MAIN::KEY_BRSR_HOME; break;

	case VK_VOLUME_MUTE: eKey = MAIN::KEY_VOLMUTE; break;
	case VK_VOLUME_DOWN: eKey = MAIN::KEY_VOLDN; break;
	case VK_VOLUME_UP: eKey = MAIN::KEY_VOLUP; break;
	case VK_MEDIA_NEXT_TRACK: eKey = MAIN::KEY_MEDIA_NEXT; break;
	case VK_MEDIA_PREV_TRACK: eKey = MAIN::KEY_MEDIA_PREV; break;
	case VK_MEDIA_STOP: eKey = MAIN::KEY_MEDIA_STOP; break;
	case VK_MEDIA_PLAY_PAUSE: eKey = MAIN::KEY_MEDIA_PLAY; break;
	case VK_LAUNCH_MAIL: eKey = MAIN::KEY_EMAIL; break;
	case VK_LAUNCH_MEDIA_SELECT: eKey = MAIN::KEY_MEDIA_SELECT; break;
	case VK_LAUNCH_APP1: eKey = MAIN::KEY_APP_1; break;
	case VK_LAUNCH_APP2: eKey = MAIN::KEY_APP_2; break;

#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0xB8 - 0xB9 : reserved
 */

	case VK_OEM_1: eKey = MAIN::KEY_SEMICOLON; break; //warning: should do this as region / keyboard specific
	case VK_OEM_PLUS: eKey = MAIN::KEY_EQUALS; break;
	case VK_OEM_COMMA: eKey = MAIN::KEY_COMMA; break;
	case VK_OEM_MINUS: eKey = MAIN::KEY_MINUS; break;
	case VK_OEM_PERIOD: eKey = MAIN::KEY_PERIOD; break;
	case VK_OEM_2: eKey = MAIN::KEY_SLASH; break; // warning: depends on region 
	case VK_OEM_3: eKey = MAIN::KEY_TILDE; break; // warning: depends on region 

/*
 * 0xC1 - 0xD7 : reserved
 */

/*
 * 0xD8 - 0xDA : unassigned
 */

	case VK_OEM_4: eKey = MAIN::KEY_LSQBRKT; break; // warning: depends on region 
	case VK_OEM_5: eKey = MAIN::KEY_BACKSLASH; break; // warning: depends on region 
	case VK_OEM_6: eKey = MAIN::KEY_RSQBRKT; break; // warning: depends on region 
	case VK_OEM_7: eKey = MAIN::KEY_SQUOTE; break; // warning: depends on region 
	case VK_OEM_8: eKey = MAIN::KEY_OEM; break;

/*
 * 0xE0 : reserved
 */

/*
 * Various extended or enhanced keyboards
 */
	case VK_OEM_AX: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_102: eKey = MAIN::KEY_OEM; break;
	case VK_ICO_HELP: eKey = MAIN::KEY_OEM; break;
	case VK_ICO_00: eKey = MAIN::KEY_OEM; break;

#if(WINVER >= 0x0400)
	case VK_PROCESSKEY: eKey = MAIN::KEY_OEM; break;
#endif /* WINVER >= 0x0400 */

case VK_ICO_CLEAR: eKey = MAIN::KEY_OEM; break;


#if(_WIN32_WINNT >= 0x0500)
	case VK_PACKET: eKey = MAIN::KEY_OEM; break;
#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0xE8 : unassigned
 */

/*
 * Nokia/Ericsson definitions
 */
	case VK_OEM_RESET: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_JUMP: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_PA1: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_PA2: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_PA3: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_WSCTRL: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_CUSEL: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_ATTN: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_FINISH: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_COPY: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_AUTO: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_ENLW: eKey = MAIN::KEY_OEM; break;
	case VK_OEM_BACKTAB: eKey = MAIN::KEY_OEM; break;

	case VK_ATTN: eKey = MAIN::KEY_ATTN; break;
	case VK_CRSEL: eKey = MAIN::KEY_CRSEL; break;
	case VK_EXSEL: eKey = MAIN::KEY_EXSEL; break;
	case VK_EREOF: eKey = MAIN::KEY_EREOF; break;
	case VK_PLAY: eKey = MAIN::KEY_PLAY; break;
	case VK_ZOOM: eKey = MAIN::KEY_ZOOM; break;
	case VK_NONAME: eKey = MAIN::KEY_NONAME; break;
	case VK_PA1: eKey = MAIN::KEY_PA1; break;
	case VK_OEM_CLEAR: eKey = MAIN::KEY_OEM; break;
	}
	return eKey;
}


LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch (msg)
   	{
    case WM_CREATE:
		g_hMainThread = _beginthread(MainThread,0,NULL);

		// no break - set window size
	case WM_SIZE:
		{
			RECT rectClient;
			GetClientRect(hwnd, &rectClient); 
			g_hWnd = hwnd;
			g_lpMain->On_Window_Move(Fix_Coordinate_Global(PAIR<unsigned int>(0,0)));
			g_lpMain->On_Window_Resize(PAIR<unsigned int>(rectClient.right,rectClient.bottom));
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
		g_lpMain->On_Mouse_Button_Double_Click(MAIN::MB_CTR, Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_MBUTTONDOWN:
		g_lpMain->On_Mouse_Button_Down(MAIN::MB_CTR, Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_MBUTTONUP:
		g_lpMain->On_Mouse_Button_Up(MAIN::MB_CTR, Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_RBUTTONDBLCLK:
		g_lpMain->On_Mouse_Button_Double_Click(MAIN::MB_RGT, Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_RBUTTONDOWN:
		g_lpMain->On_Mouse_Button_Down(MAIN::MB_RGT, Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_RBUTTONUP:
		g_lpMain->On_Mouse_Button_Up(MAIN::MB_RGT,  Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_LBUTTONDBLCLK:
		g_lpMain->On_Mouse_Button_Double_Click(MAIN::MB_LEFT, Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_LBUTTONDOWN:
		g_lpMain->On_Mouse_Button_Down(MAIN::MB_LEFT, Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_LBUTTONUP:
		g_lpMain->On_Mouse_Button_Up(MAIN::MB_LEFT, Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_MOUSEMOVE:
		g_lpMain->On_Mousemove(Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
	case WM_MOUSEWHEEL:
		g_lpMain->On_Mouse_Wheel(MAIN::MB_SCROLL_V, GET_WHEEL_DELTA_WPARAM(wParam), Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
		break;
#if !defined(__MINGW64__) && !defined(__MINGW32__)
	case WM_MOUSEHWHEEL:
		g_lpMain->On_Mouse_Wheel(MAIN::MB_SCROLL_V, GET_WHEEL_DELTA_WPARAM(wParam), Fix_Coordinate_Window(PAIR<unsigned int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))));
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

void TextBBox(const double & i_dSize, const char * i_lpszString, PAIR<double> & o_cBot_Left, PAIR<double> & o_cTop_Right)
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

std::vector<std::string> MAIN::Get_Directory_File_List(const std::string &i_szDirectory) /// get list of all files in the given direcctory
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

void DATE_TIME::Get_Current(void)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	m_uiYear = tm.tm_year + 1900;
	m_uiMonth = tm.tm_mon + 1;
	m_uiDay_of_Month = tm.tm_mday;
	m_uiDay_of_Week;
	m_uiHour = tm.tm_hour;
	m_uiMinute = tm.tm_min;
	m_dSeconds = tm.tm_sec;
}

void criticalsection::Set(void)
{
	CRITICAL_SECTION * lpbPtr = (CRITICAL_SECTION  *) m_lpvCS_Data; 
	EnterCriticalSection(lpbPtr);
}
criticalsection::criticalsection(void)
{
	m_lpvCS_Data = (void *)(new CRITICAL_SECTION[1]);
	CRITICAL_SECTION * lpbPtr = (CRITICAL_SECTION  *) m_lpvCS_Data; 
	InitializeCriticalSection(lpbPtr);
}
criticalsection::~criticalsection(void)
{
	CRITICAL_SECTION * lpbPtr = (CRITICAL_SECTION  *) m_lpvCS_Data; 
	DeleteCriticalSection(lpbPtr);
	delete [] lpbPtr;
}

void criticalsection::Unset(void)
{
	CRITICAL_SECTION * lpbPtr = (CRITICAL_SECTION  *) m_lpvCS_Data; 
	LeaveCriticalSection(lpbPtr);
}
bool criticalsection::Get(void) const 
{
	CRITICAL_SECTION * lpbPtr = (CRITICAL_SECTION  *) m_lpvCS_Data; 
	return (TryEnterCriticalSection(lpbPtr) != 0);
}
