#pragma once
#include <bwm801.h>
#include <vector>
#include <string>

namespace bwm801
{
	class main_data
	{
	private:
		void Copy(const main_data & i_cRHO);

	public:
		bool 					m_bEngine_Debug_Mode;
		std::vector<std::string> m_vszCommand_Line_Parameters;
		std::string				m_szWindow_Name;
		bool *					m_lpbMouse_Button_Status;
		bool *					m_lpbKey_Status;
		pair<unsigned int>		m_tMouse_Position;
		pair<unsigned int>		m_tMouse_Position_Last;

		bool					m_bDraw_Flag;
		bool					m_bQuit_Flag;
		main::draw_mode			m_eDraw_Mode;

		bool					m_bWindow_Has_Focus;
		quad<unsigned int> 		m_tWindow_Box;
		std::vector< pane > 	m_tUser_Panes;

		double					m_dFrame_Rate;

		bool 					m_bDraw_Pane_Grids;

		std::string 			m_szScreenshot_Default_Filename;
		std::string 			m_szScreenshot_Default_Path;

		main_data(main * i_lpMain, const main_data * i_cRHO = nullptr);
		~main_data(void);
	};
}
