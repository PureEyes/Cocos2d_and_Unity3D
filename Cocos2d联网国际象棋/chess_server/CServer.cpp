#include "stdafx.h"
#include "CServer.h"
#include <stdio.h>

using namespace rapidjson;

CServer* CServer::instance = nullptr;

CServer::CServer()
{
	//buf = new Buffer();
}

CServer::~CServer()
{
}

CServer * CServer::getInstance()
{
	if (instance == nullptr) {
		instance = new CServer();
	}
	return instance;
}

bool CServer::Create(int af, int type, int protocol)
{
	WSADATA wsa;

	retval_ = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (retval_ != 0) {
		std::cout << "WSAStartup() failed!!!" << std::endl;
	}

	SOCKET m_sSock = ::socket(af, type, protocol);
	m_pSock = new CClient(m_sSock);
	//m_pSock->setClientName("SERVER");

	if (m_sSock == INVALID_SOCKET) {
		this->errQuit(__FUNCTION__"-WSASocket()");
		return false;
	}
	
	return true;
}

bool CServer::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);

	retval_ = ::bind(m_pSock->_sClient, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if (retval_ == SOCKET_ERROR) {
		this->errQuit(__FUNCTION__ "-bind()");
		closesocket(m_pSock->_sClient);
		return false;
	}
	return true;
}

bool CServer::Listen(SOCKET listen, int backlog)
{
	retval_ = ::listen(listen, backlog);
	if (retval_ == SOCKET_ERROR) {
		this->errQuit(__FUNCTION__ "-listen()");
		return false;
	}
	
	WSAEVENT event = WSACreateEvent();
	retval_ = WSAEventSelect(listen, event, FD_ACCEPT | FD_CLOSE);
	if (retval_ == SOCKET_ERROR) {
		this->errQuit(__FUNCTION__ "-WSAEventSelct()");
	}
	eventArray[nEventTotal] = event;
	sockArray[nEventTotal] = m_pSock;
	++nEventTotal;

	return true;
}

bool CServer::Accept(SOCKET socket)
{
	sockaddr_in clientAddr;
	int addrlen = sizeof(clientAddr);

	SOCKET sock_client = accept(socket, reinterpret_cast<sockaddr*>(&clientAddr), &addrlen);
	if (sock_client == SOCKET_ERROR) {
		this->errDisplay("accept()");
	}

	WSAEVENT event = WSACreateEvent();
	retval_ = WSAEventSelect(sock_client, event, FD_READ | FD_WRITE | FD_CLOSE);
	if (retval_ == SOCKET_ERROR) {
		this->errQuit(__FUNCTION__ "-WSAEventSelect()");
	}

	CClient *_pClient = new CClient(sock_client);

	eventArray[nEventTotal] = event;
	sockArray[nEventTotal] = _pClient;
	++nEventTotal;

	if (nEventTotal >= WSA_MAXIMUM_WAIT_EVENTS) {
		std::cout << "! we can't accept client anymore!" << std::endl;
		closesocket(sock_client);
		delete _pClient;
		_pClient = nullptr;
		return false;
	}
	
	//_pClient->SendData("你好 cocos!");

	this->msgDisplay("客户端连接成功!");

	return true;
}

void CServer::InitServer()
{
	this->Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	this->Bind(6601);
	this->Listen(m_pSock->_sClient, 5);
}

void CServer::run()
{
	this->msgDisplay("服务器启动成功!");

	int nIndex;
	WSANETWORKEVENTS event;
	CClient *m_pMsgClient;

	while (true)
	{
		nIndex = WSAWaitForMultipleEvents(nEventTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
		nIndex = nIndex - WSA_WAIT_EVENT_0;
		m_pMsgClient = sockArray[nIndex];
		if (nIndex == WSA_WAIT_FAILED || nIndex == WSA_WAIT_TIMEOUT){
			continue;
		}
		else
		{
			retval_ =  WSAEnumNetworkEvents(m_pMsgClient->_sClient, eventArray[nIndex], &event);
			if (retval_ == SOCKET_ERROR)
				continue;
			if (event.lNetworkEvents & FD_ACCEPT)
			{
				if (event.iErrorCode[FD_ACCEPT_BIT] == 0)
				{
					if (nEventTotal > WSA_MAXIMUM_WAIT_EVENTS){
						continue;
					}
					
					this->Accept(m_pMsgClient->_sClient);
				}
			}
			else if (event.lNetworkEvents & FD_WRITE)
			{
				//int ret = m_pMsgClient->SendData(sockArray[nIndex]->_sClient,);
			}
			else if (event.lNetworkEvents & FD_READ)         // 处理FD_READ通知消息  
			{
				if (event.iErrorCode[FD_READ_BIT] == 0)
				{
					int ret = m_pMsgClient->RecvData(sockArray[nIndex]->_sClient);
					if(ret < 0){
						this->errDisplay(__FUNCTION__"-recv()");
					}
				}
			}
			else if (event.lNetworkEvents & FD_CLOSE)
			{
				RoomManage::getInstance()->quitFromRoom(sockArray[nIndex]);
				this->removeClient(sockArray[nIndex], nIndex);
				if (event.iErrorCode[FD_CLOSE_BIT] != 0) {
					this->errDisplay("! FD_CLOSE_BIT", event.iErrorCode[FD_CLOSE_BIT]);
					continue;
				}
			}
		}
	}
}

void CServer::removeClient(CClient* _client, int idx)
{
	SOCKET ptr = sockArray[idx]->_sClient;
	closesocket(ptr);
	/*_client = nullptr;
	delete _client;*/
	_client = nullptr;
	
	delete  sockArray[idx];
	sockArray[idx] = nullptr;

	WSACloseEvent(eventArray[idx]);
	

	if (idx != nEventTotal - 1) {
		sockArray[idx] = sockArray[nEventTotal - 1];
		eventArray[idx] = eventArray[nEventTotal - 1];
	}
	--nEventTotal;
	
	this->msgDisplay("客户端退出!");
}

void CServer::errQuit(const char * msg)
{
	LPVOID msgBuf;
	::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, ::WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&msgBuf), 0, NULL);

	std::cout << msg << " " << static_cast<CHAR*>(msgBuf);
	::LocalFree(msgBuf);
	::exit(1);
}

string CServer::getTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char temp[64] = { 0 };
	sprintf_s(temp, "[%4d-%02d-%02d %02d:%02d:%02d]", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return temp;
}


void CServer::errDisplay(const char * msg)
{
	LPVOID msgBuf;
	::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, ::WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&msgBuf), 0, NULL);

	std::cout << getTime() << msg << " " << static_cast<CHAR*>(msgBuf);
	::LocalFree(msgBuf);
}

void CServer::errDisplay(const char * msg, int errCode)
{
	std::cout << getTime() << "[" <<  msg << "] " << "errCode: " << errCode << std::endl;
}

void CServer::msgDisplay(const char * msg)
{
	std::cout << getTime() << "[" << msg << "] " << std::endl;
}
