#include <Template.hpp>
#include <string>
#include <vector>

TEMPLATE_MAIN	g_cTemplate_Main;

void TEMPLATE_MAIN::init(void) // initialization routine; rendering context not created
{
	m_idPane = Register_Pane(true);

	m_dTimer = 0.0;
	m_bFlasher_1s_50p = true;

}
void TEMPLATE_MAIN::close(void) // program exiting; rendering context destroyed
{
}

