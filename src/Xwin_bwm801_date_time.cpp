#include <bwm801.h>

using namespace bwm801;

void date_time::Get_Current(void)
{
	time_t t = time(nullptr);
	struct tm tm = *localtime(&t);
	m_uiYear = tm.tm_year + 1900;
	m_uiMonth = tm.tm_mon + 1;
	m_uiDay_of_Month = tm.tm_mday;
	m_uiDay_of_Week;
	m_uiHour = tm.tm_hour;
	m_uiMinute = tm.tm_min;
	m_dSeconds = tm.tm_sec;
}

