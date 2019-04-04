#pragma once
#include <vector>
#include <WinSock2.h>
#include "CClient.h"
#include "def.h"
#include "GameLogic.h"


class Room
{
public:
	Room();
	virtual ~Room();
	
private:
	int room_id;													//�����														
	int _maxPlayerNum;												//��������				
	bool isFall;													//�����Ƿ�����									
	int _currPlayerNum;												//��ǰ�����						
public:
	//��������б�
	std::vector<CClient*> _player_List;		
	//��ȡ��ǰ�����������
	inline int getCurrPlayerNum() { return _currPlayerNum; };
	//��ȡ����id
	inline int getRoomId() { return room_id; };		
	//���÷���id
	inline void setRoomId(int id) { room_id = id; };	
	//���ط����Ƿ�����
	inline bool getRoomFall() { return isFall; };					
	//��Ҽ��뷿��
	void addPlayer(CClient* player);	
	//����˳�����
	void subPlayer(CClient* player);	
	//��ȡ����ڷ����б�����±�
	int getPlayerInRoomIndex(CClient* player);
	//ת����ҵ��ƶ�������Ϣ
	void TranspondMoveChessMsg(CMD_S_MoveChess data, CClient* player, int msgType);
	//ת�������Ϣ
	void TranspondPeaceMsg(CClient* player, int msgType, int ret);

	string getTime();

public:
	//��Ϸ��ʼ
	bool OnEventGameStart();
	//��Ϸ����
	bool OnEventGameEnd(int cbReason, int userId);
	//�ƶ�����
	int OnUserMoveChess(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess, CClient* player);
	//��������
	bool OnUserRegretReq();
	//����Ӧ��
	bool OnUserRegretAnswer();
	//�������
	void OnUserPeaceReq(CClient* player, int msgType, int en);
	//���Ӧ��
	bool OnUserPeaceAnswer();
	//�����¼�
	bool OnUserGiveUp();

private:


protected:
	GameLogic					*m_pGameLogic;						//��Ϸ�߼�
	int							m_blackUser;						//�������
	int							m_currentUser;						//��ǰ�û�
	int							m_begStatus[2];						//����״̬
	int							m_peaceCount[2];					//��ʹ���
	int							m_regretCount[2];					//�������
};

