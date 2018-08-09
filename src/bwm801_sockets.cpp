#include <bwm801.h>

using namespace bwm801;

bool	commsocket::Is_Connected(void)
{
	return m_bIs_Connected;
}
bool	commsocket::Is_Initialized(void)
{
	return m_bIs_Initialized;
}

void commsocket::Initializer(void)
{
	m_eType = stream;
	m_bAllow_Blocking = true;
	m_bIs_Initialized = false;
	m_bIs_Connected = false;
	m_lpvSystem_Info = nullptr;
	m_uiMax_Tries = 20;
}


void commsocket::Destructor(void)
{
	Close();
}

