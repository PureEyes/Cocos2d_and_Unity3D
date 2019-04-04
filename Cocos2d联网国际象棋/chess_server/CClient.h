#pragma once
#include <WinSock2.h>
#include "buffer.h"
#include "def.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "DataBase.h"
//#include "RoomManage.h"

using namespace rapidjson;

class CClient
{
public:
	CClient( SOCKET m_sock);
	
	~CClient();
	//发送消息
	int SendData(const char* msg);
	//接收消息
	int RecvData(SOCKET client);

	inline const char* getClientName() { return __userName; };
	//inline void setClientName(const char* _cName) { __userName = _cName; };

	inline std::string getMsg() { return m_pMsg; };
	inline void setMsg(std::string str) { m_pMsg = str; };

	inline bool getIsReady() { return __isReady; };
	inline void setIsReady(bool en) { __isReady = en; };

	inline bool getIsSendUserData() { return __isSendUserData; };
	inline void setIsSendUserData(bool en) { __isSendUserData = en; };

	inline bool getIsMatch() { return __isMatch; };
	inline void setIsMatch(bool en) { __isMatch = en; };

	void EnCode();
	void DeCode();


	//解析消息头
	int  AnalysisMsgHead();	
	//返回给客户端的消息
	void AfterProcesMsg(int msgType, int ret);	
	//处理登陆注册消息
	void ProcesLoginOrRegisterMsg(int msgType);		
	//处理登陆注册消息
	void ProcesCreatRoomOrMatchMsg(int msgType);
	//处理请求读取游戏记录消息
	void ProcesReadGameDataMsg(int msgType);
	////处理请求读他人戏记录消息
	//void ProcesReadOtherGameDataMsg(int msgType);
	//处理修改个人信息消息
	void ProcesModifInfoMsg(int msgType);
	//处理退出房间消息
	void ProcesQuitRoomMsg(int msgType);
	//处理准备就绪消息
	void ProcesReadyMsg(int msgType);
	//处理移动棋子消息
	void ProcesMoveChessMsg(int msgType);
	//处理求和消息
	void ProcesPeaceMsg(int msgType);
	//处理认输消息
	void ProcesGiveUpMsg(int msgType);
	//处理悔棋消息
	void ProcesRegretMsg(int msgType);



	//发送匹配成功消息
	void SendMatchFinishMsg(int msgType, int ret);
	//发送用户信息给房间玩家
	void SendUserinfoToRoomPlayer(CMD_C_DATALOAD data);
	//发送开始游戏信息
	void SendStartGameMsg(int msgType, int ret);
	//发送游戏结束消息
	void SendGameOverMsg(CMD_S_GameEnd data, int msgType, int ret);


	string getTime();

	void printError(int errcode);
	void printMsg(string msg);
public:
	SOCKET _sClient;

	Buffer *buf;

	
private:
	char __userName[16];

	bool __isReady = false;									//是否准备好
			
	bool __isSendUserData = false;							//是否发送过对手信息

	bool __isMatch = false;									//是否匹配成功

	//char* m_pMsg;
	std::string m_pMsg = "";

private:
	
};

