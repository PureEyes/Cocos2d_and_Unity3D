#ifndef _CCLIENT_H_
#define _CCLIENT_H_

//-----------����ͻ��˽ӵ���Ϣ���֪ͨ--------------
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
	//���ӷ�����
	void Connect(const char* ip, unsigned short port);					
	void ProcessMsg();
	//�˳���½
	void Quit();														
	//������Ϣ
	void RecvData();
	//������Ϣ
	void SendData(const char *msg);		
	
	void update(float dt);
public:
	///send
	//������½ע����Ϣ
	const char* MsgCreateLoginOrRegister(const char name[], const char passwd[]);
	//������½��Ϣ
	const char* MsgCreateLogin(const char name[], const char passwd[]);
	//����ע����Ϣ
	const char* MsgCreateRegister(const char name[], const char passwd[]);
	//����������Ϣ
	const char* MsgCreateRoom();
	//�����˳�������Ϣ
	const char* MsgCreateQuitRoom();
	//����ƥ����Ϣ
	const char* MsgCreateMatch();
	//������ȡ������Ϣ��Ϣ
	const char* MsgCreateReadUserInfo();
	//������ȡ������Ϣ��Ϣ
	const char* MsgCreateReadRivalInfo();
	//�����޸ĸ�����Ϣ
	const char* MsgCreateModifUserInfo(CMD_C_MODIFINFO data);
	//����׼����Ϣ
	const char* MsgCreateReady();
	//��������Ϣͷ
	const char* CreateSimplenesMsgHead();
	//�����ƶ�������Ϣ
	const char* MsgCreateMoveChess(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess);
	//���������Ϣ
	const char* MsgCreatePeace();
	//������������Ӧ��Ϣ
	const char* MsgCreatePeaceAnswer(CMD_C_PeaceAnswer data);
	//����������Ϣ
	const char* MsgCreateGiveUp();
	//������ȡ������Ϣ
	const char* MsgCreateReadRankingList();
	//������ȡ�����б���Ϣ
	const char* MsgCreateReadRoomList();

	///read
	//���Է���˲����ķ���ֵ
	void RespondFromServerMsg(string msg);											
	//������Ϣͷ																																																												
	void AnalysisMsgHead(string msg);
	//��ȡ����ֵ
	int getRespond() { return respond; };
	//void setRespond(int res) { respond = res; };
	//��ȡ������Ϣ
	CMD_C_DATALOAD MsgReadUserData(string msg);
	//��ȡ�ƶ�������Ϣ
	CMD_S_MoveChess MsgMoveChess(string msg);
	CMD_S_GameEnd MsgGameOver(string msg);
	//��ȡ����
	CMD_S_RankingList MsgReadRankingList(string msg);

	//inline void setCurrSendMsgType(int msgType) { msg_send_type = msgType; };
	inline int getCurrSendMsgType() { return msg_send_type; };

	inline int getCurrRecvMsgType() { return msg_recv_type; };
	
public:
	bool isConnect;
	vector<string> _msgQueue;									//��Ϣ����
	string __userName;
	int	   __avta_index;
	int    __totalPlayNum;										//�ܳ���
	int	   __totalWinNum;										//��Ӯ����
	int	   __totalLoseNum;										//���䳡��
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
	
	thread recvThr;												//���������߳�
	
	int msg_send_type = -1;										//������Ϣ����
	int msg_recv_type = -1;										//�յ���Ϣ����
	int respond = -2;											//����ֵ
	char recvBuf[512];											//���ջ�����
	char sendBuf[512];											//���ͻ�����
};


#endif // _CCLIENT_H