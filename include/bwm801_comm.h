#pragma once
#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace bwm801
{
	class EXPORT commsocket
	{
	public:
		enum	stream_type {stream,datagram};
		enum	datatypes {CSchar,CSuchar,CSshort,CSushort,CSint,CSuint,CSint64,CSuint64,CSfloat,CSdouble};
	private:
		stream_type	m_eType;
		bool	m_bAllow_Blocking;
		bool	m_bIs_Initialized;
		bool	m_bIs_Connected;
		void *	m_lpvSystem_Info; // void pointer to allow for differing socket info for each OS
		unsigned int 	m_uiMax_Tries;
	public:
		virtual void Initializer(void);
		virtual void Destructor(void);
		commsocket(void){Initializer();}
		~commsocket(void){Destructor();}
		//enum	TYPE	{BROADCAST,
		virtual bool	Initialize_Server(stream_type i_eType, unsigned short i_usPort);
		virtual bool	Initialize_Client(stream_type i_eType, const std::string & i_szServer_Address, unsigned short i_usPort);
		virtual bool	Send(const int *i_lpiData, size_t i_zNum_To_Send);
		virtual bool	Send(const short * i_lpsData, size_t i_zNum_To_Send);
		virtual bool	Send(const char * i_lplpcData, size_t i_zNum_To_Send);
		virtual bool	Send(const unsigned int *i_lpiData, size_t i_zNum_To_Send);
		virtual bool	Send(const unsigned short * i_lpsData, size_t i_zNum_To_Send);
		virtual bool	Send(const unsigned char * i_lplpcData, size_t i_zNum_To_Send);
		virtual bool	Receive(int * i_lpiData, size_t i_zNum_To_Receive);
		virtual bool	Receive(short * i_lpsData, size_t i_zNum_To_Receive);
		virtual bool	Receive(char * i_lpcData, size_t i_zNum_To_Receive);
		virtual bool	Receive(unsigned int * i_lpiData, size_t i_zNum_To_Receive);
		virtual bool	Receive(unsigned short * i_lpsData, size_t i_zNum_To_Receive);
		virtual bool	Receive(unsigned char * i_lpcData, size_t i_zNum_To_Receive);
		virtual bool	Accept_Connections(void);

		virtual bool	Is_Connected(void);
		virtual bool	Is_Initialized(void);
		virtual int		Get_Error(void);
		virtual void	Close(void);
	};
};

#undef EXPORT
