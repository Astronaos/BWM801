#include <bwm801.h>
#include <bwm801_main_data.h>
#include <cstring> // for memcpy
using namespace bwm801;

extern main * g_lpMain;

void main_data::Copy(const main_data & i_cRHO)
{
	m_bEngine_Debug_Mode = i_cRHO.m_bEngine_Debug_Mode;
	m_vszCommand_Line_Parameters = i_cRHO.m_vszCommand_Line_Parameters;
	m_szWindow_Name = i_cRHO.m_szWindow_Name;
	m_tMouse_Position = i_cRHO.m_tMouse_Position;
	m_tMouse_Position_Last = i_cRHO.m_tMouse_Position_Last;

	m_bDraw_Flag = i_cRHO.m_bDraw_Flag;
	m_bQuit_Flag = i_cRHO.m_bQuit_Flag;
	m_eDraw_Mode = i_cRHO.m_eDraw_Mode;

	m_bWindow_Has_Focus = i_cRHO.m_bWindow_Has_Focus;
	m_tWindow_Box = i_cRHO.m_tWindow_Box;
	m_tUser_Panes = i_cRHO.m_tUser_Panes;

	m_dFrame_Rate = i_cRHO.m_dFrame_Rate;

	m_bDraw_Pane_Grids = i_cRHO.m_bDraw_Pane_Grids;

	m_szScreenshot_Default_Filename = i_cRHO.m_szScreenshot_Default_Filename;
	m_szScreenshot_Default_Path = i_cRHO.m_szScreenshot_Default_Path;

	unsigned int uiNum_Mouse_Buttons = (unsigned int)(main::mb_max - main::mb_noop);
	memcpy(m_lpbMouse_Button_Status, i_cRHO.m_lpbMouse_Button_Status, sizeof(bool) * uiNum_Mouse_Buttons);

	unsigned int uiNum_Keys = (unsigned int)(main::key_max - main::key_noop);
	memcpy(m_lpbKey_Status, i_cRHO.m_lpbKey_Status, sizeof(bool) * uiNum_Keys);
}

main_data::main_data(main * i_lpMain, const main_data * i_cRHO)
{
	m_bEngine_Debug_Mode = false;
	m_lpbMouse_Button_Status = nullptr;
	m_lpbKey_Status = nullptr;
	if (g_lpMain != nullptr)
	{
		fprintf(stderr, "More than one core main class has been instantiated. Exiting!\n");
		exit(0);
	}
	g_lpMain = i_lpMain;
	m_bDraw_Flag = false;
	m_bQuit_Flag = false;
	m_eDraw_Mode = main::ortho;

	unsigned int uiNum_Mouse_Buttons = (unsigned int)(main::mb_max - main::mb_noop);
	m_lpbMouse_Button_Status = new bool[uiNum_Mouse_Buttons];
	memset(m_lpbMouse_Button_Status, 0, sizeof(bool) * uiNum_Mouse_Buttons);

	unsigned int uiNum_Keys = (unsigned int)(main::key_max - main::key_noop);
	m_lpbKey_Status = new bool[uiNum_Keys];
	memset(m_lpbKey_Status, 0, sizeof(bool) * uiNum_Keys);

	m_bDraw_Pane_Grids = false;

	m_szScreenshot_Default_Filename = "BWM801-screenshot";

	if (i_cRHO != nullptr)
		Copy(*i_cRHO);

}


main_data::~main_data(void)
{
	delete[] m_lpbMouse_Button_Status;
	m_lpbMouse_Button_Status = nullptr;
	delete[] m_lpbKey_Status;
	m_lpbKey_Status = nullptr;
	g_lpMain = nullptr;
	m_szScreenshot_Default_Filename.clear();
}
