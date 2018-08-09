#include <WinSock2.h>
#include <ws2tcpip.h>
#include <bwm801.h>
#include <sstream>
#include <cstring>

using namespace bwm801;

class winsock
{
public:
	WSADATA m_wsaData;
	bool	m_bInitialized;

	winsock(void)
	{
		m_bInitialized = false;
	}
	~winsock(void)
	{
		if (m_bInitialized)
			WSACleanup();
	}
	
	void Initialize(void)
	{
		// Initialize Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		m_bInitialized = (iResult == 0);
	}
};
winsock	g_cWinsock;

class socket_info_win
{
public:
	SOCKET 			m_iSocket_ID;
	SOCKET 			m_iAccept_Socket_ID;
	struct addrinfo * m_lpsAddress_Info;
	int				m_iLast_Error;

	socket_info_win(void)
	{
		m_iAccept_Socket_ID = INVALID_SOCKET;
		m_iSocket_ID = INVALID_SOCKET;
		m_lpsAddress_Info = NULL;
		m_iLast_Error = 0;
	}
};

bool	commsocket::Initialize_Server(stream_type i_eType, unsigned short i_usPort)
{
	bool bError_Reported = false;
	struct addrinfo hints;// , *res;
//	int sockfd;
	std::ostringstream szPort;
	szPort << i_usPort;
	socket_info_win	* lpSock_Info = new socket_info_win;
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

		bError = (getaddrinfo(NULL, szPort.str().c_str(), &hints, &(lpSock_Info->m_lpsAddress_Info)) == -1);
	}
	else if (!bError_Reported)
	{
		fprintf(stderr,"Error initializing port: failed to allocate socket information\n");
		bError_Reported = true;
		lpSock_Info->m_iLast_Error = errno;
	}
	// make a socket:
	if (!bError)
	{
		lpSock_Info->m_iAccept_Socket_ID = socket(lpSock_Info->m_lpsAddress_Info->ai_family, lpSock_Info->m_lpsAddress_Info->ai_socktype, lpSock_Info->m_lpsAddress_Info->ai_protocol);
		bError = lpSock_Info->m_iAccept_Socket_ID == -1;
	}
	else if (!bError_Reported)
	{
		fprintf(stderr,"Error initializing port: failed to get address information\n");
		bError_Reported = true;
		lpSock_Info->m_iLast_Error = errno;
	}

	// bind it to the port we passed in to getaddrinfo():

	if (!bError)
	{
		bError = (bind(lpSock_Info->m_iAccept_Socket_ID, lpSock_Info->m_lpsAddress_Info->ai_addr, lpSock_Info->m_lpsAddress_Info->ai_addrlen) == -1);
	}
	else if (!bError_Reported)
	{
		fprintf(stderr,"Error initializing port: failed to open socket\n");
		bError_Reported = true;
		lpSock_Info->m_iLast_Error = errno;
	}
	if (!bError)
	{
		bError = (listen(lpSock_Info->m_iAccept_Socket_ID, 20) == -1);
	}
	else if (!bError_Reported)
	{
		fprintf(stderr,"Error initializing port: failed to bind\n");
		bError_Reported = true;
		lpSock_Info->m_iLast_Error = errno;
	}
	m_bIs_Initialized = !bError;
	if (bError)
	{
		if (lpSock_Info)
			delete lpSock_Info;
		m_lpvSystem_Info = NULL;
		if (!bError_Reported)
		{
			fprintf(stderr,"Error initializing port: failed to listen %i\n",errno);
			bError_Reported = true;
			lpSock_Info->m_iLast_Error = errno;
		}
	}
	if (bError_Reported)
	{
		fprintf(stderr,"%s\n",strerror(errno));
	}

	return !bError;
}
bool	commsocket::Initialize_Client(stream_type i_eType, const std::string &i_szServer_Addr, unsigned short i_usPort)
{
	bool bError_Reported = false;
	struct addrinfo hints;// , *res;
//	int sockfd;
	std::ostringstream szPort;
	m_eType = i_eType;
	szPort << i_usPort;
	socket_info_win	* lpSock_Info = new socket_info_win;
	m_lpvSystem_Info = (void *)lpSock_Info;
	bool bError = (lpSock_Info == NULL);
	// first, load up address structs with getaddrinfo():

	if (!bError)
	{
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
		hints.ai_socktype = SOCK_STREAM;

		bError = (getaddrinfo(i_szServer_Addr.c_str(), szPort.str().c_str(), &hints, &(lpSock_Info->m_lpsAddress_Info)) == -1);
	}
	else if (!bError_Reported)
	{
		fprintf(stderr,"Error initializing port: failed to allocate socket information\n");
		bError_Reported = true;
		lpSock_Info->m_iLast_Error = errno;
	}
	// make a socket:
	if (!bError)
	{
		lpSock_Info->m_iSocket_ID = socket(lpSock_Info->m_lpsAddress_Info->ai_family, lpSock_Info->m_lpsAddress_Info->ai_socktype, lpSock_Info->m_lpsAddress_Info->ai_protocol);
		bError = lpSock_Info->m_iSocket_ID == -1;
	}
	else if (!bError_Reported)
	{
		fprintf(stderr,"Error initializing port: failed to get addr info\n");
		bError_Reported = true;
		lpSock_Info->m_iLast_Error = errno;
	}

	// connect to the remote server we passed in to getaddrinfo():

	if (!bError)
	{
		bError = (connect(lpSock_Info->m_iSocket_ID, lpSock_Info->m_lpsAddress_Info->ai_addr, lpSock_Info->m_lpsAddress_Info->ai_addrlen) == -1);
	}
	else if (!bError_Reported)
	{
		fprintf(stderr,"Error initializing port: failed to open socket\n");
		bError_Reported = true;
		lpSock_Info->m_iLast_Error = errno;
	}
	m_bIs_Initialized = !bError;
	m_bIs_Connected = !bError;
	if (bError)
	{
		delete lpSock_Info;
		m_lpvSystem_Info = NULL;
		if (!bError_Reported)
		{
			fprintf(stderr,"Error initializing port: failed to connect\n");
			bError_Reported = true;
			lpSock_Info->m_iLast_Error = errno;
		}
	}
	else
		lpSock_Info->m_iAccept_Socket_ID = lpSock_Info->m_iSocket_ID;
	if (bError_Reported)
	{
		fprintf(stderr,"%s\n",strerror(errno));
	}
	return !bError;
}

int * g_lpiData_Container = NULL;
size_t g_zInt_Data_Container_Size = 0;
short * g_lpsData_Container = NULL;
size_t g_zShort_Data_Container_Size = 0;
bool	commsocket::Send(const int *i_lpiData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		if (i_zNum_To_Send > g_zInt_Data_Container_Size)
		{
			if (g_lpiData_Container)
				delete [] g_lpiData_Container;
			g_lpiData_Container = new int[i_zNum_To_Send];
			g_zInt_Data_Container_Size = i_zNum_To_Send;
		}
		for (unsigned int uiI = 0; uiI < i_zNum_To_Send; uiI++)
			g_lpiData_Container[uiI] = htonl(i_lpiData[uiI]);
		bSuccess = Send((const char *)g_lpiData_Container,i_zNum_To_Send * sizeof(int));
	}
	return bSuccess;
}
bool	commsocket::Send(const short *i_lpiData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		if (i_zNum_To_Send > g_zShort_Data_Container_Size)
		{
			if (g_lpsData_Container)
				delete [] g_lpsData_Container;
			g_lpsData_Container = new short[i_zNum_To_Send];
			g_zShort_Data_Container_Size = i_zNum_To_Send;
		}
		for (unsigned int uiI = 0; uiI < i_zNum_To_Send; uiI++)
			g_lpsData_Container[uiI] = htons(i_lpiData[uiI]);
		bSuccess = Send((const char *)g_lpsData_Container,sizeof(short) * i_zNum_To_Send);
	}
	return bSuccess;
}

bool	commsocket::Send(const char * i_lplpcData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		size_t zSent = 0;
		size_t zSent_Total = 0;
		unsigned int uiTries = 0;
		while (zSent != -1 && zSent_Total < i_zNum_To_Send && uiTries < m_uiMax_Tries)
		{
			unsigned int uiTransmit_Idx = zSent_Total;
			zSent = send(lpSock_Info->m_iSocket_ID,&(i_lplpcData[uiTransmit_Idx]),i_zNum_To_Send - uiTransmit_Idx,0);
			if (zSent == -1)
				lpSock_Info->m_iLast_Error = errno;
			else
			{
	//			for (unsigned int uiI = 0; uiI < zSent; uiI++)
	//			{
	//				printf("Sent %x\n",i_lplpcData[uiTransmit_Idx + uiI]);
	//			}
				zSent_Total += zSent;
				uiTries++;
			}
		}
		bSuccess = (zSent_Total == i_zNum_To_Send);
		if (zSent == -1)
			fprintf(stderr,"%s\n",strerror(errno));
	}
	return bSuccess;
}
bool	commsocket::Send(const unsigned int *i_lpiData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		if (i_zNum_To_Send > g_zInt_Data_Container_Size)
		{
			if (g_lpiData_Container)
				delete [] g_lpiData_Container;
			g_lpiData_Container = new int[i_zNum_To_Send];
			g_zInt_Data_Container_Size = i_zNum_To_Send;
		}
		for (unsigned int uiI = 0; uiI < i_zNum_To_Send; uiI++)
			g_lpiData_Container[uiI] = htonl(i_lpiData[uiI]);
		bSuccess = Send((const unsigned char *)g_lpiData_Container,i_zNum_To_Send * sizeof(int));
	}
	return bSuccess;
}
bool	commsocket::Send(const unsigned short *i_lpiData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		if (i_zNum_To_Send > g_zShort_Data_Container_Size)
		{
			if (g_lpsData_Container)
				delete [] g_lpsData_Container;
			g_lpsData_Container = new short[i_zNum_To_Send];
			g_zShort_Data_Container_Size = i_zNum_To_Send;
		}
		for (unsigned int uiI = 0; uiI < i_zNum_To_Send; uiI++)
			g_lpsData_Container[uiI] = htons(i_lpiData[uiI]);
		bSuccess = Send((const unsigned char *)g_lpsData_Container,sizeof(short) * i_zNum_To_Send);
	}
	return bSuccess;
}

bool	commsocket::Send(const unsigned char * i_lplpcData, size_t i_zNum_To_Send)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		size_t zSent = 0;
		size_t zSent_Total = 0;
		unsigned int uiTries = 0;
		while (zSent != -1 && zSent_Total < i_zNum_To_Send && uiTries < m_uiMax_Tries)
		{
			unsigned int uiTransmit_Idx = zSent_Total;
			zSent = send(lpSock_Info->m_iSocket_ID,(const char*)&(i_lplpcData[uiTransmit_Idx]),i_zNum_To_Send - uiTransmit_Idx,0);
			if (zSent == -1)
				lpSock_Info->m_iLast_Error = errno;
			else
			{
//				printf("Sent");
//				for (unsigned int uiI = 0; uiI < zSent; uiI++)
//				{
//					printf(" %x",i_lplpcData[uiTransmit_Idx + uiI]);
//				}
//				printf("\n");
				zSent_Total += zSent;
				uiTries++;
			}
		}
		bSuccess = (zSent_Total == i_zNum_To_Send);
		if (zSent == -1)
			fprintf(stderr,"%s\n",strerror(errno));
	}
	return bSuccess;
}
bool	commsocket::Receive(int * i_lpiData, size_t i_zNum_To_Receive)
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
		bSuccess = Receive((char *)g_lpiData_Container,i_zNum_To_Receive * sizeof(int));
		if (bSuccess)
		{
			for (unsigned int uiI = 0; uiI < i_zNum_To_Receive; uiI++)
				i_lpiData[uiI] = ntohl(g_lpiData_Container[uiI]);
		}
	}
	return bSuccess;
}
bool	commsocket::Receive(short * i_lpsData, size_t i_zNum_To_Receive)
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

bool	commsocket::Receive(char * i_lpcData, size_t i_zNum_To_Receive)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		size_t zSent = 0;
		size_t zSent_Total = 0;
		unsigned int uiTries = 0;
		while (zSent != -1 && zSent_Total < i_zNum_To_Receive && uiTries < m_uiMax_Tries)
		{
			unsigned int uiTransmit_Idx = zSent_Total;
			zSent = recv(lpSock_Info->m_iSocket_ID,&(i_lpcData[uiTransmit_Idx]),i_zNum_To_Receive - uiTransmit_Idx,0);
			if (zSent == -1)
				lpSock_Info->m_iLast_Error = errno;
			else
			{
				zSent_Total += zSent;
				uiTries++;
			}
		}
		if (zSent == -1)
			fprintf(stderr,"%s\n",strerror(errno));
		if (uiTries >= m_uiMax_Tries)
			fprintf(stderr,"Receive signed: max tries exceeded\n");
		bSuccess = (zSent_Total == i_zNum_To_Receive);
	}
	return bSuccess;
}
bool	commsocket::Receive(unsigned int * i_lpiData, size_t i_zNum_To_Receive)
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
		bSuccess = Receive((unsigned char *)g_lpiData_Container,i_zNum_To_Receive * sizeof(int));
		if (bSuccess)
		{
			for (unsigned int uiI = 0; uiI < i_zNum_To_Receive; uiI++)
				i_lpiData[uiI] = ntohl(g_lpiData_Container[uiI]);
		}
	}
	return bSuccess;
}
bool	commsocket::Receive(unsigned short * i_lpsData, size_t i_zNum_To_Receive)
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
		bSuccess = Receive((unsigned char *)g_lpsData_Container,i_zNum_To_Receive * sizeof(short));
		if (bSuccess)
		{
			for (unsigned int uiI = 0; uiI < i_zNum_To_Receive; uiI++)
				i_lpsData[uiI] = ntohs(g_lpsData_Container[uiI]);
		}
	}
	return bSuccess;
}

bool	commsocket::Receive(unsigned char * i_lpcData, size_t i_zNum_To_Receive)
{
	bool	bSuccess = false;
	if (m_bIs_Initialized && m_bIs_Connected && m_lpvSystem_Info)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		size_t zSent = 0;
		size_t zSent_Total = 0;
		unsigned int uiTries = 0;
		while (zSent_Total < i_zNum_To_Receive && uiTries < m_uiMax_Tries && zSent != -1)
		{
			unsigned int uiTransmit_Idx = zSent_Total;
			zSent = recv(lpSock_Info->m_iSocket_ID,(char*)&(i_lpcData[uiTransmit_Idx]),i_zNum_To_Receive - uiTransmit_Idx,0);
			if (zSent == -1)
				lpSock_Info->m_iLast_Error = errno;
			else
			{
				zSent_Total += zSent;
//				printf("Recv");
//				for (unsigned int uiI = 0; uiI < zSent; uiI++)
//				{
//					printf(" %x",i_lpcData[uiTransmit_Idx + uiI]);
//				}
//				printf("\n");
				uiTries++;
			}
		}
		if (uiTries >= m_uiMax_Tries)
			fprintf(stderr,"Receive unsigned: max tries exceeded\n");
		if (zSent == -1)
			fprintf(stderr,"%s\n",strerror(errno));
			
		bSuccess = (zSent != -1 && zSent_Total == i_zNum_To_Receive);
	}
	return bSuccess;
}

bool	commsocket::Accept_Connections(void)
{
	if (m_bIs_Initialized && m_lpvSystem_Info)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		int iAddr_Len = lpSock_Info->m_lpsAddress_Info->ai_addrlen;
		lpSock_Info->m_iSocket_ID = accept(lpSock_Info->m_iAccept_Socket_ID, lpSock_Info->m_lpsAddress_Info->ai_addr, &iAddr_Len);
		lpSock_Info->m_lpsAddress_Info->ai_addrlen = iAddr_Len;
		if (lpSock_Info->m_iSocket_ID == -1)
		{
			if (errno != EWOULDBLOCK && errno != EAGAIN)
			{
				lpSock_Info->m_iLast_Error = errno;
				fprintf(stderr,"%s\n",strerror(errno));
			}
		}
		m_bIs_Connected |= (lpSock_Info->m_iSocket_ID != -1);
	}
	return m_bIs_Connected;
}

void commsocket::Close(void)
{
	if (m_bIs_Connected)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		closesocket(lpSock_Info->m_iSocket_ID);
		if (lpSock_Info->m_iSocket_ID != lpSock_Info->m_iAccept_Socket_ID)
			closesocket(lpSock_Info->m_iAccept_Socket_ID);
		WSACleanup();
		m_bIs_Connected = false;
		lpSock_Info->m_iAccept_Socket_ID = -1;
		lpSock_Info->m_iSocket_ID = -1;
		freeaddrinfo(lpSock_Info->m_lpsAddress_Info);
	}
	if (m_bIs_Initialized)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		delete [] lpSock_Info;
		m_lpvSystem_Info = NULL;
		m_bIs_Initialized = false;
	}
}


int commsocket::Get_Error(void)
{
	int iRet = 0;
	if (m_bIs_Initialized && m_lpvSystem_Info)
	{
		socket_info_win	* lpSock_Info = (socket_info_win *)m_lpvSystem_Info;
		iRet = lpSock_Info->m_iLast_Error;
	}
	return iRet;
}
