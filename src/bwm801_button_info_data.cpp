#include <bwm801.h>
#include <bwm801_button_info_data.h>

using namespace bwm801;

button_info_data::button_info_data(void)
{
	m_eShape = button_info::rectangle;
	m_pdPosition = pair<float>(0, 0);
	m_pdSize = pair<float>(0, 0);
	m_uiID = 0;
	m_iStatus = 0;
}
button_info_data::button_info_data(button_info::shape i_eShape, const pair<float> i_pdPosition, const pair<float> i_pdSize, unsigned int i_uiID, unsigned int i_uiStatus)
{
	m_eShape = i_eShape;
	m_pdPosition = i_pdPosition;
	m_pdSize = i_pdSize;
	m_uiID = i_uiID;
	m_iStatus = i_uiStatus;
}
