#include "stdafx.h"
#include "CClientManage.h"

CClientManage* CClientManage::instance = nullptr;

CClientManage::CClientManage()
{
}
CClientManage::~CClientManage()
{
}

CClientManage * CClientManage::getInstance()
{
	if (instance == nullptr) {
		instance = new CClientManage();
	}
	return instance;
}

void CClientManage::addToMatchList(CClient * _player)
{
	for (int i = 0; i < _matchList.size(); i++) {
		if (_matchList[i] == _player) {
			return;
		}
	}
	//加入匹配则退出房间
	RoomManage::getInstance()->quitFromRoom(_player);
	std::cout << getTime() << "房间列表长度:" << RoomManage::getInstance()->_roomList.size() << std::endl;
	_matchList.push_back(_player);
	std::cout << getTime() << "匹配列表长度:" << _matchList.size() << std::endl;
}

void CClientManage::backToMatchList(CClient * _player)
{
	std::vector<CClient*>::iterator iter = _matchList.begin();
	for (int i = 0; i < _matchList.size(); i++)
	{
		if (_matchList.at(i) == _player) {
			_matchList.erase(iter + i);
			break;
		}
	}
}

void CClientManage::StartMatch(CClient* _player, int msgType)
{
	int a =0, b = 0;
	int len = _matchList.size();
	if (len < 2) {
		_player->SendMatchFinishMsg(msgType, MATCH_FAILED);
		return;
	}
	else {
		Room* room = RoomManage::getInstance()->createRoom();
		srand((unsigned)time(nullptr));
		for (int i = 0; i < 2; i++)
		{
			int index = random(0, _matchList.size()-1);
			int rId_ = room->getRoomId();
			CClient* __player = _matchList.at(index);
			__player->SendMatchFinishMsg(msgType, MATCH_SUCCEED);
			__player->setIsMatch(true);
			RoomManage::getInstance()->joinToRoom(rId_, __player);
			backToMatchList(__player);
		}
		//SendUserInfoMsgToRoomPlayers(room);
		//if (room->_player_List[0]->getIsMatch() == true && room->_player_List[1]->getIsMatch() == true) SendUserInfoMsgToRoomPlayers(room);
		
	}
}

void CClientManage::SendUserInfoMsgToRoomPlayers(Room* _room)
{
	//房间没满不发送用户信息
	if (_room->getRoomFall() == false) return;

	auto playList = _room->_player_List;

	vector<CMD_C_DATALOAD> _inFoList;
	
	if (playList[0]->getIsSendUserData() == false && playList[1]->getIsSendUserData() == false) {
		for (int i = 0; i < playList.size(); i++) {
			auto name = playList[i]->getClientName();
			CMD_C_DATALOAD data = DataBase::getInstance()->user_load_game_data(name);
			_inFoList.push_back(data);
		}
		playList[0]->SendUserinfoToRoomPlayer(_inFoList[1]);
		playList[1]->SendUserinfoToRoomPlayer(_inFoList[0]);
		playList[0]->setIsSendUserData(true);
		playList[1]->setIsSendUserData(true);
	}

	//房间用户没全部准备好，不发送开始游戏信息
	/*if (playList[0]->getIsReady() == true && playList[1]->getIsReady() == true) {
		for (int i = 0; i < playList.size(); i++) playList[i]->SendStartGameMsg(SUB_S_GAME_START, NO_RESPOND);
	}*/
}

void CClientManage::SendStartGame(Room* _room)
{
	//房间没满不发送用户信息
	if (_room->getRoomFall() == false) return;

	auto playList = _room->_player_List;

	//房间用户没全部准备好，不发送开始游戏信息
	if (playList[0]->getIsReady() == true && playList[1]->getIsReady() == true) {
		for (int i = 0; i < playList.size(); i++) {
			//ret为玩家的棋子颜色
			int ret = _room->getPlayerInRoomIndex(playList[i]);
			playList[i]->SendStartGameMsg(SUB_S_GAME_START, ret);
		} 
		_room->OnEventGameStart();
	}
}

string CClientManage::getTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char temp[64] = { 0 };
	sprintf_s(temp, "[%4d-%02d-%02d %02d:%02d:%02d]", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return temp;
}
