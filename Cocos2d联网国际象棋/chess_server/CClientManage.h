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
	//添加到匹配队列
	void addToMatchList(CClient* _player);	
	//退出匹配队列
	void backToMatchList(CClient* _player);	
	//开始匹配
	void StartMatch(CClient* _player, int msgType);
	//发送个人信息给房间玩家
	void SendUserInfoMsgToRoomPlayers(Room* _room);
	//发送开始游戏信息
	void SendStartGame(Room* _room);
	//void ProcessMessage();
	string getTime();

private:
	



public:
	
	std::vector<CClient*> _matchList;											//匹配队列						
	
private:										
	static CClientManage* instance;
	CClientManage();
};

