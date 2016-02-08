#include<cmath>
#if defined (__MINGW32__) || defined (_WIN32)
#include <windows.h>
#include <windowsx.h>
#endif
#include<GL/gl.h>

template <typename T>unsigned int MAPBASE<T>::Factorial(unsigned int i_uiN) const
{
	unsigned int uiRet = 1;
	for (unsigned int uiI = 2; uiI <= i_uiN; uiI++)
		uiRet *= uiI;
	return uiRet;
}


template <typename T>unsigned int MAPBASE<T>::Binomial_Coefficient(unsigned int i_uiN, unsigned int i_uiK) const
{
	unsigned int uiRet = 1;
	uiRet *= Factorial(i_uiN);

	uiRet /= (Factorial(i_uiK) * Factorial(i_uiN - i_uiK));
	return uiRet;
}
template <typename T>PAIR<double> MAPBASE<T>::Bezier(const std::vector<PAIR<double> > & i_vPoints, const double & i_dT) const
{
	PAIR<double>	cRet;
	if (i_vPoints.size() > 0)
	{
		unsigned int uiN = i_vPoints.size() - 1;
		for (unsigned int uiI = 0; uiI <= uiN; uiI++)
		{
			cRet += i_vPoints[uiI] * (pow(1.0 - i_dT,uiN - uiI) * pow(i_dT,uiI) * Binomial_Coefficient(uiN,uiI));
		}
	}
	return cRet;
}

template <typename T> void MAPBASE<T>::Draw_Bezier(const std::vector<PAIR<double> > & i_vPoints, unsigned int i_uiResolution) const
{
	double		dDelta_T = 1.0 / (i_uiResolution - 1);
	if (i_vPoints.size() > 0)
	{
		for (double dT = 0.0; dT <= 1.0; dT += dDelta_T)
		{
			PAIR<double>	cCurr = Bezier(i_vPoints,dT);
			glVertex3d(cCurr.m_tX,cCurr.m_tY,0.0);
		}
	}
}

template <typename T> void MAPBASE<T>::Prepare_Draw(void) const
{
	glTranslated(-m_tScroll_Position.m_tX + 0.5 * m_dDisplay_Aspect_Ratio,-m_tScroll_Position.m_tY + 0.5,0.0);
	glScaled(m_dZoom,m_dZoom,m_dZoom);
	glScaled(MAPBASE<T>::m_dR,MAPBASE<T>::m_dR,1.0);
}

