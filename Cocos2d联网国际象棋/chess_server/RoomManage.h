#pragma once
#include "Room.h"

class RoomManage
{
public:
	~RoomManage();
	static RoomManage* getInstance();
	//��������
	Room* createRoom();
	//��������
	int createRoom(CClient* player);
	//���뷿��	
	bool joinToRoom(int _id, CClient* player);
	//�˳�����
	int quitFromRoom(int _id, CClient* player);
	//�˳�����
	int quitFromRoom(CClient* player);
	//��ȡ����
	Room* getRoomById(int _id);	
	//���·����б�
	void upDateRoomList(int _id);	
	//�ж�����Ƿ��ڷ�����,�����ط������ڷ����
	int playeIsJoinRoom(CClient* player);					

	string getTime();
private:
	RoomManage();
	static RoomManage *instance;										//����					
public:
	
	std::vector<Room*> _roomList;										//�����б�
};

