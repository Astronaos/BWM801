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

std::vector<PAIR<double> > g_vpPushbutton_Ellipse;

void Draw_Pushbutton(bool i_bState, DRAW_CALLBACK * i_vDrawing_Fn, void * i_lpvDraw_Info)
{
	if (g_vpPushbutton_Ellipse.empty())
	{
		GenerateEllipse(1.0,1.0,g_vpPushbutton_Ellipse);
	}

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
				glVertexList(g_vpPushbutton_Ellipse);
			glEnd();
		glPopMatrix();
			// draw button
		glLineWidth(3.0);
		glColor4d(0.0,0.0,0.0,1.0); // black
		glBegin(GL_LINE_LOOP);
			glVertexList(g_vpPushbutton_Ellipse);
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
