#include <core.hpp>
double	g_dTwoPi = 2.0 * acos(-1.0);
void GenerateEllipse(const double & i_dRx, const double & i_dRy, std::vector<PAIR<double> > &o_vpResult, const unsigned int i_uiNum_Points)
{
	for (unsigned int uiI = 0; uiI < i_uiNum_Points; uiI++)
	{
		double dTheta = g_dTwoPi - uiI / (i_uiNum_Points - 1.0) * g_dTwoPi;
		double dX = cos(dTheta) * i_dRx;
		double	dY = sin(dTheta) * i_dRy;
		o_vpResult.push_back(PAIR<double>(dX,dY));
	}
}
void GenerateArc(const double & i_dRx, const double & i_dRy, const double & i_dStart_Angle_Deg, const double & i_dEnd_Angle_Deg, std::vector<PAIR<double> > &o_vpResult, const unsigned int i_uiNum_Points)
{
	double dStart_Angle_Rad = i_dStart_Angle_Deg * g_dTwoPi / 360.0;
	double dEnd_Angle_Rad = i_dEnd_Angle_Deg * g_dTwoPi / 360.0;
	while (dEnd_Angle_Rad < dStart_Angle_Rad)
		dEnd_Angle_Rad += g_dTwoPi;
	double dDelta_Angle = dEnd_Angle_Rad - dStart_Angle_Rad;
//	printf("(%f,%f), %f-%f\n",i_dRx,i_dRy,i_dStart_Angle_Deg,i_dEnd_Angle_Deg);
	for (unsigned int uiI = 0; uiI < i_uiNum_Points; uiI++)
	{
		double dTheta = dEnd_Angle_Rad - uiI / (i_uiNum_Points - 1.0) * dDelta_Angle;
		double dX = cos(dTheta) * i_dRx;
		double	dY = sin(dTheta) * i_dRy;
		o_vpResult.push_back(PAIR<double>(dX,dY));
//	printf("(%f,%f)\n",dX,dY);
	}
}
void glVertexList(const std::vector <PAIR<double> > & i_vpVertex_List)
{
	for (std::vector<PAIR<double> >::const_iterator cI = i_vpVertex_List.begin(); cI != i_vpVertex_List.end(); cI++)
	{
		cI->glVertex();
	}
}

void InvertList(std::vector <PAIR<double> > & io_vpVertex_List)
{
	std::vector<PAIR<double> > vList = io_vpVertex_List;
	io_vpVertex_List.clear();
	for (std::vector<PAIR<double> >::const_reverse_iterator cI = vList.rbegin(); cI != vList.rend(); cI++)
	{
		io_vpVertex_List.push_back(*cI);
	}
	//io_vpVertex_List.push_back(*vList.begin());
}

void Draw_Pushbutton(bool i_bState, DRAW_CALLBACK * i_vDrawing_Fn, void * i_lpvDraw_Info)
{
	PAIR<double> pShift;
	glPushMatrix();
		if (!i_bState)
			pShift = PAIR<double>(0.1,-0.4);
		else
			pShift = PAIR<double>(0.04,-0.16);
	
		// draw shadow of button
		glPushMatrix();
			glTranslated(pShift.m_tX,pShift.m_tY,0.0);
			glColor4d(0.0,0.0,0.0,0.75); // black
			glBegin(GL_TRIANGLES);
				glVertex2d(-1.0,0.0);
				glVertex2d(-1.0 - pShift.m_tX,-pShift.m_tY);
				glVertex2d(-pShift.m_tX,-pShift.m_tY);
				glVertex2d(1.0,0.0);
				glVertex2d(-pShift.m_tX,-pShift.m_tY);
				glVertex2d(1.0 - pShift.m_tX,-pShift.m_tY);
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glVertex2d(0.0,0.0);
				glVertexList(g_vEllipse);
			glEnd();
		glPopMatrix();
			// draw button
		glLineWidth(3.0);
		glColor4d(0.0,0.0,0.0,1.0); // black
		glBegin(GL_LINE_LOOP);
			glVertexList(g_vEllipse);
		glEnd();
		// draw user portion
		i_vDrawing_Fn(i_lpvDraw_Info);
	glPopMatrix();
}

void glPrintJustified(const double & i_dSize, const double & i_dX, const double & i_dY, const double & i_dZ, const char * i_lpszString,HORIZ_JUSTIFICATION i_eHorizontal_Justification, VERT_JUSTIFICATION i_eVertical_Justification)
{
	PAIR<double> pBL,pTR;
	TextBBox(i_dSize,i_lpszString,pBL,pTR);
	double dX = 0.0, dY = 0.0;
	pTR -= pBL;
	switch (i_eHorizontal_Justification)
	{
	case HJ_LEFT:
		break;
	case HJ_CENTER:
		dX = -pTR.m_tX * 0.5;
		break;
	case HJ_RIGHT:
		dX = -pTR.m_tX;
		break;
	}
	switch (i_eVertical_Justification)
	{
	case VJ_BOTTOM:
		break;
	case VJ_MIDDLE:
		dY = -pTR.m_tY * 0.5;
		break;
	case VJ_TOP:
		dY = -pTR.m_tY;
		break;
	}
	glPrint(i_dSize,i_dX + dX, i_dY + dY, i_dZ, i_lpszString);
}


void glLineWidth(const double & i_dX)
{
	glLineWidth((GLfloat) i_dX);
}


std::vector<PAIR<double> > g_vRounded_Rectangle_TL;
std::vector<PAIR<double> > g_vRounded_Rectangle_TR;
std::vector<PAIR<double> > g_vRounded_Rectangle_BR;
std::vector<PAIR<double> > g_vRounded_Rectangle_BL;
std::vector<PAIR<double> > g_vEllipse;

GLuint i_uiFilled_RR_List = 0;
GLuint i_uiOutline_RR_List = 0;

void Initialize_Circle_Vectors(void)
{
	if (g_vEllipse.empty())
	{
		GenerateEllipse(1.0,1.0,g_vEllipse);
	}
	double	dEdge_Radius = 0.20;
	double	dEdge_Center = 0.50 - dEdge_Radius;
	if (g_vRounded_Rectangle_TL.empty())
	{
		PAIR<double> pOffset(-dEdge_Center,dEdge_Center);
		GenerateArc(dEdge_Radius,dEdge_Radius,90.0,180.0,g_vRounded_Rectangle_TL);
		for (std::vector<PAIR<double> >::iterator cI = g_vRounded_Rectangle_TL.begin(); cI != g_vRounded_Rectangle_TL.end(); cI++)
		{
			cI[0] += pOffset;
		}
	}
	if (g_vRounded_Rectangle_TR.empty())
	{
		PAIR<double> pOffset(dEdge_Center,dEdge_Center);
		GenerateArc(dEdge_Radius,dEdge_Radius,0.0,90.0,g_vRounded_Rectangle_TR);
		for (std::vector<PAIR<double> >::iterator cI = g_vRounded_Rectangle_TR.begin(); cI != g_vRounded_Rectangle_TR.end(); cI++)
		{
			cI[0] += pOffset;
		}
	}
	if (g_vRounded_Rectangle_BR.empty())
	{
		PAIR<double> pOffset(dEdge_Center,-dEdge_Center);
		GenerateArc(dEdge_Radius,dEdge_Radius,270.0,0.0,g_vRounded_Rectangle_BR);
		for (std::vector<PAIR<double> >::iterator cI = g_vRounded_Rectangle_BR.begin(); cI != g_vRounded_Rectangle_BR.end(); cI++)
		{
			cI[0] += pOffset;
		}
	}
	if (g_vRounded_Rectangle_BL.empty())
	{
		PAIR<double> pOffset(-dEdge_Center,-dEdge_Center);
		GenerateArc(dEdge_Radius,dEdge_Radius,180.0,270.0,g_vRounded_Rectangle_BL);
		for (std::vector<PAIR<double> >::iterator cI = g_vRounded_Rectangle_BL.begin(); cI != g_vRounded_Rectangle_BL.end(); cI++)
		{
			cI[0] += pOffset;
		}
	}

}

void Draw_Rounded_Rectangle(bool i_bFilled)
{
	Initialize_Circle_Vectors();
	if (i_uiFilled_RR_List == 0)
	{
		i_uiFilled_RR_List = glGenLists(1);
		glNewList(i_uiFilled_RR_List,GL_COMPILE);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2d(0.0,0.0);
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
            glTranslated(0.5,-0.5,0.0);
		glCallList(i_uiFilled_RR_List);
        glPopMatrix();
	}
	else if (!i_bFilled && i_uiOutline_RR_List != 0)
	{
		glPushMatrix();
            glTranslated(0.5,-0.5,0.0);
		glCallList(i_uiOutline_RR_List);
        glPopMatrix();
	}
	else
	{
		glPushMatrix();
            glTranslated(1.0,-0.5,0.0);
			if (!i_bFilled)
				glLineWidth(2.0);
			glBegin(i_bFilled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
				if (i_bFilled)
					glVertex2d(0.0,0.0);
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
void Draw_Rounded_Rectangle_Button(bool i_bPressed, bool i_bActive)
{
	glPushMatrix();
		glColor4d(0.0,0.0,0.0,0.25);
		if (i_bPressed)
			glTranslated(0.02,-0.04,0.0);
		else
			glTranslated(0.10,-0.20,0.0);
		Draw_Rounded_Rectangle(true);
	glPopMatrix();

	if (i_bActive)
		glColor4d(0.0,1.0,0.0,1.0); // green
	else
		glColor4d(0.0,0.1,0.0,1.0); // green

	Draw_Rounded_Rectangle(true);
	glColor4d(0.0,0.0,0.0,1.0);
	glLineWidth(2.0);
	Draw_Rounded_Rectangle(false);
}

void Draw_Scroll_Button(SB_DIRECTION i_eDirection, bool i_bActive)
{
	glColor4d(0.75,0.75,0.75,1.0);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
		glVertex2d(0.00,0.00);
		glVertex2d(1.0,0.00);
		glVertex2d(1.0,-1.0);
		glVertex2d(0.00,-1.0);
	glEnd();
	if (i_bActive)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_LINE_STRIP);

		switch (i_eDirection)
		{
		case SBD_UP:
			glVertex2d(0.0,-1.0);
			glVertex2d(0.5,0.00);
			glVertex2d(1.0,-1.0);
			break;
		case SBD_DOWN:
			glVertex2d(1.0,0.0);
			glVertex2d(0.5,-1.00);
			glVertex2d(0.0,0.0);
			break;
		case SBD_LEFT:
			glVertex2d(1.0,-1.0);
			glVertex2d(0.0,-0.5);
			glVertex2d(1.0, 0.0);
			break;
		case SBD_RIGHT:
			glVertex2d(0.0, 0.0);
			glVertex2d(1.0,-0.5);
			glVertex2d(0.0,-1.0);
			break;
		}
	glEnd();
}
void Draw_Simple_Button(bool bStatus)
{
    glColor4d(0.0,0.25,0.0,1.0);
    glBegin(GL_QUADS);
        glVertex2d(0.0,0.0);
        glVertex2d(1.0,0.0);
        glVertex2d(1.0,-1.0);
        glVertex2d(0.0,-1.0);
    glEnd();
    if (bStatus)
    {
        glColor4d(0.0,1.0,0.0,1.0);
        glLineWidth(2.5);
    }
    else
    {
        glColor4d(0.0,0.0,0.0,1.0);
        glLineWidth(1.0);
    }
    glBegin(GL_LINE_LOOP);
        glVertex2d(0.0,0.0);
        glVertex2d(1.0,0.0);
        glVertex2d(1.0,-1.0);
        glVertex2d(0.0,-1.0);
    glEnd();

}

void main::Draw_Pane_Grid(const double &dXmax)
{
	glLineWidth(1.0);
	glColor4d(0.0,0.0,0.0,1.0);
	glBegin(GL_LINES);
	for (double dX = 0.05; dX < dXmax; dX+=0.05)
	{
		glVertex2d(dX,0.0);
		glVertex2d(dX,1.0);
	}
	for (double dY = 0.05; dY < dXmax; dY+=0.05)
	{
		glVertex2d(0.0,dY);
		glVertex2d(dXmax,dY);
	}
	glEnd();
}
