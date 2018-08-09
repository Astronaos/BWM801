#include <bwm801.h>

using namespace bwm801;

void pane::Initializer(void)
{
	m_idID = -1;
	m_uiUser_ID = -1;
	m_iLevel = 0;
}
void pane::Initializer(pane_id i_idID)
{
	m_idID = i_idID;
	m_uiUser_ID = -1;
	m_iLevel = 0;
}
void pane::Initializer(pane_id i_idID, const quad<unsigned int> i_qPosition, bool i_bVisible, unsigned int i_uiUser_ID)
{
	m_idID = i_idID;
	m_qPosition = i_qPosition;
	m_bVisible = i_bVisible;
	m_uiUser_ID = i_uiUser_ID;
	m_iLevel = 0;
}

pane_id pane::Get_ID(void) const { return m_idID;}

bool	pane::Is_In_Pane(const pair<unsigned int> &i_tLocation) const
{
	return (i_tLocation.m_tX >= m_qPosition.m_tBL.m_tX && i_tLocation.m_tX <= m_qPosition.m_tTR.m_tX && i_tLocation.m_tY >= m_qPosition.m_tBL.m_tY && i_tLocation.m_tY <= m_qPosition.m_tTR.m_tY);
}

void pane::Set_Clipping(void) const
{	// note: I tested glScissor vs glViewport on windows 7; glScissor is about 2x faster for the same view area. (render time ~ 0.0009s for viewport and 0.0005s for scissor)
	glPushAttrib(GL_SCISSOR_BIT);              // Pushes The Display List Bits     ( NEW )
	glScissor(m_qPosition.m_tBL.m_tX,m_qPosition.m_tBL.m_tY,m_qPosition.Get_Size().m_tX,m_qPosition.Get_Size().m_tY);
	glEnable(GL_SCISSOR_TEST);
}
void pane::End_Clipping(void) const
{
	glDisable(GL_SCISSOR_TEST);
	glPopAttrib();              // Pushes The Display List Bits     ( NEW )
}

bool	pane::Is_Visible(void) const
{
	return m_bVisible;
}
void	pane::Set_Visibility(bool i_bVisibility)
{
	m_bVisible = i_bVisibility;
}
unsigned int pane::Get_User_ID(void) const
{
	return m_uiUser_ID;
}

void	pane::Set_User_ID(unsigned int i_uiUser_ID)
{
	m_uiUser_ID = i_uiUser_ID;
}
int		pane::Get_Level(void) const
{
	return m_iLevel;
}
void	pane::Set_Level(int i_iLevel)
{
	m_iLevel = i_iLevel;
}

void	pane::Pull_To_Front(void)
{
	m_iLevel++;
}
void	pane::Push_To_Back(void)
{
	m_iLevel--;
}

