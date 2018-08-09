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
using namespace bwm801;


#define NIL (0)       // A name for the void pointer

FT_Library 		g_ftLibrary;

Display	* g_lpdpyDisplay = nullptr;
Window		g_wWindow;
Window 		g_wRoot;
GLXContext g_glc;

main::keyid Key_Map(unsigned int uiKey);
main::mousebutton Mouse_Button_Map(unsigned int i_uiButton);
void Sleep(unsigned int i_uiLength)
{
	usleep(i_uiLength * 1000);
}

// X11 uses display coordinates that have (0,0) as the top left corner.  
pair<unsigned int>	Fix_Coordinate_Global(const pair<unsigned int> &i_tPosition)
{
	pair<unsigned int> tRet;
	Window wRoot;
	int iX,iY;
	unsigned int uiWidth,uiHeight,uiBorder_Width,uiDepth;
	XGetGeometry(g_lpdpyDisplay, g_wRoot, &wRoot, &iX, &iY, &uiWidth, &uiHeight, &uiBorder_Width, &uiDepth);
//	printf("Global geo: %i %i\n",uiWidth,uiHeight);
	tRet.m_tX = i_tPosition.m_tX;
	tRet.m_tY = (uiHeight - i_tPosition.m_tY);
	return tRet;
}
pair<unsigned int>	Fix_Coordinate_Window(const pair<unsigned int> &i_tPosition)
{
	pair<unsigned int> tRet;
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



void Load_OGL_Extensions(void)
{
	GLubyte sglGenerateMipmap[] = {"sglGenerateMipmap"};
	GLubyte sglGenerateTextureMipmap[] = {"glGenerateTextureMipmap"};

	g_lpf_glGenerateMipmap = reinterpret_cast<oglve>(glXGetProcAddress(sglGenerateMipmap));
	g_lpf_glGenerateTextureMipmap = reinterpret_cast<oglvui>(glXGetProcAddress(sglGenerateTextureMipmap));
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
	Load_OGL_Extensions();
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

	std::ostringstream ossPath;
	ossPath << getenv("HOME");
	ossPath << "/Pictures";
	g_lpMain->Set_Screenshot_Save_Path(ossPath.str());

	vThread_List.push_back(std::thread(Main_Timer_Loop));
	vThread_List.push_back(std::thread(Gfx_Loop));
	vThread_List.push_back(std::thread(Screenshot_Loop));

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
				if (g_lpMain->Is_Debug_Mode_Enabled())
					printf("Event loop Key %i \n",evEvent.xkey.keycode);
				switch (Key_Map(XLookupKeysym(&evEvent.xkey,0)))
				{
				case main::key_rctrl:
					bCtrl_Key = false;
					break;
				case main::key_rshift:
					bShift_Key = false;
					break;
				case main::key_f12:
					bF12_Key = false;
					break;
				}
				g_lpMain->On_Key_Up(Key_Map(XLookupKeysym(&evEvent.xkey,0)),0,0, 0, 1);
				break;
			case KeyPress:
				switch (Key_Map(XLookupKeysym(&evEvent.xkey,0)))
				{
				case main::key_rctrl:
					bCtrl_Key = true;
					break;
				case main::key_rshift:
					bShift_Key = true;
					break;
				case main::key_f12:
					bF12_Key = true;
					break;
				}
				if (g_lpMain->Is_Debug_Mode_Enabled())
					printf("Event loop Key %i \n",evEvent.xkey.keycode);

				g_lpMain->On_Key_Down(Key_Map(XLookupKeysym(&evEvent.xkey,0)),0,0, 0, 0);
				break;
		// Mouse operations
			case ButtonPress:
			case ButtonRelease:
				if (g_lpMain->Is_Debug_Mode_Enabled())
					printf("Event loop Button %i (%i,%i)\n",Mouse_Button_Map(evEvent.xbutton.button), Fix_Coordinate_Window(pair<unsigned int>(evEvent.xbutton.x, evEvent.xbutton.y)).m_tX,Fix_Coordinate_Window(pair<unsigned int>(evEvent.xbutton.x, evEvent.xbutton.y)).m_tY);
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
							g_lpMain->On_Mouse_Button_Double_Click(Mouse_Button_Map(veButton_Events[0].button), Fix_Coordinate_Window(pair<unsigned int>(veButton_Events[0].x, veButton_Events[0].y)));
						}
						else
						{
							for (std::vector<XButtonEvent>::const_iterator cI = veButton_Events.begin(); cI != veButton_Events.end(); cI++)
							{
								if (cI->type == ButtonPress)
								{
									g_lpMain->On_Mouse_Button_Down(Mouse_Button_Map(cI->button), Fix_Coordinate_Window(pair<unsigned int>(cI->x, cI->y)));
								}
								else
								{
									g_lpMain->On_Mouse_Button_Up(Mouse_Button_Map(cI->button), Fix_Coordinate_Window(pair<unsigned int>(cI->x, cI->y)));
								}
							}
						}
						veButton_Events.clear();
					}
					else if (evEvent.type == ButtonPress)
					{
						g_lpMain->On_Mouse_Button_Down(Mouse_Button_Map(evEvent.xbutton.button), Fix_Coordinate_Window(pair<unsigned int>(evEvent.xbutton.x, evEvent.xbutton.y)));
					}
					else
					{
						g_lpMain->On_Mouse_Button_Up(Mouse_Button_Map(evEvent.xbutton.button), Fix_Coordinate_Window(pair<unsigned int>(evEvent.xbutton.x, evEvent.xbutton.y)));
					}
				}
				break;
			case MotionNotify:
				if (g_lpMain->Is_Debug_Mode_Enabled())
					printf("Event loop Motion\n");
				g_lpMain->On_Mousemove(Fix_Coordinate_Window(pair<unsigned int>(evEvent.xmotion.x, evEvent.xmotion.y)));
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
				if (g_lpMain->Is_Debug_Mode_Enabled())
					printf("Event loop Expose\n");
				g_lpMain->On_Window_Move(Fix_Coordinate_Global(pair<unsigned int>(evEvent.xexpose.x,evEvent.xexpose.y)));
				g_lpMain->On_Window_Resize(pair<unsigned int>(evEvent.xexpose.width,evEvent.xexpose.height));
				if (!bFirst_Draw)
					g_lpMain->gfx_reshape(g_lpMain->Get_Window_Size());
				g_lpMain->Request_Refresh();
				break;
		    case ConfigureNotify:
				if (g_lpMain->Is_Debug_Mode_Enabled())
					printf("Event loop Configure %i %i\n",evEvent.xconfigure.event, evEvent.xconfigure.window);
				g_lpMain->On_Window_Move(Fix_Coordinate_Global(pair<unsigned int>(evEvent.xconfigure.x,evEvent.xconfigure.y)));
				g_lpMain->On_Window_Resize(pair<unsigned int>(evEvent.xconfigure.width,evEvent.xconfigure.height));
				if (!bFirst_Draw)
					g_lpMain->gfx_reshape(g_lpMain->Get_Window_Size());
				g_lpMain->Request_Refresh();
		        break;
	 		case DestroyNotify:
				if (g_lpMain->Is_Debug_Mode_Enabled())
					printf("Event loop Destroy\n");
				g_lpMain->Request_Quit();
				break;
			case MapNotify:
				if (g_lpMain->Is_Debug_Mode_Enabled())
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
				if (g_lpMain->Is_Debug_Mode_Enabled())
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
				if (g_lpMain->Is_Debug_Mode_Enabled())
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


main::keyid Key_Map(unsigned int i_uiKey)
{
	main::keyid		eKey = main::key_noop;
	switch (i_uiKey)
	{
	case XK_BackSpace: eKey = main::key_backspace; break;
	case XK_Tab: eKey = main::key_tab; break;
	case XK_Linefeed: eKey = main::key_enter; break;
	case XK_Clear: eKey = main::key_clear; break;
	case XK_Return: eKey = main::key_enter; break;
	case XK_Pause: eKey = main::key_pause; break;
	case XK_Scroll_Lock: eKey = main::key_scrolllock; break;
//	case XK_Sys_Req: eKey = main::key_sysrq; break;
	case XK_Escape: eKey = main::key_escape; break;
	case XK_Delete: eKey = main::key_delete; break;
	case XK_Home: eKey = main::key_home; break;
	case XK_Left: eKey = main::key_left; break;
	case XK_Up: eKey = main::key_up; break;
	case XK_Right: eKey = main::key_right; break;
	case XK_Down: eKey = main::key_down; break;
	case XK_Prior: eKey = main::key_pgup; break;
	case XK_Next: eKey = main::key_pgdn; break;
	case XK_End: eKey = main::key_end; break;
	case XK_Select: eKey = main::key_select; break;
	case XK_Print: eKey = main::key_print; break;
	case XK_Execute: eKey = main::key_exec; break;
	case XK_Insert: eKey = main::key_insert; break;
	case XK_Cancel: eKey = main::key_cancel; break;
	case XK_Help: eKey = main::key_help; break;
//	case XK_Break: eKey = main::key_break; break;
	case XK_Num_Lock: eKey = main::key_nmlck; break;
	case XK_KP_Enter: eKey = main::key_numpd_enter; break;
	case XK_KP_Home: eKey = main::key_numpd_7; break;
	case XK_KP_Left: eKey = main::key_numpd_4; break;
	case XK_KP_Up: eKey = main::key_numpd_8; break;
	case XK_KP_Right: eKey = main::key_numpd_6; break;
	case XK_KP_Down: eKey = main::key_numpd_2; break;
//	case XK_KP_Prior: eKey = main::key_numpd_9; break;
	case XK_KP_Page_Up: eKey = main::key_numpd_9; break;
//	case XK_KP_Next: eKey = main::key_numpd_; break;
	case XK_KP_Page_Down: eKey = main::key_numpd_3; break;
	case XK_KP_End: eKey = main::key_numpd_1; break;
	case XK_KP_Begin: eKey = main::key_numpd_5; break;
	case XK_KP_Insert: eKey = main::key_numpd_0; break;
	case XK_KP_Delete: eKey = main::key_numpd_dot; break;
//	case XK_KP_Equal: eKey = main::key_escape; break;
	case XK_KP_Multiply: eKey = main::key_numpd_times; break;
	case XK_KP_Add: eKey = main::key_numpd_plus; break;
	case XK_KP_Separator: eKey = main::key_numpd_enter; break;
	case XK_KP_Subtract: eKey = main::key_numpd_minus; break;
	case XK_KP_Decimal: eKey = main::key_numpd_dot; break;
	case XK_KP_Divide: eKey = main::key_numpd_div; break;
	case XK_KP_0: eKey = main::key_numpd_0; break;
	case XK_KP_1: eKey = main::key_numpd_1; break;
	case XK_KP_2: eKey = main::key_numpd_2; break;
	case XK_KP_3: eKey = main::key_numpd_3; break;
	case XK_KP_4: eKey = main::key_numpd_4; break;
	case XK_KP_5: eKey = main::key_numpd_5; break;
	case XK_KP_6: eKey = main::key_numpd_6; break;
	case XK_KP_7: eKey = main::key_numpd_7; break;
	case XK_KP_8: eKey = main::key_numpd_8; break;
	case XK_KP_9: eKey = main::key_numpd_9; break;
	case XK_F1: eKey = main::key_f1; break;
	case XK_F2: eKey = main::key_f2; break;
	case XK_F3: eKey = main::key_f3; break;
	case XK_F4: eKey = main::key_f4; break;
	case XK_F5: eKey = main::key_f5; break;
	case XK_F6: eKey = main::key_f6; break;
	case XK_F7: eKey = main::key_f7; break;
	case XK_F8: eKey = main::key_f8; break;
	case XK_F9: eKey = main::key_f9; break;
	case XK_F10: eKey = main::key_f10; break;
	case XK_F11: eKey = main::key_f11; break;
	case XK_F12: eKey = main::key_f12; break;
	case XK_F13: eKey = main::key_f13; break;
	case XK_F14: eKey = main::key_f14; break;
	case XK_F15: eKey = main::key_f15; break;
	case XK_F16: eKey = main::key_f16; break;
	case XK_F17: eKey = main::key_f17; break;
	case XK_F18: eKey = main::key_f18; break;
	case XK_F19: eKey = main::key_f19; break;
	case XK_F20: eKey = main::key_f20; break;
	case XK_F21: eKey = main::key_f21; break;
	case XK_F22: eKey = main::key_f22; break;
	case XK_F23: eKey = main::key_f23; break;
	case XK_F24: eKey = main::key_f24; break;
//	case XK_F25: eKey = main::key_escape; break;
//	case XK_F26: eKey = main::key_escape; break;
//	case XK_F27: eKey = main::key_escape; break;
//	case XK_F28: eKey = main::key_escape; break;
//	case XK_F29: eKey = main::key_escape; break;
//	case XK_F30: eKey = main::key_escape; break;
//	case XK_F31: eKey = main::key_escape; break;
//	case XK_F32: eKey = main::key_escape; break;
//	case XK_F33: eKey = main::key_escape; break;
//	case XK_F34: eKey = main::key_escape; break;
//	case XK_F35: eKey = main::key_escape; break;
	case XK_Shift_L: eKey = main::key_lshift; break;
	case XK_Shift_R: eKey = main::key_rshift; break;
	case XK_Control_L: eKey = main::key_lctrl; break;
	case XK_Control_R: eKey = main::key_rctrl; break;
	case XK_Caps_Lock: eKey = main::key_capslock; break;
	case XK_Shift_Lock: eKey = main::key_scrolllock; break;
	case XK_Alt_L: eKey = main::key_lalt; break;
	case XK_Alt_R: eKey = main::key_ralt; break;
	case XK_space: eKey = main::key_space; break;
//	case XK_exclam: eKey = main::key_escape; break;
//	case XK_quotedbl: eKey = main::key_squote; break;
//	case XK_numbersign: eKey = main::key_escape; break;
//	case XK_dollar: eKey = main::key_escape; break;
//	case XK_percent: eKey = main::key_escape; break;
//	case XK_ampersand: eKey = main::key_escape; break;
//	case XK_apostrophe: eKey = main::key_escape; break;
	case XK_quoteright: eKey = main::key_squote; break;
//	case XK_parenleft: eKey = main::key_escape; break;
//	case XK_parenright: eKey = main::key_escape; break;
//	case XK_asterisk: eKey = main::key_escape; break;
	case XK_plus: eKey = main::key_escape; break;
	case XK_comma: eKey = main::key_comma; break;
	case XK_minus: eKey = main::key_minus; break;
	case XK_period: eKey = main::key_period; break;
	case XK_slash: eKey = main::key_slash; break;
	case XK_0: eKey = main::key_0; break;
	case XK_1: eKey = main::key_1; break;
	case XK_2: eKey = main::key_2; break;
	case XK_3: eKey = main::key_3; break;
	case XK_4: eKey = main::key_4; break;
	case XK_5: eKey = main::key_5; break;
	case XK_6: eKey = main::key_6; break;
	case XK_7: eKey = main::key_7; break;
	case XK_8: eKey = main::key_8; break;
	case XK_9: eKey = main::key_9; break;
//	case XK_colon: eKey = main::key_escape; break;
	case XK_semicolon: eKey = main::key_semicolon; break;
//	case XK_less: eKey = main::key_escape; break;
	case XK_equal: eKey = main::key_equals; break;
//	case XK_greater: eKey = main::key_escape; break;
//	case XK_question: eKey = main::key_escape; break;
//	case XK_at: eKey = main::key_escape; break;
	case XK_A: eKey = main::key_a; break;
	case XK_B: eKey = main::key_b; break;
	case XK_C: eKey = main::key_c; break;
	case XK_D: eKey = main::key_d; break;
	case XK_E: eKey = main::key_e; break;
	case XK_F: eKey = main::key_f; break;
	case XK_G: eKey = main::key_g; break;
	case XK_H: eKey = main::key_h; break;
	case XK_I: eKey = main::key_i; break;
	case XK_J: eKey = main::key_j; break;
	case XK_K: eKey = main::key_k; break;
	case XK_L: eKey = main::key_l; break;
	case XK_M: eKey = main::key_m; break;
	case XK_N: eKey = main::key_n; break;
	case XK_O: eKey = main::key_o; break;
	case XK_P: eKey = main::key_p; break;
	case XK_Q: eKey = main::key_q; break;
	case XK_R: eKey = main::key_r; break;
	case XK_S: eKey = main::key_s; break;
	case XK_T: eKey = main::key_t; break;
	case XK_U: eKey = main::key_u; break;
	case XK_V: eKey = main::key_v; break;
	case XK_W: eKey = main::key_w; break;
	case XK_X: eKey = main::key_x; break;
	case XK_Y: eKey = main::key_y; break;
	case XK_Z: eKey = main::key_z; break;
	case XK_bracketleft: eKey = main::key_lsqbrkt; break;
	case XK_backslash: eKey = main::key_backslash; break;
	case XK_bracketright: eKey = main::key_rsqbrkt; break;
//	case XK_asciicircum: eKey = main::key_escape; break;
//	case XK_underscore: eKey = main::key_escape; break;
//	case XK_grave: eKey = main::key_escape; break;
	case XK_quoteleft: eKey = main::key_tilde; break;
	case XK_a: eKey = main::key_a; break;
	case XK_b: eKey = main::key_b; break;
	case XK_c: eKey = main::key_c; break;
	case XK_d: eKey = main::key_d; break;
	case XK_e: eKey = main::key_e; break;
	case XK_f: eKey = main::key_f; break;
	case XK_g: eKey = main::key_g; break;
	case XK_h: eKey = main::key_h; break;
	case XK_i: eKey = main::key_i; break;
	case XK_j: eKey = main::key_j; break;
	case XK_k: eKey = main::key_k; break;
	case XK_l: eKey = main::key_l; break;
	case XK_m: eKey = main::key_m; break;
	case XK_n: eKey = main::key_n; break;
	case XK_o: eKey = main::key_o; break;
	case XK_p: eKey = main::key_p; break;
	case XK_q: eKey = main::key_q; break;
	case XK_r: eKey = main::key_r; break;
	case XK_s: eKey = main::key_s; break;
	case XK_t: eKey = main::key_t; break;
	case XK_u: eKey = main::key_u; break;
	case XK_v: eKey = main::key_v; break;
	case XK_w: eKey = main::key_w; break;
	case XK_x: eKey = main::key_x; break;
	case XK_y: eKey = main::key_y; break;
	case XK_z: eKey = main::key_z; break;
//	case XK_braceleft: eKey = main::key_escape; break;
//	case XK_bar: eKey = main::key_escape; break;
//	case XK_braceright: eKey = main::key_escape; break;
//	case XK_asciitilde: eKey = main::key_tilde; break;
	default:
		std::cerr << "Unrecognized keycode " << std::hex << i_uiKey << std::endl;
		break;
	}
	return eKey;
}
main::mousebutton Mouse_Button_Map(unsigned int i_uiButton)
{
	main::mousebutton eButton = main::mb_noop;
	switch (i_uiButton)
	{
	case Button1: eButton = main::mb_left; break;
	case Button2: eButton = main::mb_rgt; break; // not sure about this - check
	case Button3: eButton = main::mb_ctr; break; // not sure about this - check
	case Button4: eButton = main::mb_x1; break; // not sure about this - check
	case Button5: eButton = main::mb_x2; break; // not sure about this - check
	case 6: eButton = main::mb_scroll_v; break; // undocumented button
	case 7: eButton = main::mb_scroll_h; break; // undocumented button
	default: std::cerr << "Unidentified mouse button " << i_uiButton << std::endl; break;
	}
	return eButton;
}


//GLYPHMETRICSFLOAT g_gmfFont_Storage[256]; 
FTFont * g_lpcCurrent_Face = nullptr;
FTFont * g_lpdFontFaces[11][4] = {{nullptr,nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr,nullptr},{nullptr,nullptr,nullptr,nullptr}};

void bwm801::LoadFontFace(FONT i_eFont, const char * i_lpszPath_To_Font, bool i_bBold, bool i_bItalics)
{
	if (i_eFont >= USER_1 && i_eFont <= USER_8)
	{
		unsigned int uiFont = (i_eFont - SANS);
		unsigned int uiFace = (i_bBold ? 1 : 0) + (i_bItalics ? 2 : 0);
		g_lpdFontFaces[uiFont][uiFace] = new FTTextureFont(i_lpszPath_To_Font); // placeholder until we properly manage font types
		if (g_lpdFontFaces[uiFont][uiFace] && g_lpdFontFaces[uiFont][uiFace]->Error())
			g_lpdFontFaces[uiFont][uiFace] = nullptr;
		if (g_lpdFontFaces[uiFont][uiFace])
			g_lpdFontFaces[uiFont][uiFace]->FaceSize(96);
		g_lpcCurrent_Face = g_lpdFontFaces[uiFont][uiFace];
	}
}

void bwm801::SelectFontFace(FONT i_eFont, bool i_bBold, bool i_bItalics)
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
			g_lpdFontFaces[uiFont][uiFace] = nullptr;
		if (g_lpdFontFaces[uiFont][uiFace])
			g_lpdFontFaces[uiFont][uiFace]->FaceSize(96);
	}
	g_lpcCurrent_Face = g_lpdFontFaces[uiFont][uiFace];
}


void bwm801::glPrint(const float & i_dSize, const float & i_dX, const float & i_dY, const float & i_dZ, const char * i_lpszString)
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

void bwm801::TextBBox(const float & i_dSize, const char * i_lpszString, pair<float> & o_cBot_Left, pair<float> & o_cTop_Right)
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

