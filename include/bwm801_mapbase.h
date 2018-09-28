#pragma once
#include <bwm801.h>
#include<cmath>
#if defined (__MINGW32__) || defined (_WIN32)
#include <windows.h>
#include <windowsx.h>
#endif
#include<GL/gl.h>

namespace bwm801
{
	template<typename T> class mapbase
	{
	public:
		unsigned int 	m_uiNum_Layers;
		float			m_dZoom;
		bwm801::pair<unsigned int>	m_tGrid_Size;
		float			m_dR;
		float			m_dDisplay_Aspect_Ratio;
		bwm801::pair<float>	m_tScroll_Position;
		bwm801::pair<float>	m_tScroll_Limits[2];
	public:
		std::map<bwm801::pair<int>,T>	m_cMap;

		mapbase(void) {m_uiNum_Layers = 0; m_tGrid_Size = bwm801::pair<unsigned int>(0,0); m_dZoom = 1.0; m_dR = 0.1; m_dDisplay_Aspect_Ratio = 1.0; m_tScroll_Position = bwm801::pair<float>(0.5,0.5);}
		void	Set_Map_Size(const bwm801::pair<unsigned int> & i_tGrid_Size, unsigned int i_uiNum_Layers)
		{
			m_uiNum_Layers = i_uiNum_Layers;
			m_tGrid_Size = i_tGrid_Size;
			m_dZoom = 1.0;
		}
		void Set_Draw_Parameters(const float & i_dR, const float & i_dDisplay_Aspect_Ratio)
		{
			m_dR = i_dR;
			m_dDisplay_Aspect_Ratio = i_dDisplay_Aspect_Ratio;
	//		m_tScroll_Position = m_tGrid_Size * m_dR * 0.5; // center map on center of map
			Center_Map(m_tGrid_Size * 0.5);
			Set_Scroll_Limits();
			Check_Scroll_Limits();
		}
		void Set_Scroll_Limits(void)
		{
			m_tScroll_Limits[0].m_tX = 0.5 * m_dDisplay_Aspect_Ratio;
			m_tScroll_Limits[0].m_tY = 0.5;
			m_tScroll_Limits[1].m_tX = m_tGrid_Size.m_tX * m_dR * m_dZoom - m_dDisplay_Aspect_Ratio;
			m_tScroll_Limits[1].m_tY = m_tGrid_Size.m_tY * m_dR * m_dZoom - 0.5;

			if (m_tScroll_Limits[1].m_tX < m_tScroll_Limits[0].m_tX)
			{
				//float dPosition = 0.5 * (m_tScroll_Limits[0].m_tX + m_tScroll_Limits[1].m_tX);
				m_tScroll_Limits[0].m_tX = m_tScroll_Limits[1].m_tX = 0.5 * m_dDisplay_Aspect_Ratio;
			}
			if (m_tScroll_Limits[1].m_tY < m_tScroll_Limits[0].m_tY)
			{
				//float dPosition = 0.5 * (m_tScroll_Limits[0].m_tY + m_tScroll_Limits[1].m_tY);
				m_tScroll_Limits[0].m_tY = m_tScroll_Limits[1].m_tY = 0.50;
			}
		}
		void Check_Scroll_Limits(void)
		{
			if (m_tScroll_Position.m_tX < m_tScroll_Limits[0].m_tX)
				m_tScroll_Position.m_tX = m_tScroll_Limits[0].m_tX;
			if (m_tScroll_Position.m_tX > m_tScroll_Limits[1].m_tX)
				m_tScroll_Position.m_tX = m_tScroll_Limits[1].m_tX;

			if (m_tScroll_Position.m_tY < m_tScroll_Limits[0].m_tY)
				m_tScroll_Position.m_tY = m_tScroll_Limits[0].m_tY;
			if (m_tScroll_Position.m_tY > m_tScroll_Limits[1].m_tY)
				m_tScroll_Position.m_tY = m_tScroll_Limits[1].m_tY;
		}
		void	Set_Map_Zoom(const float & i_dZoom)
		{
			m_dZoom = i_dZoom;
			Set_Scroll_Limits();
			Check_Scroll_Limits();
		}
		void	Shift_Zoom(const float & i_dDelta)
		{
			m_dZoom *= i_dDelta;
			if (m_dZoom == 0.0)
				m_dZoom = 1.0;
			Set_Scroll_Limits();
			Check_Scroll_Limits();
		}
		void	Set_Map_Scroll_Limits(const bwm801::pair<float> & m_tLower_Limits, const bwm801::pair<float> & m_tUpper_Limits)
		{
			m_tScroll_Limits[0] = m_tLower_Limits;
			m_tScroll_Limits[1] = m_tUpper_Limits;
		}
		void	Shift_Map(const bwm801::pair<float> & i_tDelta)
		{
			m_tScroll_Position += i_tDelta;
			Check_Scroll_Limits();
		}
		float	Get_R(void) const {return m_dR;}


		unsigned int Factorial(unsigned int i_uiN) const;
		unsigned int Binomial_Coefficient(unsigned int i_uiN, unsigned int i_uiK) const;
		bwm801::pair<float> Bezier(const std::vector<bwm801::pair<float> > & i_vPoints, const float & i_dT) const;
		void Draw_Bezier(const std::vector<bwm801::pair<float> > & i_vPoints, unsigned int i_uiResolution = 128) const;

		virtual void Center_Map(const bwm801::pair<int> & i_tPosition) = 0;
		virtual void Draw_Map(void * io_lpvData) const = 0;
		virtual void Prepare_Draw(void) const;
	};

#include <bwm801_mapbase_inst.h>
}
