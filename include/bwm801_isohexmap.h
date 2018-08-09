#pragma once
namespace bwm801
{
	template <typename T> class isometric_hexmap : public mapbase<T>
	{
	private:
		double m_dCamera_Isometric_Angle;
		double m_dCamera_Rotation_Angle;
		calllist	m_clMap_Grid;
	public:
		double Get_Isometric_Camera_Angle(void){return m_dCamera_Isometric_Angle;}
		double Get_Camera_Rotation_Angle(void){return m_dCamera_Rotation_Angle;}

		isometric_hexmap(void){m_dCamera_Isometric_Angle = 60.0;  m_dCamera_Rotation_Angle = 0.0; /* 30^deg for iso angle */}

		std::deque<pair<int> > Get_Path(const pair<int> & i_pStart, const pair<int> & i_pEnd) const;

		//void Determine_Hex(const pair<double> &i_tVitual_Coord, pair<int> & o_tGrid_Position) const;
		pair<int> Determine_Hex(const pair<double> &i_tVitual_Coord) const;
		void Get_Hex_Center(const pair<int> & i_tGrid_Position, pair<double> & o_tVitual_Coord) const;
		pair<double> Get_Hex_Center(const pair<int> & i_tGrid_Position) const;

		void Draw_Filled_Hex(const pair<int> & i_tPosition) const;
		void Draw_Hex_Side(const pair<int> & i_tPosition, unsigned int i_uiSide) const;
		void Draw_Hex_Outline(const pair<int> & i_tPosition) const;

		void Draw_Grid(void);
		void Draw_Map(void * io_lpvData) const; // void * for user defined data that needs to be sent to each space for drawing

		void	Center_Map(const pair<int> & i_tPosition);
		void	Change_Isometric_Angle(const double & i_dDelta_Angle)
		{
			m_dCamera_Isometric_Angle += i_dDelta_Angle;
			if (m_dCamera_Isometric_Angle > 90.0)
				m_dCamera_Isometric_Angle = 90.0;
			if (m_dCamera_Isometric_Angle < 0.0)
				m_dCamera_Isometric_Angle = 0.0;
		}
		void	Change_Camera_Angle(const double & i_dDelta_Angle)
		{
			double dAngle = fmod(m_dCamera_Rotation_Angle + i_dDelta_Angle,360.0);
			m_dCamera_Rotation_Angle = dAngle;
		}
		void Prepare_Draw(void) const;

	};
	#include <bwm801_isohexmap_inst.h>
};
