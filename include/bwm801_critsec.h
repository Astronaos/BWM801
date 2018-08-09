#pragma once
#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

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

#undef EXPORT
