#ifndef _CCLIENT_H_
#define _CCLIENT_H_

//-----------定义客户端接到消息后的通知--------------
#define REQUEST_FINISH_NOTIFICATION								"Finish"
#define SUB_C_LOGIN_REQ_NOTIFICATION							"Login"
#define SUB_C_REGISTER_REQ_NOTIFICATION							"Register"
#define SUB_C_CREAT_ROOM_NOTIFICATION							"CreatRoom"
#define SUB_C_START_MATCH_NOTIFICATION							"StartMatch"
#define SUB_C_MODIF_INFO_NOTIFICATION							"ModifInfo"
#define SUB_C_DATA_LOAD_SELF_NOTIFICATION						"DataLoadSelf"
#define SUB_C_DATA_LOAD_RIVAL_NOTIFICATION						"DataLoadRival"
#define SUB_C_QUIT_ROOM_NOTIFICATION							"QuitRoom"

#define SUB_S_MOVE_CHESS_RIVAL_NOTIFICATION						"MoveChessRival"
#define SUB_S_MOVE_CHESS_SELF_NOTIFICATION						"MoveChessSelf"

#define SUB_S_GAME_START_NOTIFICATION							"StartGame"
#define SUB_S_GAME_END_NOTIFICATION								"GameOver"

#define SUB_S_REGRET_REQ_NOTIFICATION							"Regret"
#define SUB_S_REGRET_FAILE_NOTIFICATION							"RegretFaile"

#define SUB_S_PEACE_REQ_NOTIFICATION							"Peace"
#define SUB_S_PEACE_ANSWER_NOTIFICATION							"PeaceAnswer"

#define SUB_C_READ_RANKINGLIST_NOTIFICATION						"RankingList"

#define AVTA_CHANGE_NOTIFICATION								"chargeAvta"
#define CHESSBORD_CHANGE_NOTIFICATION							"chargeChessBord"

//--------------------------------------------------

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"


#include "ODSocket.h"
#include "buffer.h"
#include "def.h"
//#include <io.h>
//#include <regex>
#include <string>
#include <iostream>

using namespace rapidjson;
using namespace std;
USING_NS_CC;
#pragma execution_character_set("utf-8")

class CClient : public Node
{
public:
	~CClient();
	static CClient* getInstance();
	//连接服务器
	void Connect(const char* ip, unsigned short port);					
	void ProcessMsg();
	//退出登陆
	void Quit();														
	//接收消息
	void RecvData();
	//发送消息
	void SendData(const char *msg);		
	
	void update(float dt);
public:
	///send
	//创建登陆注册消息
	const char* MsgCreateLoginOrRegister(const char name[], const char passwd[]);
	//创建登陆消息
	const char* MsgCreateLogin(const char name[], const char passwd[]);
	//创建注册消息
	const char* MsgCreateRegister(const char name[], const char passwd[]);
	//创建房间消息
	const char* MsgCreateRoom();
	//创建退出房间消息
	const char* MsgCreateQuitRoom();
	//创建匹配消息
	const char* MsgCreateMatch();
	//创建读取个人信息消息
	const char* MsgCreateReadUserInfo();
	//创建读取别人信息消息
	const char* MsgCreateReadRivalInfo();
	//创建修改个人信息
	const char* MsgCreateModifUserInfo(CMD_C_MODIFINFO data);
	//创建准备消息
	const char* MsgCreateReady();
	//创建简单消息头
	const char* CreateSimplenesMsgHead();
	//创建移动棋子消息
	const char* MsgCreateMoveChess(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess);
	//创建求和消息
	const char* MsgCreatePeace();
	//创建求和请求答应消息
	const char* MsgCreatePeaceAnswer(CMD_C_PeaceAnswer data);
	//创建认输消息
	const char* MsgCreateGiveUp();
	//创建读取排行消息
	const char* MsgCreateReadRankingList();
	//创建读取房间列表消息
	const char* MsgCreateReadRoomList();

	///read
	//来自服务端操作的返回值
	void RespondFromServerMsg(string msg);											
	//解析消息头																																																												
	void AnalysisMsgHead(string msg);
	//获取返回值
	int getRespond() { return respond; };
	//void setRespond(int res) { respond = res; };
	//读取个人信息
	CMD_C_DATALOAD MsgReadUserData(string msg);
	//读取移动棋子信息
	CMD_S_MoveChess MsgMoveChess(string msg);
	CMD_S_GameEnd MsgGameOver(string msg);
	//读取排行
	CMD_S_RankingList MsgReadRankingList(string msg);

	//inline void setCurrSendMsgType(int msgType) { msg_send_type = msgType; };
	inline int getCurrSendMsgType() { return msg_send_type; };

	inline int getCurrRecvMsgType() { return msg_recv_type; };
	
public:
	bool isConnect;
	vector<string> _msgQueue;									//消息队列
	string __userName;
	int	   __avta_index;
	int    __totalPlayNum;										//总场次
	int	   __totalWinNum;										//总赢场数
	int	   __totalLoseNum;										//总输场数
	int    __chessBord_Index;
	string	m_pMsg;
	
	float _AudioVolume = 1.0f;
	bool  _OpenAudio = true;
	bool  _isWin = false;

private:
	CClient();
	static CClient* instance;
	ODSocket* client;
	

	Buffer* buf;
	
	thread recvThr;												//接收数据线程
	
	int msg_send_type = -1;										//发送消息类型
	int msg_recv_type = -1;										//收到消息类型
	int respond = -2;											//返回值
	char recvBuf[512];											//接收缓冲区
	char sendBuf[512];											//发送缓冲区
};


#endif // _CCLIENT_H