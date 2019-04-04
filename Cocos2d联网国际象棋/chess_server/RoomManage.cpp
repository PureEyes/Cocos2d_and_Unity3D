#include "stdafx.h"
#include "RoomManage.h"
#include <iostream>
#include "CClientManage.h"

RoomManage* RoomManage::instance = nullptr;

RoomManage * RoomManage::getInstance()
{
	if (instance == nullptr) {
		instance = new RoomManage();
	}
	return instance;
}

RoomManage::RoomManage(){}
RoomManage::~RoomManage(){}

/// �������亯��
/// param
/// return �������ķ���
Room* RoomManage::createRoom()
{
	Room* _currRoom = new Room();
	_currRoom->setRoomId(_roomList.size());
	_roomList.push_back(_currRoom);
	return _currRoom;
}

/// ��������ͨ����Ҵ���
/// param player ���
/// return �������ķ���
int RoomManage::createRoom(CClient* player)
{
	//�ж�����Ƿ��Ѿ��ڷ�����
	if (playeIsJoinRoom(player) == NO_RESPOND) {
		Room* _currRoom = new Room();
		_currRoom->setRoomId(_roomList.size());
		_roomList.push_back(_currRoom);
		joinToRoom(_currRoom->getRoomId(), player);
		std::cout << getTime() << "�����б���:" << _roomList.size() << std::endl;

		//�����������˳�ƥ�����
		CClientManage::getInstance()->backToMatchList(player);
		std::cout << getTime() << "ƥ���б���:" << CClientManage::getInstance()->_matchList.size() << std::endl;
		return CREAT_ROOM_SUCCEED;
	}
	return CREAT_ROOM_FAILED;
}

/// ָ�����뷿��
/// param _id ����id
/// param player ���
/// return 
bool RoomManage::joinToRoom(int _id, CClient* player)
{
	Room *room_ = getRoomById(_id);
	if (room_->getRoomFall() == false) {
		room_->addPlayer(player);
		return true;
	}
	
	return false;
}

/// �˳��뷿��
/// param _id ����id
/// param player ���
/// return 
int RoomManage::quitFromRoom(int _id, CClient* player)
{
	Room* room_ = getRoomById(_id);

	room_->subPlayer(player);
	std::cout << getTime() << "�����б��ȣ�" << _roomList.size() << "����������"<<room_->getCurrPlayerNum() << std::endl;
	
	int a = room_->getCurrPlayerNum();
	if ( a== 0) {
		std::vector<Room*>::iterator iter = _roomList.begin();
		//*iter = _roomList.at(_id);
		_roomList.erase(iter+_id);
		upDateRoomList(_id);
		return QUIT_ROOM_SUCCEED;
	}
	return QUIT_ROOM_FAILED;
}

/// �˳��뷿��
/// param player ���
/// return 
int RoomManage::quitFromRoom(CClient * player)
{
	if(player == nullptr) return QUIT_ROOM_FAILED;

	for (int i = 0; i < _roomList.size(); i++)
	{
		auto room_ = getRoomById(i);
		int id_ = room_->getRoomId();
		auto playList = room_->_player_List;
		for (int j = 0; j <playList.size(); j++)
		{
			if (playList[j] == player) {
				room_->subPlayer(player);
			}
		}

		int playNum = room_->getCurrPlayerNum();
		if (playNum == 0) {
			std::vector<Room*>::iterator iter = _roomList.begin();
			//*iter = _roomList.at(_id);
			_roomList.erase(iter + id_);
			upDateRoomList(id_);
			std::cout << getTime() << "�����б��ȣ�" << _roomList.size() << "����������" << room_->getCurrPlayerNum() << std::endl;
		}
	}
	return QUIT_ROOM_SUCCEED;
}

/// ��ȡ����
/// pamra _id ����id
/// return ����
Room* RoomManage::getRoomById(int _id)
{
	return _roomList.at(_id);
}

/// ���·����б�
/// param _id ����λ��
/// return
void RoomManage::upDateRoomList(int _id)
{
	for (int i = _id; i < _roomList.size(); i++){
		_roomList[i]->setRoomId(i);
	}
}

int RoomManage::playeIsJoinRoom(CClient * player)
{
	for (int i = 0; i < _roomList.size(); i++) {
		Room* __room = getRoomById(i);
		for (int j = 0; j < __room->_player_List.size(); j++) {
			if (__room->_player_List[j] == player) {
				return i;
			}
		}
	}
	return NO_RESPOND;
}

string RoomManage::getTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char temp[64] = { 0 };
	sprintf_s(temp, "[%4d-%02d-%02d %02d:%02d:%02d]", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return temp;
}
