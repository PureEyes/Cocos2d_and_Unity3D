#include "stdafx.h"
#include "Room.h"
#include <iostream>
#include "DataBase.h"

Room::Room()
{
	_currPlayerNum = 0;
	_maxPlayerNum = 2;
	isFall = false;
	m_pGameLogic = new GameLogic();
	m_blackUser = BLACK_PLAYER;
	m_currentUser = WHITE_PLAYER;
}

Room::~Room()
{
}

void Room::addPlayer(CClient* player)
{
	if (_currPlayerNum < _maxPlayerNum){
		_player_List.push_back(player);
		_currPlayerNum++;
		std::cout << getTime() << "房间号：" << getRoomId() << "人数："<< _currPlayerNum << std::endl;
		if (_currPlayerNum == _maxPlayerNum) {
			isFall = true;
		}
	}
	
	return;
}

void Room::subPlayer(CClient* player)
{
	std::vector<CClient*>::iterator iter = _player_List.begin();
	for (int i = 0; i < _player_List.size(); i++)
	{
		//*iter = _player_List.at(i);
		if (_player_List.at(i) == player) {
			player->setIsReady(false);
			player->setIsSendUserData(false);
			player->setIsMatch(false);
			_player_List.erase(iter + i);
			player = nullptr;
			_currPlayerNum--;
			isFall = false;
			std::cout << getTime() << "房间号：" << getRoomId() << "人数：" << _currPlayerNum << std::endl;
			break;
		}
	}
}

int Room::getPlayerInRoomIndex(CClient* player)
{
	for (int i = 0; i < _player_List.size(); i++) {
		if (_player_List[i] == player) {
			return i;
			break;
		}
	}
}

void Room::TranspondMoveChessMsg(CMD_S_MoveChess data, CClient* player, int msgType)
{
	Document _doc;
	_doc.SetObject();
	Document::AllocatorType& alloctor = _doc.GetAllocator();
	_doc.AddMember("msgType", msgType, alloctor);
	_doc.AddMember("ret", enMoveType_Normal, alloctor);
	rapidjson::Value obj(kObjectType);
	obj.AddMember("xSourcePos", data.xSourcePos, alloctor);
	obj.AddMember("ySourcePos", data.ySourcePos, alloctor);
	obj.AddMember("xTargetPos", data.xTargetPos, alloctor);
	obj.AddMember("yTargetPos", data.yTargetPos, alloctor);
	obj.AddMember("switchChess", data.switchChess, alloctor);
	obj.AddMember("currentUser", data.currentUser, alloctor);
	_doc.AddMember("CMD_S_MoveChess", obj, alloctor);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_doc.Accept(writer);
	const char* msg = buffer.GetString();

	CClient* rPlayer = nullptr;
	for (int i = 0; i < _player_List.size(); i++) {
		if (player != _player_List[i]) rPlayer = _player_List[i];
	}
	rPlayer->SendData(msg);
}

void Room::TranspondPeaceMsg(CClient * player, int msgType,int ret)
{
	CClient* rPlayer = nullptr;
	for (int i = 0; i < _player_List.size(); i++) {
		if (player != _player_List[i]) {
			rPlayer = _player_List[i];
			break;
		} 
	}
	rPlayer->AfterProcesMsg(msgType, ret);
}

string Room::getTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char temp[64] = { 0 };
	sprintf_s(temp, "[%4d-%02d-%02d %02d:%02d:%02d]", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return temp;
}
//----------------游戏消息-------------------
bool Room::OnEventGameStart()
{
	//设置变量
	//m_currentUser = m_blackUser;
	//重置棋盘
	m_pGameLogic->ResetChessBorad();

	m_pGameLogic->printChessBord();
	m_pGameLogic->printChessID();
	m_pGameLogic->printChessPos();
	m_pGameLogic->printChessColor();

	return true;
}

bool Room::OnEventGameEnd(int cbReason, int userId)
{
	switch (cbReason)
	{
	case GAMEOVER_NORMAL:
	case GAMEOVER_GIVEUP:
		CMD_S_GameEnd data;
		data.wWinUser = userId;
		for (int i = 0; i < _player_List.size(); i++) _player_List[i]->SendGameOverMsg(data, SUB_S_GAME_END, cbReason);
		break;
	case GAMEOVER_USER_LEFT:
		break;
	}
	return false;
}

int Room::OnUserMoveChess(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess, CClient* player)
{
	assert(xSourcePos < 8 && ySourcePos < 8);
	assert(xTargetPos < 8 && yTargetPos < 8);
	if (xSourcePos >= 8 || ySourcePos >= 8) return MOVECHESS_FAILED;
	if (xTargetPos >= 8 || yTargetPos >= 8) return MOVECHESS_FAILED;

	int userColor = (m_currentUser == m_blackUser) ? BLACK_CHESS : WHITE_CHESS;

	const chessIten* pSourceChessItem = m_pGameLogic->GetChessItem(xSourcePos, ySourcePos);
	assert((pSourceChessItem != nullptr) && (pSourceChessItem->color == userColor));
	if ((pSourceChessItem == NULL) || (pSourceChessItem->color != userColor)) return MOVECHESS_FAILED;

	//移动棋子
	if (m_pGameLogic->IsWalkLegality(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false) return MOVECHESS_FAILED;
	if (m_pGameLogic->MoveChess(xSourcePos, ySourcePos, xTargetPos, yTargetPos, switchChess) == enMoveType_Error) return MOVECHESS_FAILED;

	//结束判断
	bool bGameEnd = m_pGameLogic->IsGameFinish((m_currentUser == m_blackUser) ? WHITE_CHESS : BLACK_CHESS);

	//切换用户
	m_currentUser = (m_currentUser + 1) % _maxPlayerNum;
	//构造数据
	CMD_S_MoveChess MoveChess;
	MoveChess.xSourcePos = xSourcePos;
	MoveChess.ySourcePos = ySourcePos;
	MoveChess.xTargetPos = xTargetPos;
	MoveChess.yTargetPos = yTargetPos;
	MoveChess.switchChess = switchChess;
	MoveChess.currentUser = m_currentUser;
	TranspondMoveChessMsg(MoveChess, player, SUB_S_MOVE_CHESS);

	m_pGameLogic->printChessBord();
	m_pGameLogic->printChessID();
	m_pGameLogic->printChessPos();
	m_pGameLogic->printChessColor();

	if (bGameEnd == true)  OnEventGameEnd(GAMEOVER_NORMAL, m_currentUser);

	return MOVECHESS_SUCCEED;
}

void Room::OnUserPeaceReq(CClient* player, int msgType, int en)
{
	TranspondPeaceMsg(player, msgType, en);
}

bool Room::OnUserGiveUp()
{
	return OnEventGameEnd(GAMEOVER_GIVEUP, (m_currentUser + 1) % _maxPlayerNum);
}
