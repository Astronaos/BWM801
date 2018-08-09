#include <bwm801.h>

using namespace bwm801;

triplet<float> operator *(const triplet<float> & i_cLHO, const float &i_dRHO)
{
	triplet<float> cRet = i_cLHO;
	cRet.m_tX *= i_dRHO;
	cRet.m_tY *= i_dRHO;
	cRet.m_tZ *= i_dRHO;
	return cRet;
}
triplet<float> operator *(const float &i_dRHO, const triplet<float> & i_cLHO)
{
	triplet<float> cRet = i_cLHO;
	cRet.m_tX *= i_dRHO;
	cRet.m_tY *= i_dRHO;
	cRet.m_tZ *= i_dRHO;
	return cRet;
}

triplet<float> operator *=(triplet<float> & i_cLHO, const float &i_dRHO)
{
	i_cLHO.m_tX *= i_dRHO;
	i_cLHO.m_tY *= i_dRHO;
	i_cLHO.m_tZ *= i_dRHO;
	return i_cLHO;
}

triplet<float> operator /(const triplet<float> & i_cLHO, const float &i_dRHO)
{
	triplet<float> cRet = i_cLHO;
	cRet.m_tX /= i_dRHO;
	cRet.m_tY /= i_dRHO;
	cRet.m_tZ /= i_dRHO;
	return cRet;
}

triplet<float> operator /=(triplet<float> & i_cLHO, const float &i_dRHO)
{
	i_cLHO.m_tX /= i_dRHO;
	i_cLHO.m_tY /= i_dRHO;
	i_cLHO.m_tZ /= i_dRHO;
	return i_cLHO;
}
