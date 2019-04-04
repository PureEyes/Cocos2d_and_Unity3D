#pragma once
#include "def.h"
#include <assert.h>
#include <vector>

//游戏逻辑类
class GameLogic
{
public:
	GameLogic();
	~GameLogic();
public:
	//棋子步数
	int	m_wStepCount[2];					
	//棋子
	chessIten m_chessIten[2][16];
	//棋盘
	chessIten* m_chessBord[8][8];
	//最近走棋
	chessIten* m_itemLately[2];
	//棋谱信息
	vector<chessManual> m_chessStepInfo;
public:
	//重置棋盘
	void ResetChessBorad();
	//更新棋盘
	void UpdateChessBorad();
	//获取步数
	int GetStepCount(int color);
	//获取棋谱
	chessManual* GetLastChessManual();
	//获取棋子
	chessIten* GetChessItem(int xPos, int yPos);
	//获取索引
	inline int GetColorIndex(int chessColor);
	//获取偏移
	inline int GetExcursion(int sourcePos, int targetPos);
	//阻塞分析
	inline bool ObstructAnalyse(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos);
	//设置棋谱
	void InsertChessManual(chessManual chessManual[], int manualCount);
	//悔棋控制
	bool RegretChess(int stepCount);
	//移动棋子
	enMoveType MoveChess(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess);
	//结束判断
	bool IsGameFinish(int color);
	//杀王判断
	bool IsKingDanger(int color);
	//危险判断
	bool IsChessDanger(int xTargetPos, int yTargetPos);
	//效验步法
	bool IsWalkLegality(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos);

	void printChessBord();
	void printChessID();
	void printChessPos();
	void printChessColor();
};

