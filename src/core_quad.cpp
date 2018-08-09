#include <bwm801.h>
using namespace bwm801;

quad<float> operator *(const quad<float> & i_cLHO, const float &i_dRHO)
{
	quad<float> cRet = i_cLHO;
	cRet.m_tBL *= i_dRHO;
	cRet.m_tTR *= i_dRHO;
	return cRet;
}
quad<float> operator *(const float &i_dRHO, const quad<float> & i_cLHO)
{
	quad<float> cRet = i_cLHO;
	cRet.m_tBL *= i_dRHO;
	cRet.m_tTR *= i_dRHO;
	return cRet;
}

quad<float> operator *=(quad<float> & i_cLHO, const float &i_dRHO)
{
	i_cLHO.m_tBL *= i_dRHO;
	i_cLHO.m_tTR *= i_dRHO;
	return i_cLHO;
}

quad<float> operator /(const quad<float> & i_cLHO, const float &i_dRHO)
{
	quad<float> cRet = i_cLHO;
	cRet.m_tBL /= i_dRHO;
	cRet.m_tTR /= i_dRHO;
	return cRet;
}

quad<float> operator /=(quad<float> & i_cLHO, const float &i_dRHO)
{
	i_cLHO.m_tBL /= i_dRHO;
	i_cLHO.m_tTR /= i_dRHO;
	return i_cLHO;
}
