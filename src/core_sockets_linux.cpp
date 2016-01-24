#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <core.hpp>
#include <sstream>
#include <cstring>


class SOCKET_INFO_LINUX
{
public:
	int 			m_iSocket_ID;
	struct addrinfo * m_lpsAddress_Info;
	int				m_iLast_Error;

	SOCKET_INFO_LINUX(void)
	{
		m_iSocket_ID = -1;
		m_lpsAddress_Info = NULL;
		m_iLast_Error = 0;
	}
};

bool	COMMSOCKET::Initialize_Server(TYPE i_eType, unsigned short i_usPort)
{
	struct addrinfo hints, *res;
	int sockfd;
	std::ostringstream szPort;
	szPort << i_usPort;
	SOCKET_INFO_LINUX	* lpSock_Info = new SOCKET_INFO_LINUX;
	m_lpvSystem_Info = (void *)lpSock_Info;
	bool bError = (lpSock_Info == NULL);
	// first, load up address structs with getaddrinfo():
	m_eType = i_eType;
	if (!bError)
	{
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

		bError = (getaddrinfo(NULL, szPort.str().c_str(), &hints, &(lpSock_Info->m_lpsAddress_Info)) != -1);
	}
	// make a socket:
	if (!bError)
	{
		lpSock_Info->m_iSocket_ID = socket(lpSock_Info->m_lpsAddress_Info->ai_family, lpSock_Info->m_lpsAddress_Info->ai_socktype, lpSock_Info->m_lpsAddress_Info->ai_protocol);
		bError = lpSock_Info->m_iSocket_ID == -1;
	}

	// bind it to the port we passed in to getaddrinfo():

	if (!bError)
	{
		bError = (bind(lpSock_Info->m_iSocket_ID, lpSock_Info->m_lpsAddress_Info->ai_addr, lpSock_Info->m_lpsAddress_Info->ai_addrlen) == -1);
	}
	if (!bError)
	{
		bError = (listen(lpSock_Info->m_iSocket_ID, 2) == -1);
	}
	m_bIs_Initialized = !bError;
	if (bError)
	{
		delete lpSock_Info;
		m_lpvSystem_Info = NULL;
	}

	return !bError;
}
bool	COMMSOCKET::Initialize_Client(TYPE i_eType, const std::string &i_szServer_Addr, unsigned short i_usPort)
{
	struct addrinfo hints, *res;
	int sockfd;
	std::ostringstream szPort;
	m_eType = i_eType;
	szPort << i_usPort;
	SOCKET_INFO_LINUX	* lpSock_Info = new SOCKET_INFO_LINUX;
	m_lpvSystem_Info = (void *)lpSock_Info;
	bool bError = (lpSock_Info == NULL);
	// first, load up address structs with getaddrinfo():

	if (!bError)
	{
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
		hints.ai_socktype = SOCK_STREAM;

		bError = (getaddrinfo(NULL, szPort.str().c_str(), &hints, &(lpSock_Info->m_lpsAddress_Info)) != -1);
	}
	// make a socket:
	if (!bError)
	{
		lpSock_Info->m_iSocket_ID = socket(lpSock_Info->m_lpsAddress_Info->ai_family, lpSock_Info->m_lpsAddress_Info->ai_socktype, lpSock_Info->m_lpsAddress_Info->ai_protocol);
		bError = lpSock_Info->m_iSocket_ID == -1;
	}

	// connect to the remote server we passed in to getaddrinfo():

	if (!bError)
	{
		bError = (connect(lpSock_Info->m_iSocket_ID, lpSock_Info->m_lpsAddress_Info->ai_addr, lpSock_Info->m_lpsAddress_Info->ai_addrlen) == -1);
	}
	m_bIs_Initialized = !bError;
	m_bIs_Connected = !bError;
	if (bError)
	{
		delete lpSock_Info;
		m_lpvSystem_Info = NULL;
	}
	return !bError;
}

int * g_lpiData_Container = NULL;
size_t g_zInt_Data_Container_Size = 0;
short * g_lpsData_Container = NULL;
size_t g_zShort_Data_Container_Size = 0;
bool	COMMSOCKET::Send(int *i_lpiData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		if (i_zNum_To_Send > g_zInt_Data_Container_Size)
		{
			if (g_lpiData_Container)
				delete [] g_lpiData_Container;
			g_lpiData_Container = new int[i_zNum_To_Send];
			g_zInt_Data_Container_Size = i_zNum_To_Send;
		}
		for (unsigned int uiI = 0; uiI < i_zNum_To_Send; uiI++)
			g_lpiData_Container[uiI] = htonl(i_lpiData[uiI]);
		bSuccess = Send((char *)g_lpiData_Container,i_zNum_To_Send * sizeof(int));
	}
	return bSuccess;
}
bool	COMMSOCKET::Send(short *i_lpiData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		if (i_zNum_To_Send > g_zShort_Data_Container_Size)
		{
			if (g_lpsData_Container)
				delete [] g_lpsData_Container;
			g_lpsData_Container = new short[i_zNum_To_Send];
			g_zShort_Data_Container_Size = i_zNum_To_Send;
		}
		for (unsigned int uiI = 0; uiI < i_zNum_To_Send; uiI++)
			g_lpsData_Container[uiI] = htons(i_lpiData[uiI]);
		bSuccess = Send(g_lpsData_Container,sizeof(short) * i_zNum_To_Send);
	}
	return bSuccess;
}

bool	COMMSOCKET::Send(char * i_lplpcData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		size_t zSent;
		size_t zSent_Total = 0;
		unsigned int uiTries = 0;
		while (zSent_Total < i_zNum_To_Send && uiTries < m_uiMax_Tries)
		{
			unsigned int uiTransmit_Idx = zSent_Total;
			zSent = send(lpSock_Info->m_iSocket_ID,&(i_lplpcData[uiTransmit_Idx]),i_zNum_To_Send - uiTransmit_Idx,0);
			zSent_Total += zSent;
			uiTries++;
		}
		bSuccess = (zSent_Total == i_zNum_To_Send);
	}
	return bSuccess;
}
bool	COMMSOCKET::Send(unsigned int *i_lpiData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		if (i_zNum_To_Send > g_zInt_Data_Container_Size)
		{
			if (g_lpiData_Container)
				delete [] g_lpiData_Container;
			g_lpiData_Container = new int[i_zNum_To_Send];
			g_zInt_Data_Container_Size = i_zNum_To_Send;
		}
		for (unsigned int uiI = 0; uiI < i_zNum_To_Send; uiI++)
			g_lpiData_Container[uiI] = htonl(i_lpiData[uiI]);
		bSuccess = Send((char *)g_lpiData_Container,i_zNum_To_Send * sizeof(int));
	}
	return bSuccess;
}
bool	COMMSOCKET::Send(unsigned short *i_lpiData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		if (i_zNum_To_Send > g_zShort_Data_Container_Size)
		{
			if (g_lpsData_Container)
				delete [] g_lpsData_Container;
			g_lpsData_Container = new short[i_zNum_To_Send];
			g_zShort_Data_Container_Size = i_zNum_To_Send;
		}
		for (unsigned int uiI = 0; uiI < i_zNum_To_Send; uiI++)
			g_lpsData_Container[uiI] = htons(i_lpiData[uiI]);
		bSuccess = Send(g_lpsData_Container,sizeof(short) * i_zNum_To_Send);
	}
	return bSuccess;
}

bool	COMMSOCKET::Send(unsigned char * i_lplpcData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		size_t zSent;
		size_t zSent_Total = 0;
		unsigned int uiTries = 0;
		while (zSent_Total < i_zNum_To_Send && uiTries < m_uiMax_Tries)
		{
			unsigned int uiTransmit_Idx = zSent_Total;
			zSent = send(lpSock_Info->m_iSocket_ID,&(i_lplpcData[uiTransmit_Idx]),i_zNum_To_Send - uiTransmit_Idx,0);
			zSent_Total += zSent;
			uiTries++;
		}
		bSuccess = (zSent_Total == i_zNum_To_Send);
	}
	return bSuccess;
}
bool	COMMSOCKET::Receive(int * i_lpiData, size_t i_zNum_To_Receive)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		if (i_zNum_To_Receive > g_zInt_Data_Container_Size)
		{
			if (g_lpiData_Container)
				delete [] g_lpiData_Container;
			g_lpiData_Container = new int[i_zNum_To_Receive];
			g_zInt_Data_Container_Size = i_zNum_To_Receive;
		}
		bSuccess = Receive((char *)g_zInt_Data_Container_Size,i_zNum_To_Receive * sizeof(int));
		if (bSuccess)
		{
			for (unsigned int uiI = 0; uiI < i_zNum_To_Receive; uiI++)
				i_lpiData[uiI] = ntohl(g_lpiData_Container[uiI]);
		}
	}
	return bSuccess;
}
bool	COMMSOCKET::Receive(short * i_lpsData, size_t i_zNum_To_Receive)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		if (i_zNum_To_Receive > g_zShort_Data_Container_Size)
		{
			if (g_lpsData_Container)
				delete [] g_lpsData_Container;
			g_lpsData_Container = new short[i_zNum_To_Receive];
			g_zShort_Data_Container_Size = i_zNum_To_Receive;
		}
		bSuccess = Receive((char *)g_lpsData_Container,i_zNum_To_Receive * sizeof(short));
		if (bSuccess)
		{
			for (unsigned int uiI = 0; uiI < i_zNum_To_Receive; uiI++)
				i_lpsData[uiI] = ntohs(g_lpsData_Container[uiI]);
		}
	}
	return bSuccess;
}

bool	COMMSOCKET::Receive(char * i_lpcData, size_t i_zNum_To_Receive)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		size_t zSent;
		size_t zSent_Total = 0;
		unsigned int uiTries = 0;
		while (zSent_Total < i_zNum_To_Receive && uiTries < m_uiMax_Tries)
		{
			unsigned int uiTransmit_Idx = zSent_Total;
			zSent = recv(lpSock_Info->m_iSocket_ID,&(i_lpcData[uiTransmit_Idx]),i_zNum_To_Receive - uiTransmit_Idx,0);
			zSent_Total += zSent;
			uiTries++;
		}
		bSuccess = (zSent_Total == i_zNum_To_Receive);
	}
	return bSuccess;
}
bool	COMMSOCKET::Receive(unsigned int * i_lpiData, size_t i_zNum_To_Receive)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		if (i_zNum_To_Receive > g_zInt_Data_Container_Size)
		{
			if (g_lpiData_Container)
				delete [] g_lpiData_Container;
			g_lpiData_Container = new int[i_zNum_To_Receive];
			g_zInt_Data_Container_Size = i_zNum_To_Receive;
		}
		bSuccess = Receive((char *)g_zInt_Data_Container_Size,i_zNum_To_Receive * sizeof(int));
		if (bSuccess)
		{
			for (unsigned int uiI = 0; uiI < i_zNum_To_Receive; uiI++)
				i_lpiData[uiI] = ntohl(g_lpiData_Container[uiI]);
		}
	}
	return bSuccess;
}
bool	COMMSOCKET::Receive(unsigned short * i_lpsData, size_t i_zNum_To_Receive)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		if (i_zNum_To_Receive > g_zShort_Data_Container_Size)
		{
			if (g_lpsData_Container)
				delete [] g_lpsData_Container;
			g_lpsData_Container = new short[i_zNum_To_Receive];
			g_zShort_Data_Container_Size = i_zNum_To_Receive;
		}
		bSuccess = Receive((char *)g_lpsData_Container,i_zNum_To_Receive * sizeof(short));
		if (bSuccess)
		{
			for (unsigned int uiI = 0; uiI < i_zNum_To_Receive; uiI++)
				i_lpsData[uiI] = ntohs(g_lpsData_Container[uiI]);
		}
	}
	return bSuccess;
}

bool	COMMSOCKET::Receive(unsigned char * i_lpcData, size_t i_zNum_To_Receive)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		size_t zSent;
		size_t zSent_Total = 0;
		unsigned int uiTries = 0;
		while (zSent_Total < i_zNum_To_Receive && uiTries < m_uiMax_Tries)
		{
			unsigned int uiTransmit_Idx = zSent_Total;
			zSent = recv(lpSock_Info->m_iSocket_ID,&(i_lpcData[uiTransmit_Idx]),i_zNum_To_Receive - uiTransmit_Idx,0);
			zSent_Total += zSent;
			uiTries++;
		}
		bSuccess = (zSent_Total == i_zNum_To_Receive);
	}
	return bSuccess;
}

bool	COMMSOCKET::Accept_Connections(void)
{
	if (m_bIs_Initialized && m_lpvSystem_Info)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		m_bIs_Connected |= (accept(lpSock_Info->m_iSocket_ID, lpSock_Info->m_lpsAddress_Info->ai_addr, &lpSock_Info->m_lpsAddress_Info->ai_addrlen) != -1);
	}
	return m_bIs_Connected;
}

bool COMMSOCKET::Close(void)
{
	if (m_bIs_Connected)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		close(lpSock_Info->m_iSocket_ID);
		m_bIs_Connected = false;
		lpSock_Info->m_iSocket_ID = -1;
		freeaddrinfo(lpSock_Info->m_lpsAddress_Info);
	}
	if (m_bIs_Initialized)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		delete [] lpSock_Info;
		m_lpvSystem_Info = NULL;
		m_bIs_Initialized = false;
	}
}

COMMSOCKET::~COMMSOCKET(void)
{
	Close();
}

int COMMSOCKET::Get_Error(void)
{
	int iRet = 0;
	if (m_bIs_Initialized && m_lpvSystem_Info)
	{
		SOCKET_INFO_LINUX	* lpSock_Info = (SOCKET_INFO_LINUX *)m_lpvSystem_Info;
		iRet = lpSock_Info->m_iLast_Error;
	}
	return iRet;
}
