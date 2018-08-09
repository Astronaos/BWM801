#include <bwm801.h>

using namespace bwm801;

triplet<float> bwm801::operator *(const triplet<float> & i_cLHO, const float &i_dRHO)
{
	triplet<float> cRet = i_cLHO;
	cRet.m_tX *= i_dRHO;
	cRet.m_tY *= i_dRHO;
	cRet.m_tZ *= i_dRHO;
	return cRet;
}
triplet<float> bwm801::operator *(const float &i_dRHO, const triplet<float> & i_cLHO)
{
	triplet<float> cRet = i_cLHO;
	cRet.m_tX *= i_dRHO;
	cRet.m_tY *= i_dRHO;
	cRet.m_tZ *= i_dRHO;
	return cRet;
}

triplet<float> bwm801::operator *=(triplet<float> & i_cLHO, const float &i_dRHO)
{
	i_cLHO.m_tX *= i_dRHO;
	i_cLHO.m_tY *= i_dRHO;
	i_cLHO.m_tZ *= i_dRHO;
	return i_cLHO;
}

triplet<float> bwm801::operator /(const triplet<float> & i_cLHO, const float &i_dRHO)
{
	triplet<float> cRet = i_cLHO;
	cRet.m_tX /= i_dRHO;
	cRet.m_tY /= i_dRHO;
	cRet.m_tZ /= i_dRHO;
	return cRet;
}

triplet<float> bwm801::operator /=(triplet<float> & i_cLHO, const float &i_dRHO)
{
	i_cLHO.m_tX /= i_dRHO;
	i_cLHO.m_tY /= i_dRHO;
	i_cLHO.m_tZ /= i_dRHO;
	return i_cLHO;
}
