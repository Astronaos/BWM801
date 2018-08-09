#pragma once
namespace bwm801
{
	template <typename T> class hexmap : public mapbase<T>
	{
	private:
		calllist	m_clMap_Grid;

	public:
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

	};
#include <bwm801_hexmap_inst.h>
};
