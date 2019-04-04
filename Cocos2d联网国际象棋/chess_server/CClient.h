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
	//������Ϣ
	int SendData(const char* msg);
	//������Ϣ
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


	//������Ϣͷ
	int  AnalysisMsgHead();	
	//���ظ��ͻ��˵���Ϣ
	void AfterProcesMsg(int msgType, int ret);	
	//�����½ע����Ϣ
	void ProcesLoginOrRegisterMsg(int msgType);		
	//�����½ע����Ϣ
	void ProcesCreatRoomOrMatchMsg(int msgType);
	//���������ȡ��Ϸ��¼��Ϣ
	void ProcesReadGameDataMsg(int msgType);
	////�������������Ϸ��¼��Ϣ
	//void ProcesReadOtherGameDataMsg(int msgType);
	//�����޸ĸ�����Ϣ��Ϣ
	void ProcesModifInfoMsg(int msgType);
	//�����˳�������Ϣ
	void ProcesQuitRoomMsg(int msgType);
	//����׼��������Ϣ
	void ProcesReadyMsg(int msgType);
	//�����ƶ�������Ϣ
	void ProcesMoveChessMsg(int msgType);
	//���������Ϣ
	void ProcesPeaceMsg(int msgType);
	//����������Ϣ
	void ProcesGiveUpMsg(int msgType);
	//���������Ϣ
	void ProcesRegretMsg(int msgType);



	//����ƥ��ɹ���Ϣ
	void SendMatchFinishMsg(int msgType, int ret);
	//�����û���Ϣ���������
	void SendUserinfoToRoomPlayer(CMD_C_DATALOAD data);
	//���Ϳ�ʼ��Ϸ��Ϣ
	void SendStartGameMsg(int msgType, int ret);
	//������Ϸ������Ϣ
	void SendGameOverMsg(CMD_S_GameEnd data, int msgType, int ret);


	string getTime();

	void printError(int errcode);
	void printMsg(string msg);
public:
	SOCKET _sClient;

	Buffer *buf;

	
private:
	char __userName[16];

	bool __isReady = false;									//�Ƿ�׼����
			
	bool __isSendUserData = false;							//�Ƿ��͹�������Ϣ

	bool __isMatch = false;									//�Ƿ�ƥ��ɹ�

	//char* m_pMsg;
	std::string m_pMsg = "";

private:
	
};

