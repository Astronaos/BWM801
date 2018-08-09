

template <typename T> pair<int> isometric_hexmap<T>::Determine_Hex(const pair<double> &i_tView_Coord) const
{
	pair<int> tRet;
	pair<double> tTrue_Coord = (i_tView_Coord + mapbase<T>::m_tScroll_Position - pair<double>(0.5 * mapbase<T>::m_dDisplay_Aspect_Ratio,0.5)) / mapbase<T>::m_dZoom;
	tRet.m_tX = -1;
	tRet.m_tY = -1;
	unsigned int uiBlock_X = (unsigned int)((tTrue_Coord.m_tX / mapbase<T>::m_dR) / 1.5);
	unsigned int uiBlock_Y = (unsigned int)((tTrue_Coord.m_tY / mapbase<T>::m_dR));
	double dHex_X = fmod(tTrue_Coord.m_tX / mapbase<T>::m_dR,1.5);
	double dHex_Y = fmod(tTrue_Coord.m_tY / mapbase<T>::m_dR,1.0);

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


template <typename T>void isometric_hexmap<T>::Get_Hex_Center(const pair<int> & i_tGrid_Position, pair<double> & o_tVitual_Coord) const
{
	o_tVitual_Coord = Get_Hex_Center(i_tGrid_Position);
}
template <typename T>pair<double> isometric_hexmap<T>::Get_Hex_Center(const pair<int> & i_tGrid_Position) const
{
	pair<double> cRet;
	 // a little easier than going the other way....
	int iY_Offset = (i_tGrid_Position.m_tX % 2);
	if (i_tGrid_Position.m_tX < 0)
		iY_Offset *= -1;

	cRet.m_tX = (i_tGrid_Position.m_tX * 1.5 + 1.0) * 0.5;
	cRet.m_tY = (i_tGrid_Position.m_tY * 2.0 + 1.0 + iY_Offset) * 0.5;
 //@@TODO: rotations and projection
	return cRet;
}

template <typename T>void isometric_hexmap<T>::Draw_Filled_Hex(const pair<int> & i_tPosition) const
{
	pair<double> tPos;
	mapspacemin cSpace;
	Get_Hex_Center(i_tPosition,tPos);
	glPushMatrix();
		isometric_hexmap<T>::Prepare_Draw();
		glTranslated(tPos.m_tX,tPos.m_tY,0.0);
		cSpace.Draw_Filled_Hex();
	glPopMatrix();
}

template <typename T>void isometric_hexmap<T>::Draw_Hex_Side(const pair<int> & i_tPosition, unsigned int i_uiSide) const
{
	pair<double> tPos;
	mapspacemin cSpace;
	Get_Hex_Center(i_tPosition,tPos);
	glPushMatrix();
		isometric_hexmap<T>::Prepare_Draw();
		glTranslated(tPos.m_tX,tPos.m_tY,0.0);
		cSpace.Draw_Hex_Side(i_uiSide);
	glPopMatrix();
}
template <typename T>void isometric_hexmap<T>::Draw_Hex_Outline(const pair<int> & i_tPosition) const
{
	pair<double> tPos;
	mapspacemin cSpace;
	Get_Hex_Center(i_tPosition,tPos);
	glPushMatrix();
		isometric_hexmap<T>::Prepare_Draw();
		glTranslated(tPos.m_tX,tPos.m_tY,0.0);
		cSpace.Draw_Hex_Outline();
	glPopMatrix();
}
template <typename T>void isometric_hexmap<T>::Draw_Grid(void)
{
	double dRoff = 0.5 * mapbase<T>::m_dR;
	pair<double> tPos;
	mapspacemin cSpace;
	glPushMatrix();
	isometric_hexmap<T>::Prepare_Draw();
	if (!m_clMap_Grid.Is_Valid())
	{
		m_clMap_Grid.Compile(true);
		/// Draw grid lines
		for (int uiI = 0; uiI < (int)(mapbase<T>::m_tGrid_Size.m_tX); uiI++)
		{
			glBegin(GL_LINE_STRIP);
			for (int uiJ = 0; uiJ < (int)(mapbase<T>::m_tGrid_Size.m_tY); uiJ++)
			{
				pair<double> pdCenter = Get_Hex_Center(pair<int>(uiI,uiJ));
				if (uiJ == 0)
				{
					if (uiI & 1 != 0)
					{
						pair<double> pdCenterOdd = Get_Hex_Center(pair<int>(uiI - 1,uiJ));
						(cSpace.Get_Hex_Vertex(5) + pdCenterOdd).glVertex();
					}
					(cSpace.Get_Hex_Vertex(0) + pdCenter).glVertex();
				}
				(cSpace.Get_Hex_Vertex(1) + pdCenter).glVertex();
				(cSpace.Get_Hex_Vertex(2) + pdCenter).glVertex();
			}
			if (uiI > 0 && ((uiI & 1) == 0))
			{
				pair<double> pdCenterOdd = Get_Hex_Center(pair<int>(uiI - 1,(int)(mapbase<T>::m_tGrid_Size.m_tY - 1)));
				(cSpace.Get_Hex_Vertex(3) + pdCenterOdd).glVertex();
			}
			glEnd();
		}
		glBegin(GL_LINE_STRIP);
		for (int uiJ = 0; uiJ < (int)(mapbase<T>::m_tGrid_Size.m_tY); uiJ++)
		{
			pair<double> pdCenter = Get_Hex_Center(pair<int>((int)(mapbase<T>::m_tGrid_Size.m_tX - 1),uiJ));
			if (uiJ == 0)
			{
				(cSpace.Get_Hex_Vertex(5) + pdCenter).glVertex();
			}
			(cSpace.Get_Hex_Vertex(4) + pdCenter).glVertex();
			(cSpace.Get_Hex_Vertex(3) + pdCenter).glVertex();
		}
		glEnd();
		glBegin(GL_LINES);
		for (int uiI = 0; uiI < (int)(mapbase<T>::m_tGrid_Size.m_tX); uiI++)
		{
			for (int uiJ = 0; uiJ < (int)(mapbase<T>::m_tGrid_Size.m_tY); uiJ++)
			{
				pair<double> pdCenter = Get_Hex_Center(pair<int>(uiI,uiJ));
				(cSpace.Get_Hex_Vertex(0) + pdCenter).glVertex();
				(cSpace.Get_Hex_Vertex(5) + pdCenter).glVertex();
			}
			pair<double> pdCenter = Get_Hex_Center(pair<int>(uiI,(int)(mapbase<T>::m_tGrid_Size.m_tY - 1)));
			(cSpace.Get_Hex_Vertex(2) + pdCenter).glVertex();
			(cSpace.Get_Hex_Vertex(3) + pdCenter).glVertex();
		}
		glEnd();
		m_clMap_Grid.End_Compile();
	}
	else
	{
		m_clMap_Grid.Draw();
	}
	glPopMatrix();
}


template <typename T> void isometric_hexmap<T>::Draw_Map(void * io_lpvData) const
{
	pair<double> tPos;
	mapspacemin cSpace;
	cSpace.Init(); // make sure the space has set up call lists
	glPushMatrix();
		isometric_hexmap<T>::Prepare_Draw();
		typename std::map<pair<int>,T>::const_iterator mapiterator;
		for (unsigned int uiLayer = 0; uiLayer < mapbase<T>::m_uiNum_Layers; uiLayer++)
		{
			for (mapiterator = mapbase<T>::m_cMap.begin(); mapiterator != mapbase<T>::m_cMap.end(); mapiterator++)
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

template <typename T> void isometric_hexmap<T>::Center_Map(const pair<int> & i_tPosition)
{
	if (i_tPosition.m_tX >= 0 && i_tPosition.m_tX <= (int)mapbase<T>::m_tGrid_Size.m_tX &&
		i_tPosition.m_tY >= 0 && i_tPosition.m_tY <= (int)mapbase<T>::m_tGrid_Size.m_tY)
	{

		pair<double> pdCenter;
		Get_Hex_Center(i_tPosition,pdCenter);
		mapbase<T>::m_tScroll_Position = pdCenter * (mapbase<T>::m_dR * mapbase<T>::m_dZoom);
//		mapbase<T>::m_tScroll_Position.m_tX = pdCenter.m_tX * (mapbase<T>::m_dR * mapbase<T>::m_dZoom);
//		mapbase<T>::m_tScroll_Position.m_tY = pdCenter.m_tY * (mapbase<T>::m_dR * mapbase<T>::m_dZoom);
	}

	// make sure corners of map are within the viewable area
//	double dX = 

	mapbase<T>::Check_Scroll_Limits();
}

template <typename T> std::deque<pair<int> > isometric_hexmap<T>::Get_Path(const pair<int> & i_pStart, const pair<int> & i_pEnd) const
{
	std::deque<pair<int> > cRet;

//	cRet.clear();
	pair<int> tCurr_Position = i_pStart;
	pair<double> tdEnd;
	pair<double> tdCurr;
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

template <typename T> void isometric_hexmap<T>::Prepare_Draw(void) const
{
	glTranslated(-mapbase<T>::m_tScroll_Position.m_tX + 0.5 * mapbase<T>::m_dDisplay_Aspect_Ratio,-mapbase<T>::m_tScroll_Position.m_tY + 0.5,0.0);
	glScaled(mapbase<T>::m_dZoom,mapbase<T>::m_dZoom,mapbase<T>::m_dZoom);
	glScaled(mapbase<T>::m_dR,mapbase<T>::m_dR,1.0);
	glRotated(-m_dCamera_Isometric_Angle,1.0,0.0,0.0);
	glRotated(m_dCamera_Rotation_Angle,0.0,0.0,1.0);
}

