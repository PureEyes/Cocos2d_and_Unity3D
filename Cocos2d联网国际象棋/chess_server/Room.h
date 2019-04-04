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
	int room_id;													//房间号														
	int _maxPlayerNum;												//最大玩家数				
	bool isFall;													//房间是否满了									
	int _currPlayerNum;												//当前玩家数						
public:
	//房间玩家列表
	std::vector<CClient*> _player_List;		
	//获取当前房间玩家数量
	inline int getCurrPlayerNum() { return _currPlayerNum; };
	//获取房间id
	inline int getRoomId() { return room_id; };		
	//设置房间id
	inline void setRoomId(int id) { room_id = id; };	
	//返回房间是否已满
	inline bool getRoomFall() { return isFall; };					
	//玩家加入房间
	void addPlayer(CClient* player);	
	//玩家退出房间
	void subPlayer(CClient* player);	
	//获取玩家在房间列表里的下标
	int getPlayerInRoomIndex(CClient* player);
	//转发玩家的移动棋子信息
	void TranspondMoveChessMsg(CMD_S_MoveChess data, CClient* player, int msgType);
	//转发求和信息
	void TranspondPeaceMsg(CClient* player, int msgType, int ret);

	string getTime();

public:
	//游戏开始
	bool OnEventGameStart();
	//游戏结束
	bool OnEventGameEnd(int cbReason, int userId);
	//移动棋子
	int OnUserMoveChess(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess, CClient* player);
	//悔棋请求
	bool OnUserRegretReq();
	//悔棋应答
	bool OnUserRegretAnswer();
	//求和请求
	void OnUserPeaceReq(CClient* player, int msgType, int en);
	//求和应答
	bool OnUserPeaceAnswer();
	//认输事件
	bool OnUserGiveUp();

private:


protected:
	GameLogic					*m_pGameLogic;						//游戏逻辑
	int							m_blackUser;						//黑棋玩家
	int							m_currentUser;						//当前用户
	int							m_begStatus[2];						//请求状态
	int							m_peaceCount[2];					//求和次数
	int							m_regretCount[2];					//悔棋次数
};

