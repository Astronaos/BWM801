
#include <core.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <core_screenshot.h>
// This routine contains the common main methods that are OS independant


main * g_lpMain = NULL;
main::main(void)
{
	m_bEngine_Debug_Mode = false;
	m_lpbMouse_Button_Status = NULL;
	m_lpbKey_Status = NULL;
	if (g_lpMain != NULL)
	{
		fprintf(stderr,"More than one core main class has been instantiated. Exiting!\n");
		exit(0);
	}
	g_lpMain = this;
	m_bDraw_Flag = false;
	m_bQuit_Flag = false;
	m_eDraw_Mode = ORTHO;

	unsigned int uiNum_Mouse_Buttons = (unsigned int)(MB_MAX - MB_NOOP);
	m_lpbMouse_Button_Status = new bool [uiNum_Mouse_Buttons];
	memset(m_lpbMouse_Button_Status,0,sizeof(bool) * uiNum_Mouse_Buttons);

	unsigned int uiNum_Keys = (unsigned int)(KEY_MAX - KEY_NOOP);
	m_lpbKey_Status = new bool [uiNum_Keys];
	memset(m_lpbKey_Status,0,sizeof(bool) * uiNum_Keys);

	m_bDraw_Pane_Grids = false;

	m_szScreenshot_Default_Filename = "BWM801-screenshot";

}
void main::Process_Command_Line(unsigned int i_uiNum_Parameters, const char * i_lpszParameter_Values[])
{
	for (unsigned int uiI = 1; uiI < i_uiNum_Parameters; uiI++)
	{
		m_vszCommand_Line_Parameters.push_back(i_lpszParameter_Values[uiI]);
	}
}

void main::On_Key_Down(main::KEYID eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State)
{
	m_lpbKey_Status[eKey_ID - KEY_NOOP] = true;
	on_key_down(eKey_ID, chScan_Code, uiRepeat_Count, bExtended_Key, bPrevious_Key_State);
	bool bCtrl = m_lpbKey_Status[KEY_LCTRL - KEY_NOOP] || m_lpbKey_Status[KEY_RCTRL - KEY_NOOP];
	bool bShift = m_lpbKey_Status[KEY_LSHIFT - KEY_NOOP] || m_lpbKey_Status[KEY_RSHIFT - KEY_NOOP];

	if (bCtrl && bShift && m_lpbKey_Status[KEY_F12 - KEY_NOOP])
	{
		m_bEngine_Debug_Mode = !m_bEngine_Debug_Mode;
		printf("Debugging mode %s\n",m_bEngine_Debug_Mode ? "Enabled" : "Disabled");
		if (!m_bEngine_Debug_Mode)
			m_bDraw_Pane_Grids = false;
	}
	else if (m_bEngine_Debug_Mode && (bCtrl && m_lpbKey_Status[KEY_F12 - KEY_NOOP]))
	{
		m_bDraw_Pane_Grids = !m_bDraw_Pane_Grids;
		printf("Grid mode %s\n",m_bDraw_Pane_Grids ? "Enabled" : "Disabled");
	}

}
void main::On_Key_Up(main::KEYID eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State)
{
	m_lpbKey_Status[eKey_ID - KEY_NOOP] = false;
	on_key_up(eKey_ID, chScan_Code, uiRepeat_Count, bExtended_Key, bPrevious_Key_State);
}
void main::On_Mouse_Button_Double_Click(main::MOUSEBUTTON i_eButton, const PAIR<unsigned int> & i_tMouse_Position)
{
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_button_double_click(i_eButton, m_tMouse_Position);
}
void main::On_Mouse_Button_Down(main::MOUSEBUTTON i_eButton, const PAIR<unsigned int> & i_tMouse_Position)
{
	m_lpbMouse_Button_Status[i_eButton - KEY_NOOP] = true;
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_button_down(i_eButton, m_tMouse_Position);
}
void main::On_Mouse_Button_Up(main::MOUSEBUTTON i_eButton, const PAIR<unsigned int> & i_tMouse_Position)
{
	m_lpbMouse_Button_Status[i_eButton - KEY_NOOP] = false;
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_button_up(i_eButton, m_tMouse_Position);
}
void main::On_Mousemove(const PAIR<unsigned int> & i_tMouse_Position)
{
//	printf("%i %i\n",i_tMouse_Position.m_tX,i_tMouse_Position.m_tY);
	m_tMouse_Position_Last = m_tMouse_Position;
	m_tMouse_Position = i_tMouse_Position;
	on_mousemove(m_tMouse_Position);
}
void main::On_Mouse_Wheel(main::MOUSEBUTTON i_eWheel, int i_iWheel_Delta, const PAIR<unsigned int> & i_tMouse_Position)
{
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_wheel(i_eWheel,i_iWheel_Delta,m_tMouse_Position);
}
void main::On_Timer(unsigned int i_uiTimer_ID, const double & i_dDelta_Time_s)
{
	on_timer(i_uiTimer_ID,i_dDelta_Time_s);
}





void main::Draw(void)
{
	PAIR<unsigned int> tWindow_Size = m_tWindow_Box.Get_Size();
	double dWindow_AR = tWindow_Size.m_tX / (double)tWindow_Size.m_tY;
	double dScale(1.0 / tWindow_Size.m_tY);
	glViewport(0,0,(GLsizei) tWindow_Size.m_tX, (GLsizei) tWindow_Size.m_tY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	if (m_eDraw_Mode == ORTHO)
		glOrtho(0.0,dWindow_AR,0.0,1.0,1.0,-1.0);
//	else
//		glFrustum(0.0,dWindow_AR,0.0,1.0,0.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	std::map<int,unsigned int> cMap;
	for (std::vector<PANE>::const_iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end(); cIter++)
	{
		cMap[cIter->m_iLevel] = 0;
	}
	for (std::vector<PANE>::const_iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end(); cIter++)
	{
		(cMap[cIter->m_iLevel])++;
	}
	for (std::map<int,unsigned int>::const_iterator cLvlItr = cMap.begin(); cLvlItr != cMap.end(); cLvlItr++)
	{
		for (std::vector<PANE>::iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end(); cIter++)
		{
			if (cIter->m_bVisible && cLvlItr->first == cIter->m_iLevel)
			{
				glPushMatrix();
					cIter->Set_Clipping();
					// translate to window position
					glTranslated(cIter->m_qPosition.m_tBL.m_tX * dScale,cIter->m_qPosition.m_tBL.m_tY * dScale, 0.0);
					PAIR<unsigned int> tPane_Size = cIter->m_qPosition.Get_Size();
					//double dPane_AR = tPane_Size.m_tX / (double)tPane_Size.m_tY;
					double dPane_Scale_Y = (double)tPane_Size.m_tY / tWindow_Size.m_tY;
					//double dPane_Scale_X = dPane_AR / dWindow_AR;
					// scale to window: top right corner of pane is ((x/y),1)
					//glScaled(dPane_Scale_X, dPane_Scale_Y, 1.0);
					glScaled(dPane_Scale_Y, dPane_Scale_Y, 1.0);
						gfx_display(cIter->Get_ID()); // call user drawing routine for the pane
						if (m_bDraw_Pane_Grids)
							Draw_Pane_Grid(Get_Pane_Aspect_Ratio(cIter->Get_ID()));
					cIter->End_Clipping();
				glPopMatrix();
			}
		}
	}
	glFlush();

	if (g_cOGL_Screenshot.m_bRequest)
	{
		g_cOGL_Screenshot.m_bReady = false;
		if (g_cOGL_Screenshot.m_lpvData != nullptr)
		{
			if (g_cOGL_Screenshot.m_tWidth != tWindow_Size.m_tX ||
					g_cOGL_Screenshot.m_tHeight != tWindow_Size.m_tY ||
					g_cOGL_Screenshot.m_tColor_Depth_Bits != 24)
			{
				unsigned char * lpData = (unsigned char *)g_cOGL_Screenshot.m_lpvData;
				delete [] lpData;
				g_cOGL_Screenshot.m_lpvData = nullptr;
			}

		}
		g_cOGL_Screenshot.m_tWidth = (GLsizei) tWindow_Size.m_tX;
		g_cOGL_Screenshot.m_tHeight = (GLsizei) tWindow_Size.m_tY;
		g_cOGL_Screenshot.m_tColor_Depth_Bits = 24;
		if (g_cOGL_Screenshot.m_lpvData == nullptr)
			g_cOGL_Screenshot.m_lpvData = new unsigned char[g_cOGL_Screenshot.m_tWidth * g_cOGL_Screenshot.m_tWidth * 3];

		glReadPixels(0, 0, (GLsizei) tWindow_Size.m_tX, (GLsizei) tWindow_Size.m_tY, GL_RGB,	GL_UNSIGNED_BYTE, g_cOGL_Screenshot.m_lpvData);
		g_cOGL_Screenshot.m_bRequest = false;
		g_cOGL_Screenshot.m_bReady = true;
	}
	m_bDraw_Flag = false;
}


