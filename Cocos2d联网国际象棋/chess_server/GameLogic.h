#pragma once
#include "def.h"
#include <assert.h>
#include <vector>

//��Ϸ�߼���
class GameLogic
{
public:
	GameLogic();
	~GameLogic();
public:
	//���Ӳ���
	int	m_wStepCount[2];					
	//����
	chessIten m_chessIten[2][16];
	//����
	chessIten* m_chessBord[8][8];
	//�������
	chessIten* m_itemLately[2];
	//������Ϣ
	vector<chessManual> m_chessStepInfo;
public:
	//��������
	void ResetChessBorad();
	//��������
	void UpdateChessBorad();
	//��ȡ����
	int GetStepCount(int color);
	//��ȡ����
	chessManual* GetLastChessManual();
	//��ȡ����
	chessIten* GetChessItem(int xPos, int yPos);
	//��ȡ����
	inline int GetColorIndex(int chessColor);
	//��ȡƫ��
	inline int GetExcursion(int sourcePos, int targetPos);
	//��������
	inline bool ObstructAnalyse(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos);
	//��������
	void InsertChessManual(chessManual chessManual[], int manualCount);
	//�������
	bool RegretChess(int stepCount);
	//�ƶ�����
	enMoveType MoveChess(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess);
	//�����ж�
	bool IsGameFinish(int color);
	//ɱ���ж�
	bool IsKingDanger(int color);
	//Σ���ж�
	bool IsChessDanger(int xTargetPos, int yTargetPos);
	//Ч�鲽��
	bool IsWalkLegality(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos);

	void printChessBord();
	void printChessID();
	void printChessPos();
	void printChessColor();
};

