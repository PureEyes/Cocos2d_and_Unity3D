#pragma once

#include "def.h"
#include <WinSock2.h>
#include <cstdlib>
#include "CClient.h"
#include "RoomManage.h"

#pragma comment(lib,"ws2_32.lib") 

class CServer
{
public:
	//CServer();
	~CServer();

	static CServer* getInstance();

	void InitServer();
	void run();

private:
	bool Create(int af, int type, int protocol = 0);

	bool Bind(unsigned short port);

	bool Listen(SOCKET listen, int backlog = 5);

	bool Accept(SOCKET socket);

	//int Send(SOCKET client, const char* buf, int len, int flags = 0);

	//bool Recv(char* buf, int len, int flags = 0);

	void errQuit(const char* msg);

	void errDisplay(const char* msg);

	void errDisplay(const char* msg, int errCode);

	void msgDisplay(const char* msg);

	void removeClient(CClient* _client, int idx);								//当连接退出时

	//转发消息

	//

	string getTime();

	CServer();

private:

	static CServer* instance;

	CClient* m_pSock;										//监听套接字

	WSAEVENT    eventArray[200];		//WSA_MAXIMUM_WAIT_EVENTS = 64
	//SOCKET		sockArray[WSA_MAXIMUM_WAIT_EVENTS];
	CClient*	sockArray[200];
	//std::vector<CClient*> sockArray;

	int retval_;											//返回信息

	int nEventTotal = 0;									//总连接数
	
	//BufferOperate* bufOper = BufferOperate::getInstance();
	//test code
	bool isFall = true;
	int i = 0;
	//
	//Buffer* buf;
};

