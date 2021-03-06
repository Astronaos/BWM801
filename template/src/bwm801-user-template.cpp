#include <bwm801-user-template.h>
#include <string>
#include <vector>

using namespace bwm801;

user_template_main	g_cMain; // there must be an instantiation of your main class somewhere, this is a good place to do it. 

void user_template_main::init(void) // initialization routine; rendering context not created
{
	Set_Screenshot_Default_Filename("screenshot");

	m_idPane = Register_Pane(true);

	m_dTimer = 0.0;
	m_bQuit_Request_Pending = false;
}
void user_template_main::close(void) // program exiting; rendering context destroyed
{
	// a good place to do any deallocations of your data
}

