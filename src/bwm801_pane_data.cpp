#include <bwm801.h>
#include <bwm801_pane_data.h>

using namespace bwm801;

pane_data::pane_data(void)
{
	m_idID = (unsigned int)-1;
	m_uiUser_ID = (unsigned int)-1;
	m_iLevel = 0;
}
pane_data::pane_data(pane_id i_idID)
{
	m_idID = i_idID;
	m_uiUser_ID = (unsigned int)-1;
	m_iLevel = 0;
}
pane_data::pane_data(pane_id i_idID, const quad<unsigned int> i_qPosition, bool i_bVisible, unsigned int i_uiUser_ID)
{
	m_idID = i_idID;
	m_qPosition = i_qPosition;
	m_bVisible = i_bVisible;
	m_uiUser_ID = i_uiUser_ID;
	m_iLevel = 0;
}
