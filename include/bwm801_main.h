#pragma once
#include <vector>
#include <string>

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif


namespace bwm801
{
	class EXPORT main
	{
	private:
		void * m_lpvData;
	public:
		enum draw_mode { ortho, projection };
	public:
		virtual void Set_Screenshot_Save_Path(std::string i_szPath);
		virtual std::string Get_Screenshot_Save_Path(void);
		virtual void Set_Screenshot_Default_Filename(std::string i_szFilename);
		virtual std::string Get_Screenshot_Default_Filename(void);

		virtual void Initializer(void);
		virtual void Initializer(const main &);
		virtual void Destructor(void);

		virtual bool Is_Debug_Mode_Enabled(void) const;
		virtual void Set_Debug_Mode(bool i_bDebug_Enabled);

		main(void) { m_lpvData = nullptr; Initializer(); }
		main(const main & i_cRHO) { m_lpvData = nullptr; Initializer(i_cRHO); }
		~main(void) { Destructor(); }

		virtual void Process_Command_Line(unsigned int i_uiNum_Parameters, const char * i_lpszParameter_Values[]);
		virtual void Set_Window_Name(const std::string &i_szName);
		virtual std::string Get_Window_Name(void);
		virtual double	Get_Frame_Rate(void) const;
		virtual void	Set_Frame_Rate(const double & i_dFrame_Rate);

		virtual void Set_Draw_Mode(draw_mode i_eMode);
		virtual pane_id Register_Pane(bool i_bVisible, unsigned int i_uiUser_ID = -1);
		virtual pane_id Register_Pane(const quad <unsigned int> & i_qPosition, bool i_bVisible, unsigned int i_uiUser_ID = -1);
		virtual pane_id Find_Pane(const pair <unsigned int> & i_tLocation, bool i_bRequire_Pane_Visible = true) const;
		virtual void Move_Pane(pane_id i_idPane_ID, const quad <unsigned int> & i_qPosition);
		virtual void Raise_Pane(pane_id i_idPane_ID);
		virtual void Lower_Pane(pane_id i_idPane_ID);
		virtual void Change_Pane_Visibility(pane_id i_idPane_ID, bool i_bVisible);
		virtual bool Get_Pane_Visibility(pane_id i_idPane_ID);
		virtual quad<unsigned int> Get_Pane_Position(pane_id i_idPane_ID);
		virtual unsigned int Get_Pane_User_ID(pane_id i_idPane_ID);
		virtual double	Get_Pane_Aspect_Ratio(pane_id i_idPane_ID);
		enum keyid
			{
				key_noop, 
				key_oem, // reserved for oem specified keys; use scancode to identify
				key_f1, key_f2, key_f3, key_f4, key_f5, key_f6, key_f7, key_f8, key_f9, key_f10, key_f11, key_f12,
				key_f13, key_f14, key_f15, key_f16, key_f17, key_f18, key_f19, key_f20, key_f21, key_f22, key_f23, key_f24,
				key_shift,key_control,key_alt,
				key_escape, key_tab, key_lshift, key_lctrl, key_lalt, key_rshift, key_rctrl, key_ralt, key_lwin, key_rwin, 
				key_capslock, key_scrolllock, key_prscrn, key_pause, key_insert, key_delete, key_home, key_end, key_pgup, key_pgdn, key_backspace,
				key_up, key_down, key_left, key_right,
				key_nmlck, key_numpd_div, key_numpd_times, key_numpd_minus, key_numpd_plus, key_numpd_enter,
				key_numpd_dot, key_numpd_0, key_numpd_1, key_numpd_2, key_numpd_3, key_numpd_4, key_numpd_5,
				key_numpd_6, key_numpd_7, key_numpd_8, key_numpd_9,
				key_enter, key_space, key_apps,
				key_tilde, key_0, key_1, key_2, key_3, key_4, key_5, key_6, key_7, key_8, key_9,
				key_minus, key_equals, key_backslash, 
				key_a, key_b, key_c, key_d, key_e, key_f, key_g, key_h, key_i, key_j, key_k, key_l, key_m,
				key_n, key_o, key_p, key_q, key_r, key_s, key_t, key_u, key_v, key_w, key_x, key_y, key_z,
				key_lsqbrkt,key_rsqbrkt, key_semicolon, key_squote, key_comma, key_period, key_slash,
				key_volup, key_voldn, key_volmute, 
				key_brsr_back, key_brsr_fav, key_brsr_fwd, key_brsr_refresh, key_brsr_search, key_brsr_stop,key_brsr_home,
				key_media_next, key_media_prev, key_media_stop, key_media_play, key_media_select, 
				key_email, 
				key_app_1, key_app_2,
				key_sleep, key_wake, key_power, key_help, key_select, key_exec, 
				key_print,
				key_convert, key_nonconvert, key_accept, key_modechange, key_clear,
				key_kana, key_hangul, key_junja, key_final, key_hanja, key_kanji,
				key_mb_left, key_mb_ctr, key_mb_rgt, key_mb_scroll_v, key_mb_scroll_h, key_mb_x1, key_mb_x2,
				key_cancel,
				key_attn, key_crsel, key_exsel, key_ereof, key_play, key_zoom, key_noname, key_pa1,
				key_max // not a key - used internally
			};
		enum mousebutton {mb_noop, mb_left, mb_ctr, mb_rgt, mb_scroll_v, mb_scroll_h, mb_x1, mb_x2, mb_max};// mb_max not a key - used internally
		// 

		// Member functions with capitalized names are called by the common / top level library routines.  Users should not call these functions
		virtual void On_Key_Down(keyid eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State);
		virtual void On_Key_Up(keyid eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State);
		virtual void On_Mouse_Button_Double_Click(mousebutton i_eButton, const pair<unsigned int> & i_tMouse_Position);
		virtual void On_Mouse_Button_Down(mousebutton i_eButton, const pair<unsigned int> &i_tMouse_Position);
		virtual void On_Mouse_Button_Up(mousebutton i_eButton, const pair<unsigned int> &i_tMouse_Position);
		virtual void On_Mousemove(const pair<unsigned int> & i_tMouse_Position);
		virtual void On_Mouse_Wheel(mousebutton i_eWheel, int i_iWheel_Delta, const pair<unsigned int> & i_tMouse_Position);
		virtual void On_Timer(unsigned int i_uiTimer_ID, const double & i_dDelta_Time_s);
		virtual void On_Window_Move(const pair<unsigned int> & i_tWindow_Position);
		virtual void On_Window_Resize(const pair<unsigned int> & i_tWindow_Size);
		virtual void Gain_Focus(void);
		virtual void Lose_Focus(void);

		// Member functions with lower case names are defined by the user. 
		virtual void on_key_down(keyid eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State) = 0;
		virtual void on_key_up(keyid eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State) = 0;
		virtual void on_mouse_button_double_click(mousebutton i_eButton, const pair<unsigned int> &i_tMouse_Position) = 0;
		virtual void on_mouse_button_down(mousebutton i_eButton, const pair<unsigned int> &i_tMouse_Position) = 0;
		virtual void on_mouse_button_up(mousebutton i_eButton, const pair<unsigned int> &i_tMouse_Position) = 0;
		virtual void on_mousemove(const pair<unsigned int> &i_tMouse_Position) = 0;
		virtual void on_mouse_wheel(mousebutton i_eWheel, int i_iWheel_Delta, const pair<unsigned int> &i_tMouse_Position) = 0;
		virtual void on_timer(unsigned int i_uiTimer_ID, const double & i_dDelta_Time_s) = 0;

		virtual void init(void) = 0; // initialization routine; rendering context not created
		virtual void gfx_display(pane_id i_idPane) = 0; // primary display routine
		virtual void gfx_init(void) = 0; // initialization routine; rendering context already created
		virtual void gfx_reshape(const pair<unsigned int> & i_tNew_Size) = 0; // window size change
		virtual void gfx_close(void) = 0; // graphics exiting; rendering context still active
		virtual void close(void) = 0; // program exiting; rendering context destroyed

		virtual bool	Get_Mouse_Button(mousebutton i_eButton) const; // get status of mouse button
		virtual bool	Get_Key_State(keyid i_eKey) const; // get status of key
		virtual pair<unsigned int>	Get_Previous_Mouse_Position(void) const;
		virtual pair<unsigned int>	Get_Mouse_Position(void) const;
		virtual pair<unsigned int>	Get_Window_Size(void) const;
		virtual quad<unsigned int> Get_Window_Quad(void) const;

		virtual unsigned int	Request_Timer(const double & i_dDelta_Time); // request a timer call into on_timer with the specified delta t between calls; return value is timer ID
		virtual void 			Request_Refresh(void);
		virtual void 			Request_Quit(void);

		virtual void 			Draw(void);
		virtual bool			Pending_Quit(void) const;
		virtual bool			Pending_Draw(void) const;

		virtual std::vector<std::string> Get_Directory_File_List(const std::string &i_szDirectory); /// get list of all files in the given direcctory
		virtual void Draw_Pane_Grid(const float &i_dXmax);
		virtual void Request_Screenshot(const std::string & i_szFilename);

	};
};
extern "C" 
{
	EXPORT extern bwm801::main * g_lpMain;
}

#undef EXPORT
