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

	// load textures here
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
// best place to delete any calllists or textures.
	m_cTexture.Delete();
}

void user_template_main::gfx_display(pane_id i_idPane) // primary display routine
{
	if (i_idPane == m_idPane)
	{
		float dSize = Get_Pane_Aspect_Ratio(m_idPane);
		// draw a white rectangle, filling the pane
		glColor4f(1.0,1.0,1.0,1.0);
		glBegin(GL_QUADS);
			glVertex2f(0.0,1.0);
			glVertex2f(dSize,1.0);
			glVertex2f(dSize,0.0);
			glVertex2f(0.0,0.0);
		glEnd();
		// draw rectangle that is covered with the texture
		glPushMatrix();
			glTranslatef(m_dTimer * 0.1f,m_dTimer * 0.1f,0.0); // make it move slowly
			glColor4f(0.0,0.0,0.0,1.0);
			glEnable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
			m_cTexture.Apply();
			glBegin(GL_QUADS);
				glTexCoord2f(0.0,0.0);
				glVertex2f(0.0,0.0);
				glTexCoord2f(1.0,0.0);
				glVertex2f(0.3f,0.0);
				glTexCoord2f(1.0,1.0);
				glVertex2f(0.3f,0.15f);
				glTexCoord2f(0.0,1.0);
				glVertex2f(0.0,0.15f);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	}
}

