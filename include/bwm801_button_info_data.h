#pragma once
#include <bwm801.h>
namespace bwm801
{
	class button_info_data
	{
	public:
		button_info::shape	m_eShape;
		pair<float>			m_pdPosition;
		pair<float>			m_pdSize;
		unsigned int 		m_uiID;
		int					m_iStatus;

		button_info_data(void);
		button_info_data(button_info::shape i_eShape, const pair<float> i_pdPosition, const pair<float> i_pdSize, unsigned int i_uiID, int i_iStatus);
	};
}
