#pragma once
#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif


namespace bwm801
{
	class EXPORT button_info
	{
	public:
		enum shape {ellipse,rectangle};
	private:
		shape			m_eShape;
		pair<double>	m_pdPosition;
		pair<double>	m_pdSize;
		unsigned int 	m_uiID;
		int				m_iStatus;
	public:
		virtual shape GetShape(void) const;
		virtual pair<double> GetPosition(void) const;
		virtual pair<double> GetSize(void) const;
		virtual unsigned int GetID(void) const;
		virtual int	GetStatus(void) const;
		virtual void	SetStatus(int i_iStatus);
		virtual bool TestHit(const pair<double> &i_pdPosition) const;
		virtual void Initializer(void);
		virtual void Initializer(shape i_eShape, const pair<double> i_pdPosition, const pair<double> i_pdSize, unsigned int i_uiID, unsigned int i_uiStatus = 0);
		button_info(void) {Initializer();}
		button_info(shape i_eShape, const pair<double> i_pdPosition, const pair<double> i_pdSize, unsigned int i_uiID, unsigned int i_uiStatus = 0) {Initializer(i_eShape, i_pdPosition, i_pdSize, i_uiID, i_uiStatus);}
	};
};

#undef EXPORT

