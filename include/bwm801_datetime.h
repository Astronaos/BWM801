#pragma once
#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace bwm801
{
	class EXPORT date_time
	{
	public:
		unsigned int m_uiYear;
		unsigned int m_uiMonth;
		unsigned int m_uiDay_of_Month;
		unsigned int m_uiDay_of_Week;
		unsigned int m_uiHour;
		unsigned int m_uiMinute;
		double		m_dSeconds;
	public:
		virtual void Get_Current(void);
	};
};

#undef EXPORT
