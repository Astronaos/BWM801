#include <core.hpp>

QUAD<double> operator *(const QUAD<double> & i_cLHO, const double &i_dRHO)
{
	QUAD<double> cRet = i_cLHO;
	cRet.m_tBL *= i_dRHO;
	cRet.m_tTR *= i_dRHO;
	return cRet;
}
QUAD<double> operator *(const double &i_dRHO, const QUAD<double> & i_cLHO)
{
	QUAD<double> cRet = i_cLHO;
	cRet.m_tBL *= i_dRHO;
	cRet.m_tTR *= i_dRHO;
	return cRet;
}

QUAD<double> operator *=(QUAD<double> & i_cLHO, const double &i_dRHO)
{
	i_cLHO.m_tBL *= i_dRHO;
	i_cLHO.m_tTR *= i_dRHO;
	return i_cLHO;
}

QUAD<double> operator /(const QUAD<double> & i_cLHO, const double &i_dRHO)
{
	QUAD<double> cRet = i_cLHO;
	cRet.m_tBL /= i_dRHO;
	cRet.m_tTR /= i_dRHO;
	return cRet;
}

QUAD<double> operator /=(QUAD<double> & i_cLHO, const double &i_dRHO)
{
	i_cLHO.m_tBL /= i_dRHO;
	i_cLHO.m_tTR /= i_dRHO;
	return i_cLHO;
}
