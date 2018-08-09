#include <bwm801.h>
#include <bwm801_button_info_data.h>
#include <cmath>

using namespace bwm801;


button_info::shape button_info::GetShape(void) const
{
	return ((button_info_data*)m_lpvData)->m_eShape;
}
pair<double> button_info::GetPosition(void) const
{
	return ((button_info_data*)m_lpvData)->m_pdPosition;
}
pair<double> button_info::GetSize(void) const
{
	return ((button_info_data*)m_lpvData)->m_pdSize;
}
unsigned int button_info::GetID(void) const
{ 
	return ((button_info_data*)m_lpvData)->m_uiID;
}
int	button_info::GetStatus(void) const
{
	return ((button_info_data*)m_lpvData)->m_iStatus;
}
void	button_info::SetStatus(int i_iStatus)
{
	((button_info_data*)m_lpvData)->m_iStatus = i_iStatus;
}
bool button_info::TestHit(const pair<float> &i_pdPosition) const
{
	pair<float> pOffset = i_pdPosition - ((button_info_data*)m_lpvData)->m_pdPosition;
	bool bHit = false;
	switch (((button_info_data*)m_lpvData)->m_eShape)
	{
	case rectangle:
		bHit = pOffset.m_tX >= 0.0 && pOffset.m_tX <= ((button_info_data*)m_lpvData)->m_pdSize.m_tX && pOffset.m_tY >= -((button_info_data*)m_lpvData)->m_pdSize.m_tY && pOffset.m_tY <= 0.0;
		break;
	case ellipse:
		bHit = fabs(pOffset.m_tX) <= (((button_info_data*)m_lpvData)->m_pdSize.m_tX) && fabs(pOffset.m_tY) <= (((button_info_data*)m_lpvData)->m_pdSize.m_tY);
		if (bHit) 
		{
			double dX_a = pOffset.m_tX / ((button_info_data*)m_lpvData)->m_pdSize.m_tX;
			double dY = ((button_info_data*)m_lpvData)->m_pdSize.m_tY * sqrt(1.0 - dX_a * dX_a);
			bHit = fabs(pOffset.m_tY) <= dY;
		}
		break;
	}
	return bHit;
}
void button_info::Initializer(void)
{
	m_lpvData = new button_info_data();
}
void button_info::Initializer(const button_info & i_cRHO)
{
	m_lpvData = new button_info_data(*((button_info_data*)i_cRHO.m_lpvData));
}
void button_info::Initializer(button_info::shape i_eShape, const pair<float> i_pdPosition, const pair<float> i_pdSize, unsigned int i_uiID, unsigned int i_uiStatus)
{
	m_lpvData = new button_info_data(i_eShape, i_pdPosition, i_pdSize, i_uiID, i_uiStatus);
}
void button_info::Destructor(void)
{
	button_info_data * lpbiData = (button_info_data *)m_lpvData;
	if (lpbiData != nullptr)
		delete lpbiData;
	m_lpvData = nullptr;
}
