#include <bwm801.h>
#include <cmath>

using namespace bwm801;


button_info::shape button_info::GetShape(void) const
{
	return m_eShape;
}
pair<double> button_info::GetPosition(void) const
{
	return m_pdPosition;
}
pair<double> button_info::GetSize(void) const
{
	return m_pdSize;
}
unsigned int button_info::GetID(void) const
{ 
	return m_uiID;
}
int	button_info::GetStatus(void) const
{
	return m_iStatus;
}
void	button_info::SetStatus(int i_iStatus)
{
	m_iStatus = i_iStatus;
}
bool button_info::TestHit(const pair<double> &i_pdPosition) const
{
	pair<double> pOffset = i_pdPosition - m_pdPosition;
	bool bHit = false;
	switch (m_eShape)
	{
	case rectangle:
		bHit = pOffset.m_tX >= 0.0 && pOffset.m_tX <= m_pdSize.m_tX && pOffset.m_tY >= -m_pdSize.m_tY && pOffset.m_tY <= 0.0;
		break;
	case ellipse:
		bHit = fabs(pOffset.m_tX) <= (m_pdSize.m_tX) && fabs(pOffset.m_tY) <= (m_pdSize.m_tY);
		if (bHit) 
		{
			double dX_a = pOffset.m_tX / m_pdSize.m_tX;
			double dY = m_pdSize.m_tY * sqrt(1.0 - dX_a * dX_a);
			bHit = fabs(pOffset.m_tY) <= dY;
		}
		break;
	}
	return bHit;
}
void button_info::Initializer(void)
{
	m_eShape = rectangle;
	m_pdPosition = pair<double>(0,0);
	m_pdSize = pair<double>(0,0);
	m_uiID = 0;
	m_iStatus = 0;
}
void button_info::Initializer(button_info::shape i_eShape, const pair<double> i_pdPosition, const pair<double> i_pdSize, unsigned int i_uiID, unsigned int i_uiStatus)
{
	m_eShape = i_eShape;
	m_pdPosition = i_pdPosition;
	m_pdSize = i_pdSize;
	m_uiID = i_uiID;
	m_iStatus = i_uiStatus;
}
