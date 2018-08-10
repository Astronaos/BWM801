#pragma once

namespace bwm801
{
	class EXPORT date_time
	{
	public:
		int m_iYear;
		unsigned int m_uiMonth;
		unsigned int m_uiDay_of_Month;
		unsigned int m_uiDay_of_Week;
		unsigned int m_uiHour;
		unsigned int m_uiMinute;
		double		m_dSeconds;
	public:
		virtual void Get_Current(void);
	};
}

