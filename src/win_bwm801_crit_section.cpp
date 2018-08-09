#include <bwm801.h>
using namespace bwm801;

void criticalsection::Set(void)
{
	CRITICAL_SECTION * lpbPtr = (CRITICAL_SECTION  *) m_lpvCS_Data; 
	EnterCriticalSection(lpbPtr);
}
criticalsection::criticalsection(void)
{
	m_lpvCS_Data = (void *)(new CRITICAL_SECTION[1]);
	CRITICAL_SECTION * lpbPtr = (CRITICAL_SECTION  *) m_lpvCS_Data; 
	InitializeCriticalSection(lpbPtr);
}
criticalsection::~criticalsection(void)
{
	CRITICAL_SECTION * lpbPtr = (CRITICAL_SECTION  *) m_lpvCS_Data; 
	DeleteCriticalSection(lpbPtr);
	delete [] lpbPtr;
}

void criticalsection::Unset(void)
{
	CRITICAL_SECTION * lpbPtr = (CRITICAL_SECTION  *) m_lpvCS_Data; 
	LeaveCriticalSection(lpbPtr);
}
bool criticalsection::Get(void) const 
{
	CRITICAL_SECTION * lpbPtr = (CRITICAL_SECTION  *) m_lpvCS_Data; 
	return (TryEnterCriticalSection(lpbPtr) != 0);
}
