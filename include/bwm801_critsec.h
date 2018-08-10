#pragma once

namespace bwm801
{
	class EXPORT criticalsection
	{
	private:
		void * m_lpvCS_Data;
	public:
		virtual  void Initializer(void);
		virtual  void Destructor(void);
		virtual void Set(void);
		virtual void Unset(void);
		virtual bool Get(void) const ;

		criticalsection(void){Initializer();}
		~criticalsection(void){Destructor();}

	};
};

