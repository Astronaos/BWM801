#pragma once
#include <bwm801.h>
#include <bwm801_mapbase.h>
#include <deque>

namespace bwm801
{
	template <typename T> class hexmap : public bwm801::mapbase<T>
	{
	private:
		calllist	m_clMap_Grid;

	public:
		std::deque<bwm801::pair<int> > Get_Path(const bwm801::pair<int> & i_pStart, const bwm801::pair<int> & i_pEnd) const;

		//void Determine_Hex(const pair<double> &i_tVitual_Coord, pair<int> & o_tGrid_Position) const;
		bwm801::pair<int> Determine_Hex(const bwm801::pair<float> &i_tVitual_Coord) const;
		void Get_Hex_Center(const bwm801::pair<int> & i_tGrid_Position, bwm801::pair<float> & o_tVitual_Coord) const;
		bwm801::pair<float> Get_Hex_Center(const bwm801::pair<int> & i_tGrid_Position) const;

		void Draw_Filled_Hex(const bwm801::pair<int> & i_tPosition) const;
		void Draw_Hex_Side(const bwm801::pair<int> & i_tPosition, unsigned int i_uiSide) const;
		void Draw_Hex_Outline(const bwm801::pair<int> & i_tPosition) const;

		void Draw_Grid(void);
		void Draw_Map(void * io_lpvData) const; // void * for user defined data that needs to be sent to each space for drawing

		void	Center_Map(const bwm801::pair<int> & i_tPosition);

	};
#include <bwm801_hexmap_inst.h>
}
