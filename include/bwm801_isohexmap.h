#pragma once
#include <bwm801.h>
#include <bwm801_mapbase.h>

namespace bwm801
{
	template <typename T> class isometric_hexmap : public bwm801::mapbase<T>
	{
	private:
		float m_dCamera_Isometric_Angle;
		float m_dCamera_Rotation_Angle;
		bwm801::calllist	m_clMap_Grid;
	public:
		float Get_Isometric_Camera_Angle(void) const{return m_dCamera_Isometric_Angle;}
		float Get_Camera_Rotation_Angle(void) const{return m_dCamera_Rotation_Angle;}

		isometric_hexmap(void){m_dCamera_Isometric_Angle = 60.0;  m_dCamera_Rotation_Angle = 0.0; /* 30^deg for iso angle */}

		std::deque<pair<int> > Get_Path(const pair<int> & i_pStart, const pair<int> & i_pEnd) const;

		//void Determine_Hex(const pair<float> &i_tVitual_Coord, pair<int> & o_tGrid_Position) const;
		bwm801::pair<int> Determine_Hex(const bwm801::pair<float> &i_tVitual_Coord) const;
		void Get_Hex_Center(const bwm801::pair<int> & i_tGrid_Position, bwm801::pair<float> & o_tVitual_Coord) const;
		bwm801::pair<float> Get_Hex_Center(const bwm801::pair<int> & i_tGrid_Position) const;

		void Draw_Filled_Hex(const bwm801::pair<int> & i_tPosition) const;
		void Draw_Hex_Side(const bwm801::pair<int> & i_tPosition, unsigned int i_uiSide) const;
		void Draw_Hex_Outline(const bwm801::pair<int> & i_tPosition) const;

		void Draw_Grid(void);
		void Draw_Map(void * io_lpvData) const; // void * for user defined data that needs to be sent to each space for drawing

		void	Center_Map(const bwm801::pair<int> & i_tPosition);
		void	Change_Isometric_Angle(const float & i_dDelta_Angle)
		{
			m_dCamera_Isometric_Angle += i_dDelta_Angle;
			if (m_dCamera_Isometric_Angle > 90.0)
				m_dCamera_Isometric_Angle = 90.0;
			if (m_dCamera_Isometric_Angle < 0.0)
				m_dCamera_Isometric_Angle = 0.0;
		}
		void	Change_Camera_Angle(const float & i_dDelta_Angle)
		{
			float dAngle = fmod(m_dCamera_Rotation_Angle + i_dDelta_Angle,360.0);
			m_dCamera_Rotation_Angle = dAngle;
		}
		void Prepare_Draw(void) const;

	};
	#include <bwm801_isohexmap_inst.h>
}
