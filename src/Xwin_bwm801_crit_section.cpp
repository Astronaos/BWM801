#include <bwm801.h>
#include <unistd.h>
using namespace bwm801;

void criticalsection::Set(void)
{
	bool * lpbPtr = (bool *) m_lpvCS_Data;
	while (lpbPtr[0]) 
		usleep(1); 
	lpbPtr[0] = true;
}
void criticalsection::Initializer(void)
{
	m_lpvCS_Data = (void *)(new bool[1]); 
	((bool *)m_lpvCS_Data)[0] = false;
}
void criticalsection::Destructor(void)
{
	bool * lpbPtr = (bool *) m_lpvCS_Data;
	delete [] lpbPtr;
	lpbPtr = nullptr;
}

void criticalsection::Unset(void)
{
	((bool *) m_lpvCS_Data)[0] = false;
}
bool criticalsection::Get(void) const
{
	return ((bool *) m_lpvCS_Data)[0];
}
