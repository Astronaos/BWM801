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
#include <GL/gl.h>

#include <bwm801_pair.h>
#include <bwm801_triplet.h>
#include <bwm801_quad.h>

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace bwm801
{

	// graphics text operations
	enum FONT{SANS,SERIF,MONO,USER_1,USER_2,USER_3,USER_4,USER_5,USER_6,USER_7,USER_8};
	typedef void (*draw_callback)(void * io_lpvDraw_Info);

	EXPORT void LoadFontFace(FONT i_eFont, const char * i_lpszPath_To_Font, bool i_bBold, bool i_bItalics);
	EXPORT void SelectFontFace(FONT i_eFont, bool i_bBold, bool i_bItalics);
	EXPORT void glPrint(const double & i_dSize, const double & i_dX, const double & i_dY, const double & i_dZ, const char * i_lpszString);
	EXPORT void TextBBox(const double & i_dSize, const char * i_lpszString, pair<float> & o_cBot_Left, pair<float> & o_cTop_Right);
	enum horiz_justification {hj_left,hj_center,hj_right};
	enum vert_justification {vj_top,vj_middle,vj_bottom};
	EXPORT void glPrintJustified(const double & i_dSize, const double & i_dX, const double & i_dY, const double & i_dZ, const char * i_lpszString,horiz_justification i_eHorizontal_Justification, vert_justification i_eVertical_Justification);

	EXPORT void GenerateEllipse(const double & i_dRx, const double & i_dRy, std::vector<pair<float> > &o_vpResult, const unsigned int i_uiNum_Points = 64);
	EXPORT void GenerateArc(const double & i_dRx, const double & i_dRy, const double & i_dStart_Angle_Deg, const double & i_dEnd_Angle_Deg, std::vector<pair<float> > &o_vpResult, const unsigned int i_uiNum_Points = 64);
	EXPORT void InvertList(std::vector <pair<float> > & io_vpVertex_List);
	EXPORT void glVertexList(const std::vector <pair<float> > & i_vpVertex_List);
	EXPORT void Draw_Pushbutton(bool i_bState, draw_callback i_vDrawing_Fn, void * i_lpvDraw_Info);

	EXPORT void glLineWidth(const double &i_dLW);

	// common functions for drawing buttons
	EXPORT extern std::vector<pair<float> > g_vEllipse;

	enum sb_direction {sbd_up,sbd_down,sbd_left,sbd_right};
	EXPORT void Initialize_Circle_Vectors(void);
	EXPORT void Draw_Rounded_Rectangle(bool i_bFilled);
	EXPORT void Draw_Rounded_Rectangle_Button(bool i_bPressed, bool i_bActive);
	EXPORT void Draw_Scroll_Button(sb_direction i_eDirection, bool i_bActive);
	EXPORT void Draw_Simple_Button(bool bStatus);


	EXPORT void glGenerateMipmap(GLenum target);
	EXPORT void glGenerateTextureMipmap(GLuint texture);
};

#undef EXPORT

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


