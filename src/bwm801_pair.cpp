#include <bwm801.h>

using bwm801::pair;

pair<float> operator *(const pair<float> & i_cLHO, const double &i_dRHO)
{
	pair<float> cRet = i_cLHO;
	cRet.m_tX *= i_dRHO;
	cRet.m_tY *= i_dRHO;
	return cRet;
}
pair<float> operator *(const double &i_dRHO, const pair<float> & i_cLHO)
{
	pair<float> cRet = i_cLHO;
	cRet.m_tX *= i_dRHO;
	cRet.m_tY *= i_dRHO;
	return cRet;
}

pair<float> operator *=(pair<float> & i_cLHO, const double &i_dRHO)
{
	i_cLHO.m_tX *= i_dRHO;
	i_cLHO.m_tY *= i_dRHO;
	return i_cLHO;
}

pair<float> operator /(const pair<float> & i_cLHO, const double &i_dRHO)
{
	pair<float> cRet = i_cLHO;
	cRet.m_tX /= i_dRHO;
	cRet.m_tY /= i_dRHO;
	return cRet;
}

pair<float> operator /=(pair<float> & i_cLHO, const double &i_dRHO)
{
	i_cLHO.m_tX /= i_dRHO;
	i_cLHO.m_tY /= i_dRHO;
	return i_cLHO;
}
