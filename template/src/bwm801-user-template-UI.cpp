#include <bwm801-user-template.h>
#include <cfloat>
#include <ctime>

using namespace bwm801;

#ifndef WIN32
#define ATTRUNUSED __attribute__((unused))
#else
#define ATTRUNUSED
#endif

void user_template_main::on_key_down(keyid eKey_ID, unsigned char chScan_Code ATTRUNUSED, unsigned int uiRepeat_Count ATTRUNUSED, bool bExtended_Key ATTRUNUSED, bool bPrevious_Key_State ATTRUNUSED)
{
	switch (eKey_ID)
	{
	default: break;
	case key_escape:
	case key_q: // handle a request to exit
		m_qEvent_List.push_back(quit_request);
		break;
	case key_p: // produce a screenshot when the user selects ctrl-p
		if (Get_Key_State(key_lctrl) || Get_Key_State(key_rctrl) || Get_Key_State(key_control))
			Request_Screenshot(std::string());
		break;
	case key_prscrn: // produce a screenshow when the user selected the prt sc button
		Request_Screenshot(std::string());
		break;
	}
}

void user_template_main::on_key_up(keyid eKey_ID ATTRUNUSED, unsigned char chScan_Code ATTRUNUSED, unsigned int uiRepeat_Count ATTRUNUSED, bool bExtended_Key ATTRUNUSED, bool bPrevious_Key_State ATTRUNUSED)
{ // handle any key releases
}
void user_template_main::on_mouse_button_double_click(mousebutton i_eButton ATTRUNUSED, const pair<unsigned int> &i_tMouse_Position ATTRUNUSED)
{ // handle double click
}
void user_template_main::on_mouse_button_down(mousebutton i_eButton ATTRUNUSED, const pair<unsigned int> &i_tMouse_Position ATTRUNUSED)
{ // handle clicking on something
/*	pair<float> tMouse = i_tMouse_Position;
	pane_id idMouse_Pane = Find_Pane(tMouse);
	quad<unsigned int> qPane_Position = Get_Pane_Position(idMouse_Pane);
	pair<unsigned int> tMouse_TR = tMouse - qPane_Position.m_tTR;
	tMouse -= qPane_Position.m_tBL;
	float dPane_Scale = (float)(1.0 / qPane_Position.Get_Size().m_tY);
	pair<float> tMouse_Scaled = tMouse * dPane_Scale;
	bool bProcessed = false;
	if (idMouse_Pane == m_idError_Pane)
	{
//		for (std::map<button_id,BUTTON_INFO>::iterator cI = m_mError_Pane_Buttons.begin(); cI != m_mError_Pane_Buttons.end() && !bProcessed; cI++)
//		{
//			if (cI->second.TestHit(tMouse_Scaled))
//			{
//				m_qEvent_List.push_back((button_id)(cI->second.GetID()));
//				bProcessed = true;
//			}
//		}
	}
	else //if (idMouse_Pane == m_idPane)
	{
//		for (std::map<button_id,BUTTON_INFO>::iterator cI = m_mMain_Pane_Buttons.begin(); cI != m_mMain_Pane_Buttons.end() && !bProcessed; cI++)
//		{
//			if (cI->second.TestHit(tMouse_Scaled))
//			{
//				m_qEvent_List.push_back((button_id)(cI->second.GetID()));
//				bProcessed = true;
//			}
//		}
	}*/
}
void user_template_main::on_mouse_button_up(mousebutton i_eButton ATTRUNUSED, const pair<unsigned int> &i_tMouse_Position ATTRUNUSED)
{ // handle mouse button release
}
void user_template_main::on_mousemove(const pair<unsigned int> &i_tMouse_Position ATTRUNUSED)
{ // handle moving the mouse
}
void user_template_main::on_mouse_wheel(mousebutton i_eWheel ATTRUNUSED, int i_iWheel_Delta ATTRUNUSED, const pair<unsigned int> &i_tMouse_Position ATTRUNUSED)
{ // handle using the mouse wheel (scroll wheel)
}
void user_template_main::on_timer(unsigned int i_uiTimer_ID ATTRUNUSED, const double & i_dDelta_Time_s)
{ // handle real-time processing
//	if (!m_vsError_Info.empty())
//	{
//		Change_Pane_Visibility(m_idError_Pane,true);
//	}
	while (!m_qEvent_List.empty())
	{
		button_id eEvent = m_qEvent_List.front();
		m_qEvent_List.pop_front();
		switch (eEvent)
		{
		default: break;
		case error_ack:
//			m_vsError_Info.clear();
//			Change_Pane_Visibility(m_idError_Pane,false);
			Request_Refresh();
			break;
		case quit_request:
			m_bQuit_Request_Pending = true;
			break;
		}
	}

	if (m_bQuit_Request_Pending)
	{
		Request_Quit(); // the if user requested an exit, pass that on to bwm801::main
	}
	m_dTimer += i_dDelta_Time_s; // internal timer
	Request_Refresh(); // reqest a refresh

}


void user_template_main::on_joystick_move(unsigned int i_uiJoystick_ID ATTRUNUSED, bwm801::main::joystick_axis i_eAxis ATTRUNUSED, int i_iPosition ATTRUNUSED)
{
}
void user_template_main::on_joystick_button(unsigned int i_uiJoystick_ID ATTRUNUSED, bwm801::main::joystick_button i_eButton ATTRUNUSED, bool i_bState ATTRUNUSED)
{
}
void user_template_main::on_gamepad_move(unsigned int i_uiGamepad_ID ATTRUNUSED, bwm801::main::gamepad_axis i_eAxis ATTRUNUSED, int i_iPosition ATTRUNUSED)
{
}
void user_template_main::on_gamepad_button(unsigned int i_uiGamepad_ID ATTRUNUSED, bwm801::main::gamepad_button i_eButton ATTRUNUSED, bool i_bState ATTRUNUSED)
{
}


#undef ATTRUNUSED
