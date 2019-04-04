#pragma once
#include "Room.h"

class RoomManage
{
public:
	~RoomManage();
	static RoomManage* getInstance();
	//创建房间
	Room* createRoom();
	//创建房间
	int createRoom(CClient* player);
	//加入房间	
	bool joinToRoom(int _id, CClient* player);
	//退出房间
	int quitFromRoom(int _id, CClient* player);
	//退出房间
	int quitFromRoom(CClient* player);
	//获取房间
	Room* getRoomById(int _id);	
	//更新房间列表
	void upDateRoomList(int _id);	
	//判断玩家是否在房间中,并返回房间所在房间号
	int playeIsJoinRoom(CClient* player);					

	string getTime();
private:
	RoomManage();
	static RoomManage *instance;										//单例					
public:
	
	std::vector<Room*> _roomList;										//房间列表
};

