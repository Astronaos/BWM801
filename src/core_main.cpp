
#include <bwm801.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bwm801_screenshot.h>
#include <map>
// This routine contains the common main methods that are OS independant

using namespace bwm801;

main * g_lpMain = nullptr;
void main::Initializer(void)
{
	m_bEngine_Debug_Mode = false;
	m_lpbMouse_Button_Status = nullptr;
	m_lpbKey_Status = nullptr;
	if (g_lpMain != nullptr)
	{
		fprintf(stderr,"More than one core main class has been instantiated. Exiting!\n");
		exit(0);
	}
	g_lpMain = this;
	m_bDraw_Flag = false;
	m_bQuit_Flag = false;
	m_eDraw_Mode = ortho;

	unsigned int uiNum_Mouse_Buttons = (unsigned int)(mb_max - mb_noop);
	m_lpbMouse_Button_Status = new bool [uiNum_Mouse_Buttons];
	memset(m_lpbMouse_Button_Status,0,sizeof(bool) * uiNum_Mouse_Buttons);

	unsigned int uiNum_Keys = (unsigned int)(key_max - key_noop);
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

void main::On_Key_Down(main::keyid eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State)
{
	m_lpbKey_Status[eKey_ID - key_noop] = true;
	on_key_down(eKey_ID, chScan_Code, uiRepeat_Count, bExtended_Key, bPrevious_Key_State);
	bool bCtrl = m_lpbKey_Status[key_lctrl - key_noop] || m_lpbKey_Status[key_rctrl - key_noop];
	bool bShift = m_lpbKey_Status[key_lshift - key_noop] || m_lpbKey_Status[key_rshift - key_noop];

	if (bCtrl && bShift && m_lpbKey_Status[key_f12 - key_noop])
	{
		m_bEngine_Debug_Mode = !m_bEngine_Debug_Mode;
		printf("Debugging mode %s\n",m_bEngine_Debug_Mode ? "Enabled" : "Disabled");
		if (!m_bEngine_Debug_Mode)
			m_bDraw_Pane_Grids = false;
	}
	else if (m_bEngine_Debug_Mode && (bCtrl && m_lpbKey_Status[key_f12 - key_noop]))
	{
		m_bDraw_Pane_Grids = !m_bDraw_Pane_Grids;
		printf("Grid mode %s\n",m_bDraw_Pane_Grids ? "Enabled" : "Disabled");
	}

}
void main::On_Key_Up(main::keyid eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State)
{
	m_lpbKey_Status[eKey_ID - key_noop] = false;
	on_key_up(eKey_ID, chScan_Code, uiRepeat_Count, bExtended_Key, bPrevious_Key_State);
}
void main::On_Mouse_Button_Double_Click(main::mousebutton i_eButton, const pair<unsigned int> & i_tMouse_Position)
{
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_button_double_click(i_eButton, m_tMouse_Position);
}
void main::On_Mouse_Button_Down(main::mousebutton i_eButton, const pair<unsigned int> & i_tMouse_Position)
{
	m_lpbMouse_Button_Status[i_eButton - key_noop] = true;
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_button_down(i_eButton, m_tMouse_Position);
}
void main::On_Mouse_Button_Up(main::mousebutton i_eButton, const pair<unsigned int> & i_tMouse_Position)
{
	m_lpbMouse_Button_Status[i_eButton - key_noop] = false;
//	m_tMouse_Position = i_tMouse_Position;
	on_mouse_button_up(i_eButton, m_tMouse_Position);
}
void main::On_Mousemove(const pair<unsigned int> & i_tMouse_Position)
{
//	printf("%i %i\n",i_tMouse_Position.m_tX,i_tMouse_Position.m_tY);
	m_tMouse_Position_Last = m_tMouse_Position;
	m_tMouse_Position = i_tMouse_Position;
	on_mousemove(m_tMouse_Position);
}
void main::On_Mouse_Wheel(main::mousebutton i_eWheel, int i_iWheel_Delta, const pair<unsigned int> & i_tMouse_Position)
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
	pair<unsigned int> tWindow_Size = m_tWindow_Box.Get_Size();
	double dWindow_AR = tWindow_Size.m_tX / (double)tWindow_Size.m_tY;
	double dScale(1.0 / tWindow_Size.m_tY);
	glViewport(0,0,(GLsizei) tWindow_Size.m_tX, (GLsizei) tWindow_Size.m_tY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	if (m_eDraw_Mode == ortho)
		glOrtho(0.0,dWindow_AR,0.0,1.0,1.0,-1.0);
//	else
//		glFrustum(0.0,dWindow_AR,0.0,1.0,0.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	std::map<int,unsigned int> cMap;
	for (std::vector<pane>::const_iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end(); cIter++)
	{
		cMap[cIter->Get_Level()] = 0;
	}
	for (std::vector<pane>::const_iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end(); cIter++)
	{
		(cMap[cIter->Get_Level()])++;
	}
	for (std::map<int,unsigned int>::const_iterator cLvlItr = cMap.begin(); cLvlItr != cMap.end(); cLvlItr++)
	{
		for (std::vector<pane>::iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end(); cIter++)
		{
			if (cIter->Is_Visible() && cLvlItr->first == cIter->Get_Level())
			{
				glPushMatrix();
					cIter->Set_Clipping();
					// translate to window position
					glTranslatef((GLfloat)(cIter->Get_Position().m_tBL.m_tX * dScale),(GLfloat)(cIter->Get_Position().m_tBL.m_tY * dScale), 0.0);
					pair<unsigned int> tPane_Size = cIter->Get_Position().Get_Size();
					//double dPane_AR = tPane_Size.m_tX / (double)tPane_Size.m_tY;
					double dPane_Scale_Y = (double)tPane_Size.m_tY / tWindow_Size.m_tY;
					//double dPane_Scale_X = dPane_AR / dWindow_AR;
					// scale to window: top right corner of pane is ((x/y),1)
					//glScaled(dPane_Scale_X, dPane_Scale_Y, 1.0);
					glScalef((GLfloat)dPane_Scale_Y, (GLfloat)dPane_Scale_Y, 1.0);
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

void main::Set_Screenshot_Save_Path(std::string i_szPath)\
{
	m_szScreenshot_Default_Path = i_szPath;
}
std::string main::Get_Screenshot_Save_Path(void)
{
	return m_szScreenshot_Default_Path;
}
void main::Set_Screenshot_Default_Filename(std::string i_szFilename)
{
	m_szScreenshot_Default_Filename = i_szFilename;
}
std::string main::Get_Screenshot_Default_Filename(void)
{
	return m_szScreenshot_Default_Filename;
}

void main::Set_Window_Name(const std::string &i_szName)
{
	m_szWindow_Name = i_szName;
}
std::string main::Get_Window_Name(void)
{
	return m_szWindow_Name;
}
double	main::Get_Frame_Rate(void) const
{
	return m_dFrame_Rate;
}
void	main::Set_Frame_Rate(const double & i_dFrame_Rate)
{
	m_dFrame_Rate = i_dFrame_Rate;
}

void main::Set_Draw_Mode(draw_mode i_eMode)
{
	m_eDraw_Mode = i_eMode;
}

pane_id main::Register_Pane(bool i_bVisible, unsigned int i_uiUser_ID)
{
	return Register_Pane(quad<unsigned int> (pair<unsigned int> (0,0), pair <unsigned int> (1,1)),i_bVisible,i_uiUser_ID);
}

pane_id main::Register_Pane(const quad <unsigned int> & i_qPosition, bool i_bVisible, unsigned int i_uiUser_ID)
{
	pane_id idID = m_tUser_Panes.size() + PANE_ID_PREFIX;
	m_tUser_Panes.push_back(pane(idID,i_qPosition,i_bVisible,i_uiUser_ID));
	return idID;
}
pane_id main::Find_Pane(const pair <unsigned int> & i_tLocation, bool i_bRequire_Pane_Visible) const
{
	pane_id idID = 0;
	std::map<int,unsigned int> cMap;
	for (std::vector<pane>::const_iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end(); cIter++)
	{
		cMap.insert(std::pair<int, unsigned int> (cIter->Get_Level(),0));
//			cMap[cIter->m_iLevel] = 0;
	}
	for (std::vector<pane>::const_iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end(); cIter++)
	{
		(cMap[cIter->Get_Level()])++;
	}
	for (std::map<int,unsigned int>::const_reverse_iterator cLvlItr = cMap.rbegin(); cLvlItr != cMap.rend(); cLvlItr++)
	{
		for (std::vector<pane>::const_iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end() && idID == 0; cIter++)
		{
			if (cLvlItr->first == cIter->Get_Level())
			{
				if ((!i_bRequire_Pane_Visible || cIter->Is_Visible()) && cIter->Is_In_Pane(i_tLocation))
					idID = cIter->Get_ID();
			}
		}
	}
	return idID;
}
void main::Move_Pane(pane_id i_idPane_ID, const quad <unsigned int> & i_qPosition)
{
	if (i_idPane_ID & PANE_ID_PREFIX)
	{
		unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
		m_tUser_Panes[uiIdx].Set_Position(i_qPosition);
	}
}
void main::Raise_Pane(pane_id i_idPane_ID)
{
	if (i_idPane_ID & PANE_ID_PREFIX)
	{
		unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
		m_tUser_Panes[uiIdx].Pull_To_Front();
	}
}
void main::Lower_Pane(pane_id i_idPane_ID)
{
	if (i_idPane_ID & PANE_ID_PREFIX)
	{
		unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
		m_tUser_Panes[uiIdx].Push_To_Back();
	}
}
void main::Change_Pane_Visibility(pane_id i_idPane_ID, bool i_bVisible)
{
	if (i_idPane_ID & PANE_ID_PREFIX)
	{
		unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
		m_tUser_Panes[uiIdx].Set_Visibility(i_bVisible);
	}
}
bool main::Get_Pane_Visibility(pane_id i_idPane_ID)
{
    bool bRet = false;
	if (i_idPane_ID & PANE_ID_PREFIX)
	{
		unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
		bRet = m_tUser_Panes[uiIdx].Is_Visible();
	}
    return bRet;
}
quad<unsigned int> main::Get_Pane_Position(pane_id i_idPane_ID)
{
	quad<unsigned int> cRet;
	if (i_idPane_ID & PANE_ID_PREFIX)
	{
		unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
		cRet = m_tUser_Panes[uiIdx].Get_Position();
	}
	return cRet;
}
unsigned int main::Get_Pane_User_ID(pane_id i_idPane_ID)
{
	unsigned int uiRet = -1;
	if (i_idPane_ID & PANE_ID_PREFIX)
	{
		unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
		uiRet = m_tUser_Panes[uiIdx].Get_User_ID();
	}
	return uiRet;
}
double	main::Get_Pane_Aspect_Ratio(pane_id i_idPane_ID)
{
	double	dRet = 1.0;
	if (i_idPane_ID & PANE_ID_PREFIX)
	{
		quad<unsigned int> cRet;
		unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
		pair<unsigned int> tSize = m_tUser_Panes[uiIdx].Get_Position().Get_Size();
		dRet = tSize.m_tX / (double)tSize.m_tY;
	}
	return dRet;
}
void main::On_Window_Move(const pair<unsigned int> & i_tWindow_Position)
{
	//printf("move %i %i\n",i_tWindow_Position.m_tX,i_tWindow_Position.m_tY);
	pair<unsigned int> tWindow_Size = m_tWindow_Box.m_tTR - m_tWindow_Box.m_tBL;
	//printf("move %i %i\n",tWindow_Size.m_tX,tWindow_Size.m_tY);
	m_tWindow_Box.m_tBL = i_tWindow_Position;
	m_tWindow_Box.m_tTR = i_tWindow_Position + tWindow_Size;
}
void main::On_Window_Resize(const pair<unsigned int> & i_tWindow_Size)
{
	//printf("w resize %i %i\n",i_tWindow_Size.m_tX,i_tWindow_Size.m_tY);
	m_tWindow_Box.m_tTR = m_tWindow_Box.m_tBL + i_tWindow_Size;
	//printf("w resize %i %i\n",m_tWindow_Box.m_tTR.m_tX,m_tWindow_Box.m_tTR.m_tY);
}
void main::Gain_Focus(void)
{
	m_bWindow_Has_Focus = true;
}
void main::Lose_Focus(void)
{
	m_bWindow_Has_Focus = false;
}

bool	main::Get_Mouse_Button(mousebutton i_eButton) const // get status of mouse button
{
	bool bRet = false;
	if (i_eButton > mb_noop && i_eButton < mb_max)
		bRet = m_lpbMouse_Button_Status[i_eButton - mb_noop];

	return bRet;
}
bool	main::Get_Key_State(keyid i_eKey) const // get status of key
{
	bool bRet = false;
	if (i_eKey > key_noop && i_eKey < key_max)
		bRet = m_lpbKey_Status[i_eKey - key_noop];

	return bRet;
}
pair<unsigned int>	main::Get_Previous_Mouse_Position(void) const
{
	return m_tMouse_Position_Last;
}
pair<unsigned int>	main::Get_Mouse_Position(void) const
{
	return m_tMouse_Position;
}
pair<unsigned int>	main::Get_Window_Size(void) const
{
	pair<unsigned int> tWindow_Size = m_tWindow_Box.m_tTR - m_tWindow_Box.m_tBL;
	return tWindow_Size;
}
quad<unsigned int> main::Get_Window_Quad(void) const
{
	return m_tWindow_Box;
}

void main::Request_Refresh(void)
{
	m_bDraw_Flag = true;
}
void main::Request_Quit(void)
{
	m_bQuit_Flag = true;
}

bool			main::Pending_Quit(void) const
{
	return m_bQuit_Flag;
}
bool			main::Pending_Draw(void) const
{
	return m_bDraw_Flag;
}




