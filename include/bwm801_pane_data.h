#pragma once
#include <bwm801.h>
namespace bwm801
{
	class pane_data
	{
	public:
		pane_id				m_idID;
		quad<unsigned int> 	m_qPosition;
		bool				m_bVisible;
		unsigned int 		m_uiUser_ID;
		int					m_iLevel;

		pane_data(void);
		pane_data(pane_id i_idID);
		pane_data(pane_id i_idID, const quad<unsigned int> i_qPosition, bool i_bVisible, unsigned int i_uiUser_ID);

	};
}
