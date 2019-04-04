#define _CRT_SECURE_NO_DEPRECATE
#include "ODSocket.h"

ODSocket::ODSocket(SOCKET sock)
{
	m_sock = sock;
}

ODSocket::~ODSocket()
{
}

int ODSocket::Init()
{
#ifdef WIN32
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if (ret) {
		return -1;
	}
#endif

	return 0;
}

int ODSocket::Clean()
{
#ifdef WIN32
	return (WSACleanup());
#endif
	return 0;
}

bool ODSocket::Create(int af, int type, int protocol)
{
	m_sock = socket(af, type, protocol);
	if ( m_sock == INVALID_SOCKET ) {
		return false;
	}
    
	return true;
}

bool ODSocket::Connect(const char* ip, unsigned short port)
{
    
        struct sockaddr_in svraddr;
        svraddr.sin_family = AF_INET;
        svraddr.sin_addr.s_addr = inet_addr(ip);
        svraddr.sin_port = htons(port);
        int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
        if ( ret == SOCKET_ERROR ) {
            return false;
        }
        return true;
    
}


int ODSocket::Send(const char* buf, int len, int flags)
{
	int bytes;
	int count = 0;
    
	while ( count < len ) {

		bytes = (int)send(m_sock, buf + count, len, flags);
        if ( bytes == -1 || bytes == 0 ){
			return -1;
        }
		count += bytes;
	} 

	return count;
}

int ODSocket::Recv(char* buf, int len, int flags)
{
	return (int)(recv(m_sock, buf, len, flags));
}

int ODSocket::Close()
{
#ifdef WIN32
	return (closesocket(m_sock));
#else
	return (close(m_sock));
#endif
}

int ODSocket::GetError()
{
#ifdef WIN32
	return (WSAGetLastError());
#else
	return (0);
#endif
}
