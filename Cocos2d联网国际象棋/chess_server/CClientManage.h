#pragma once

#include "RoomManage.h"
#include <cstdlib>
#include <ctime>

#define random(a,b) (rand()%(b-a+1)+a)

class CClientManage
{
public:
	
	~CClientManage();
	//CClientManage();
	static CClientManage* getInstance();
	//��ӵ�ƥ�����
	void addToMatchList(CClient* _player);	
	//�˳�ƥ�����
	void backToMatchList(CClient* _player);	
	//��ʼƥ��
	void StartMatch(CClient* _player, int msgType);
	//���͸�����Ϣ���������
	void SendUserInfoMsgToRoomPlayers(Room* _room);
	//���Ϳ�ʼ��Ϸ��Ϣ
	void SendStartGame(Room* _room);
	//void ProcessMessage();
	string getTime();

private:
	



public:
	
	std::vector<CClient*> _matchList;											//ƥ�����						
	
private:										
	static CClientManage* instance;
	CClientManage();
};

