
#include <core.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
// This routine contains the common MAIN methods that are OS independant


MAIN * g_lpMain = NULL;
MAIN::MAIN(void)
{
	m_lpbMouse_Button_Status = NULL;
	m_lpbKey_Status = NULL;
	if (g_lpMain != NULL)
	{
		fprintf(stderr,"More than one core MAIN class has been instantiated. Exiting!\n");
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

}

void MAIN::On_Key_Down(MAIN::KEYID eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State)
{
	m_lpbKey_Status[eKey_ID - KEY_NOOP] = true;
	on_key_down(eKey_ID, chScan_Code, uiRepeat_Count, bExtended_Key, bPrevious_Key_State);
}
void MAIN::On_Key_Up(MAIN::KEYID eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State)
{
	m_lpbKey_Status[eKey_ID - KEY_NOOP] = false;
	on_key_up(eKey_ID, chScan_Code, uiRepeat_Count, bExtended_Key, bPrevious_Key_State);
}
void MAIN::On_Mouse_Button_Double_Click(MAIN::MOUSEBUTTON i_eButton, const PAIR<unsigned int> & i_tMouse_Position)
{
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_button_double_click(i_eButton, m_tMouse_Position);
}
void MAIN::On_Mouse_Button_Down(MAIN::MOUSEBUTTON i_eButton, const PAIR<unsigned int> & i_tMouse_Position)
{
	m_lpbMouse_Button_Status[i_eButton - KEY_NOOP] = true;
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_button_down(i_eButton, m_tMouse_Position);
}
void MAIN::On_Mouse_Button_Up(MAIN::MOUSEBUTTON i_eButton, const PAIR<unsigned int> & i_tMouse_Position)
{
	m_lpbMouse_Button_Status[i_eButton - KEY_NOOP] = false;
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_button_up(i_eButton, m_tMouse_Position);
}
void MAIN::On_Mousemove(const PAIR<unsigned int> & i_tMouse_Position)
{
//	printf("%i %i\n",i_tMouse_Position.m_tX,i_tMouse_Position.m_tY);
	m_tMouse_Position_Last = m_tMouse_Position;
	m_tMouse_Position = i_tMouse_Position;
	on_mousemove(m_tMouse_Position);
}
void MAIN::On_Mouse_Wheel(MAIN::MOUSEBUTTON i_eWheel, int i_iWheel_Delta, const PAIR<unsigned int> & i_tMouse_Position)
{
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_wheel(i_eWheel,i_iWheel_Delta,m_tMouse_Position);
}
void MAIN::On_Timer(unsigned int i_uiTimer_ID, const double & i_dDelta_Time_s)
{
	on_timer(i_uiTimer_ID,i_dDelta_Time_s);
}
void MAIN::Draw(void)
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
					cIter->End_Clipping();
				glPopMatrix();
			}
		}
	}
	glFlush();

	m_bDraw_Flag = false;
}


