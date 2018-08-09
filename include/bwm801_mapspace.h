#pragma once
#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
namespace bwm801
{
	class EXPORT mapspace
	{
	private:
		static GLuint m_uiHex_Background_List;
		static GLuint m_uiHex_Side_List[6];
		static GLuint m_uiHex_Outline_List;
		static GLuint m_uiQuad_Background_List;
		static GLuint m_uiQuad_Side_List[4];
		static GLuint m_uiQuad_Outline_List;

	public:
		pair<int>	m_tPosition;

		virtual void	Init(void);

		virtual void	Draw_Layer(unsigned int i_uiLayer, void * io_lpvData) const = 0;

		virtual void Initializer(const pair<int> & i_tPosition) {m_tPosition = i_tPosition;};

		mapspace(void) {};
		mapspace(const pair<int> & i_tPosition) {Initializer(i_tPosition);};

		virtual void Draw_Filled_Hex(void) const;
		virtual void Draw_Hex_Side(unsigned int i_uiSide) const;
		virtual void Draw_Hex_Outline(void) const;
		virtual pair<double> Get_Hex_Vertex(unsigned int i_uiSide) const;

		virtual void Draw_Filled_Quad(void) const;
		virtual void Draw_Quad_Side(unsigned int i_uiSide) const;
		virtual void Draw_Quad_Outline(void) const;
	};
};
#undef EXPORT
