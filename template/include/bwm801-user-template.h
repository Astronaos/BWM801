#pragma once

#include <bwm801.h>
#include <thread>
#include <deque>

enum button_id
{
	error_ack,
	quit_request
};

using namespace bwm801;

class EXPORT user_template_main : public bwm801::main
{
private:
	pane_id	m_idPane;
	pane_id m_idError_Pane;


	bool			m_bQuit_Request_Pending;



	double				m_dTimer;
	std::deque<button_id> m_qEvent_List;

	texture		m_cTexture;


public:
	user_template_main(void){};
private:
	virtual void on_key_down(keyid eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State);
	virtual void on_key_up(keyid eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State);
	virtual void on_mouse_button_double_click(mousebutton i_eButton, const pair<unsigned int> &i_tMouse_Position);
	virtual void on_mouse_button_down(mousebutton i_eButton, const pair<unsigned int> &i_tMouse_Position);
	virtual void on_mouse_button_up(mousebutton i_eButton, const pair<unsigned int> &i_tMouse_Position);
	virtual void on_mousemove(const pair<unsigned int> &i_tMouse_Position);
	virtual void on_mouse_wheel(mousebutton i_eWheel, int i_iWheel_Delta, const pair<unsigned int> &i_tMouse_Position);
	virtual void on_timer(unsigned int i_uiTimer_ID, const double & i_dDelta_Time_s);

	virtual void init(void); // initialization routine; rendering context not created
	virtual void gfx_display(pane_id i_idPane); // primary display routine
	virtual void gfx_init(void); // initialization routine; rendering context already created
	virtual void gfx_reshape(const pair<unsigned int> & i_tNew_Size); // window size change
	virtual void gfx_close(void); // graphics exiting; rendering context still active
	virtual void close(void); // program exiting; rendering context destroyed
};

// for Windows compilation, export the main class
#ifdef WIN32
	__declspec(dllexport) extern user_template_main g_cMain;
#endif



