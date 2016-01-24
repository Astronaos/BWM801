#if defined (__MINGW32__) || defined (_WIN32)
#include <windows.h>
#include <windowsx.h>
#endif
#include <GL/gl.h>
#include <core.hpp>

unsigned int MAPSPACE::m_uiQuad_Background_List = (unsigned int)-1;
unsigned int MAPSPACE::m_uiQuad_Side_List[4] = {(unsigned int)-1,(unsigned int)-1,(unsigned int)-1,(unsigned int)-1};
unsigned int MAPSPACE::m_uiQuad_Outline_List = (unsigned int)-1;
unsigned int MAPSPACE::m_uiHex_Background_List = (unsigned int)-1;
unsigned int MAPSPACE::m_uiHex_Side_List[6] = {(unsigned int)-1,(unsigned int)-1,(unsigned int)-1,(unsigned int)-1,(unsigned int)-1,(unsigned int)-1};
unsigned int MAPSPACE::m_uiHex_Outline_List = (unsigned int)-1;


void MAPSPACE::Init(void)
{
	unsigned int uiList;
	if (m_uiHex_Background_List == -1)
	{
		uiList = glGenLists(1);
		glNewList(uiList,GL_COMPILE);
			Draw_Filled_Hex();
		glEndList();
		m_uiHex_Background_List = uiList; 
	}
	for (unsigned int uiSide = 0; uiSide < 6; uiSide++)
	{
		if (m_uiHex_Side_List[uiSide] == -1)
		{
			uiList = glGenLists(1);
			glNewList(uiList,GL_COMPILE);
				Draw_Hex_Side(uiSide);
			glEndList();
			m_uiHex_Side_List[uiSide] = uiList;
		}
	}
	if (m_uiHex_Outline_List == -1)
	{
		uiList = glGenLists(1);
		glNewList(uiList,GL_COMPILE);
			Draw_Hex_Outline();
		glEndList();
		m_uiHex_Outline_List = uiList;
	}
	if (m_uiQuad_Background_List == -1)
	{
		uiList = glGenLists(1);
		glNewList(uiList,GL_COMPILE);
			Draw_Filled_Hex();
		glEndList();
		m_uiQuad_Background_List = uiList; 
	}
	for (unsigned int uiSide = 0; uiSide < 4; uiSide++)
	{
		if (m_uiQuad_Side_List[uiSide] == -1)
		{
			uiList = glGenLists(1);
			glNewList(uiList,GL_COMPILE);
				Draw_Hex_Side(uiSide);
			glEndList();
			m_uiQuad_Side_List[uiSide] = uiList;
		}
	}
	if (m_uiQuad_Outline_List == -1)
	{
		uiList = glGenLists(1);
		glNewList(uiList,GL_COMPILE);
			Draw_Hex_Outline();
		glEndList();
		m_uiQuad_Outline_List = uiList;
	}
}

void MAPSPACE::Draw_Filled_Hex(void) const
{
	if (m_uiHex_Background_List == -1)
	{
		glBegin(GL_TRIANGLE_FAN);
			glVertex3d(0.0,0.0,0.0);
			glVertex3d(-0.25,-0.5,0.0);
			glVertex3d(-0.5,0.0,0.0);
			glVertex3d(-0.25,0.5,0.0);
			glVertex3d(0.25,0.5,0.0);
			glVertex3d(0.5,0.0,0.0);
			glVertex3d(0.25,-0.5,0.0);
			glVertex3d(-0.25,-0.5,0.0);
		glEnd();
	}
	else
		glCallList(m_uiHex_Background_List);
}
void MAPSPACE::Draw_Hex_Side(unsigned int i_uiSide) const
{
	i_uiSide %= 6;

	if (m_uiHex_Side_List[i_uiSide] == -1)
	{
		glBegin(GL_LINES);
			if (i_uiSide == 4 || i_uiSide == 5)
				glVertex3d(-0.25,-0.5,0.0);
			if (i_uiSide == 5 || i_uiSide == 0)
				glVertex3d(-0.5,0.0,0.0);
			if (i_uiSide == 0 || i_uiSide == 1)
				glVertex3d(-0.25,0.5,0.0);
			if (i_uiSide == 1 || i_uiSide == 2)
				glVertex3d(0.25,0.5,0.0);
			if (i_uiSide == 2 || i_uiSide == 3)
				glVertex3d(0.5,0.0,0.0);
			if (i_uiSide == 3 || i_uiSide == 4)
				glVertex3d(0.25,-0.5,0.0);
		glEnd();
	}
	else
		glCallList(m_uiHex_Side_List[i_uiSide]);
}
void MAPSPACE::Draw_Hex_Outline(void) const
{
	if (m_uiHex_Outline_List == -1)
	{
		glBegin(GL_LINE_LOOP);
			glVertex3d(-0.25,-0.5,0.0);
			glVertex3d(-0.5,0.0,0.0);
			glVertex3d(-0.25,0.5,0.0);
			glVertex3d(0.25,0.5,0.0);
			glVertex3d(0.5,0.0,0.0);
			glVertex3d(0.25,-0.5,0.0);
		glEnd();
	}
	else
		glCallList(m_uiHex_Outline_List);
}


void MAPSPACE::Draw_Filled_Quad(void) const
{
	if (m_uiQuad_Background_List == -1)
	{
		glBegin(GL_QUADS);
			glVertex3d(-0.5,-0.5,0.0);
			glVertex3d(-0.5,0.5,0.0);
			glVertex3d(0.5,0.5,0.0);
			glVertex3d(0.5,-0.5,0.0);
		glEnd();
	}
	else
		glCallList(m_uiQuad_Background_List);
}
void MAPSPACE::Draw_Quad_Side(unsigned int i_uiSide) const
{
	i_uiSide %= 4;

	if (m_uiQuad_Side_List[i_uiSide] == -1)
	{
		glBegin(GL_LINES);
			if (i_uiSide == 3 || i_uiSide == 0)
				glVertex3d(-0.5,-0.5,0.0);
			if (i_uiSide == 0 || i_uiSide == 1)
				glVertex3d(-0.5,0.5,0.0);
			if (i_uiSide == 1 || i_uiSide == 2)
				glVertex3d(0.5,0.5,0.0);
			if (i_uiSide == 2 || i_uiSide == 3)
				glVertex3d(0.5,-0.5,0.0);
		glEnd();
	}
	else
		glCallList(m_uiQuad_Side_List[i_uiSide]);
}
void MAPSPACE::Draw_Quad_Outline(void) const
{
	if (m_uiQuad_Outline_List == -1)
	{
		glBegin(GL_LINE_LOOP);
			glVertex3d(-0.5,-0.5,0.0);
			glVertex3d(-0.5,0.5,0.0);
			glVertex3d(0.5,0.5,0.0);
			glVertex3d(0.5,-0.5,0.0);
		glEnd();
	}
	else
		glCallList(m_uiQuad_Outline_List);
}
