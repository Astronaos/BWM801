#pragma once

template <typename T>unsigned int mapbase<T>::Factorial(unsigned int i_uiN) const
{
	unsigned int uiRet = 1;
	for (unsigned int uiI = 2; uiI <= i_uiN; uiI++)
		uiRet *= uiI;
	return uiRet;
}


template <typename T>unsigned int mapbase<T>::Binomial_Coefficient(unsigned int i_uiN, unsigned int i_uiK) const
{
	unsigned int uiRet = 1;
	uiRet *= Factorial(i_uiN);

	uiRet /= (Factorial(i_uiK) * Factorial(i_uiN - i_uiK));
	return uiRet;
}
template <typename T>pair<float> mapbase<T>::Bezier(const std::vector<pair<float> > & i_vPoints, const float & i_dT) const
{
	pair<float>	cRet;
	if (i_vPoints.size() > 0)
	{
		unsigned int uiN = i_vPoints.size() - 1;
		for (unsigned int uiI = 0; uiI <= uiN; uiI++)
		{
			pair<float> pfValue = i_vPoints[uiI] * (std::pow(1.0 - i_dT,uiN - uiI) * std::pow(i_dT,uiI) * Binomial_Coefficient(uiN,uiI));
			cRet += pfValue;
		}
	}
	return cRet;
}

template <typename T> void mapbase<T>::Draw_Bezier(const std::vector<pair<float> > & i_vPoints, unsigned int i_uiResolution) const
{
	float		dDelta_T = 1.0 / (i_uiResolution - 1);
	if (i_vPoints.size() > 0)
	{
		for (float dT = 0.0; dT <= 1.0; dT += dDelta_T)
		{
			pair<float>	cCurr = Bezier(i_vPoints,dT);
			glVertex3f(cCurr.m_tX,cCurr.m_tY,0.0);
		}
	}
}

template <typename T> void mapbase<T>::Prepare_Draw(void) const
{
	glTranslatef(-m_tScroll_Position.m_tX + 0.5 * m_dDisplay_Aspect_Ratio,-m_tScroll_Position.m_tY + 0.5,0.0);
	glScalef(m_dZoom,m_dZoom,m_dZoom);
	glScalef(mapbase<T>::m_dR,mapbase<T>::m_dR,1.0);
}

