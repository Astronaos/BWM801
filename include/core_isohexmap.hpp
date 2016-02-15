

template <typename T> PAIR<int> ISOMETRIC_HEXMAP<T>::Determine_Hex(const PAIR<double> &i_tView_Coord) const
{
	PAIR<int> tRet;
	PAIR<double> tTrue_Coord = (i_tView_Coord + MAPBASE<T>::m_tScroll_Position - PAIR<double>(0.5 * MAPBASE<T>::m_dDisplay_Aspect_Ratio,0.5)) / MAPBASE<T>::m_dZoom;
	tRet.m_tX = -1;
	tRet.m_tY = -1;
	unsigned int uiBlock_X = (unsigned int)((tTrue_Coord.m_tX / MAPBASE<T>::m_dR) / 1.5);
	unsigned int uiBlock_Y = (unsigned int)((tTrue_Coord.m_tY / MAPBASE<T>::m_dR));
	double dHex_X = fmod(tTrue_Coord.m_tX / MAPBASE<T>::m_dR,1.5);
	double dHex_Y = fmod(tTrue_Coord.m_tY / MAPBASE<T>::m_dR,1.0);

	tRet.m_tX = uiBlock_X * 2;
	tRet.m_tY = uiBlock_Y;

	if (dHex_X >= 1.0)
	{
		tRet.m_tX++;
		if (dHex_Y < 0.5)
			tRet.m_tY--;
	}
	else if (dHex_X >= 0.5)
	{
		dHex_X -= 0.5;
		if (dHex_X >= 0.25) // may be in a different hex
		{
			dHex_X -= 0.25;
			if (dHex_Y >= 0.5)
			{
				dHex_Y -= 0.5;
				if ((0.5 - dHex_X * 2.0) <= dHex_Y)
				{
					tRet.m_tX++;
				}
			}
			else
			{
				if (dHex_X * 2.0 >= dHex_Y)
				{
					tRet.m_tX++;
					tRet.m_tY--;
				}
			}
		}
	}
	else
	{
		if (dHex_X < 0.25) // may be in a different hex
		{
			if (dHex_Y >= 0.5)
			{
				dHex_Y -= 0.5;
				if (dHex_X * 2.0 < dHex_Y)
				{
					tRet.m_tX--;
				}
			}
			else
			{
				if ((0.5 - dHex_X * 2.0) >= dHex_Y)
				{
					tRet.m_tX--;
					tRet.m_tY--;
				}
			}
		}
	}
	return tRet;
}


template <typename T>void ISOMETRIC_HEXMAP<T>::Get_Hex_Center(const PAIR<int> & i_tGrid_Position, PAIR<double> & o_tVitual_Coord) const
{
	o_tVitual_Coord = Get_Hex_Center(i_tGrid_Position);
}
template <typename T>PAIR<double> ISOMETRIC_HEXMAP<T>::Get_Hex_Center(const PAIR<int> & i_tGrid_Position) const
{
	PAIR<double> cRet;
	 // a little easier than going the other way....
	int iY_Offset = (i_tGrid_Position.m_tX % 2);
	if (i_tGrid_Position.m_tX < 0)
		iY_Offset *= -1;

	cRet.m_tX = (i_tGrid_Position.m_tX * 1.5 + 1.0) * 0.5;
	cRet.m_tY = (i_tGrid_Position.m_tY * 2.0 + 1.0 + iY_Offset) * 0.5;
	return cRet;
}

template <typename T>void ISOMETRIC_HEXMAP<T>::Draw_Filled_Hex(const PAIR<int> & i_tPosition) const
{
	PAIR<double> tPos;
	MAPSPACEMIN cSpace;
	Get_Hex_Center(i_tPosition,tPos);
	glPushMatrix();
		ISOMETRIC_HEXMAP<T>::Prepare_Draw();
		glTranslated(tPos.m_tX,tPos.m_tY,0.0);
		cSpace.Draw_Filled_Hex();
	glPopMatrix();
}

template <typename T>void ISOMETRIC_HEXMAP<T>::Draw_Hex_Side(const PAIR<int> & i_tPosition, unsigned int i_uiSide) const
{
	PAIR<double> tPos;
	MAPSPACEMIN cSpace;
	Get_Hex_Center(i_tPosition,tPos);
	glPushMatrix();
		ISOMETRIC_HEXMAP<T>::Prepare_Draw();
		glTranslated(tPos.m_tX,tPos.m_tY,0.0);
		cSpace.Draw_Hex_Side(i_uiSide);
	glPopMatrix();
}
template <typename T>void ISOMETRIC_HEXMAP<T>::Draw_Hex_Outline(const PAIR<int> & i_tPosition) const
{
	PAIR<double> tPos;
	MAPSPACEMIN cSpace;
	Get_Hex_Center(i_tPosition,tPos);
	glPushMatrix();
		ISOMETRIC_HEXMAP<T>::Prepare_Draw();
		glTranslated(tPos.m_tX,tPos.m_tY,0.0);
		cSpace.Draw_Hex_Outline();
	glPopMatrix();
}
template <typename T>void ISOMETRIC_HEXMAP<T>::Draw_Grid(void) const
{
	double dRoff = 0.5 * MAPBASE<T>::m_dR;
	PAIR<double> tPos;
	MAPSPACEMIN cSpace;
	glPushMatrix();
	ISOMETRIC_HEXMAP<T>::Prepare_Draw();
	/// Draw grid lines
	for (int uiI = 0; uiI < (int)(MAPBASE<T>::m_tGrid_Size.m_tX); uiI++)
	{
		for (int uiJ = 0; uiJ < (int)(MAPBASE<T>::m_tGrid_Size.m_tY); uiJ++)
		{
			glPushMatrix();
				Get_Hex_Center(PAIR<int>(uiI,uiJ),tPos);
				glTranslated(tPos.m_tX,tPos.m_tY,0.0);
				if (uiI % 2 == 0)
				{
					if (uiJ % 2 == 0)
						cSpace.Draw_Hex_Outline();
					else
					{
						cSpace.Draw_Hex_Side(0);
						cSpace.Draw_Hex_Side(2);
						cSpace.Draw_Hex_Side(3);
						cSpace.Draw_Hex_Side(5);
					}
				}
				else
				{
					if (uiJ != ((int)(MAPBASE<T>::m_tGrid_Size.m_tY - 1)))
						cSpace.Draw_Hex_Side(1);
					cSpace.Draw_Hex_Side(4);
				}
			glPopMatrix();
		}
	}
	glPopMatrix();
}


template <typename T> void ISOMETRIC_HEXMAP<T>::Draw_Map(void * io_lpvData) const
{
	PAIR<double> tPos;
	MAPSPACEMIN cSpace;
	cSpace.Init(); // make sure the space has set up call lists
	glPushMatrix();
		ISOMETRIC_HEXMAP<T>::Prepare_Draw();
		typename std::map<PAIR<int>,T>::const_iterator mapiterator;
		for (unsigned int uiLayer = 0; uiLayer < MAPBASE<T>::m_uiNum_Layers; uiLayer++)
		{
			for (mapiterator = MAPBASE<T>::m_cMap.begin(); mapiterator != MAPBASE<T>::m_cMap.end(); mapiterator++)
			{
				glPushMatrix();
					Get_Hex_Center(mapiterator->first,tPos);
					glTranslated(tPos.m_tX,tPos.m_tY,0.0);
					mapiterator->second.Draw_Layer(uiLayer,io_lpvData);
				glPopMatrix();
			}
		}
	glPopMatrix();
}

template <typename T> void ISOMETRIC_HEXMAP<T>::Center_Map(const PAIR<int> & i_tPosition)
{
	if (i_tPosition.m_tX >= 0 && i_tPosition.m_tX <= (int)MAPBASE<T>::m_tGrid_Size.m_tX &&
		i_tPosition.m_tY >= 0 && i_tPosition.m_tY <= (int)MAPBASE<T>::m_tGrid_Size.m_tY)
	{

		PAIR<double> pdCenter;
		Get_Hex_Center(i_tPosition,pdCenter);
		MAPBASE<T>::m_tScroll_Position = pdCenter * (MAPBASE<T>::m_dR * MAPBASE<T>::m_dZoom);
//		MAPBASE<T>::m_tScroll_Position.m_tX = pdCenter.m_tX * (MAPBASE<T>::m_dR * MAPBASE<T>::m_dZoom);
//		MAPBASE<T>::m_tScroll_Position.m_tY = pdCenter.m_tY * (MAPBASE<T>::m_dR * MAPBASE<T>::m_dZoom);
	}

	// make sure corners of map are within the viewable area
//	double dX = 

	MAPBASE<T>::Check_Scroll_Limits();
}

template <typename T> std::deque<PAIR<int> > ISOMETRIC_HEXMAP<T>::Get_Path(const PAIR<int> & i_pStart, const PAIR<int> & i_pEnd) const
{
	std::deque<PAIR<int> > cRet;

//	cRet.clear();
	PAIR<int> tCurr_Position = i_pStart;
	PAIR<double> tdEnd;
	PAIR<double> tdCurr;
	Get_Hex_Center(i_pEnd,tdEnd);
	double	dPi = acos(-1.0);
	double dRad_To_Deg = 360.0 / (2.0 * dPi);
	cRet.push_back(tCurr_Position);
	while (tCurr_Position != i_pEnd)
	{
		Get_Hex_Center(tCurr_Position,tdCurr);
		double dAngle = atan2((tdEnd.m_tY - tdCurr.m_tY),(tdEnd.m_tX - tdCurr.m_tX)) *dRad_To_Deg;
		if (dAngle < 0.0)
			dAngle += 360.0;
		if (dAngle <= 60.0)
		{
			tCurr_Position.m_tX++;
			if ((tCurr_Position.m_tX % 2) == 0)
				tCurr_Position.m_tY++;
		}
		else if (dAngle <= 120.0)
		{
			tCurr_Position.m_tY++;
		}
		else if (dAngle <= 180.0)
		{
			tCurr_Position.m_tX--;
			if ((tCurr_Position.m_tX % 2) == 0)
				tCurr_Position.m_tY++;
		}
		else if (dAngle <= 240.0)
		{
			tCurr_Position.m_tX--;
			if ((tCurr_Position.m_tX % 2) == 1)
				tCurr_Position.m_tY--;
		}
		else if (dAngle <= 300.0)
		{
			tCurr_Position.m_tY--;
		}
		else// if (dAngle <= 180.0)
		{
			tCurr_Position.m_tX++;
			if ((tCurr_Position.m_tX % 2) == 1)
				tCurr_Position.m_tY--;
		}
		cRet.push_back(tCurr_Position);
	}
//	if (i_pStart != i_pEnd && tCurr_Position != i_pEnd)
//		cRet.push_back(tCurr_Position);
	return cRet;
}

template <typename T> void ISOMETRIC_HEXMAP<T>::Prepare_Draw(void) const
{
	glTranslated(-MAPBASE<T>::m_tScroll_Position.m_tX + 0.5 * MAPBASE<T>::m_dDisplay_Aspect_Ratio,-MAPBASE<T>::m_tScroll_Position.m_tY + 0.5,0.0);
	glScaled(MAPBASE<T>::m_dZoom,MAPBASE<T>::m_dZoom,MAPBASE<T>::m_dZoom);
	glScaled(MAPBASE<T>::m_dR,MAPBASE<T>::m_dR,1.0);
	glRotated(m_dCamera_Rotation_Angle,0.0,0.0,1.0);
	glRotated(m_dCamera_Isometric_Angle,0.0,1.0,0.0);
}

