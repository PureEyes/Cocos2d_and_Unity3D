// chess_server.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CServer.h"

int main()
{
	CServer *server = CServer::getInstance();

	server->InitServer();

	server->run();

	return 0;
}
