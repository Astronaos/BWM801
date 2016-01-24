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

#define NIL (0)       // A name for the void pointer

FT_Library 		g_ftLibrary;

Display	* g_lpdpyDisplay = NULL;
Window		g_wWindow;
Window 		g_wRoot;
MAIN::KEYID Key_Map(unsigned int uiKey);
MAIN::MOUSEBUTTON Mouse_Button_Map(unsigned int i_uiButton);
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
int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	bool	bWindow_Debugging_Mode = false;
	bool	bCtrl_Key = false;
	bool	bShift_Key = false;
	bool	bF12_Key = false;

	if (!g_lpMain)
	{
		std::cerr << "No main class instantiated.  Exiting." << std::endl;
		exit(1);
	}
	//printf("%i %i %i %i %i %i %i\n",MAIN::MB_LEFT, MAIN::MB_CTR, MAIN::MB_RGT, MAIN::MB_SCROLL_V, MAIN::MB_SCROLL_H, MAIN::MB_X1, MAIN::MB_X2);
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
	GLXContext glc = glXCreateContext(g_lpdpyDisplay, lpXVisual, 0, GL_TRUE);
	 glXMakeCurrent(g_lpdpyDisplay, g_wWindow, glc);
	// We want to get MapNotify events
//	XSelectInput(g_lpdpyDisplay, g_wWindow, StructureNotifyMask|ExposureMask|PointerMotionMask|ButtonPressMask|ButtonReleaseMask||KeyPressMask|KeyReleaseMask|EnterWindowMask|LeaveWindowMask);



	std::vector<XButtonEvent> veButton_Events;
	bool	bReady_To_Process_Buttons = true;
	timespec tTime_Last;
	clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Last);
	do
	{
		while (XEventsQueued(g_lpdpyDisplay,QueuedAfterFlush) > 0)
		{
			XEvent	evEvent;
			XNextEvent(g_lpdpyDisplay,&evEvent);
			switch (evEvent.type)
			{
			case KeyRelease:
				if (bWindow_Debugging_Mode)
					printf("Event loop Key %i \n",evEvent.xkey.keycode);
				switch (Key_Map(XLookupKeysym(&evEvent.xkey,0)))
				{
				case MAIN::KEY_RCTRL:
					bCtrl_Key = false;
					break;
				case MAIN::KEY_RSHIFT:
					bShift_Key = false;
					break;
				case MAIN::KEY_F12:
					bF12_Key = false;
					break;
				}
				g_lpMain->On_Key_Up(Key_Map(XLookupKeysym(&evEvent.xkey,0)),0,0, 0, 1);
				break;
			case KeyPress:
				switch (Key_Map(XLookupKeysym(&evEvent.xkey,0)))
				{
				case MAIN::KEY_RCTRL:
					bCtrl_Key = true;
					break;
				case MAIN::KEY_RSHIFT:
					bShift_Key = true;
					break;
				case MAIN::KEY_F12:
					bF12_Key = true;
					break;
				}
				if (bCtrl_Key && bShift_Key && bF12_Key)
				{
					bWindow_Debugging_Mode = !bWindow_Debugging_Mode;
					printf("Debugging mode %s\n",bWindow_Debugging_Mode ? "Enabled" : "Disabled");
				}

				if (bWindow_Debugging_Mode)
					printf("Event loop Key %i \n",evEvent.xkey.keycode);
				g_lpMain->On_Key_Down(Key_Map(XLookupKeysym(&evEvent.xkey,0)),0,0, 0, 0);
				break;
		// Mouse operations
			case ButtonPress:
			case ButtonRelease:
				if (bWindow_Debugging_Mode)
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
				if (bWindow_Debugging_Mode)
					printf("Event loop Motion\n");
				g_lpMain->On_Mousemove(Fix_Coordinate_Window(PAIR<unsigned int>(evEvent.xmotion.x, evEvent.xmotion.y)));
				break;
	// Mouse wheel events not supported under X windows
	//		case WM_MOUSEWHEEL:
	//			g_lpMain->on_mouse_wheel(MAIN::MB_SCROLL_V, (short)(wParam >> 16), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//			break;
	//		case WM_MOUSEHWHEEL:
	//			g_lpMain->on_mouse_wheel(MAIN::MB_SCROLL_V, (short)(wParam >> 16), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//			break;
			case Expose:
			case GraphicsExpose:
				if (bWindow_Debugging_Mode)
					printf("Event loop Expose\n");
				g_lpMain->On_Window_Move(Fix_Coordinate_Global(PAIR<unsigned int>(evEvent.xexpose.x,evEvent.xexpose.y)));
				g_lpMain->On_Window_Resize(PAIR<unsigned int>(evEvent.xexpose.width,evEvent.xexpose.height));
				if (!bFirst_Draw)
					g_lpMain->gfx_reshape(g_lpMain->Get_Window_Size());
				g_lpMain->Request_Refresh();
				break;
		    case ConfigureNotify:
				if (bWindow_Debugging_Mode)
					printf("Event loop Configure %i %i\n",evEvent.xconfigure.event, evEvent.xconfigure.window);
				g_lpMain->On_Window_Move(Fix_Coordinate_Global(PAIR<unsigned int>(evEvent.xconfigure.x,evEvent.xconfigure.y)));
				g_lpMain->On_Window_Resize(PAIR<unsigned int>(evEvent.xconfigure.width,evEvent.xconfigure.height));
				if (!bFirst_Draw)
					g_lpMain->gfx_reshape(g_lpMain->Get_Window_Size());
				g_lpMain->Request_Refresh();
		        break;
	 		case DestroyNotify:
				if (bWindow_Debugging_Mode)
					printf("Event loop Destroy\n");
				g_lpMain->Request_Quit();
				break;
			case MapNotify:
				if (bWindow_Debugging_Mode)
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
				if (bWindow_Debugging_Mode)
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
				if (bWindow_Debugging_Mode)
					printf("Event %i\n",evEvent.type);
				break;
			}
		}
//		printf("Main loop\n");
		timespec tTime_Curr;
		clock_gettime(CLOCK_MONOTONIC_RAW,&tTime_Curr);
		double	dTimestep = (tTime_Curr.tv_sec - tTime_Last.tv_sec) + (tTime_Curr.tv_nsec - tTime_Last.tv_nsec) * 1.0e-9;
		tTime_Last = tTime_Curr;
		if (!bFirst_Draw)
			g_lpMain->On_Timer(0,dTimestep);
		if (g_lpMain->Pending_Draw() && g_lpMain->Get_Window_Size().m_tX > 0 && g_lpMain->Get_Window_Size().m_tY > 0)
		{
			glXMakeCurrent(g_lpdpyDisplay, g_wWindow, glc);
			if (bFirst_Draw)
			{
				if(FT_Init_FreeType(&g_ftLibrary))
				{
				  fprintf(stderr, "Could not init freetype library\n");
				}
				g_lpMain->gfx_init();
				g_lpMain->gfx_reshape(g_lpMain->Get_Window_Size());
			}
			g_lpMain->Draw();
			glFlush();
			glXSwapBuffers(g_lpdpyDisplay, g_wWindow);
			bFirst_Draw = false;
		}
	}
	while (!g_lpMain->Pending_Quit());
	g_lpMain->gfx_close();
	glXDestroyContext(g_lpdpyDisplay, glc);

	//XFreeGC(g_lpdpyDisplay, gc);
	XDestroyWindow(g_lpdpyDisplay,g_wWindow);
	XCloseDisplay(g_lpdpyDisplay);	
	return 0;
}


MAIN::KEYID Key_Map(unsigned int i_uiKey)
{
	MAIN::KEYID		eKey = MAIN::KEY_NOOP;
	switch (i_uiKey)
	{
	case XK_BackSpace: eKey = MAIN::KEY_BACKSPACE; break;
	case XK_Tab: eKey = MAIN::KEY_TAB; break;
	case XK_Linefeed: eKey = MAIN::KEY_ENTER; break;
	case XK_Clear: eKey = MAIN::KEY_CLEAR; break;
	case XK_Return: eKey = MAIN::KEY_ENTER; break;
	case XK_Pause: eKey = MAIN::KEY_PAUSE; break;
	case XK_Scroll_Lock: eKey = MAIN::KEY_SCROLLLOCK; break;
//	case XK_Sys_Req: eKey = MAIN::KEY_SYSRQ; break;
	case XK_Escape: eKey = MAIN::KEY_ESCAPE; break;
	case XK_Delete: eKey = MAIN::KEY_DELETE; break;
	case XK_Home: eKey = MAIN::KEY_HOME; break;
	case XK_Left: eKey = MAIN::KEY_LEFT; break;
	case XK_Up: eKey = MAIN::KEY_UP; break;
	case XK_Right: eKey = MAIN::KEY_RIGHT; break;
	case XK_Down: eKey = MAIN::KEY_DOWN; break;
	case XK_Prior: eKey = MAIN::KEY_PGUP; break;
	case XK_Next: eKey = MAIN::KEY_PGDN; break;
	case XK_End: eKey = MAIN::KEY_END; break;
	case XK_Select: eKey = MAIN::KEY_SELECT; break;
	case XK_Print: eKey = MAIN::KEY_PRINT; break;
	case XK_Execute: eKey = MAIN::KEY_EXEC; break;
	case XK_Insert: eKey = MAIN::KEY_INSERT; break;
	case XK_Cancel: eKey = MAIN::KEY_CANCEL; break;
	case XK_Help: eKey = MAIN::KEY_HELP; break;
//	case XK_Break: eKey = MAIN::KEY_BREAK; break;
	case XK_Num_Lock: eKey = MAIN::KEY_NMLCK; break;
	case XK_KP_Enter: eKey = MAIN::KEY_NUMPD_ENTER; break;
	case XK_KP_Home: eKey = MAIN::KEY_NUMPD_7; break;
	case XK_KP_Left: eKey = MAIN::KEY_NUMPD_4; break;
	case XK_KP_Up: eKey = MAIN::KEY_NUMPD_8; break;
	case XK_KP_Right: eKey = MAIN::KEY_NUMPD_6; break;
	case XK_KP_Down: eKey = MAIN::KEY_NUMPD_2; break;
//	case XK_KP_Prior: eKey = MAIN::KEY_NUMPD_9; break;
	case XK_KP_Page_Up: eKey = MAIN::KEY_NUMPD_9; break;
//	case XK_KP_Next: eKey = MAIN::KEY_NUMPD_; break;
	case XK_KP_Page_Down: eKey = MAIN::KEY_NUMPD_3; break;
	case XK_KP_End: eKey = MAIN::KEY_NUMPD_1; break;
	case XK_KP_Begin: eKey = MAIN::KEY_NUMPD_5; break;
	case XK_KP_Insert: eKey = MAIN::KEY_NUMPD_0; break;
	case XK_KP_Delete: eKey = MAIN::KEY_NUMPD_DOT; break;
//	case XK_KP_Equal: eKey = MAIN::KEY_ESCAPE; break;
	case XK_KP_Multiply: eKey = MAIN::KEY_NUMPD_TIMES; break;
	case XK_KP_Add: eKey = MAIN::KEY_NUMPD_PLUS; break;
	case XK_KP_Separator: eKey = MAIN::KEY_NUMPD_ENTER; break;
	case XK_KP_Subtract: eKey = MAIN::KEY_NUMPD_MINUS; break;
	case XK_KP_Decimal: eKey = MAIN::KEY_NUMPD_DOT; break;
	case XK_KP_Divide: eKey = MAIN::KEY_NUMPD_DIV; break;
	case XK_KP_0: eKey = MAIN::KEY_NUMPD_0; break;
	case XK_KP_1: eKey = MAIN::KEY_NUMPD_1; break;
	case XK_KP_2: eKey = MAIN::KEY_NUMPD_2; break;
	case XK_KP_3: eKey = MAIN::KEY_NUMPD_3; break;
	case XK_KP_4: eKey = MAIN::KEY_NUMPD_4; break;
	case XK_KP_5: eKey = MAIN::KEY_NUMPD_5; break;
	case XK_KP_6: eKey = MAIN::KEY_NUMPD_6; break;
	case XK_KP_7: eKey = MAIN::KEY_NUMPD_7; break;
	case XK_KP_8: eKey = MAIN::KEY_NUMPD_8; break;
	case XK_KP_9: eKey = MAIN::KEY_NUMPD_9; break;
	case XK_F1: eKey = MAIN::KEY_F1; break;
	case XK_F2: eKey = MAIN::KEY_F2; break;
	case XK_F3: eKey = MAIN::KEY_F3; break;
	case XK_F4: eKey = MAIN::KEY_F4; break;
	case XK_F5: eKey = MAIN::KEY_F5; break;
	case XK_F6: eKey = MAIN::KEY_F6; break;
	case XK_F7: eKey = MAIN::KEY_F7; break;
	case XK_F8: eKey = MAIN::KEY_F8; break;
	case XK_F9: eKey = MAIN::KEY_F9; break;
	case XK_F10: eKey = MAIN::KEY_F10; break;
	case XK_F11: eKey = MAIN::KEY_F11; break;
	case XK_F12: eKey = MAIN::KEY_F12; break;
	case XK_F13: eKey = MAIN::KEY_F13; break;
	case XK_F14: eKey = MAIN::KEY_F14; break;
	case XK_F15: eKey = MAIN::KEY_F15; break;
	case XK_F16: eKey = MAIN::KEY_F16; break;
	case XK_F17: eKey = MAIN::KEY_F17; break;
	case XK_F18: eKey = MAIN::KEY_F18; break;
	case XK_F19: eKey = MAIN::KEY_F19; break;
	case XK_F20: eKey = MAIN::KEY_F20; break;
	case XK_F21: eKey = MAIN::KEY_F21; break;
	case XK_F22: eKey = MAIN::KEY_F22; break;
	case XK_F23: eKey = MAIN::KEY_F23; break;
	case XK_F24: eKey = MAIN::KEY_F24; break;
//	case XK_F25: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_F26: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_F27: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_F28: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_F29: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_F30: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_F31: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_F32: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_F33: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_F34: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_F35: eKey = MAIN::KEY_ESCAPE; break;
	case XK_Shift_L: eKey = MAIN::KEY_LSHIFT; break;
	case XK_Shift_R: eKey = MAIN::KEY_RSHIFT; break;
	case XK_Control_L: eKey = MAIN::KEY_LCTRL; break;
	case XK_Control_R: eKey = MAIN::KEY_RCTRL; break;
	case XK_Caps_Lock: eKey = MAIN::KEY_CAPSLOCK; break;
	case XK_Shift_Lock: eKey = MAIN::KEY_SCROLLLOCK; break;
	case XK_Alt_L: eKey = MAIN::KEY_LALT; break;
	case XK_Alt_R: eKey = MAIN::KEY_RALT; break;
	case XK_space: eKey = MAIN::KEY_SPACE; break;
//	case XK_exclam: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_quotedbl: eKey = MAIN::KEY_SQUOTE; break;
//	case XK_numbersign: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_dollar: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_percent: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_ampersand: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_apostrophe: eKey = MAIN::KEY_ESCAPE; break;
	case XK_quoteright: eKey = MAIN::KEY_SQUOTE; break;
//	case XK_parenleft: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_parenright: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_asterisk: eKey = MAIN::KEY_ESCAPE; break;
	case XK_plus: eKey = MAIN::KEY_ESCAPE; break;
	case XK_comma: eKey = MAIN::KEY_COMMA; break;
	case XK_minus: eKey = MAIN::KEY_MINUS; break;
	case XK_period: eKey = MAIN::KEY_PERIOD; break;
	case XK_slash: eKey = MAIN::KEY_SLASH; break;
	case XK_0: eKey = MAIN::KEY_0; break;
	case XK_1: eKey = MAIN::KEY_1; break;
	case XK_2: eKey = MAIN::KEY_2; break;
	case XK_3: eKey = MAIN::KEY_3; break;
	case XK_4: eKey = MAIN::KEY_4; break;
	case XK_5: eKey = MAIN::KEY_5; break;
	case XK_6: eKey = MAIN::KEY_6; break;
	case XK_7: eKey = MAIN::KEY_7; break;
	case XK_8: eKey = MAIN::KEY_8; break;
	case XK_9: eKey = MAIN::KEY_9; break;
//	case XK_colon: eKey = MAIN::KEY_ESCAPE; break;
	case XK_semicolon: eKey = MAIN::KEY_SEMICOLON; break;
//	case XK_less: eKey = MAIN::KEY_ESCAPE; break;
	case XK_equal: eKey = MAIN::KEY_EQUALS; break;
//	case XK_greater: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_question: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_at: eKey = MAIN::KEY_ESCAPE; break;
	case XK_A: eKey = MAIN::KEY_A; break;
	case XK_B: eKey = MAIN::KEY_B; break;
	case XK_C: eKey = MAIN::KEY_C; break;
	case XK_D: eKey = MAIN::KEY_D; break;
	case XK_E: eKey = MAIN::KEY_E; break;
	case XK_F: eKey = MAIN::KEY_F; break;
	case XK_G: eKey = MAIN::KEY_G; break;
	case XK_H: eKey = MAIN::KEY_H; break;
	case XK_I: eKey = MAIN::KEY_I; break;
	case XK_J: eKey = MAIN::KEY_J; break;
	case XK_K: eKey = MAIN::KEY_K; break;
	case XK_L: eKey = MAIN::KEY_L; break;
	case XK_M: eKey = MAIN::KEY_M; break;
	case XK_N: eKey = MAIN::KEY_N; break;
	case XK_O: eKey = MAIN::KEY_O; break;
	case XK_P: eKey = MAIN::KEY_P; break;
	case XK_Q: eKey = MAIN::KEY_Q; break;
	case XK_R: eKey = MAIN::KEY_R; break;
	case XK_S: eKey = MAIN::KEY_S; break;
	case XK_T: eKey = MAIN::KEY_T; break;
	case XK_U: eKey = MAIN::KEY_U; break;
	case XK_V: eKey = MAIN::KEY_V; break;
	case XK_W: eKey = MAIN::KEY_W; break;
	case XK_X: eKey = MAIN::KEY_X; break;
	case XK_Y: eKey = MAIN::KEY_Y; break;
	case XK_Z: eKey = MAIN::KEY_Z; break;
	case XK_bracketleft: eKey = MAIN::KEY_LSQBRKT; break;
	case XK_backslash: eKey = MAIN::KEY_BACKSLASH; break;
	case XK_bracketright: eKey = MAIN::KEY_RSQBRKT; break;
//	case XK_asciicircum: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_underscore: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_grave: eKey = MAIN::KEY_ESCAPE; break;
	case XK_quoteleft: eKey = MAIN::KEY_TILDE; break;
	case XK_a: eKey = MAIN::KEY_A; break;
	case XK_b: eKey = MAIN::KEY_B; break;
	case XK_c: eKey = MAIN::KEY_C; break;
	case XK_d: eKey = MAIN::KEY_D; break;
	case XK_e: eKey = MAIN::KEY_E; break;
	case XK_f: eKey = MAIN::KEY_F; break;
	case XK_g: eKey = MAIN::KEY_G; break;
	case XK_h: eKey = MAIN::KEY_H; break;
	case XK_i: eKey = MAIN::KEY_I; break;
	case XK_j: eKey = MAIN::KEY_J; break;
	case XK_k: eKey = MAIN::KEY_K; break;
	case XK_l: eKey = MAIN::KEY_L; break;
	case XK_m: eKey = MAIN::KEY_M; break;
	case XK_n: eKey = MAIN::KEY_N; break;
	case XK_o: eKey = MAIN::KEY_O; break;
	case XK_p: eKey = MAIN::KEY_P; break;
	case XK_q: eKey = MAIN::KEY_Q; break;
	case XK_r: eKey = MAIN::KEY_R; break;
	case XK_s: eKey = MAIN::KEY_S; break;
	case XK_t: eKey = MAIN::KEY_T; break;
	case XK_u: eKey = MAIN::KEY_U; break;
	case XK_v: eKey = MAIN::KEY_V; break;
	case XK_w: eKey = MAIN::KEY_W; break;
	case XK_x: eKey = MAIN::KEY_X; break;
	case XK_y: eKey = MAIN::KEY_Y; break;
	case XK_z: eKey = MAIN::KEY_Z; break;
//	case XK_braceleft: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_bar: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_braceright: eKey = MAIN::KEY_ESCAPE; break;
//	case XK_asciitilde: eKey = MAIN::KEY_TILDE; break;
	default:
		std::cerr << "Unrecognized keycode " << std::hex << i_uiKey << std::endl;
		break;
	}
	return eKey;
}
MAIN::MOUSEBUTTON Mouse_Button_Map(unsigned int i_uiButton)
{
	MAIN::MOUSEBUTTON eButton = MAIN::MB_NOOP;
	switch (i_uiButton)
	{
	case Button1: eButton = MAIN::MB_LEFT; break;
	case Button2: eButton = MAIN::MB_RGT; break; // not sure about this - check
	case Button3: eButton = MAIN::MB_CTR; break; // not sure about this - check
	case Button4: eButton = MAIN::MB_X1; break; // not sure about this - check
	case Button5: eButton = MAIN::MB_X2; break; // not sure about this - check
	case 6: eButton = MAIN::MB_SCROLL_V; break; // undocumented button
	case 7: eButton = MAIN::MB_SCROLL_H; break; // undocumented button
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

std::vector<std::string> MAIN::Get_Directory_File_List(const std::string &i_szDirectory) /// get list of all files in the given direcctory
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
