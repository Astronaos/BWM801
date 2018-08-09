#pragma once
#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#define PANE_ID_PREFIX		0x0010000
#define PANE_ID_IDX_MASK	0x000ffff
#define PANE_ID_NULL		0x0000000

namespace bwm801
{
	typedef unsigned int pane_id;
	class EXPORT pane
	{
	private:
		void *				m_lpvPane_Data;
	public:

		pane(void) { m_lpvPane_Data = nullptr;  Initializer(); }
		pane(const pane & i_cRHO) { m_lpvPane_Data = nullptr;  Initializer(i_cRHO); }
		pane(pane_id i_idID) { m_lpvPane_Data = nullptr; Initializer(i_idID);}
		pane(pane_id i_idID, const quad<unsigned int> i_qPosition, bool i_bVisible, unsigned int i_uiUser_ID) { m_lpvPane_Data = nullptr; Initializer(i_idID,i_qPosition,i_bVisible,i_uiUser_ID);}
		~pane(void) {Destructor();}
		virtual void 	Initializer(void);
		virtual void 	Initializer(const pane & i_cRHO);
		virtual void 	Initializer(pane_id i_idID);
		virtual void 	Initializer(pane_id i_idID, const quad<unsigned int> i_qPosition, bool i_bVisible, unsigned int i_uiUser_ID);
		virtual void	Destructor(void);

		virtual pane_id Get_ID(void) const;

		virtual bool	Is_In_Pane(const pair<unsigned int> &i_tLocation) const;
		virtual void 	Set_Clipping(void) const;
		virtual void 	End_Clipping(void) const;

		virtual quad<unsigned int> Get_Position(void) const;
		virtual void Set_Position(const quad<unsigned int> & i_qPosition);

		virtual bool	Is_Visible(void) const;
		virtual void	Set_Visibility(bool i_bVisibility);
		virtual unsigned int Get_User_ID(void) const;
		virtual void	Set_User_ID(unsigned int i_uiUser_ID);
		virtual int		Get_Level(void) const;
		virtual void	Set_Level(int i_iLevel);
		virtual void	Pull_To_Front(void);
		virtual void	Push_To_Back(void);
		
	};
};

#undef EXPORT
