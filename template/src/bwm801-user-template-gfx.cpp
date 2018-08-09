#include <bwm801-user-template.h>
#include <sstream>
#include <iomanip>

using namespace bwm801;

void user_template_main::gfx_init(void) // initialization routine; rendering context already created
{
	glClearColor(1.0,1.0,1.0,0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialf(GL_FRONT,GL_SPECULAR,10.0);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	SelectFontFace(SANS,true,false);

	glEnable(GL_TEXTURE_2D);
	m_cTexture.Load_Image("spectrum.png",-1);
	glDisable(GL_TEXTURE_2D);
}
void user_template_main::gfx_reshape(const pair<unsigned int> & i_tNew_Size) // window size change
{
	pair<unsigned int> m_tViewport_Size = i_tNew_Size;
	glViewport(0,0,(GLsizei) m_tViewport_Size.m_tX, (GLsizei) m_tViewport_Size.m_tY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,m_tViewport_Size.m_tX / m_tViewport_Size.m_tY,0.0,1.0,0.0,1.0);
	glMatrixMode(GL_MODELVIEW);

	pair<unsigned int> tWindow_BL(0,0);
	pair<unsigned int> tWindow_TR((unsigned int)m_tViewport_Size.m_tX,(unsigned int)(m_tViewport_Size.m_tY));
	pair<unsigned int> tError_BL = tWindow_TR * 0.25;
	pair<unsigned int> tError_TR = tWindow_TR * 0.75;

	Move_Pane(m_idPane,quad<unsigned int> (tWindow_BL,tWindow_TR));

	double dAR = Get_Pane_Aspect_Ratio(m_idError_Pane);


	dAR = Get_Pane_Aspect_Ratio(m_idPane);


}
void user_template_main::gfx_close(void) // graphics exiting; rendering context still active
{
}

void user_template_main::gfx_display(pane_id i_idPane) // primary display routine
{
	if (g_vEllipse.empty())
	{
		GenerateEllipse(1.0,1.0,g_vEllipse);
	}
	if (i_idPane == m_idPane)
	{
		double dSize = Get_Pane_Aspect_Ratio(m_idPane);
		glColor4f(1.0,1.0,1.0,1.0);
		glBegin(GL_QUADS);
			glVertex2f(0.0,1.0);
			glVertex2f(dSize,1.0);
			glVertex2f(dSize,0.0);
			glVertex2f(0.0,0.0);
		glEnd();
		glPushMatrix();
			glTranslatef(m_dTimer * 0.1,m_dTimer * 0.1,0.0);
			glColor4f(0.0,0.0,0.0,1.0);
			glEnable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
			m_cTexture.Apply();
			glBegin(GL_QUADS);
				glTexCoord2f(0.0,0.0);
				glVertex2f(0.0,0.0);
				glTexCoord2f(1.0,0.0);
				glVertex2f(0.3,0.0);
				glTexCoord2f(1.0,1.0);
				glVertex2f(0.3,0.15);
				glTexCoord2f(0.0,1.0);
				glVertex2f(0.0,0.15);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	}
}

