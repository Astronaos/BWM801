#include <core.hpp>

PAIR<double> operator *(const PAIR<double> & i_cLHO, const double &i_dRHO)
{
	PAIR<double> cRet = i_cLHO;
	cRet.m_tX *= i_dRHO;
	cRet.m_tY *= i_dRHO;
	return cRet;
}
PAIR<double> operator *(const double &i_dRHO, const PAIR<double> & i_cLHO)
{
	PAIR<double> cRet = i_cLHO;
	cRet.m_tX *= i_dRHO;
	cRet.m_tY *= i_dRHO;
	return cRet;
}

PAIR<double> operator *=(PAIR<double> & i_cLHO, const double &i_dRHO)
{
	i_cLHO.m_tX *= i_dRHO;
	i_cLHO.m_tY *= i_dRHO;
	return i_cLHO;
}

PAIR<double> operator /(const PAIR<double> & i_cLHO, const double &i_dRHO)
{
	PAIR<double> cRet = i_cLHO;
	cRet.m_tX /= i_dRHO;
	cRet.m_tY /= i_dRHO;
	return cRet;
}

PAIR<double> operator /=(PAIR<double> & i_cLHO, const double &i_dRHO)
{
	i_cLHO.m_tX /= i_dRHO;
	i_cLHO.m_tY /= i_dRHO;
	return i_cLHO;
}
