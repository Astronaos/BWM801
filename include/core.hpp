#pragma once
#include	<cmath>
#include <vector>
#include <deque>
#include <map>
#include <iostream>
//#include <cstdio>
#if defined (__MINGW32__) || defined (_WIN32)
#include <windows.h>
#include <windowsx.h>
#endif
#ifdef _WIN32
//	double nan(const char * i_lpszFlags);//{return NAN;}
//	bool isnan(const double & i_dValue);//{return _isnan(i_dValue);}
#elif !defined __MINGW32__
	#include <unistd.h>
	void Sleep(unsigned int i_uiLength);
#endif
#include <GL/gl.h>
//#include <cstdio>
template <typename T> class PAIR
{
public:
	T m_tX;
	T m_tY;

	PAIR(void)
	{
		m_tX = 0;
		m_tY = 0;
	}
	PAIR(T i_tX, T i_tY)
	{
		m_tX = i_tX;
		m_tY = i_tY;
	}
	PAIR operator +(const PAIR & i_cRHO) const
	{
		return PAIR(m_tX + i_cRHO.m_tX,m_tY + i_cRHO.m_tY);
	}
	PAIR & operator +=(const PAIR & i_cRHO)
	{
		m_tX += i_cRHO.m_tX;
		m_tY += i_cRHO.m_tY;
		return *this;
	}
	PAIR operator -(const PAIR & i_cRHO) const
	{
		return PAIR(m_tX - i_cRHO.m_tX,m_tY - i_cRHO.m_tY);
	}
	PAIR & operator -=(const PAIR & i_cRHO)
	{
		m_tX -= i_cRHO.m_tX;
		m_tY -= i_cRHO.m_tY;
		return *this;
	}
	bool operator < (const PAIR & i_cRHO) const
	{
		return (m_tX < i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY < i_cRHO.m_tY));
	}
	bool operator <= (const PAIR & i_cRHO) const
	{
		return (m_tX < i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY <= i_cRHO.m_tY));
	}
	bool operator > (const PAIR & i_cRHO) const
	{
		return (m_tX > i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY > i_cRHO.m_tY));
	}
	bool operator >= (const PAIR & i_cRHO) const
	{
		return (m_tX > i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY >= i_cRHO.m_tY));
	}
	bool operator == (const PAIR & i_cRHO) const
	{
		return (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY);
	}
	bool operator != (const PAIR & i_cRHO) const
	{
		return (m_tX != i_cRHO.m_tX || m_tY != i_cRHO.m_tY);
	}

	template <typename S> PAIR & operator =(const PAIR<S> &i_cRHO)
	{
		m_tX = (T) i_cRHO.m_tX;
		m_tY = (T) i_cRHO.m_tY;
		return *this;
	}
	template <typename S> PAIR (const PAIR<S> &i_cRHO)
	{
		m_tX = (T) i_cRHO.m_tX;
		m_tY = (T) i_cRHO.m_tY;
	}
	void glVertex(void) const
	{
		glVertex3d((GLfloat)m_tX,(GLfloat)m_tY,0.0);
	}
	void glTranslate(void) const
	{
		glTranslated((GLfloat)m_tX,(GLfloat)m_tY,0.0);
	}
	void glScale(void) const
	{
		glScaled((GLfloat)m_tX,(GLfloat)m_tY,1.0);
	}

	PAIR <double> ScalePAIR(const PAIR <double> &i_tScalar) const
	{
		PAIR<double> tRet;
		tRet.m_tX = m_tX * i_tScalar.m_tX;
		tRet.m_tY = m_tY * i_tScalar.m_tY;
		return tRet;
	}
};
PAIR<double> operator *(const PAIR<double> & i_cLHO, const double &i_dRHO);
PAIR<double> operator *=(PAIR<double> & io_cLHO, const double &i_dRHO);
PAIR<double> operator /(const PAIR<double> & i_cLHO, const double &i_dRHO);
PAIR<double> operator /=(PAIR<double> & io_cLHO, const double &i_dRHO);
PAIR<double> operator *(const double &i_dRHO, const PAIR<double> & i_cLHO );


template <typename T> class TRIPLET
{
public:
	T m_tX;
	T m_tY;
	T m_tZ;

	TRIPLET(void)
	{
		m_tX = 0;
		m_tY = 0;
		m_tZ = 0;
	}
	TRIPLET(T i_tX, T i_tY, T i_tZ)
	{
		m_tX = i_tX;
		m_tY = i_tY;
		m_tZ = i_tZ;
	}
	TRIPLET operator +(const TRIPLET & i_cRHO) const
	{
		return TRIPLET(m_tX + i_cRHO.m_tX,m_tY + i_cRHO.m_tY,m_tZ + i_cRHO.m_tZ);
	}
	TRIPLET & operator +=(const TRIPLET & i_cRHO)
	{
		m_tX += i_cRHO.m_tX;
		m_tY += i_cRHO.m_tY;
		m_tZ += i_cRHO.m_tZ;
		return *this;
	}
	TRIPLET operator -(const TRIPLET & i_cRHO) const
	{
		return TRIPLET(m_tX - i_cRHO.m_tX,m_tY - i_cRHO.m_tY,m_tZ - i_cRHO.m_tZ);
	}
	TRIPLET & operator -=(const TRIPLET & i_cRHO)
	{
		m_tX -= i_cRHO.m_tX;
		m_tY -= i_cRHO.m_tY;
		m_tZ -= i_cRHO.m_tZ;
		return *this;
	}
	bool operator < (const TRIPLET & i_cRHO) const
	{
		return (m_tX < i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY < i_cRHO.m_tY) || (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY && m_tZ < i_cRHO.m_tZ));
	}
	bool operator <= (const TRIPLET & i_cRHO) const
	{
		return (m_tX < i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY <= i_cRHO.m_tY) || (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY && m_tZ <= i_cRHO.m_tZ));
	}
	bool operator > (const TRIPLET & i_cRHO) const
	{
		return (m_tX > i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY > i_cRHO.m_tY) || (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY && m_tZ > i_cRHO.m_tZ));
	}
	bool operator >= (const TRIPLET & i_cRHO) const
	{
		return (m_tX > i_cRHO.m_tX || (m_tX == i_cRHO.m_tX && m_tY >= i_cRHO.m_tY) || (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY && m_tZ == i_cRHO.m_tZ));
	}
	bool operator == (const TRIPLET & i_cRHO) const
	{
		return (m_tX == i_cRHO.m_tX && m_tY == i_cRHO.m_tY && m_tZ == i_cRHO.m_tZ);
	}
	bool operator != (const TRIPLET & i_cRHO) const
	{
		return (m_tX != i_cRHO.m_tX || m_tY != i_cRHO.m_tY || m_tZ != i_cRHO.m_tZ);
	}

	template <typename S> TRIPLET & operator =(const TRIPLET<S> &i_cRHO)
	{
		m_tX = (T) i_cRHO.m_tX;
		m_tY = (T) i_cRHO.m_tY;
		m_tZ = (T) i_cRHO.m_tZ;
	}
	template <typename S> TRIPLET (const TRIPLET<S> &i_cRHO)
	{
		m_tX = (T) i_cRHO.m_tX;
		m_tY = (T) i_cRHO.m_tY;
		m_tZ = (T) i_cRHO.m_tZ;
	}
	void glVertex(void) const
	{
		glVertex3d((GLfloat)m_tX,(GLfloat)m_tY,(GLfloat)m_tZ);
	}
	void glTranslate(void) const
	{
		glTranslated((GLfloat)m_tX,(GLfloat)m_tY,(GLfloat)m_tZ);
	}
	void glScale(void) const
	{
		glScaled((GLfloat)m_tX,(GLfloat)m_tY,(GLfloat)m_tZ);
	}

	TRIPLET <double> ScaleTriplet(const TRIPLET <double> &i_tScalar) const
	{
		TRIPLET<double> tRet;
		tRet.m_tX = m_tX * i_tScalar.m_tX;
		tRet.m_tY = m_tY * i_tScalar.m_tY;
		tRet.m_tZ = m_tZ * i_tScalar.m_tZ;
		return tRet;
	}
	void glColor(const double & i_dAlpha = 1.0) const
	{
		glColor4d((GLfloat) m_tX, (GLfloat) m_tY, (GLfloat) m_tZ, (GLfloat)i_dAlpha);
	}
};
TRIPLET<double> operator *(const TRIPLET<double> & i_cLHO, const double &i_dRHO);
TRIPLET<double> operator *=(TRIPLET<double> & io_cLHO, const double &i_dRHO);
TRIPLET<double> operator /(const TRIPLET<double> & i_cLHO, const double &i_dRHO);
TRIPLET<double> operator /=(TRIPLET<double> & io_cLHO, const double &i_dRHO);
TRIPLET<double> operator *(const double &i_dRHO, const TRIPLET<double> & i_cLHO );

//@@TODO: create volume class from triplets

template <typename T> class QUAD
{
public:
	PAIR<T> m_tBL; // bottom left
	PAIR<T> m_tTR; // top right

	QUAD(void) : m_tBL(), m_tTR()
	{
	}
	QUAD(PAIR<T> i_tBL, PAIR<T> i_tTR)
	{
		m_tBL = i_tBL;
		m_tTR = i_tTR;
	}
	QUAD operator +(const PAIR<T> & i_cRHO) const
	{
		return QUAD(m_tBL + i_cRHO,m_tTR + i_cRHO);
	}
	QUAD & operator +=(const PAIR<T> & i_cRHO)
	{
		m_tBL += i_cRHO;
		m_tTR += i_cRHO;
		return *this;
	}
	QUAD operator -(const PAIR<T> & i_cRHO) const
	{
		return QUAD(m_tBL - i_cRHO,m_tTR - i_cRHO);
	}
	QUAD & operator -=(const PAIR<T> & i_cRHO)
	{
		m_tBL -= i_cRHO;
		m_tTR -= i_cRHO;
		return *this;
	}
	bool operator ==(const QUAD<T> & i_cRHO) const
	{
		return (i_cRHO.m_tBL == i_cRHO.m_tBL && i_cRHO.m_tTR == i_cRHO.m_tTR);
	}
	bool operator !=(const QUAD<T> & i_cRHO) const
	{
		return (i_cRHO.m_tBL != i_cRHO.m_tBL || i_cRHO.m_tTR != i_cRHO.m_tTR);
	}
	PAIR<T> Get_Size(void) const
	{
		return (m_tTR - m_tBL);
	}

	template <typename S> QUAD & operator =(const QUAD<S> &i_cRHO)
	{
		m_tBL = (T) i_cRHO.m_tBL;
		m_tTR = (T) i_cRHO.m_tTR;
	}
	template <typename S> QUAD (const QUAD<S> &i_cRHO)
	{
		m_tBL = (T) i_cRHO.m_tBL;
		m_tTR = (T) i_cRHO.m_tTR;
	}
	void glDrawPolygon(void)
	{
		glBegin(GL_QUADS);
			glVertex2d(m_tBL.m_tX,m_tBL.m_tY);
			glVertex2d(m_tBL.m_tX,m_tTR.m_tY);
			glVertex2d(m_tTR.m_tX,m_tTR.m_tY);
			glVertex2d(m_tTR.m_tX,m_tBL.m_tY);
		glEnd();
	}
	void glDrawOutline(void)
	{
		glBegin(GL_LINE_LOOP);
			glVertex2d(m_tBL.m_tX,m_tBL.m_tY);
			glVertex2d(m_tBL.m_tX,m_tTR.m_tY);
			glVertex2d(m_tTR.m_tX,m_tTR.m_tY);
			glVertex2d(m_tTR.m_tX,m_tBL.m_tY);
		glEnd();
	}
};
QUAD<double> operator *(const QUAD<double> & i_cLHO, const double &i_dRHO);
QUAD<double> operator *=(QUAD<double> & io_cLHO, const double &i_dRHO);
QUAD<double> operator /(const QUAD<double> & i_cLHO, const double &i_dRHO);
QUAD<double> operator /=(QUAD<double> & io_cLHO, const double &i_dRHO);
QUAD<double> operator *(const double &i_dRHO, const QUAD<double> & i_cLHO );

#define PANE_ID_PREFIX		0x0010000
#define PANE_ID_IDX_MASK	0x000ffff
#define PANE_ID_NULL		0x0000000

typedef unsigned int pane_id;

class PANE
{
private:
	pane_id m_idID;
public:
	QUAD<unsigned int> m_qPosition;
	bool	m_bVisible;
	unsigned int m_uiUser_ID;
	int		m_iLevel;

	PANE(void)
	{
		m_idID = -1;
		m_uiUser_ID = -1;
		m_iLevel = 0;
	}
	PANE(pane_id i_idID)
	{
		m_idID = i_idID;
		m_uiUser_ID = -1;
		m_iLevel = 0;
	}
	PANE(pane_id i_idID, const QUAD<unsigned int> i_qPosition, bool i_bVisible, unsigned int i_uiUser_ID)
	{
		m_idID = i_idID;
		m_qPosition = i_qPosition;
		m_bVisible = i_bVisible;
		m_uiUser_ID = i_uiUser_ID;
		m_iLevel = 0;
	}
	
	pane_id Get_ID(void) const { return m_idID;}

	bool	Is_In_Pane(const PAIR<unsigned int> &i_tLocation) const
	{
		return (i_tLocation.m_tX >= m_qPosition.m_tBL.m_tX && i_tLocation.m_tX <= m_qPosition.m_tTR.m_tX && i_tLocation.m_tY >= m_qPosition.m_tBL.m_tY && i_tLocation.m_tY <= m_qPosition.m_tTR.m_tY);
	}

	void Set_Clipping(void) const
	{	// note: I tested glScissor vs glViewport on windows 7; glScissor is about 2x faster for the same view area. (render time ~ 0.0009s for viewport and 0.0005s for scissor)
		glPushAttrib(GL_SCISSOR_BIT);              // Pushes The Display List Bits     ( NEW )
		glScissor(m_qPosition.m_tBL.m_tX,m_qPosition.m_tBL.m_tY,m_qPosition.Get_Size().m_tX,m_qPosition.Get_Size().m_tY);
		glEnable(GL_SCISSOR_TEST);
	}
	void End_Clipping(void) const
	{
		glDisable(GL_SCISSOR_TEST);
		glPopAttrib();              // Pushes The Display List Bits     ( NEW )
	}
};

class main
{
public:
	enum DRAW_MODE	{ORTHO,PROJECTION};
	bool m_bEngine_Debug_Mode;
protected:
	std::vector<std::string> m_vszCommand_Line_Parameters;

private:
	std::string	m_szWindow_Name;
	bool *	m_lpbMouse_Button_Status;
	bool *	m_lpbKey_Status;
	PAIR<unsigned int>	m_tMouse_Position;
	PAIR<unsigned int>	m_tMouse_Position_Last;

	bool	m_bDraw_Flag;
	bool	m_bQuit_Flag;
	DRAW_MODE	m_eDraw_Mode;

	bool	m_bWindow_Has_Focus;
	QUAD<unsigned int> m_tWindow_Box;
	std::vector< PANE > m_tUser_Panes;

	double	m_dFrame_Rate;

	bool m_bDraw_Pane_Grids;

	std::string m_szScreenshot_Default_Filename;
	std::string m_szScreenshot_Default_Path;


	
public:
	inline void Set_Screenshot_Save_Path(std::string i_szPath){m_szScreenshot_Default_Path = i_szPath;}
	inline std::string Get_Screenshot_Save_Path(void){return m_szScreenshot_Default_Path;}
	inline void Set_Screenshot_Default_Filename(std::string i_szFilename){m_szScreenshot_Default_Filename = i_szFilename;}
	inline std::string Get_Screenshot_Default_Filename(void){return m_szScreenshot_Default_Filename;}

	main(void);
	void Process_Command_Line(unsigned int i_uiNum_Parameters, const char * i_lpszParameter_Values[]);
	inline void Set_Window_Name(const std::string &i_szName) {m_szWindow_Name = i_szName;};
	inline std::string Get_Window_Name(void) {return m_szWindow_Name;}
	inline double	Get_Frame_Rate(void) const {return m_dFrame_Rate;}
	inline void	Set_Frame_Rate(const double & i_dFrame_Rate) {m_dFrame_Rate = i_dFrame_Rate;}

	inline void Set_Draw_Mode(DRAW_MODE i_eMode) {m_eDraw_Mode = i_eMode;}
	inline pane_id Register_Pane(bool i_bVisible, unsigned int i_uiUser_ID = -1)
	{
		return Register_Pane(QUAD<unsigned int> (PAIR<unsigned int> (0,0), PAIR <unsigned int> (1,1)),i_bVisible,i_uiUser_ID);
	}

	inline pane_id Register_Pane(const QUAD <unsigned int> & i_qPosition, bool i_bVisible, unsigned int i_uiUser_ID = -1)
	{
		pane_id idID = m_tUser_Panes.size() + PANE_ID_PREFIX;
		m_tUser_Panes.push_back(PANE(idID,i_qPosition,i_bVisible,i_uiUser_ID));
		return idID;
	}
	inline pane_id Find_Pane(const PAIR <unsigned int> & i_tLocation, bool i_bRequire_Pane_Visible = true) const
	{
		pane_id idID = 0;
		std::map<int,unsigned int> cMap;
		for (std::vector<PANE>::const_iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end(); cIter++)
		{
			cMap.insert(std::pair<int, unsigned int> (cIter->m_iLevel,0));
//			cMap[cIter->m_iLevel] = 0;
		}
		for (std::vector<PANE>::const_iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end(); cIter++)
		{
			(cMap[cIter->m_iLevel])++;
		}
		for (std::map<int,unsigned int>::const_reverse_iterator cLvlItr = cMap.rbegin(); cLvlItr != cMap.rend(); cLvlItr++)
		{
			for (std::vector<PANE>::const_iterator cIter = m_tUser_Panes.begin(); cIter != m_tUser_Panes.end() && idID == 0; cIter++)
			{
				if (cLvlItr->first == cIter->m_iLevel)
				{
					if ((!i_bRequire_Pane_Visible || cIter->m_bVisible) && cIter->Is_In_Pane(i_tLocation))
						idID = cIter->Get_ID();
				}
			}
		}
		return idID;
	}
	inline void Move_Pane(pane_id i_idPane_ID, const QUAD <unsigned int> & i_qPosition)
	{
		if (i_idPane_ID & PANE_ID_PREFIX)
		{
			unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
			m_tUser_Panes[uiIdx].m_qPosition = i_qPosition;
		}
	}
	inline void Raise_Pane(pane_id i_idPane_ID)
	{
		if (i_idPane_ID & PANE_ID_PREFIX)
		{
			unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
			m_tUser_Panes[uiIdx].m_iLevel++;
		}
	}
	inline void Lower_Pane(pane_id i_idPane_ID)
	{
		if (i_idPane_ID & PANE_ID_PREFIX)
		{
			unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
			m_tUser_Panes[uiIdx].m_iLevel--;
		}
	}
	inline void Change_Pane_Visibility(pane_id i_idPane_ID, bool i_bVisible)
	{
		if (i_idPane_ID & PANE_ID_PREFIX)
		{
			unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
			m_tUser_Panes[uiIdx].m_bVisible = i_bVisible;
		}
	}
    inline bool Get_Pane_Visibility(pane_id i_idPane_ID)
    {
        bool bRet = false;
		if (i_idPane_ID & PANE_ID_PREFIX)
		{
			unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
			bRet = m_tUser_Panes[uiIdx].m_bVisible;
		}
        return bRet;
    }
	inline QUAD<unsigned int> Get_Pane_Position(pane_id i_idPane_ID)
	{
		QUAD<unsigned int> cRet;
		if (i_idPane_ID & PANE_ID_PREFIX)
		{
			unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
			cRet = m_tUser_Panes[uiIdx].m_qPosition;
		}
		return cRet;
	}
	inline unsigned int Get_Pane_User_ID(pane_id i_idPane_ID)
	{
		unsigned int uiRet = -1;
		if (i_idPane_ID & PANE_ID_PREFIX)
		{
			unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
			uiRet = m_tUser_Panes[uiIdx].m_uiUser_ID;
		}
		return uiRet;
	}
	inline double	Get_Pane_Aspect_Ratio(pane_id i_idPane_ID)
	{
		double	dRet = 1.0;
		if (i_idPane_ID & PANE_ID_PREFIX)
		{
			QUAD<unsigned int> cRet;
			unsigned int uiIdx = i_idPane_ID & PANE_ID_IDX_MASK;
			PAIR<unsigned int> tSize = m_tUser_Panes[uiIdx].m_qPosition.Get_Size();
			dRet = tSize.m_tX / (double)tSize.m_tY;
		}
		return dRet;
	}
	enum KEYID
		{
			KEY_NOOP, 
			KEY_OEM, // reserved for oem specified keys; use scancode to identify
			KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
			KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18, KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23, KEY_F24,
			KEY_SHIFT,KEY_CONTROL,KEY_ALT,
			KEY_ESCAPE, KEY_TAB, KEY_LSHIFT, KEY_LCTRL, KEY_LALT, KEY_RSHIFT, KEY_RCTRL, KEY_RALT, KEY_LWIN, KEY_RWIN, 
			KEY_CAPSLOCK, KEY_SCROLLLOCK, KEY_PRSCRN, KEY_PAUSE, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN, KEY_BACKSPACE,
			KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
			KEY_NMLCK, KEY_NUMPD_DIV, KEY_NUMPD_TIMES, KEY_NUMPD_MINUS, KEY_NUMPD_PLUS, KEY_NUMPD_ENTER,
			KEY_NUMPD_DOT, KEY_NUMPD_0, KEY_NUMPD_1, KEY_NUMPD_2, KEY_NUMPD_3, KEY_NUMPD_4, KEY_NUMPD_5,
			KEY_NUMPD_6, KEY_NUMPD_7, KEY_NUMPD_8, KEY_NUMPD_9,
			KEY_ENTER, KEY_SPACE, KEY_APPS,
			KEY_TILDE, KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
			KEY_MINUS, KEY_EQUALS, KEY_BACKSLASH, 
			KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
			KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
			KEY_LSQBRKT,KEY_RSQBRKT, KEY_SEMICOLON, KEY_SQUOTE, KEY_COMMA, KEY_PERIOD, KEY_SLASH,
			KEY_VOLUP, KEY_VOLDN, KEY_VOLMUTE, 
			KEY_BRSR_BACK, KEY_BRSR_FAV, KEY_BRSR_FWD, KEY_BRSR_REFRESH, KEY_BRSR_SEARCH, KEY_BRSR_STOP,KEY_BRSR_HOME,
			KEY_MEDIA_NEXT, KEY_MEDIA_PREV, KEY_MEDIA_STOP, KEY_MEDIA_PLAY, KEY_MEDIA_SELECT, 
			KEY_EMAIL, 
			KEY_APP_1, KEY_APP_2,
			KEY_SLEEP, KEY_WAKE, KEY_POWER, KEY_HELP, KEY_SELECT, KEY_EXEC, 
			KEY_PRINT,
			KEY_CONVERT, KEY_NONCONVERT, KEY_ACCEPT, KEY_MODECHANGE, KEY_CLEAR,
			KEY_KANA, KEY_HANGUL, KEY_JUNJA, KEY_FINAL, KEY_HANJA, KEY_KANJI,
			KEY_MB_LEFT, KEY_MB_CTR, KEY_MB_RGT, KEY_MB_SCROLL_V, KEY_MB_SCROLL_H, KEY_MB_X1, KEY_MB_X2,
			KEY_CANCEL,
			KEY_ATTN, KEY_CRSEL, KEY_EXSEL, KEY_EREOF, KEY_PLAY, KEY_ZOOM, KEY_NONAME, KEY_PA1,
			KEY_MAX // not a key - used internally
		};
	enum MOUSEBUTTON {MB_NOOP, MB_LEFT, MB_CTR, MB_RGT, MB_SCROLL_V, MB_SCROLL_H, MB_X1, MB_X2, MB_MAX};// MB_MAX not a key - used internally
	// 

	// Member functions with capitalized names are called by the common / top level library routines.  Users should not call these functions
	void On_Key_Down(KEYID eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State);
	void On_Key_Up(KEYID eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State);
	void On_Mouse_Button_Double_Click(MOUSEBUTTON i_eButton, const PAIR<unsigned int> & i_tMouse_Position);
	void On_Mouse_Button_Down(MOUSEBUTTON i_eButton, const PAIR<unsigned int> &i_tMouse_Position);
	void On_Mouse_Button_Up(MOUSEBUTTON i_eButton, const PAIR<unsigned int> &i_tMouse_Position);
	void On_Mousemove(const PAIR<unsigned int> & i_tMouse_Position);
	void On_Mouse_Wheel(MOUSEBUTTON i_eWheel, int i_iWheel_Delta, const PAIR<unsigned int> & i_tMouse_Position);
	void On_Timer(unsigned int i_uiTimer_ID, const double & i_dDelta_Time_s);
	void On_Window_Move(const PAIR<unsigned int> & i_tWindow_Position)
	{
		//printf("move %i %i\n",i_tWindow_Position.m_tX,i_tWindow_Position.m_tY);
		PAIR<unsigned int> tWindow_Size = m_tWindow_Box.m_tTR - m_tWindow_Box.m_tBL;
		//printf("move %i %i\n",tWindow_Size.m_tX,tWindow_Size.m_tY);
		m_tWindow_Box.m_tBL = i_tWindow_Position;
		m_tWindow_Box.m_tTR = i_tWindow_Position + tWindow_Size;
	}
	void On_Window_Resize(const PAIR<unsigned int> & i_tWindow_Size)
	{
		//printf("w resize %i %i\n",i_tWindow_Size.m_tX,i_tWindow_Size.m_tY);
		m_tWindow_Box.m_tTR = m_tWindow_Box.m_tBL + i_tWindow_Size;
		//printf("w resize %i %i\n",m_tWindow_Box.m_tTR.m_tX,m_tWindow_Box.m_tTR.m_tY);
	}
	inline void Gain_Focus(void)
	{
		m_bWindow_Has_Focus = true;
	}
	inline void Lose_Focus(void)
	{
		m_bWindow_Has_Focus = false;
	}

	// Member functions with lower case names are defined by the user. 
	virtual void on_key_down(KEYID eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State) = 0;
	virtual void on_key_up(KEYID eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State) = 0;
	virtual void on_mouse_button_double_click(MOUSEBUTTON i_eButton, const PAIR<unsigned int> &i_tMouse_Position) = 0;
	virtual void on_mouse_button_down(MOUSEBUTTON i_eButton, const PAIR<unsigned int> &i_tMouse_Position) = 0;
	virtual void on_mouse_button_up(MOUSEBUTTON i_eButton, const PAIR<unsigned int> &i_tMouse_Position) = 0;
	virtual void on_mousemove(const PAIR<unsigned int> &i_tMouse_Position) = 0;
	virtual void on_mouse_wheel(MOUSEBUTTON i_eWheel, int i_iWheel_Delta, const PAIR<unsigned int> &i_tMouse_Position) = 0;
	virtual void on_timer(unsigned int i_uiTimer_ID, const double & i_dDelta_Time_s) = 0;

	virtual void init(void) = 0; // initialization routine; rendering context not created
	virtual void gfx_display(pane_id i_idPane) = 0; // primary display routine
	virtual void gfx_init(void) = 0; // initialization routine; rendering context already created
	virtual void gfx_reshape(const PAIR<unsigned int> & i_tNew_Size) = 0; // window size change
	virtual void gfx_close(void) = 0; // graphics exiting; rendering context still active
	virtual void close(void) = 0; // program exiting; rendering context destroyed

	inline bool	Get_Mouse_Button(MOUSEBUTTON i_eButton) const // get status of mouse button
	{
		bool bRet = false;
		if (i_eButton > MB_NOOP && i_eButton < MB_MAX)
			bRet = m_lpbMouse_Button_Status[i_eButton - MB_NOOP];
		
		return bRet;
	}
	inline bool	Get_Key_State(KEYID i_eKey) const // get status of key
	{
		bool bRet = false;
		if (i_eKey > KEY_NOOP && i_eKey < KEY_MAX)
			bRet = m_lpbKey_Status[i_eKey - KEY_NOOP];
		
		return bRet;
	}
	inline PAIR<unsigned int>	Get_Previous_Mouse_Position(void) const
	{
		return m_tMouse_Position_Last;
	}
	inline PAIR<unsigned int>	Get_Mouse_Position(void) const
	{
		return m_tMouse_Position;
	}
	inline PAIR<unsigned int>	Get_Window_Size(void) const
	{
		PAIR<unsigned int> tWindow_Size = m_tWindow_Box.m_tTR - m_tWindow_Box.m_tBL;
		return tWindow_Size;
	}
	inline QUAD<unsigned int> Get_Window_Quad(void) const
	{
		return m_tWindow_Box;
	}

	unsigned int	Request_Timer(const double & i_dDelta_Time); // request a timer call into on_timer with the specified delta t between calls; return value is timer ID
	inline void Request_Refresh(void)
	{
		m_bDraw_Flag = true;
	}
	inline void Request_Quit(void)
	{
		m_bQuit_Flag = true;
	}

	void Draw(void);
	inline bool			Pending_Quit(void) const {return m_bQuit_Flag;}
	inline bool			Pending_Draw(void) const {return m_bDraw_Flag;}

	std::vector<std::string> Get_Directory_File_List(const std::string &i_szDirectory); /// get list of all files in the given direcctory
	void Draw_Pane_Grid(const double &dXmax);
	void Request_Screenshot(const std::string & i_szFilename);




};
typedef main MAIN;
extern main * g_lpMain;

class commsocket
{
public:
	enum	TYPE {STREAM,DATAGRAM};
	enum	DATATYPES {CHAR,UCHAR,SHORT,USHORT,INT,UINT,INT64,UINT64,FLOAT,DOUBLE};
private:
	TYPE	m_eType;
	bool	m_bAllow_Blocking;
	bool	m_bIs_Initialized;
	bool	m_bIs_Connected;
	void *	m_lpvSystem_Info; // void pointer to allow for differing socket info for each OS
	unsigned int 	m_uiMax_Tries;
public:
	commsocket(void){m_bIs_Initialized = m_bIs_Connected = false;m_lpvSystem_Info = NULL;m_uiMax_Tries = 20;}
	~commsocket(void);
	//enum	TYPE	{BROADCAST,
	bool	Initialize_Server(TYPE i_eType, unsigned short i_usPort);
	bool	Initialize_Client(TYPE i_eType, const std::string & i_szServer_Address, unsigned short i_usPort);
	bool	Send(const int *i_lpiData, size_t i_zNum_To_Send);
	bool	Send(const short * i_lpsData, size_t i_zNum_To_Send);
	bool	Send(const char * i_lplpcData, size_t i_zNum_To_Send);
	bool	Send(const unsigned int *i_lpiData, size_t i_zNum_To_Send);
	bool	Send(const unsigned short * i_lpsData, size_t i_zNum_To_Send);
	bool	Send(const unsigned char * i_lplpcData, size_t i_zNum_To_Send);
	bool	Receive(int * i_lpiData, size_t i_zNum_To_Receive);
	bool	Receive(short * i_lpsData, size_t i_zNum_To_Receive);
	bool	Receive(char * i_lpcData, size_t i_zNum_To_Receive);
	bool	Receive(unsigned int * i_lpiData, size_t i_zNum_To_Receive);
	bool	Receive(unsigned short * i_lpsData, size_t i_zNum_To_Receive);
	bool	Receive(unsigned char * i_lpcData, size_t i_zNum_To_Receive);
	bool	Accept_Connections(void);

	bool	Is_Connected(void) {return m_bIs_Connected;}
	bool	Is_Initialized(void) {return m_bIs_Initialized;}
	int		Get_Error(void);
	bool	Close(void);
};
typedef commsocket COMMSOCKET;

class calllist
{
private:
	GLuint	m_uiList_ID;
public:
	bool Is_Valid(void) const
	{
		return (m_uiList_ID != -1);
	}
	void Compile(bool i_bCompile_and_Exec = false)
	{
		if (m_uiList_ID == -1)
			m_uiList_ID = glGenLists(1);
#ifdef _DEBUG
		_RPT1(_CRT_WARN, "Call list created: %i\n", m_uiList_ID);
#endif
		glNewList(m_uiList_ID, i_bCompile_and_Exec ? GL_COMPILE_AND_EXECUTE : GL_COMPILE);
	}
	void End_Compile(void)
	{
		glEndList();
	}
	void Draw(void)
	{
		if (m_uiList_ID != -1)
			glCallList(m_uiList_ID);
	}
	void	Delete(void)
	{
		if (m_uiList_ID != -1)
			glDeleteLists(m_uiList_ID, 1);
		m_uiList_ID = -1;
	}

	calllist(void) { m_uiList_ID = -1; }
	~calllist(void) { Delete(); }
};

// graphics text operations
enum FONT{SANS,SERIF,MONO,USER_1,USER_2,USER_3,USER_4,USER_5,USER_6,USER_7,USER_8};
typedef void DRAW_CALLBACK(void * io_lpvDraw_Info);

void LoadFontFace(FONT i_eFont, const char * i_lpszPath_To_Font, bool i_bBold, bool i_bItalics);
void SelectFontFace(FONT i_eFont, bool i_bBold, bool i_bItalics);
void glPrint(const double & i_dSize, const double & i_dX, const double & i_dY, const double & i_dZ, const char * i_lpszString);
void TextBBox(const double & i_dSize, const char * i_lpszString, PAIR<double> & o_cBot_Left, PAIR<double> & o_cTop_Right);
enum HORIZ_JUSTIFICATION {HJ_LEFT,HJ_CENTER,HJ_RIGHT};
enum VERT_JUSTIFICATION {VJ_TOP,VJ_MIDDLE,VJ_BOTTOM};
void glPrintJustified(const double & i_dSize, const double & i_dX, const double & i_dY, const double & i_dZ, const char * i_lpszString,HORIZ_JUSTIFICATION i_eHorizontal_Justification, VERT_JUSTIFICATION i_eVertical_Justification);

void GenerateEllipse(const double & i_dRx, const double & i_dRy, std::vector<PAIR<double> > &o_vpResult, const unsigned int i_uiNum_Points = 64);
void GenerateArc(const double & i_dRx, const double & i_dRy, const double & i_dStart_Angle_Deg, const double & i_dEnd_Angle_Deg, std::vector<PAIR<double> > &o_vpResult, const unsigned int i_uiNum_Points = 64);
void InvertList(std::vector <PAIR<double> > & io_vpVertex_List);
void glVertexList(const std::vector <PAIR<double> > & i_vpVertex_List);
void Draw_Pushbutton(bool i_bState, DRAW_CALLBACK * i_vDrawing_Fn, void * i_lpvDraw_Info);

class BUTTON_INFO
{
public:
	enum SHAPE {ELLIPSE,RECTANGLE};
private:
	SHAPE			m_eShape;
	PAIR<double>	m_pdPosition;
	PAIR<double>	m_pdSize;
	unsigned int 	m_uiID;
	int				m_iStatus;
public:
	SHAPE GetShape(void) const {return m_eShape;}
	PAIR<double> GetPosition(void) const {return m_pdPosition;}
	PAIR<double> GetSize(void) const {return m_pdSize;}
	unsigned int GetID(void) const { return m_uiID;}
	int	GetStatus(void) const {return m_iStatus;}
	void	SetStatus(int i_iStatus) {m_iStatus = i_iStatus;}
	bool TestHit(const PAIR<double> &i_pdPosition) const
	{
		PAIR<double> pOffset = i_pdPosition - m_pdPosition;
		bool bHit = false;
		switch (m_eShape)
		{
		case RECTANGLE:
			bHit = pOffset.m_tX >= 0.0 && pOffset.m_tX <= m_pdSize.m_tX && pOffset.m_tY >= -m_pdSize.m_tY && pOffset.m_tY <= 0.0;
			break;
		case ELLIPSE:
			bHit = fabs(pOffset.m_tX) <= (m_pdSize.m_tX) && fabs(pOffset.m_tY) <= (m_pdSize.m_tY);
			if (bHit) 
			{
				double dX_a = pOffset.m_tX / m_pdSize.m_tX;
				double dY = m_pdSize.m_tY * sqrt(1.0 - dX_a * dX_a);
				bHit = fabs(pOffset.m_tY) <= dY;
			}
			break;
		}
		return bHit;
	}
	BUTTON_INFO(void)
	{
		m_eShape = RECTANGLE;
		m_pdPosition = PAIR<double>(0,0);
		m_pdSize = PAIR<double>(0,0);
		m_uiID = 0;
		m_iStatus = 0;
	}
	BUTTON_INFO(SHAPE i_eShape, const PAIR<double> i_pdPosition, const PAIR<double> i_pdSize, unsigned int i_uiID, unsigned int i_uiStatus = 0)
	{
		m_eShape = i_eShape;
		m_pdPosition = i_pdPosition;
		m_pdSize = i_pdSize;
		m_uiID = i_uiID;
		m_iStatus = i_uiStatus;
	}
};
class DATE_TIME
{
public:
	unsigned int m_uiYear;
	unsigned int m_uiMonth;
	unsigned int m_uiDay_of_Month;
	unsigned int m_uiDay_of_Week;
	unsigned int m_uiHour;
	unsigned int m_uiMinute;
	double		m_dSeconds;
public:
	void Get_Current(void);
};

class criticalsection
{
private:
	void * m_lpvCS_Data;
public:
	criticalsection(void);
	~criticalsection(void);
	void Set(void);
	void Unset(void);
	bool Get(void) const ;
};
class MAPSPACE
{
private:
	static GLuint m_uiHex_Background_List;
	static GLuint m_uiHex_Side_List[6];
	static GLuint m_uiHex_Outline_List;
	static GLuint m_uiQuad_Background_List;
	static GLuint m_uiQuad_Side_List[4];
	static GLuint m_uiQuad_Outline_List;

public:
	PAIR<int>	m_tPosition;

	void	Init(void);

	virtual void	Draw_Layer(unsigned int i_uiLayer, void * io_lpvData) const = 0;

	MAPSPACE(void) {};
	MAPSPACE(const PAIR<int> & i_tPosition) {m_tPosition = i_tPosition;};

	void Draw_Filled_Hex(void) const;
	void Draw_Hex_Side(unsigned int i_uiSide) const;
	void Draw_Hex_Outline(void) const;
	PAIR<double> Get_Hex_Vertex(unsigned int i_uiSide) const;

	void Draw_Filled_Quad(void) const;
	void Draw_Quad_Side(unsigned int i_uiSide) const;
	void Draw_Quad_Outline(void) const;
};


template<typename T> class MAPBASE
{
public:
	unsigned int 	m_uiNum_Layers;
	double			m_dZoom;
	PAIR<unsigned int>	m_tGrid_Size;
	double			m_dR;
	double			m_dDisplay_Aspect_Ratio;
	PAIR<double>	m_tScroll_Position;
	PAIR<double>	m_tScroll_Limits[2];
public:
	std::map<PAIR<int>,T>	m_cMap;

	MAPBASE(void) {m_uiNum_Layers = 0; m_tGrid_Size = PAIR<unsigned int>(0,0); m_dZoom = 1.0; m_dR = 0.1; m_dDisplay_Aspect_Ratio = 1.0; m_tScroll_Position = PAIR<double>(0.5,0.5);}
	void	Set_Map_Size(const PAIR<unsigned int> & i_tGrid_Size, unsigned int i_uiNum_Layers)
	{
		m_uiNum_Layers = i_uiNum_Layers;
		m_tGrid_Size = i_tGrid_Size;
		m_dZoom = 1.0;
	}
	void Set_Draw_Parameters(const double & i_dR, const double & i_dDisplay_Aspect_Ratio)
	{
		m_dR = i_dR;
		m_dDisplay_Aspect_Ratio = i_dDisplay_Aspect_Ratio;
//		m_tScroll_Position = m_tGrid_Size * m_dR * 0.5; // center map on center of map
		Center_Map(m_tGrid_Size * 0.5);
		Set_Scroll_Limits();
		Check_Scroll_Limits();
	}
	void Set_Scroll_Limits(void)
	{
		m_tScroll_Limits[0].m_tX = 0.5 * m_dDisplay_Aspect_Ratio;
		m_tScroll_Limits[0].m_tY = 0.5;
		m_tScroll_Limits[1].m_tX = m_tGrid_Size.m_tX * m_dR * m_dZoom - m_dDisplay_Aspect_Ratio;
		m_tScroll_Limits[1].m_tY = m_tGrid_Size.m_tY * m_dR * m_dZoom - 0.5;

		if (m_tScroll_Limits[1].m_tX < m_tScroll_Limits[0].m_tX)
		{
			double dPosition = 0.5 * (m_tScroll_Limits[0].m_tX + m_tScroll_Limits[1].m_tX);
			m_tScroll_Limits[0].m_tX = m_tScroll_Limits[1].m_tX = 0.5 * m_dDisplay_Aspect_Ratio;
		}
		if (m_tScroll_Limits[1].m_tY < m_tScroll_Limits[0].m_tY)
		{
			double dPosition = 0.5 * (m_tScroll_Limits[0].m_tY + m_tScroll_Limits[1].m_tY);
			m_tScroll_Limits[0].m_tY = m_tScroll_Limits[1].m_tY = 0.50;
		}
	}
	void Check_Scroll_Limits(void)
	{
		if (m_tScroll_Position.m_tX < m_tScroll_Limits[0].m_tX)
			m_tScroll_Position.m_tX = m_tScroll_Limits[0].m_tX;
		if (m_tScroll_Position.m_tX > m_tScroll_Limits[1].m_tX)
			m_tScroll_Position.m_tX = m_tScroll_Limits[1].m_tX;

		if (m_tScroll_Position.m_tY < m_tScroll_Limits[0].m_tY)
			m_tScroll_Position.m_tY = m_tScroll_Limits[0].m_tY;
		if (m_tScroll_Position.m_tY > m_tScroll_Limits[1].m_tY)
			m_tScroll_Position.m_tY = m_tScroll_Limits[1].m_tY;
	}
	void	Set_Map_Zoom(const double & i_dZoom)
	{
		m_dZoom = i_dZoom;
		Set_Scroll_Limits();
		Check_Scroll_Limits();
	}
	void	Shift_Zoom(const double & i_dDelta)
	{
		m_dZoom *= i_dDelta;
		if (m_dZoom == 0.0)
			m_dZoom = 1.0;
		Set_Scroll_Limits();
		Check_Scroll_Limits();
	}
	void	Set_Map_Scroll_Limits(const PAIR<double> & m_tLower_Limits, const PAIR<double> & m_tUpper_Limits)
	{
		m_tScroll_Limits[0] = m_tLower_Limits;
		m_tScroll_Limits[1] = m_tUpper_Limits;
	}
	void	Shift_Map(const PAIR<double> & i_tDelta)
	{
		m_tScroll_Position += i_tDelta;
		Check_Scroll_Limits();
	}
	double	Get_R(void) const {return m_dR;}


	unsigned int Factorial(unsigned int i_uiN) const;
	unsigned int Binomial_Coefficient(unsigned int i_uiN, unsigned int i_uiK) const;
	PAIR<double> Bezier(const std::vector<PAIR<double> > & i_vPoints, const double & i_dT) const;
	void Draw_Bezier(const std::vector<PAIR<double> > & i_vPoints, unsigned int i_uiResolution = 128) const;

	virtual void Center_Map(const PAIR<int> & i_tPosition) = 0;
	virtual void Draw_Map(void * io_lpvData) const = 0;
	virtual void Prepare_Draw(void) const;
};

template <typename T> class HEXMAP : public MAPBASE<T>
{
private:
	calllist	m_clMap_Grid;

public:
	std::deque<PAIR<int> > Get_Path(const PAIR<int> & i_pStart, const PAIR<int> & i_pEnd) const;

	//void Determine_Hex(const PAIR<double> &i_tVitual_Coord, PAIR<int> & o_tGrid_Position) const;
	PAIR<int> Determine_Hex(const PAIR<double> &i_tVitual_Coord) const;
	void Get_Hex_Center(const PAIR<int> & i_tGrid_Position, PAIR<double> & o_tVitual_Coord) const;
	PAIR<double> Get_Hex_Center(const PAIR<int> & i_tGrid_Position) const;

	void Draw_Filled_Hex(const PAIR<int> & i_tPosition) const;
	void Draw_Hex_Side(const PAIR<int> & i_tPosition, unsigned int i_uiSide) const;
	void Draw_Hex_Outline(const PAIR<int> & i_tPosition) const;

	void Draw_Grid(void);
	void Draw_Map(void * io_lpvData) const; // void * for user defined data that needs to be sent to each space for drawing

	void	Center_Map(const PAIR<int> & i_tPosition);

};

template <typename T> class ISOMETRIC_HEXMAP : public MAPBASE<T>
{
private:
	double m_dCamera_Isometric_Angle;
	double m_dCamera_Rotation_Angle;
	calllist	m_clMap_Grid;
public:
	double Get_Isometric_Camera_Angle(void){return m_dCamera_Isometric_Angle;}
	double Get_Camera_Rotation_Angle(void){return m_dCamera_Rotation_Angle;}

	ISOMETRIC_HEXMAP(void){m_dCamera_Isometric_Angle = 60.0;  m_dCamera_Rotation_Angle = 0.0; /* 30^deg for iso angle */}

	std::deque<PAIR<int> > Get_Path(const PAIR<int> & i_pStart, const PAIR<int> & i_pEnd) const;

	//void Determine_Hex(const PAIR<double> &i_tVitual_Coord, PAIR<int> & o_tGrid_Position) const;
	PAIR<int> Determine_Hex(const PAIR<double> &i_tVitual_Coord) const;
	void Get_Hex_Center(const PAIR<int> & i_tGrid_Position, PAIR<double> & o_tVitual_Coord) const;
	PAIR<double> Get_Hex_Center(const PAIR<int> & i_tGrid_Position) const;

	void Draw_Filled_Hex(const PAIR<int> & i_tPosition) const;
	void Draw_Hex_Side(const PAIR<int> & i_tPosition, unsigned int i_uiSide) const;
	void Draw_Hex_Outline(const PAIR<int> & i_tPosition) const;

	void Draw_Grid(void);
	void Draw_Map(void * io_lpvData) const; // void * for user defined data that needs to be sent to each space for drawing

	void	Center_Map(const PAIR<int> & i_tPosition);
	void	Change_Isometric_Angle(const double & i_dDelta_Angle)
	{
		m_dCamera_Isometric_Angle += i_dDelta_Angle;
		if (m_dCamera_Isometric_Angle > 90.0)
			m_dCamera_Isometric_Angle = 90.0;
		if (m_dCamera_Isometric_Angle < 0.0)
			m_dCamera_Isometric_Angle = 0.0;
	}
	void	Change_Camera_Angle(const double & i_dDelta_Angle)
	{
		double dAngle = fmod(m_dCamera_Rotation_Angle + i_dDelta_Angle,360.0);
		m_dCamera_Rotation_Angle = dAngle;
	}
	void Prepare_Draw(void) const;

};
#include <core_mapbase.hpp>
#include <core_hexmap.hpp>
#include <core_isohexmap.hpp>

void glLineWidth(const double &i_dLW);

// common functions for drawing buttons
extern std::vector<PAIR<double> > g_vEllipse;

enum SB_DIRECTION {SBD_UP,SBD_DOWN,SBD_LEFT,SBD_RIGHT};
void Initialize_Circle_Vectors(void);
void Draw_Rounded_Rectangle(bool i_bFilled);
void Draw_Rounded_Rectangle_Button(bool i_bPressed, bool i_bActive);
void Draw_Scroll_Button(SB_DIRECTION i_eDirection, bool i_bActive);
void Draw_Simple_Button(bool bStatus);

