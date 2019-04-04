#ifndef _ODSOCKET_H_
#define _ODSOCKET_H_

#ifdef WIN32
#include<iostream>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib") 
typedef int				socklen_t;
#else
#include "cocos2d.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
typedef int				SOCKET;

//#pragma region define win32 const variable in linux
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
//#pragma endregion
#endif


class ODSocket {

public:
	ODSocket(SOCKET sock = INVALID_SOCKET);
	~ODSocket();

	// Create socket object for snd/recv data
	bool Create(int af, int type, int protocol = 0);

	// Connect socket
	bool Connect(const char* ip, unsigned short port);
	
	// Send socket
	int Send(const char* buf, int len, int flags = 0);

	// Recv socket
	int Recv(char* buf, int len, int flags = 0);
	
	// Close socket
	int Close();

	// Get errno
	int GetError();
	
	// Init winsock DLL 
	static int Init();	
	// Clean winsock DLL
	static int Clean();
    
protected:
	SOCKET m_sock;

};

#endif
