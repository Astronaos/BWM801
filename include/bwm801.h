#pragma once
//#include	<cmath>
//#include <vector>
//#include <deque>
//#include <map>
//#include <iostream>
//#include <cstdio>
//#if defined (__MINGW32__) || defined (_WIN32)
//#include <windows.h>
//#include <windowsx.h>
//#endif
//#ifdef _WIN32
//	double nan(const char * i_lpszFlags);//{return NAN;}
//	bool isnan(const double & i_dValue);//{return _isnan(i_dValue);}
//#elif !defined __MINGW32__
//	#include <unistd.h>
//	void Sleep(unsigned int i_uiLength);
//#endif
#include<vector>
#if defined (__MINGW32__) || defined (_WIN32)
#include <windows.h>
#include <windowsx.h>
#endif
#include <GL/gl.h>

#ifndef EXPORT
#define EXPORT
#endif

#include <bwm801_pair.h>
#include <bwm801_triplet.h>
#include <bwm801_quad.h>


namespace bwm801
{

	// graphics text operations
	enum FONT{SANS,SERIF,MONO,USER_1,USER_2,USER_3,USER_4,USER_5,USER_6,USER_7,USER_8};
	typedef void (*draw_callback)(void * io_lpvDraw_Info);
	extern "C" {

		EXPORT extern void LoadFontFace(FONT i_eFont, const char * i_lpszPath_To_Font, bool i_bBold, bool i_bItalics);
		EXPORT extern void SelectFontFace(FONT i_eFont, bool i_bBold, bool i_bItalics);
		EXPORT extern void glPrint(const float & i_dSize, const float & i_dX, const float & i_dY, const float & i_dZ, const char * i_lpszString);
		EXPORT extern void TextBBox(const float & i_dSize, const char * i_lpszString, pair<float> & o_cBot_Left, pair<float> & o_cTop_Right);
		enum horiz_justification { hj_left, hj_center, hj_right };
		enum vert_justification { vj_top, vj_middle, vj_bottom };
		EXPORT extern void glPrintJustified(const float & i_dSize, const float & i_dX, const float & i_dY, const float & i_dZ, const char * i_lpszString, horiz_justification i_eHorizontal_Justification, vert_justification i_eVertical_Justification);

		EXPORT extern void GenerateEllipse(const float & i_dRx, const float & i_dRy, std::vector<pair<float> > &o_vpResult, const unsigned int i_uiNum_Points = 64);
		EXPORT extern void GenerateArc(const float & i_dRx, const float & i_dRy, const float & i_dStart_Angle_Deg, const float & i_dEnd_Angle_Deg, std::vector<pair<float> > &o_vpResult, const unsigned int i_uiNum_Points = 64);
		EXPORT extern void InvertList(std::vector <pair<float> > & io_vpVertex_List);
		EXPORT extern void glVertexList(const std::vector <pair<float> > & i_vpVertex_List);
		EXPORT extern void Draw_Pushbutton(bool i_bState, draw_callback i_vDrawing_Fn, void * i_lpvDraw_Info);

		// common functions for drawing buttons
		//EXPORT extern std::vector<pair<float> > g_vEllipse;

		enum sb_direction { sbd_up, sbd_down, sbd_left, sbd_right };
		EXPORT extern void Initialize_Circle_Vectors(void);
		EXPORT extern void Draw_Rounded_Rectangle(bool i_bFilled);
		EXPORT extern void Draw_Rounded_Rectangle_Button(bool i_bPressed, bool i_bActive);
		EXPORT extern void Draw_Scroll_Button(sb_direction i_eDirection, bool i_bActive);
		EXPORT extern void Draw_Simple_Button(bool bStatus);


		EXPORT extern void glGenerateMipmap(GLenum target);
		EXPORT extern void glGenerateTextureMipmap(GLuint texture);
	}
}

#include <bwm801_pane.h>
#include <bwm801_texture.h>
#include <bwm801_comm.h>
#include <bwm801_main.h>
#include <bwm801_button_info.h>
#include <bwm801_datetime.h>
#include <bwm801_critsec.h>
#include <bwm801_mapspace.h>
#include <bwm801_calllist.h>
//#include <bwm801_mapbase.h>
//#include <bwm801_hexmap.h>
//#include <bwm801_isohexmap.h>


