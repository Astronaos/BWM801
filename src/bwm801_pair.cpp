#include <bwm801.h>

using namespace bwm801;

pair<float> bwm801::operator *(const pair<float> & i_cLHO, const float &i_dRHO)
{
	pair<float> cRet = i_cLHO;
	cRet.m_tX = (float)(i_cLHO.m_tX * i_dRHO);
	cRet.m_tY = (float)(i_cLHO.m_tY * i_dRHO);
	return cRet;
}
pair<float> bwm801::operator *(const float &i_dRHO, const pair<float> & i_cLHO)
{
	pair<float> cRet = i_cLHO;
	cRet.m_tX = (float)(i_cLHO.m_tX * i_dRHO);
	cRet.m_tY = (float)(i_cLHO.m_tY * i_dRHO);
	return cRet;
}

pair<float> bwm801::operator *=(pair<float> & i_cLHO, const float &i_dRHO)
{
	i_cLHO.m_tX = (float)(i_cLHO.m_tX * i_dRHO);
	i_cLHO.m_tY = (float)(i_cLHO.m_tY * i_dRHO);
	return i_cLHO;
}

pair<float> bwm801::operator /(const pair<float> & i_cLHO, const float &i_dRHO)
{
	pair<float> cRet = i_cLHO;
	cRet.m_tX = (float)(i_cLHO.m_tX / i_dRHO);
	cRet.m_tY = (float)(i_cLHO.m_tY / i_dRHO);
	return cRet;
}

pair<float> bwm801::operator /=(pair<float> & i_cLHO, const float &i_dRHO)
{
	i_cLHO.m_tX = (float)(i_cLHO.m_tX / i_dRHO);
	i_cLHO.m_tY = (float)(i_cLHO.m_tY / i_dRHO);
	return i_cLHO;
}
