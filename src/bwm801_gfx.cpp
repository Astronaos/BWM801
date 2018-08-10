#include <bwm801.h>
#include <cmath>

std::vector<bwm801::pair<float> > g_vEllipse;

float	g_dTwoPi = 2.0f * std::acos(-1.0f);
void bwm801::GenerateEllipse(const float & i_dRx, const float & i_dRy, std::vector<bwm801::pair<float> > &o_vpResult, const unsigned int i_uiNum_Points)
{
	for (unsigned int uiI = 0; uiI < i_uiNum_Points; uiI++)
	{
		float dTheta = g_dTwoPi - uiI / (i_uiNum_Points - 1.0f) * g_dTwoPi;
		float dX = std::cos(dTheta) * i_dRx;
		float	dY = std::sin(dTheta) * i_dRy;
		o_vpResult.push_back(pair<float>(dX,dY));
	}
}
void bwm801::GenerateArc(const float & i_dRx, const float & i_dRy, const float & i_dStart_Angle_Deg, const float & i_dEnd_Angle_Deg, std::vector<bwm801::pair<float> > &o_vpResult, const unsigned int i_uiNum_Points)
{
	float dStart_Angle_Rad = i_dStart_Angle_Deg * g_dTwoPi / 360.0f;
	float dEnd_Angle_Rad = i_dEnd_Angle_Deg * g_dTwoPi / 360.0f;
	while (dEnd_Angle_Rad < dStart_Angle_Rad)
		dEnd_Angle_Rad += g_dTwoPi;
	float dDelta_Angle = dEnd_Angle_Rad - dStart_Angle_Rad;
//	printf("(%f,%f), %f-%f\n",i_dRx,i_dRy,i_dStart_Angle_Deg,i_dEnd_Angle_Deg);
	for (unsigned int uiI = 0; uiI < i_uiNum_Points; uiI++)
	{
		float dTheta = dEnd_Angle_Rad - uiI / (i_uiNum_Points - 1.0f) * dDelta_Angle;
		float dX = std::cos(dTheta) * i_dRx;
		float	dY = std::sin(dTheta) * i_dRy;
		o_vpResult.push_back(pair<float>(dX,dY));
//	printf("(%f,%f)\n",dX,dY);
	}
}
void bwm801::glVertexList(const std::vector <bwm801::pair<float> > & i_vpVertex_List)
{
	for (std::vector<bwm801::pair<float> >::const_iterator cI = i_vpVertex_List.begin(); cI != i_vpVertex_List.end(); cI++)
	{
		cI->glVertex();
	}
}

void bwm801::InvertList(std::vector <bwm801::pair<float> > & io_vpVertex_List)
{
	std::vector<bwm801::pair<float> > vList = io_vpVertex_List;
	io_vpVertex_List.clear();
	for (std::vector<bwm801::pair<float> >::const_reverse_iterator cI = vList.rbegin(); cI != vList.rend(); cI++)
	{
		io_vpVertex_List.push_back(*cI);
	}
	//io_vpVertex_List.push_back(*vList.begin());
}

void bwm801::Draw_Pushbutton(bool i_bState, draw_callback i_vDrawing_Fn, void * i_lpvDraw_Info)
{
	bwm801::pair<float> pShift;
	glPushMatrix();
		if (!i_bState)
			pShift = pair<float>(0.1f,-0.4f);
		else
			pShift = pair<float>(0.04f,-0.16f);
	
		// draw shadow of button
		glPushMatrix();
			glTranslated(pShift.m_tX,pShift.m_tY,0.0);
			glColor4f(0.0,0.0,0.0,0.75); // black
			glBegin(GL_TRIANGLES);
				glVertex2f(-1.0,0.0);
				glVertex2f(-1.0f - pShift.m_tX,-pShift.m_tY);
				glVertex2f(-pShift.m_tX,-pShift.m_tY);
				glVertex2f(1.0f,0.0);
				glVertex2f(-pShift.m_tX,-pShift.m_tY);
				glVertex2f(1.0f - pShift.m_tX,-pShift.m_tY);
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(0.0,0.0);
				glVertexList(g_vEllipse);
			glEnd();
		glPopMatrix();
			// draw button
		glLineWidth(3.0);
		glColor4f(0.0,0.0,0.0,1.0); // black
		glBegin(GL_LINE_LOOP);
			glVertexList(g_vEllipse);
		glEnd();
		// draw user portion
		i_vDrawing_Fn(i_lpvDraw_Info);
	glPopMatrix();
}

void bwm801::glPrintJustified(const float & i_dSize, const float & i_dX, const float & i_dY, const float & i_dZ, const char * i_lpszString,bwm801::horiz_justification i_eHorizontal_Justification, bwm801::vert_justification i_eVertical_Justification)
{
	pair<float> pBL,pTR;
	TextBBox(i_dSize,i_lpszString,pBL,pTR);
	float dX = 0.0, dY = 0.0;
	pTR -= pBL;
	switch (i_eHorizontal_Justification)
	{
	case hj_left:
		break;
	case hj_center:
		dX = -pTR.m_tX * 0.5f;
		break;
	case hj_right:
		dX = -pTR.m_tX;
		break;
	}
	switch (i_eVertical_Justification)
	{
	case vj_bottom:
		break;
	case vj_middle:
		dY = -pTR.m_tY * 0.5f;
		break;
	case vj_top:
		dY = -pTR.m_tY;
		break;
	}
	glPrint(i_dSize,i_dX + dX, i_dY + dY, i_dZ, i_lpszString);
}


std::vector<bwm801::pair<float> > g_vRounded_Rectangle_TL;
std::vector<bwm801::pair<float> > g_vRounded_Rectangle_TR;
std::vector<bwm801::pair<float> > g_vRounded_Rectangle_BR;
std::vector<bwm801::pair<float> > g_vRounded_Rectangle_BL;

GLuint i_uiFilled_RR_List = 0;
GLuint i_uiOutline_RR_List = 0;

void bwm801::Initialize_Circle_Vectors(void)
{
	if (g_vEllipse.empty())
	{
		GenerateEllipse(1.0,1.0,g_vEllipse);
	}
	float	dEdge_Radius = 0.20f;
	float	dEdge_Center = 0.50f - dEdge_Radius;
	if (g_vRounded_Rectangle_TL.empty())
	{
		bwm801::pair<float> pOffset(-dEdge_Center,dEdge_Center);
		GenerateArc(dEdge_Radius,dEdge_Radius,90.0,180.0,g_vRounded_Rectangle_TL);
		for (std::vector<bwm801::pair<float> >::iterator cI = g_vRounded_Rectangle_TL.begin(); cI != g_vRounded_Rectangle_TL.end(); cI++)
		{
			cI[0] += pOffset;
		}
	}
	if (g_vRounded_Rectangle_TR.empty())
	{
		bwm801::pair<float> pOffset(dEdge_Center,dEdge_Center);
		GenerateArc(dEdge_Radius,dEdge_Radius,0.0,90.0,g_vRounded_Rectangle_TR);
		for (std::vector<bwm801::pair<float> >::iterator cI = g_vRounded_Rectangle_TR.begin(); cI != g_vRounded_Rectangle_TR.end(); cI++)
		{
			cI[0] += pOffset;
		}
	}
	if (g_vRounded_Rectangle_BR.empty())
	{
		bwm801::pair<float> pOffset(dEdge_Center,-dEdge_Center);
		GenerateArc(dEdge_Radius,dEdge_Radius,270.0,0.0,g_vRounded_Rectangle_BR);
		for (std::vector<bwm801::pair<float> >::iterator cI = g_vRounded_Rectangle_BR.begin(); cI != g_vRounded_Rectangle_BR.end(); cI++)
		{
			cI[0] += pOffset;
		}
	}
	if (g_vRounded_Rectangle_BL.empty())
	{
		bwm801::pair<float> pOffset(-dEdge_Center,-dEdge_Center);
		GenerateArc(dEdge_Radius,dEdge_Radius,180.0,270.0,g_vRounded_Rectangle_BL);
		for (std::vector<bwm801::pair<float> >::iterator cI = g_vRounded_Rectangle_BL.begin(); cI != g_vRounded_Rectangle_BL.end(); cI++)
		{
			cI[0] += pOffset;
		}
	}

}

void bwm801::Draw_Rounded_Rectangle(bool i_bFilled)
{
	Initialize_Circle_Vectors();
	if (i_uiFilled_RR_List == 0)
	{
		i_uiFilled_RR_List = glGenLists(1);
		glNewList(i_uiFilled_RR_List,GL_COMPILE);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0.0,0.0);
			glVertexList(g_vRounded_Rectangle_TL);
			glVertexList(g_vRounded_Rectangle_TR);
			glVertexList(g_vRounded_Rectangle_BR);
			glVertexList(g_vRounded_Rectangle_BL);
			g_vRounded_Rectangle_TL[0].glVertex();
		glEnd();
		glEndList();
	}
	if (i_uiOutline_RR_List == 0)
	{
		i_uiOutline_RR_List = glGenLists(1);
		glNewList(i_uiOutline_RR_List,GL_COMPILE);
		glBegin(GL_LINE_LOOP);
			glVertexList(g_vRounded_Rectangle_TL);
			glVertexList(g_vRounded_Rectangle_TR);
			glVertexList(g_vRounded_Rectangle_BR);
			glVertexList(g_vRounded_Rectangle_BL);
		glEnd();
		glEndList();
	}

	if (i_bFilled && i_uiFilled_RR_List != 0)
	{
		glPushMatrix();
            glTranslatef(0.5,-0.5,0.0);
		glCallList(i_uiFilled_RR_List);
        glPopMatrix();
	}
	else if (!i_bFilled && i_uiOutline_RR_List != 0)
	{
		glPushMatrix();
            glTranslatef(0.5,-0.5,0.0);
		glCallList(i_uiOutline_RR_List);
        glPopMatrix();
	}
	else
	{
		glPushMatrix();
            glTranslatef(1.0,-0.5,0.0);
			if (!i_bFilled)
				glLineWidth(2.0);
			glBegin(i_bFilled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
				if (i_bFilled)
					glVertex2f(0.0,0.0);
	//			glTranslated(-0.95, 0.95,0.0);
				glVertexList(g_vRounded_Rectangle_TL);
	//			glTranslated( 1.90, 0.00,0.0);
				glVertexList(g_vRounded_Rectangle_TR);
	//			glTranslated( 0.00, 1.90,0.0);
				glVertexList(g_vRounded_Rectangle_BR);
	//			glTranslated(-1.90, 0.00,0.0);
				glVertexList(g_vRounded_Rectangle_BL);
	//			glTranslated( 0.00,-1.90,0.0);
				if (i_bFilled)
					g_vRounded_Rectangle_TL[0].glVertex();
			glEnd();
		glPopMatrix();
	}
}
void bwm801::Draw_Rounded_Rectangle_Button(bool i_bPressed, bool i_bActive)
{
	glPushMatrix();
		glColor4f(0.0,0.0,0.0,0.25f);
		if (i_bPressed)
			glTranslatef(0.02f,-0.04f,0.0);
		else
			glTranslatef(0.10f,-0.20f,0.0);
		Draw_Rounded_Rectangle(true);
	glPopMatrix();

	if (i_bActive)
		glColor4f(0.0,1.0,0.0,1.0); // green
	else
		glColor4f(0.0,0.1f,0.0,1.0); // green

	Draw_Rounded_Rectangle(true);
	glColor4f(0.0,0.0,0.0,1.0);
	glLineWidth(2.0);
	Draw_Rounded_Rectangle(false);
}

void bwm801::Draw_Scroll_Button(bwm801::sb_direction i_eDirection, bool i_bActive)
{
	glColor4f(0.75,0.75,0.75,1.0);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.00,0.00);
		glVertex2f(1.0,0.00);
		glVertex2f(1.0,-1.0);
		glVertex2f(0.00,-1.0);
	glEnd();
	if (i_bActive)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_LINE_STRIP);

		switch (i_eDirection)
		{
		case sbd_up:
			glVertex2f(0.0,-1.0);
			glVertex2f(0.5,0.00);
			glVertex2f(1.0,-1.0);
			break;
		case sbd_down:
			glVertex2f(1.0,0.0);
			glVertex2f(0.5,-1.00);
			glVertex2f(0.0,0.0);
			break;
		case sbd_left:
			glVertex2f(1.0,-1.0);
			glVertex2f(0.0,-0.5);
			glVertex2f(1.0, 0.0);
			break;
		case sbd_right:
			glVertex2f(0.0, 0.0);
			glVertex2f(1.0,-0.5);
			glVertex2f(0.0,-1.0);
			break;
		}
	glEnd();
}
void bwm801::Draw_Simple_Button(bool bStatus)
{
    glColor4f(0.0,0.25,0.0,1.0);
    glBegin(GL_QUADS);
        glVertex2f(0.0,0.0);
        glVertex2f(1.0,0.0);
        glVertex2f(1.0,-1.0);
        glVertex2f(0.0,-1.0);
    glEnd();
    if (bStatus)
    {
        glColor4f(0.0,1.0,0.0,1.0);
        glLineWidth(2.5);
    }
    else
    {
        glColor4f(0.0,0.0,0.0,1.0);
        glLineWidth(1.0);
    }
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.0,0.0);
        glVertex2f(1.0,0.0);
        glVertex2f(1.0,-1.0);
        glVertex2f(0.0,-1.0);
    glEnd();

}

void bwm801::main::Draw_Pane_Grid(const float &i_dXmax)
{
	glLineWidth(1.0);
	glColor4f(0.0,0.0,0.0,1.0);
	glBegin(GL_LINES);
	for (float dX = 0.05f; dX < i_dXmax; dX+=0.05f)
	{
		glVertex2f(dX,0.0);
		glVertex2f(dX,1.0);
	}
	for (float dY = 0.05f; dY < i_dXmax; dY+=0.05f)
	{
		glVertex2f(0.0,dY);
		glVertex2f(i_dXmax,dY);
	}
	glEnd();
}
