#include <core.hpp>

TRIPLET<double> operator *(const TRIPLET<double> & i_cLHO, const double &i_dRHO)
{
	TRIPLET<double> cRet = i_cLHO;
	cRet.m_tX *= i_dRHO;
	cRet.m_tY *= i_dRHO;
	cRet.m_tZ *= i_dRHO;
	return cRet;
}
TRIPLET<double> operator *(const double &i_dRHO, const TRIPLET<double> & i_cLHO)
{
	TRIPLET<double> cRet = i_cLHO;
	cRet.m_tX *= i_dRHO;
	cRet.m_tY *= i_dRHO;
	cRet.m_tZ *= i_dRHO;
	return cRet;
}

TRIPLET<double> operator *=(TRIPLET<double> & i_cLHO, const double &i_dRHO)
{
	i_cLHO.m_tX *= i_dRHO;
	i_cLHO.m_tY *= i_dRHO;
	i_cLHO.m_tZ *= i_dRHO;
	return i_cLHO;
}

TRIPLET<double> operator /(const TRIPLET<double> & i_cLHO, const double &i_dRHO)
{
	TRIPLET<double> cRet = i_cLHO;
	cRet.m_tX /= i_dRHO;
	cRet.m_tY /= i_dRHO;
	cRet.m_tZ /= i_dRHO;
	return cRet;
}

TRIPLET<double> operator /=(TRIPLET<double> & i_cLHO, const double &i_dRHO)
{
	i_cLHO.m_tX /= i_dRHO;
	i_cLHO.m_tY /= i_dRHO;
	i_cLHO.m_tZ /= i_dRHO;
	return i_cLHO;
}
