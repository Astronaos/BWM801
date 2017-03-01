#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <GL/gl.h>
#include <GL/glx.h>
#include <core.hpp>
#include <FTGL/ftgl.h>
#include <iostream>
#include <dirent.h> // for working with the file tree / directories
#include <thread>
#include <GL/glext.h>
#include <string>

#define NIL (0)       // A name for the void pointer

FT_Library 		g_ftLibrary;

Display	* g_lpdpyDisplay = NULL;
Window		g_wWindow;
Window 		g_wRoot;
GLXContext g_glc;

main::KEYID Key_Map(unsigned int uiKey);
main::MOUSEBUTTON Mouse_Button_Map(unsigned int i_uiButton);
void Sleep(unsigned int i_uiLength)
{
	usleep(i_uiLength * 1000);
}

// X11 uses display coordinates that have (0,0) as the top left corner.  
PAIR<unsigned int>	Fix_Coordinate_Global(const PAIR<unsigned int> &i_tPosition)
{
	PAIR<unsigned int> tRet;
	Window wRoot;
	int iX,iY;
	unsigned int uiWidth,uiHeight,uiBorder_Width,uiDepth;
	XGetGeometry(g_lpdpyDisplay, g_wRoot, &wRoot, &iX, &iY, &uiWidth, &uiHeight, &uiBorder_Width, &uiDepth);
//	printf("Global geo: %i %i\n",uiWidth,uiHeight);
	tRet.m_tX = i_tPosition.m_tX;
	tRet.m_tY = (uiHeight - i_tPosition.m_tY);
	return tRet;
}
PAIR<unsigned int>	Fix_Coordinate_Window(const PAIR<unsigned int> &i_tPosition)
{
	PAIR<unsigned int> tRet;
	Window wRoot;
	int iX,iY;
	unsigned int uiWidth,uiHeight,uiBorder_Width,uiDepth;
	XGetGeometry(g_lpdpyDisplay, g_wWindow, &wRoot, &iX, &iY, &uiWidth, &uiHeight, &uiBorder_Width, &uiDepth);
//	printf("window geo: %i %i\n",uiWidth,uiHeight);
	tRet.m_tX = i_tPosition.m_tX;
	tRet.m_tY = (uiHeight - i_tPosition.m_tY);
	return tRet;
}

void Gfx_Loop(void)
{
	bool bFirst_Draw = true;
	timespec tTime_Last;
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Last);
	std::deque<double> vFrame_Rate;
	while (!g_lpMain->Pending_Quit())
	{
		if (g_lpMain->Pending_Draw() && g_lpMain->Get_Window_Size().m_tX > 0 && g_lpMain->Get_Window_Size().m_tY > 0)
		{
			glXMakeCurrent(g_lpdpyDisplay, g_wWindow, g_glc);
			if (bFirst_Draw)
			{
				if(FT_Init_FreeType(&g_ftLibrary))
				{
				  fprintf(stderr, "Could not init freetype library\n");
				}
				g_lpMain->gfx_init();
				g_lpMain->gfx_reshape(g_lpMain->Get_Window_Size());
			}
			timespec tTime_Curr;
			clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Curr);
			double	dTimestep = (tTime_Curr.tv_sec - tTime_Last.tv_sec) + (tTime_Curr.tv_nsec - tTime_Last.tv_nsec) * 1.0e-9;
			tTime_Last = tTime_Curr;
			vFrame_Rate.push_back(dTimestep);
			double dDraw_Time = 0.0;
			for (unsigned int uiI = 0; uiI < vFrame_Rate.size(); uiI++)
				dDraw_Time += vFrame_Rate[uiI];
			g_lpMain->Set_Frame_Rate(vFrame_Rate.size() / dDraw_Time);
			if (vFrame_Rate.size() > 10)
				vFrame_Rate.pop_front();
			g_lpMain->Draw();
			glFlush();
			glXSwapBuffers(g_lpdpyDisplay, g_wWindow);
			bFirst_Draw = false;
		}
		else
			usleep(0);
	}
	g_lpMain->gfx_close();
}

void Main_Timer_Loop(void)
{
	timespec tTime_Last;
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Last);
	while (!g_lpMain->Pending_Quit())
	{
		timespec tTime_Curr;
		clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Curr);
		double	dTimestep = (tTime_Curr.tv_sec - tTime_Last.tv_sec) + (tTime_Curr.tv_nsec - tTime_Last.tv_nsec) * 1.0e-9;
		tTime_Last = tTime_Curr;
		g_lpMain->On_Timer(0,dTimestep);
		usleep(10);
	}
}


int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	bool	bCtrl_Key = false;
	bool	bShift_Key = false;
	bool	bF12_Key = false;
	std::vector<std::thread> vThread_List;

	if (!g_lpMain)
	{
		std::cerr << "No main class instantiated.  Exiting." << std::endl;
		exit(1);
	}
	g_lpMain->Process_Command_Line(i_iArg_Count, i_lpszArg_Values);
	Initialize_Circle_Vectors();
	XInitThreads();
	//printf("%i %i %i %i %i %i %i\n",main::MB_LEFT, main::MB_CTR, main::MB_RGT, main::MB_SCROLL_V, main::MB_SCROLL_H, main::MB_X1, main::MB_X2);
	// Open the display

	g_lpMain->init();

	g_lpdpyDisplay = XOpenDisplay(NIL);
	assert(g_lpdpyDisplay);


	bool	bFirst_Draw = true;
	GLint                   iAttributeList[] = { GLX_RGBA, GLX_RED_SIZE,8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8, GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8, GLX_DOUBLEBUFFER, None };
//	GLint                   iAttributeList[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8, GLX_DOUBLEBUFFER, None };
//	GLint                   iAttributeList[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };
//	printf("root\n");
	g_wRoot = DefaultRootWindow(g_lpdpyDisplay);
	XVisualInfo * lpXVisual = glXChooseVisual(g_lpdpyDisplay, DefaultScreen(g_lpdpyDisplay),iAttributeList);
	GLint uiAttribs_Test[] = {GLX_USE_GL, GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, GLX_STENCIL_SIZE, GLX_RED_SIZE, GLX_GREEN_SIZE, GLX_BLUE_SIZE, GLX_ALPHA_SIZE, None};
	
//	for (unsigned int uiI = 0; uiAttribs_Test[uiI] != None; uiI++)
//	{
//		int uiVal;
//		glXGetConfig(g_lpdpyDisplay,lpXVisual,uiAttribs_Test[uiI],&uiVal);
//		printf("%i: %i\n",uiI,uiVal);
//	}

	Colormap cmap = XCreateColormap(g_lpdpyDisplay, g_wRoot, lpXVisual->visual, AllocNone);
	XSetWindowAttributes    swa;
	swa.colormap = cmap;
	swa.event_mask = StructureNotifyMask|ExposureMask|PointerMotionMask|ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|EnterWindowMask|LeaveWindowMask;
//	printf("window\n");

	Window wRoot;
	int iX,iY;
	unsigned int uiWidth,uiHeight,uiBorder_Width,uiDepth;
	XGetGeometry(g_lpdpyDisplay, g_wRoot, &wRoot, &iX, &iY, &uiWidth, &uiHeight, &uiBorder_Width, &uiDepth);

	g_wWindow = XCreateWindow(g_lpdpyDisplay, g_wRoot, 0, 0, uiWidth - 2*uiBorder_Width, uiHeight - 2*uiBorder_Width, 0, lpXVisual->depth, InputOutput, lpXVisual->visual, CWColormap | CWEventMask, &swa);

	// capture WM_DELETE_WINDOW messages from the server - we want to shut down cleanly
	Atom aWM_Delete_Window = XInternAtom(g_lpdpyDisplay, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(g_lpdpyDisplay, g_wWindow, &aWM_Delete_Window, 1);

	XMapWindow(g_lpdpyDisplay,g_wWindow);
	if (g_lpMain && g_lpMain->Get_Window_Name().size() > 0)
		XStoreName(g_lpdpyDisplay, g_wWindow, g_lpMain->Get_Window_Name().c_str());
//	printf("glx\n");
	g_glc = glXCreateContext(g_lpdpyDisplay, lpXVisual, 0, GL_TRUE);
	 glXMakeCurrent(g_lpdpyDisplay, g_wWindow, g_glc);

		printf("Vendor: %s\n",glGetString(GL_VENDOR));
		printf("Renderer: %s\n",glGetString(GL_RENDERER));
		printf("Version: %s\n",glGetString(GL_VERSION));
		printf("SL Version: %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

	std::map<std::string,int> vstrGL_Extensions_list;

	std::string szExtensions = glXQueryExtensionsString(g_lpdpyDisplay,0);
	GLint iNum_Ext;
	PFNGLGETSTRINGIPROC glGetStringi = (PFNGLGETSTRINGIPROC)glXGetProcAddress((GLubyte *)"glGetStringi");
	if (glGetStringi)
	{
		glGetIntegerv(GL_NUM_EXTENSIONS,&iNum_Ext);
		unsigned int uiMax = iNum_Ext - 1;
		for (unsigned int uiI = 0; uiI < uiMax; uiI++)
		{
			if (glGetStringi(GL_EXTENSIONS,uiI))
			{
				vstrGL_Extensions_list[std::string((char *)glGetStringi(GL_EXTENSIONS,uiI))] = 1;
			}
		}
	}
	if (!szExtensions.empty())
	{
		size_t sEnd = szExtensions.find_first_of(' ');
		size_t sLast = 0;
		while (sEnd != std::string::npos)
		{
			vstrGL_Extensions_list[szExtensions.substr(sLast,(sEnd - sLast))] = 1;
			sLast = sEnd + 1;
			sEnd = szExtensions.find_first_of(' ',sLast);
		}
		vstrGL_Extensions_list[szExtensions.substr(sLast,(szExtensions.size() - sLast))] = 1;
	}
//	for (std::vector<std::string>::iterator cI = vstrGL_Extensions_list.begin(); cI != vstrGL_Extensions_list.end(); cI++)
//	{
//		printf("%s\n",cI->c_str());
//	}
	if (vstrGL_Extensions_list.count("GLX_EXT_swap_control") > 0)
	{
		PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;
		glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress((GLubyte *)"glXSwapIntervalEXT");
		GLXDrawable drawable = glXGetCurrentDrawable();
		if (glXSwapIntervalEXT)
			glXSwapIntervalEXT(g_lpdpyDisplay,drawable,0);
	}
	else if (vstrGL_Extensions_list.count("GLX_SGI_swap_control") > 0)
	{
		PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI;
		glXSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC) glXGetProcAddress((GLubyte *)"glXSwapIntervalSGI");
		glXSwapIntervalSGI(1);
	}
	else if (vstrGL_Extensions_list.count("MESA_swap_control") > 0)
	{
		PFNGLXSWAPINTERVALMESAPROC glXSwapIntervalMESA;
		glXSwapIntervalMESA = (PFNGLXSWAPINTERVALMESAPROC) glXGetProcAddress((GLubyte *)"glXSwapIntervalMESA");
		glXSwapIntervalMESA(1);
	}


	// We want to get MapNotify events
//	XSelectInput(g_lpdpyDisplay, g_wWindow, StructureNotifyMask|ExposureMask|PointerMotionMask|ButtonPressMask|ButtonReleaseMask||KeyPressMask|KeyReleaseMask|EnterWindowMask|LeaveWindowMask);


	vThread_List.push_back(std::thread(Main_Timer_Loop));
	vThread_List.push_back(std::thread(Gfx_Loop));

	std::vector<XButtonEvent> veButton_Events;
	bool	bReady_To_Process_Buttons = true;
	do
	{
		while (XEventsQueued(g_lpdpyDisplay,QueuedAfterFlush) > 0)
		{
			XEvent	evEvent;
			XNextEvent(g_lpdpyDisplay,&evEvent);
			switch (evEvent.type)
			{
			case KeyRelease:
				if (g_lpMain->m_bEngine_Debug_Mode)
					printf("Event loop Key %i \n",evEvent.xkey.keycode);
				switch (Key_Map(XLookupKeysym(&evEvent.xkey,0)))
				{
				case main::KEY_RCTRL:
					bCtrl_Key = false;
					break;
				case main::KEY_RSHIFT:
					bShift_Key = false;
					break;
				case main::KEY_F12:
					bF12_Key = false;
					break;
				}
				g_lpMain->On_Key_Up(Key_Map(XLookupKeysym(&evEvent.xkey,0)),0,0, 0, 1);
				break;
			case KeyPress:
				switch (Key_Map(XLookupKeysym(&evEvent.xkey,0)))
				{
				case main::KEY_RCTRL:
					bCtrl_Key = true;
					break;
				case main::KEY_RSHIFT:
					bShift_Key = true;
					break;
				case main::KEY_F12:
					bF12_Key = true;
					break;
				}
				if (g_lpMain->m_bEngine_Debug_Mode)
					printf("Event loop Key %i \n",evEvent.xkey.keycode);

				g_lpMain->On_Key_Down(Key_Map(XLookupKeysym(&evEvent.xkey,0)),0,0, 0, 0);
				break;
		// Mouse operations
			case ButtonPress:
			case ButtonRelease:
				if (g_lpMain->m_bEngine_Debug_Mode)
					printf("Event loop Button %i (%i,%i)\n",Mouse_Button_Map(evEvent.xbutton.button), Fix_Coordinate_Window(PAIR<unsigned int>(evEvent.xbutton.x, evEvent.xbutton.y)).m_tX,Fix_Coordinate_Window(PAIR<unsigned int>(evEvent.xbutton.x, evEvent.xbutton.y)).m_tY);
				bReady_To_Process_Buttons = (XEventsQueued(g_lpdpyDisplay,QueuedAfterFlush) == 0);
				if (!bReady_To_Process_Buttons)
				{
				
					XEvent evNext;
					// peek at the next event; if it is a button
					XPeekEvent(g_lpdpyDisplay,&evNext); 
					bReady_To_Process_Buttons = ((evNext.type != ButtonPress && evNext.type != ButtonRelease) || ((evNext.type == ButtonPress || evNext.type == ButtonRelease) && evEvent.xbutton.button != evNext.xbutton.button) || ((evNext.type == ButtonPress || evNext.type == ButtonRelease) && (evEvent.xbutton.button == Button4 || evEvent.xbutton.button == Button5 || evEvent.xbutton.button == 6 || evEvent.xbutton.button == 7))); // process immediately if different buttons are pressed or if button 4,5,6, or 7 is pressed
					if (!bReady_To_Process_Buttons)
					{
						veButton_Events.push_back(evEvent.xbutton);
					}
				}
				if (bReady_To_Process_Buttons)
				{
					if (veButton_Events.size() > 0)
					{
						if (veButton_Events.size() == 4 &&
							veButton_Events[0].type == ButtonPress && 
							veButton_Events[1].type == ButtonRelease &&
							veButton_Events[2].type == ButtonPress &&
							veButton_Events[3].type == ButtonRelease &&
							veButton_Events[0].button == veButton_Events[1].button &&
							veButton_Events[0].button == veButton_Events[2].button &&
							veButton_Events[0].button == veButton_Events[3].button &&
							(veButton_Events[3].time - veButton_Events[0].time) < 500) //@@TODO make this user selectable
						{ // double click detected
							g_lpMain->On_Mouse_Button_Double_Click(Mouse_Button_Map(veButton_Events[0].button), Fix_Coordinate_Window(PAIR<unsigned int>(veButton_Events[0].x, veButton_Events[0].y)));
						}
						else
						{
							for (std::vector<XButtonEvent>::const_iterator cI = veButton_Events.begin(); cI != veButton_Events.end(); cI++)
							{
								if (cI->type == ButtonPress)
								{
									g_lpMain->On_Mouse_Button_Down(Mouse_Button_Map(cI->button), Fix_Coordinate_Window(PAIR<unsigned int>(cI->x, cI->y)));
								}
								else
								{
									g_lpMain->On_Mouse_Button_Up(Mouse_Button_Map(cI->button), Fix_Coordinate_Window(PAIR<unsigned int>(cI->x, cI->y)));
								}
							}
						}
						veButton_Events.clear();
					}
					else if (evEvent.type == ButtonPress)
					{
						g_lpMain->On_Mouse_Button_Down(Mouse_Button_Map(evEvent.xbutton.button), Fix_Coordinate_Window(PAIR<unsigned int>(evEvent.xbutton.x, evEvent.xbutton.y)));
					}
					else
					{
						g_lpMain->On_Mouse_Button_Up(Mouse_Button_Map(evEvent.xbutton.button), Fix_Coordinate_Window(PAIR<unsigned int>(evEvent.xbutton.x, evEvent.xbutton.y)));
					}
				}
				break;
			case MotionNotify:
				if (g_lpMain->m_bEngine_Debug_Mode)
					printf("Event loop Motion\n");
				g_lpMain->On_Mousemove(Fix_Coordinate_Window(PAIR<unsigned int>(evEvent.xmotion.x, evEvent.xmotion.y)));
				break;
	// Mouse wheel events not supported under X windows
	//		case WM_MOUSEWHEEL:
	//			g_lpMain->on_mouse_wheel(main::MB_SCROLL_V, (short)(wParam >> 16), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//			break;
	//		case WM_MOUSEHWHEEL:
	//			g_lpMain->on_mouse_wheel(main::MB_SCROLL_V, (short)(wParam >> 16), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//			break;
			case Expose:
			case GraphicsExpose:
				if (g_lpMain->m_bEngine_Debug_Mode)
					printf("Event loop Expose\n");
				g_lpMain->On_Window_Move(Fix_Coordinate_Global(PAIR<unsigned int>(evEvent.xexpose.x,evEvent.xexpose.y)));
				g_lpMain->On_Window_Resize(PAIR<unsigned int>(evEvent.xexpose.width,evEvent.xexpose.height));
				if (!bFirst_Draw)
					g_lpMain->gfx_reshape(g_lpMain->Get_Window_Size());
				g_lpMain->Request_Refresh();
				break;
		    case ConfigureNotify:
				if (g_lpMain->m_bEngine_Debug_Mode)
					printf("Event loop Configure %i %i\n",evEvent.xconfigure.event, evEvent.xconfigure.window);
				g_lpMain->On_Window_Move(Fix_Coordinate_Global(PAIR<unsigned int>(evEvent.xconfigure.x,evEvent.xconfigure.y)));
				g_lpMain->On_Window_Resize(PAIR<unsigned int>(evEvent.xconfigure.width,evEvent.xconfigure.height));
				if (!bFirst_Draw)
					g_lpMain->gfx_reshape(g_lpMain->Get_Window_Size());
				g_lpMain->Request_Refresh();
		        break;
	 		case DestroyNotify:
				if (g_lpMain->m_bEngine_Debug_Mode)
					printf("Event loop Destroy\n");
				g_lpMain->Request_Quit();
				break;
			case MapNotify:
				if (g_lpMain->m_bEngine_Debug_Mode)
					printf("Event loop Map Notify\n");
				g_lpMain->Request_Refresh();
				break;
			case ReparentNotify:
				// don't care
				break;
			case PropertyNotify:
				// may care - ignore for now...
				//printf("Property %i %i\n",evEvent.xproperty.atom,evEvent.xproperty.state);
				break;
			case KeymapNotify:
				// may care - ignore for now...
				//printf("Keymap %i %i\n",evEvent.xkeymap.key_vector[0],evEvent.xkeymap.key_vector[1]);
				break;
			case ClientMessage:
				if (g_lpMain->m_bEngine_Debug_Mode)
					printf("Event loop Client Message\n");
				if ((Atom)evEvent.xclient.data.l[0] == aWM_Delete_Window)
					g_lpMain->Request_Quit();
				break;
			case EnterNotify:
				g_lpMain->Gain_Focus();
				break;
			case LeaveNotify:
				g_lpMain->Lose_Focus();
				break;
			default:
				if (g_lpMain->m_bEngine_Debug_Mode)
					printf("Event %i\n",evEvent.type);
				break;
			}
		}
	}
	while (!g_lpMain->Pending_Quit());
	for (std::vector<std::thread>::iterator cI = vThread_List.begin(); cI != vThread_List.end(); cI++)
	{
		cI->join();
	}
	
	glXDestroyContext(g_lpdpyDisplay, g_glc);

	//XFreeGC(g_lpdpyDisplay, gc);
	XDestroyWindow(g_lpdpyDisplay,g_wWindow);
	XCloseDisplay(g_lpdpyDisplay);	
	return 0;
}


main::KEYID Key_Map(unsigned int i_uiKey)
{
	main::KEYID		eKey = main::KEY_NOOP;
	switch (i_uiKey)
	{
	case XK_BackSpace: eKey = main::KEY_BACKSPACE; break;
	case XK_Tab: eKey = main::KEY_TAB; break;
	case XK_Linefeed: eKey = main::KEY_ENTER; break;
	case XK_Clear: eKey = main::KEY_CLEAR; break;
	case XK_Return: eKey = main::KEY_ENTER; break;
	case XK_Pause: eKey = main::KEY_PAUSE; break;
	case XK_Scroll_Lock: eKey = main::KEY_SCROLLLOCK; break;
//	case XK_Sys_Req: eKey = main::KEY_SYSRQ; break;
	case XK_Escape: eKey = main::KEY_ESCAPE; break;
	case XK_Delete: eKey = main::KEY_DELETE; break;
	case XK_Home: eKey = main::KEY_HOME; break;
	case XK_Left: eKey = main::KEY_LEFT; break;
	case XK_Up: eKey = main::KEY_UP; break;
	case XK_Right: eKey = main::KEY_RIGHT; break;
	case XK_Down: eKey = main::KEY_DOWN; break;
	case XK_Prior: eKey = main::KEY_PGUP; break;
	case XK_Next: eKey = main::KEY_PGDN; break;
	case XK_End: eKey = main::KEY_END; break;
	case XK_Select: eKey = main::KEY_SELECT; break;
	case XK_Print: eKey = main::KEY_PRINT; break;
	case XK_Execute: eKey = main::KEY_EXEC; break;
	case XK_Insert: eKey = main::KEY_INSERT; break;
	case XK_Cancel: eKey = main::KEY_CANCEL; break;
	case XK_Help: eKey = main::KEY_HELP; break;
//	case XK_Break: eKey = main::KEY_BREAK; break;
	case XK_Num_Lock: eKey = main::KEY_NMLCK; break;
	case XK_KP_Enter: eKey = main::KEY_NUMPD_ENTER; break;
	case XK_KP_Home: eKey = main::KEY_NUMPD_7; break;
	case XK_KP_Left: eKey = main::KEY_NUMPD_4; break;
	case XK_KP_Up: eKey = main::KEY_NUMPD_8; break;
	case XK_KP_Right: eKey = main::KEY_NUMPD_6; break;
	case XK_KP_Down: eKey = main::KEY_NUMPD_2; break;
//	case XK_KP_Prior: eKey = main::KEY_NUMPD_9; break;
	case XK_KP_Page_Up: eKey = main::KEY_NUMPD_9; break;
//	case XK_KP_Next: eKey = main::KEY_NUMPD_; break;
	case XK_KP_Page_Down: eKey = main::KEY_NUMPD_3; break;
	case XK_KP_End: eKey = main::KEY_NUMPD_1; break;
	case XK_KP_Begin: eKey = main::KEY_NUMPD_5; break;
	case XK_KP_Insert: eKey = main::KEY_NUMPD_0; break;
	case XK_KP_Delete: eKey = main::KEY_NUMPD_DOT; break;
//	case XK_KP_Equal: eKey = main::KEY_ESCAPE; break;
	case XK_KP_Multiply: eKey = main::KEY_NUMPD_TIMES; break;
	case XK_KP_Add: eKey = main::KEY_NUMPD_PLUS; break;
	case XK_KP_Separator: eKey = main::KEY_NUMPD_ENTER; break;
	case XK_KP_Subtract: eKey = main::KEY_NUMPD_MINUS; break;
	case XK_KP_Decimal: eKey = main::KEY_NUMPD_DOT; break;
	case XK_KP_Divide: eKey = main::KEY_NUMPD_DIV; break;
	case XK_KP_0: eKey = main::KEY_NUMPD_0; break;
	case XK_KP_1: eKey = main::KEY_NUMPD_1; break;
	case XK_KP_2: eKey = main::KEY_NUMPD_2; break;
	case XK_KP_3: eKey = main::KEY_NUMPD_3; break;
	case XK_KP_4: eKey = main::KEY_NUMPD_4; break;
	case XK_KP_5: eKey = main::KEY_NUMPD_5; break;
	case XK_KP_6: eKey = main::KEY_NUMPD_6; break;
	case XK_KP_7: eKey = main::KEY_NUMPD_7; break;
	case XK_KP_8: eKey = main::KEY_NUMPD_8; break;
	case XK_KP_9: eKey = main::KEY_NUMPD_9; break;
	case XK_F1: eKey = main::KEY_F1; break;
	case XK_F2: eKey = main::KEY_F2; break;
	case XK_F3: eKey = main::KEY_F3; break;
	case XK_F4: eKey = main::KEY_F4; break;
	case XK_F5: eKey = main::KEY_F5; break;
	case XK_F6: eKey = main::KEY_F6; break;
	case XK_F7: eKey = main::KEY_F7; break;
	case XK_F8: eKey = main::KEY_F8; break;
	case XK_F9: eKey = main::KEY_F9; break;
	case XK_F10: eKey = main::KEY_F10; break;
	case XK_F11: eKey = main::KEY_F11; break;
	case XK_F12: eKey = main::KEY_F12; break;
	case XK_F13: eKey = main::KEY_F13; break;
	case XK_F14: eKey = main::KEY_F14; break;
	case XK_F15: eKey = main::KEY_F15; break;
	case XK_F16: eKey = main::KEY_F16; break;
	case XK_F17: eKey = main::KEY_F17; break;
	case XK_F18: eKey = main::KEY_F18; break;
	case XK_F19: eKey = main::KEY_F19; break;
	case XK_F20: eKey = main::KEY_F20; break;
	case XK_F21: eKey = main::KEY_F21; break;
	case XK_F22: eKey = main::KEY_F22; break;
	case XK_F23: eKey = main::KEY_F23; break;
	case XK_F24: eKey = main::KEY_F24; break;
//	case XK_F25: eKey = main::KEY_ESCAPE; break;
//	case XK_F26: eKey = main::KEY_ESCAPE; break;
//	case XK_F27: eKey = main::KEY_ESCAPE; break;
//	case XK_F28: eKey = main::KEY_ESCAPE; break;
//	case XK_F29: eKey = main::KEY_ESCAPE; break;
//	case XK_F30: eKey = main::KEY_ESCAPE; break;
//	case XK_F31: eKey = main::KEY_ESCAPE; break;
//	case XK_F32: eKey = main::KEY_ESCAPE; break;
//	case XK_F33: eKey = main::KEY_ESCAPE; break;
//	case XK_F34: eKey = main::KEY_ESCAPE; break;
//	case XK_F35: eKey = main::KEY_ESCAPE; break;
	case XK_Shift_L: eKey = main::KEY_LSHIFT; break;
	case XK_Shift_R: eKey = main::KEY_RSHIFT; break;
	case XK_Control_L: eKey = main::KEY_LCTRL; break;
	case XK_Control_R: eKey = main::KEY_RCTRL; break;
	case XK_Caps_Lock: eKey = main::KEY_CAPSLOCK; break;
	case XK_Shift_Lock: eKey = main::KEY_SCROLLLOCK; break;
	case XK_Alt_L: eKey = main::KEY_LALT; break;
	case XK_Alt_R: eKey = main::KEY_RALT; break;
	case XK_space: eKey = main::KEY_SPACE; break;
//	case XK_exclam: eKey = main::KEY_ESCAPE; break;
//	case XK_quotedbl: eKey = main::KEY_SQUOTE; break;
//	case XK_numbersign: eKey = main::KEY_ESCAPE; break;
//	case XK_dollar: eKey = main::KEY_ESCAPE; break;
//	case XK_percent: eKey = main::KEY_ESCAPE; break;
//	case XK_ampersand: eKey = main::KEY_ESCAPE; break;
//	case XK_apostrophe: eKey = main::KEY_ESCAPE; break;
	case XK_quoteright: eKey = main::KEY_SQUOTE; break;
//	case XK_parenleft: eKey = main::KEY_ESCAPE; break;
//	case XK_parenright: eKey = main::KEY_ESCAPE; break;
//	case XK_asterisk: eKey = main::KEY_ESCAPE; break;
	case XK_plus: eKey = main::KEY_ESCAPE; break;
	case XK_comma: eKey = main::KEY_COMMA; break;
	case XK_minus: eKey = main::KEY_MINUS; break;
	case XK_period: eKey = main::KEY_PERIOD; break;
	case XK_slash: eKey = main::KEY_SLASH; break;
	case XK_0: eKey = main::KEY_0; break;
	case XK_1: eKey = main::KEY_1; break;
	case XK_2: eKey = main::KEY_2; break;
	case XK_3: eKey = main::KEY_3; break;
	case XK_4: eKey = main::KEY_4; break;
	case XK_5: eKey = main::KEY_5; break;
	case XK_6: eKey = main::KEY_6; break;
	case XK_7: eKey = main::KEY_7; break;
	case XK_8: eKey = main::KEY_8; break;
	case XK_9: eKey = main::KEY_9; break;
//	case XK_colon: eKey = main::KEY_ESCAPE; break;
	case XK_semicolon: eKey = main::KEY_SEMICOLON; break;
//	case XK_less: eKey = main::KEY_ESCAPE; break;
	case XK_equal: eKey = main::KEY_EQUALS; break;
//	case XK_greater: eKey = main::KEY_ESCAPE; break;
//	case XK_question: eKey = main::KEY_ESCAPE; break;
//	case XK_at: eKey = main::KEY_ESCAPE; break;
	case XK_A: eKey = main::KEY_A; break;
	case XK_B: eKey = main::KEY_B; break;
	case XK_C: eKey = main::KEY_C; break;
	case XK_D: eKey = main::KEY_D; break;
	case XK_E: eKey = main::KEY_E; break;
	case XK_F: eKey = main::KEY_F; break;
	case XK_G: eKey = main::KEY_G; break;
	case XK_H: eKey = main::KEY_H; break;
	case XK_I: eKey = main::KEY_I; break;
	case XK_J: eKey = main::KEY_J; break;
	case XK_K: eKey = main::KEY_K; break;
	case XK_L: eKey = main::KEY_L; break;
	case XK_M: eKey = main::KEY_M; break;
	case XK_N: eKey = main::KEY_N; break;
	case XK_O: eKey = main::KEY_O; break;
	case XK_P: eKey = main::KEY_P; break;
	case XK_Q: eKey = main::KEY_Q; break;
	case XK_R: eKey = main::KEY_R; break;
	case XK_S: eKey = main::KEY_S; break;
	case XK_T: eKey = main::KEY_T; break;
	case XK_U: eKey = main::KEY_U; break;
	case XK_V: eKey = main::KEY_V; break;
	case XK_W: eKey = main::KEY_W; break;
	case XK_X: eKey = main::KEY_X; break;
	case XK_Y: eKey = main::KEY_Y; break;
	case XK_Z: eKey = main::KEY_Z; break;
	case XK_bracketleft: eKey = main::KEY_LSQBRKT; break;
	case XK_backslash: eKey = main::KEY_BACKSLASH; break;
	case XK_bracketright: eKey = main::KEY_RSQBRKT; break;
//	case XK_asciicircum: eKey = main::KEY_ESCAPE; break;
//	case XK_underscore: eKey = main::KEY_ESCAPE; break;
//	case XK_grave: eKey = main::KEY_ESCAPE; break;
	case XK_quoteleft: eKey = main::KEY_TILDE; break;
	case XK_a: eKey = main::KEY_A; break;
	case XK_b: eKey = main::KEY_B; break;
	case XK_c: eKey = main::KEY_C; break;
	case XK_d: eKey = main::KEY_D; break;
	case XK_e: eKey = main::KEY_E; break;
	case XK_f: eKey = main::KEY_F; break;
	case XK_g: eKey = main::KEY_G; break;
	case XK_h: eKey = main::KEY_H; break;
	case XK_i: eKey = main::KEY_I; break;
	case XK_j: eKey = main::KEY_J; break;
	case XK_k: eKey = main::KEY_K; break;
	case XK_l: eKey = main::KEY_L; break;
	case XK_m: eKey = main::KEY_M; break;
	case XK_n: eKey = main::KEY_N; break;
	case XK_o: eKey = main::KEY_O; break;
	case XK_p: eKey = main::KEY_P; break;
	case XK_q: eKey = main::KEY_Q; break;
	case XK_r: eKey = main::KEY_R; break;
	case XK_s: eKey = main::KEY_S; break;
	case XK_t: eKey = main::KEY_T; break;
	case XK_u: eKey = main::KEY_U; break;
	case XK_v: eKey = main::KEY_V; break;
	case XK_w: eKey = main::KEY_W; break;
	case XK_x: eKey = main::KEY_X; break;
	case XK_y: eKey = main::KEY_Y; break;
	case XK_z: eKey = main::KEY_Z; break;
//	case XK_braceleft: eKey = main::KEY_ESCAPE; break;
//	case XK_bar: eKey = main::KEY_ESCAPE; break;
//	case XK_braceright: eKey = main::KEY_ESCAPE; break;
//	case XK_asciitilde: eKey = main::KEY_TILDE; break;
	default:
		std::cerr << "Unrecognized keycode " << std::hex << i_uiKey << std::endl;
		break;
	}
	return eKey;
}
main::MOUSEBUTTON Mouse_Button_Map(unsigned int i_uiButton)
{
	main::MOUSEBUTTON eButton = main::MB_NOOP;
	switch (i_uiButton)
	{
	case Button1: eButton = main::MB_LEFT; break;
	case Button2: eButton = main::MB_RGT; break; // not sure about this - check
	case Button3: eButton = main::MB_CTR; break; // not sure about this - check
	case Button4: eButton = main::MB_X1; break; // not sure about this - check
	case Button5: eButton = main::MB_X2; break; // not sure about this - check
	case 6: eButton = main::MB_SCROLL_V; break; // undocumented button
	case 7: eButton = main::MB_SCROLL_H; break; // undocumented button
	default: std::cerr << "Unidentified mouse button " << i_uiButton << std::endl; break;
	}
	return eButton;
}


//GLYPHMETRICSFLOAT g_gmfFont_Storage[256]; 
FTFont * g_lpcCurrent_Face = NULL;
FTFont * g_lpdFontFaces[11][4] = {{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL}};

void LoadFontFace(FONT i_eFont, const char * i_lpszPath_To_Font, bool i_bBold, bool i_bItalics)
{
	if (i_eFont >= USER_1 && i_eFont <= USER_8)
	{
		unsigned int uiFont = (i_eFont - SANS);
		unsigned int uiFace = (i_bBold ? 1 : 0) + (i_bItalics ? 2 : 0);
		g_lpdFontFaces[uiFont][uiFace] = new FTTextureFont(i_lpszPath_To_Font); // placeholder until we properly manage font types
		if (g_lpdFontFaces[uiFont][uiFace] && g_lpdFontFaces[uiFont][uiFace]->Error())
			g_lpdFontFaces[uiFont][uiFace] = NULL;
		if (g_lpdFontFaces[uiFont][uiFace])
			g_lpdFontFaces[uiFont][uiFace]->FaceSize(96);
		g_lpcCurrent_Face = g_lpdFontFaces[uiFont][uiFace];
	}
}

void SelectFontFace(FONT i_eFont, bool i_bBold, bool i_bItalics)
{
	unsigned int uiFont = (i_eFont - SANS);
	unsigned int uiFace = (i_bBold ? 1 : 0) + (i_bItalics ? 2 : 0);
	if (!g_lpdFontFaces[uiFont][uiFace])
	{
		std::string cstrPath;
		switch (i_eFont)
		{
		case SANS:
			switch(uiFace)
			{
			case 0:
				cstrPath = "/usr/share/fonts/gnu-free/FreeSans.ttf";
				break;
			case 1:
				cstrPath = "/usr/share/fonts/gnu-free/FreeSansBold.ttf";
				break;
			case 2:
				cstrPath = "/usr/share/fonts/gnu-free/FreeSansOblique.ttf";
				break;
			case 3:
				cstrPath = "/usr/share/fonts/gnu-free/FreeSansBoldOblique.ttf";
				break;
			}
			break;
		case SERIF:
			switch(uiFace)
			{
			case 0:
				cstrPath = "/usr/share/fonts/gnu-free/FreeSerif.ttf";
				break;
			case 1:
				cstrPath = "/usr/share/fonts/gnu-free/FreeSerifBold.ttf";
				break;
			case 2:
				cstrPath = "/usr/share/fonts/gnu-free/FreeSerifItalic.ttf";
				break;
			case 3:
				cstrPath = "/usr/share/fonts/gnu-free/FreeSerifBoldItalic.ttf";
				break;
			}
			break;
		case MONO:
			switch(uiFace)
			{
			case 0:
				cstrPath = "/usr/share/fonts/gnu-free/FreeMono.ttf";
				break;
			case 1:
				cstrPath = "/usr/share/fonts/gnu-free/FreeMonoBold.ttf";
				break;
			case 2:
				cstrPath = "/usr/share/fonts/gnu-free/FreeMonoOblique.ttf";
				break;
			case 3:
				cstrPath = "/usr/share/fonts/gnu-free/FreeMonoBoldOblique.ttf";
				break;
			}
			break;
		}
		g_lpdFontFaces[uiFont][uiFace] = new FTTextureFont(cstrPath.c_str()); // placeholder until we properly manage font types
		if (g_lpdFontFaces[uiFont][uiFace] && g_lpdFontFaces[uiFont][uiFace]->Error())
			g_lpdFontFaces[uiFont][uiFace] = NULL;
		if (g_lpdFontFaces[uiFont][uiFace])
			g_lpdFontFaces[uiFont][uiFace]->FaceSize(96);
	}
	g_lpcCurrent_Face = g_lpdFontFaces[uiFont][uiFace];
}


void glPrint(const double & i_dSize, const double & i_dX, const double & i_dY, const double & i_dZ, const char * i_lpszString)
{
	if (g_lpcCurrent_Face)
	{
		glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPushMatrix();
				glTranslatef(i_dX, i_dY, i_dZ);
				glScaled(i_dSize * 0.0125,i_dSize * 0.0125,i_dSize * 0.0125);
				g_lpcCurrent_Face->Render(i_lpszString);
			glPopMatrix();
		glPopAttrib();
	}
}

void TextBBox(const double & i_dSize, const char * i_lpszString, PAIR<double> & o_cBot_Left, PAIR<double> & o_cTop_Right)
{
	FTBBox	cBBox;
	if (g_lpcCurrent_Face)
	{
		cBBox = g_lpcCurrent_Face->BBox(i_lpszString);
		o_cBot_Left.m_tX = cBBox.Lower().X();
		o_cBot_Left.m_tY = cBBox.Lower().Y();
		
		o_cTop_Right.m_tX = cBBox.Upper().X();
		o_cTop_Right.m_tY = cBBox.Upper().Y();

		if (o_cTop_Right.m_tX < o_cBot_Left.m_tX)
		{
			double dTemp = o_cBot_Left.m_tX;
			o_cBot_Left.m_tX = o_cTop_Right.m_tX;
			o_cTop_Right.m_tX = dTemp;
		}
		o_cBot_Left *= i_dSize * 0.0125;
		o_cTop_Right *= i_dSize * 0.0125; // scaling to size used in rendering
	}
}

std::vector<std::string> main::Get_Directory_File_List(const std::string &i_szDirectory) /// get list of all files in the given direcctory
{
	std::vector<std::string> szRet;
	DIR * dirList = opendir (i_szDirectory.c_str());
	if (dirList)
	{
		dirent64 * direntInfo = readdir64(dirList);
		while (direntInfo)
		{
			if (direntInfo->d_name[0] != '.') // don't include . and ..; these can be assumed
				szRet.push_back(direntInfo->d_name);
			direntInfo = readdir64(dirList);
		}
	}
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
	bool * lpbPtr = (bool *) m_lpvCS_Data;
	while (lpbPtr[0]) 
		sleep(0); 
	lpbPtr[0] = true;
}
criticalsection::criticalsection(void){m_lpvCS_Data = (void *)(new bool[1]); ((bool *)m_lpvCS_Data)[0] = false;}
criticalsection::~criticalsection(void){bool * lpbPtr = (bool *) m_lpvCS_Data; delete [] lpbPtr;}

void criticalsection::Unset(void){((bool *) m_lpvCS_Data)[0] = false;}
bool criticalsection::Get(void) const {return ((bool *) m_lpvCS_Data)[0];}
