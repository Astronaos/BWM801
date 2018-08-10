#pragma once

namespace bwm801
{
	class EXPORT button_info
	{
	public:
		enum shape {ellipse,rectangle};
	private:
		void * m_lpvData;
	public:
		virtual shape GetShape(void) const;
		virtual pair<double> GetPosition(void) const;
		virtual pair<double> GetSize(void) const;
		virtual unsigned int GetID(void) const;
		virtual int	GetStatus(void) const;
		virtual void	SetStatus(int i_iStatus);
		virtual bool TestHit(const pair<float> &i_pdPosition) const;
		virtual void Initializer(void);
		virtual void Initializer(const button_info & i_cRHO);
		virtual void Initializer(shape i_eShape, const pair<float> i_pdPosition, const pair<float> i_pdSize, unsigned int i_uiID, int i_iStatus = 0);
		virtual void Destructor(void);

		button_info(void) { m_lpvData = nullptr;  Initializer(); }
		button_info(const button_info & i_cRHO) { m_lpvData = nullptr;  Initializer(i_cRHO); }
		button_info(shape i_eShape, const pair<float> i_pdPosition, const pair<float> i_pdSize, unsigned int i_uiID, int i_iStatus = 0) { m_lpvData = nullptr; Initializer(i_eShape, i_pdPosition, i_pdSize, i_uiID, i_iStatus); }
		~button_info(void) { Destructor(); }
	};
}

