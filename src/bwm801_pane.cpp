#include <bwm801.h>
#include <bwm801_pane_data.h>

using namespace bwm801;

void pane::Initializer(void)
{
	m_lpvPane_Data = new pane_data();
}
void pane::Initializer(const pane & i_cRHO)
{
	m_lpvPane_Data = new pane_data(*((pane_data*)i_cRHO.m_lpvPane_Data));
}
void pane::Initializer(pane_id i_idID)
{
	m_lpvPane_Data = new pane_data(i_idID);
}
void pane::Initializer(pane_id i_idID, const quad<unsigned int> i_qPosition, bool i_bVisible, unsigned int i_uiUser_ID)
{
	m_lpvPane_Data = new pane_data(i_idID, i_qPosition, i_bVisible, i_uiUser_ID);
}
void pane::Destructor(void)
{
	pane_data * lppdData = (pane_data*)m_lpvPane_Data;
	if (lppdData != nullptr)
		delete lppdData;
	m_lpvPane_Data = nullptr;
}

pane_id pane::Get_ID(void) const { return ((pane_data*)m_lpvPane_Data)->m_idID;}

bool	pane::Is_In_Pane(const pair<unsigned int> &i_tLocation) const
{
	return (i_tLocation.m_tX >= ((pane_data*)m_lpvPane_Data)->m_qPosition.m_tBL.m_tX && i_tLocation.m_tX <= ((pane_data*)m_lpvPane_Data)->m_qPosition.m_tTR.m_tX && i_tLocation.m_tY >= ((pane_data*)m_lpvPane_Data)->m_qPosition.m_tBL.m_tY && i_tLocation.m_tY <= ((pane_data*)m_lpvPane_Data)->m_qPosition.m_tTR.m_tY);
}

void pane::Set_Clipping(void) const
{	// note: I tested glScissor vs glViewport on windows 7; glScissor is about 2x faster for the same view area. (render time ~ 0.0009s for viewport and 0.0005s for scissor)
	glPushAttrib(GL_SCISSOR_BIT);              // Pushes The Display List Bits     ( NEW )
	glScissor(((pane_data*)m_lpvPane_Data)->m_qPosition.m_tBL.m_tX, ((pane_data*)m_lpvPane_Data)->m_qPosition.m_tBL.m_tY, ((pane_data*)m_lpvPane_Data)->m_qPosition.Get_Size().m_tX, ((pane_data*)m_lpvPane_Data)->m_qPosition.Get_Size().m_tY);
	glEnable(GL_SCISSOR_TEST);
}
void pane::End_Clipping(void) const
{
	glDisable(GL_SCISSOR_TEST);
	glPopAttrib();              // Pushes The Display List Bits     ( NEW )
}

bool	pane::Is_Visible(void) const
{
	return ((pane_data*)m_lpvPane_Data)->m_bVisible;
}
void	pane::Set_Visibility(bool i_bVisibility)
{
	((pane_data*)m_lpvPane_Data)->m_bVisible = i_bVisibility;
}
unsigned int pane::Get_User_ID(void) const
{
	return ((pane_data*)m_lpvPane_Data)->m_uiUser_ID;
}

void	pane::Set_User_ID(unsigned int i_uiUser_ID)
{
	((pane_data*)m_lpvPane_Data)->m_uiUser_ID = i_uiUser_ID;
}
int		pane::Get_Level(void) const
{
	return((pane_data*)m_lpvPane_Data)->m_iLevel;
}
void	pane::Set_Level(int i_iLevel)
{
	((pane_data*)m_lpvPane_Data)->m_iLevel = i_iLevel;
}

void	pane::Pull_To_Front(void)
{
	((pane_data*)m_lpvPane_Data)->m_iLevel++;
}
void	pane::Push_To_Back(void)
{
	((pane_data*)m_lpvPane_Data)->m_iLevel--;
}


quad<unsigned int> pane::Get_Position(void) const
{
	return ((pane_data*)m_lpvPane_Data)->m_qPosition;
}
void pane::Set_Position(const quad<unsigned int> & i_qPosition)
{
	((pane_data*)m_lpvPane_Data)->m_qPosition = i_qPosition;
}
