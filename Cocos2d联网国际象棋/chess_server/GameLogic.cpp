#include "stdafx.h"
#include "GameLogic.h"


GameLogic::GameLogic()
{
	//棋盘变量
	//辅助变量
	memset(m_itemLately, 0, sizeof(m_itemLately));

	//棋盘变量
	memset(m_chessIten, 0, sizeof(m_chessIten));
	memset(m_wStepCount, 0, sizeof(m_wStepCount));
	memset(m_chessBord, 0, sizeof(m_chessBord));
}

GameLogic::~GameLogic()
{
}

void GameLogic::ResetChessBorad()
{
	//变量定义
	int ChessInfo[8] = { CHESS_ROOK,CHESS_HORSE,CHESS_ELEPHANT,CHESS_KING,CHESS_QUEEN,
		CHESS_ELEPHANT,CHESS_HORSE,CHESS_ROOK };
	memset(m_chessIten, 0, sizeof(m_chessIten));
	memset(m_wStepCount, 0, sizeof(m_wStepCount));
	memset(m_chessBord, 0, sizeof(m_chessBord));

	int yPos = 255;
	for (int user = 0; user<2; user++)
	{
		for (int nIndex = 0; nIndex<8; nIndex++)
		{
			//设置棋子
			yPos = (user == 0) ? 7 : 0;
			m_chessIten[user][nIndex].yPos = yPos;
			m_chessIten[user][nIndex].xPos = nIndex;
			m_chessIten[user][nIndex].chessID = nIndex;
			m_chessIten[user][nIndex].chess = ChessInfo[nIndex];
			m_chessIten[user][nIndex].color = BLACK_CHESS + user;
			m_chessBord[nIndex][yPos] = &m_chessIten[user][nIndex];

			//设置棋子
			yPos = (user == 0) ? 6 : 1;
			m_chessIten[user][nIndex + 8].yPos = yPos;
			m_chessIten[user][nIndex + 8].xPos = nIndex;
			m_chessIten[user][nIndex + 8].chessID = nIndex + 8;
			m_chessIten[user][nIndex + 8].chess = CHESS_SOLDIER;
			m_chessIten[user][nIndex + 8].color = BLACK_CHESS + user;
			m_chessBord[nIndex][yPos] = &m_chessIten[user][nIndex + 8];
		}
	}
	return;
}

void GameLogic::UpdateChessBorad()
{
	//重置棋盘
	memset(m_chessBord, 0, sizeof(m_chessBord));
	memset(m_itemLately, 0, sizeof(m_itemLately));

	//更新棋盘
	for (int i = 0; i<2; i++)
	{
		for (int j = 0; j<16; j++)
		{
			if ((m_chessIten[i][j].xPos != 255) && (m_chessIten[i][j].yPos != 255))
			{
				assert(m_chessIten[i][j].xPos<8);
				assert(m_chessIten[i][j].yPos<8);
				m_chessBord[m_chessIten[i][j].xPos][m_chessIten[i][j].yPos] = &m_chessIten[i][j];
			}
		}
	}

	chessManual * pChessManual = nullptr;
	int nManualCount = m_chessStepInfo.size(), nIndex = 0;
	if (nManualCount >= 1)
	{
		nIndex = (nManualCount - 1) % 2;
		pChessManual = &m_chessStepInfo[nManualCount - 1];
		m_itemLately[nIndex] = &m_chessIten[nIndex][pChessManual->sourceID];
	}
	if (nManualCount >= 2)
	{
		nIndex = nManualCount % 2;
		pChessManual = &m_chessStepInfo[nManualCount - 2];
		m_itemLately[nIndex] = &m_chessIten[nIndex][pChessManual->sourceID];
	}
	
}

int GameLogic::GetStepCount(int color)
{
	assert((color == BLACK_CHESS) || (color == WHITE_CHESS));
	return m_wStepCount[GetColorIndex(color)];
}

chessManual* GameLogic::GetLastChessManual()
{
	int nManualCount = m_chessStepInfo.size();
	return (nManualCount>0) ? &m_chessStepInfo[nManualCount - 1] : nullptr;
}

chessIten* GameLogic::GetChessItem(int xPos, int yPos)
{
	//效验参数
	assert((xPos<8) && (yPos<8));
	return m_chessBord[xPos][yPos];
}

inline int GameLogic::GetColorIndex(int chessColor)
{
	return (chessColor == BLACK_CHESS) ? 0 : 1;
}

inline int GameLogic::GetExcursion(int sourcePos, int targetPos)
{
	return (sourcePos>targetPos) ? (sourcePos - targetPos) : (targetPos - sourcePos);
}

inline bool GameLogic::ObstructAnalyse(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos)
{
	//效验坐标
	assert((xSourcePos != xTargetPos) || (ySourcePos != yTargetPos));

	//变量定义
	int nXStep = 0, nYStep = 0;
	if (xTargetPos != xSourcePos) {
		nXStep = (xTargetPos > xSourcePos) ? 1 : -1;
	}
	if (yTargetPos != ySourcePos) {
		nYStep = (yTargetPos > ySourcePos) ? 1 : -1;
	}

	//阻塞分析
	do
	{
		xSourcePos += nXStep;
		ySourcePos += nYStep;
		if ((xSourcePos == xTargetPos) && (ySourcePos == yTargetPos))	return false;
	} while (m_chessBord[xSourcePos][ySourcePos] == nullptr);

	return true;
}

void GameLogic::InsertChessManual(chessManual chessManual[], int manualCount)
{
	int nScrCount = m_chessStepInfo.size();
	//m_chessStepInfo.SetSize(nScrCount + wManualCount);
	for (int i = 0; i < manualCount; i++) {
		m_chessStepInfo[nScrCount + i] = chessManual[i];
	}

	return;
}

bool GameLogic::RegretChess(int wStepCount)
{
	//效验参数
	assert(wStepCount <= (int)m_chessStepInfo.size());

	//变量定义
	chessManual * pChessManual = nullptr;
	int nManualCount = m_chessStepInfo.size();

	//悔棋操作
	for (int i = 0; i<wStepCount; i++)
	{
		//变量定义
		pChessManual = &m_chessStepInfo[nManualCount - i - 1];

		//获取棋子
		chessIten * pSourceChessItem = &m_chessIten[GetColorIndex(pChessManual->sourceColor)][pChessManual->sourceID];
		chessIten * pTargetChessItem = (pChessManual->uniteChess != NO_CHESS) ? pTargetChessItem = 
			&m_chessIten[GetColorIndex(pChessManual->uniteColor)][pChessManual->uniteID] : nullptr;

		//效验坐标
		assert(pSourceChessItem->xPos != 255);
		assert(pSourceChessItem->yPos != 255);

		//设置步数
		m_wStepCount[GetColorIndex(pChessManual->sourceColor)]--;

		//设置源棋
		pSourceChessItem->walkCount--;
		pSourceChessItem->chess = pChessManual->sourceChess;
		pSourceChessItem->xPos = pChessManual->xSourceChessPos;
		pSourceChessItem->yPos = pChessManual->ySourceChessPos;

		//关联棋子
		if (pTargetChessItem != nullptr)
		{
			pTargetChessItem->xPos = pChessManual->xUniteChessPos;
			pTargetChessItem->yPos = pChessManual->yUniteChessPos;
			if (pSourceChessItem->color == pTargetChessItem->color) pTargetChessItem->walkCount--;
		}
	}

	//删除棋谱
	//auto step = m_chessStepInfo.begin();
	//m_chessStepInfo.RemoveAt(nManualCount - wStepCount, wStepCount);
	m_chessStepInfo.pop_back();

	//更新棋盘
	UpdateChessBorad();

	return true;
}

enMoveType GameLogic::MoveChess(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess)
{
	//效验坐标
	assert((xSourcePos<8) && (ySourcePos<8));
	assert((xTargetPos<8) && (yTargetPos<8));

	//获取棋子
	chessIten * pSourceChessItem = m_chessBord[xSourcePos][ySourcePos];
	chessIten * pTargetChessItem = m_chessBord[xTargetPos][yTargetPos];

	//棋谱变量
	chessManual ChessManual;
	memset(&ChessManual, 0, sizeof(ChessManual));
	ChessManual.xSourceChessPos = xSourcePos;
	ChessManual.ySourceChessPos = ySourcePos;
	ChessManual.xTargetChessPos = xTargetPos;
	ChessManual.yTargetChessPos = yTargetPos;
	ChessManual.sourceID = pSourceChessItem->chessID;
	ChessManual.sourceColor = pSourceChessItem->color;
	ChessManual.sourceChess = pSourceChessItem->chess;

	//兵的升变
	enMoveType MoveChessType = enMoveType_Normal;
	if ((pSourceChessItem->chess == CHESS_SOLDIER) && ((yTargetPos == 0) || (yTargetPos == 7)))
	{
		if ((switchChess<CHESS_QUEEN) || (switchChess>CHESS_HORSE)) return enMoveType_Error;
		pSourceChessItem->chess = switchChess;
		ChessManual.switchChess = switchChess;
		MoveChessType = enMoveType_SoldierUp;
	}
	else
	{
		switchChess = NO_CHESS;
		ChessManual.switchChess = switchChess;
	}

	//王车易位
	if ((pTargetChessItem != nullptr) && (pTargetChessItem->color == pSourceChessItem->color))
	{
		//获取棋子
		chessIten * pKingChessItem = (pTargetChessItem->chess == CHESS_KING) ? pTargetChessItem : pSourceChessItem;
		chessIten * pRookChessItem = (pTargetChessItem->chess == CHESS_ROOK) ? pTargetChessItem : pSourceChessItem;

		//位置定义
		int xKingPos = pKingChessItem->xPos;
		int xRookPos = pRookChessItem->xPos;
		int xKingExcursion = (xKingPos>xRookPos) ? -2 : 2;
		int xRookExcursion = (xKingPos>xRookPos) ? 1 : -1;

		//记录棋谱
		ChessManual.uniteID = pTargetChessItem->chessID;
		ChessManual.uniteColor = pTargetChessItem->color;
		ChessManual.uniteChess = pTargetChessItem->chess;
		ChessManual.xUniteChessPos = pTargetChessItem->yPos;
		ChessManual.yUniteChessPos = pTargetChessItem->yPos;
		m_chessStepInfo.push_back(ChessManual);

		//设置棋子
		pKingChessItem->walkCount++;
		pRookChessItem->walkCount++;
		pKingChessItem->xPos += xKingExcursion;
		pRookChessItem->xPos = pKingChessItem->xPos + xRookExcursion;

		//设置棋盘
		m_chessBord[xKingPos][pKingChessItem->yPos] = nullptr;
		m_chessBord[xRookPos][pRookChessItem->yPos] = nullptr;
		m_chessBord[pKingChessItem->yPos][pKingChessItem->yPos] = pKingChessItem;
		m_chessBord[pRookChessItem->xPos][pRookChessItem->yPos] = pRookChessItem;

		return enMoveType_KingRook;
	}

	//吃子判断
	if (pTargetChessItem != nullptr)
	{
		//记录棋谱
		ChessManual.uniteID = pTargetChessItem->chessID;
		ChessManual.uniteColor = pTargetChessItem->color;
		ChessManual.uniteChess = pTargetChessItem->chess;
		ChessManual.xUniteChessPos = pTargetChessItem->xPos;
		ChessManual.yUniteChessPos = pTargetChessItem->yPos;

		//设置棋子
		pTargetChessItem->xPos = 255;
		pTargetChessItem->yPos = 255;
		m_chessBord[xTargetPos][yTargetPos] = nullptr;
	}

	//过路兵判断
	if ((pTargetChessItem == nullptr) && (pSourceChessItem->chess == CHESS_SOLDIER) && (xSourcePos != xTargetPos))
	{
		//获取棋子
		chessIten * pEnemyChessItem = m_chessBord[xTargetPos][ySourcePos];
		assert(pEnemyChessItem != nullptr);

		//记录棋谱
		ChessManual.uniteID = pEnemyChessItem->chessID;
		ChessManual.uniteColor = pEnemyChessItem->color;
		ChessManual.uniteChess = pEnemyChessItem->chess;
		ChessManual.xUniteChessPos = pEnemyChessItem->xPos;
		ChessManual.yUniteChessPos = pEnemyChessItem->yPos;

		//设置棋子
		pEnemyChessItem->yPos = 255;
		pEnemyChessItem->yPos = 255;
		m_chessBord[xTargetPos][ySourcePos] = nullptr;
	}

	//记录棋谱
	m_chessStepInfo.push_back(ChessManual);
	m_wStepCount[pSourceChessItem->color - 1]++;
	m_itemLately[pSourceChessItem->color - 1] = pSourceChessItem;

	//移动棋子
	pSourceChessItem->walkCount++;
	pSourceChessItem->xPos = xTargetPos;
	pSourceChessItem->yPos = yTargetPos;
	m_chessBord[xSourcePos][ySourcePos] = nullptr;
	m_chessBord[xTargetPos][yTargetPos] = pSourceChessItem;

	return MoveChessType;
}

bool GameLogic::IsGameFinish(int color)
{
	//获取王棋
	chessIten * pKindChessItem = &m_chessIten[(color == BLACK_CHESS) ? 0 : 1][3];

	//死亡判断
	return ((pKindChessItem->xPos == 255) && (pKindChessItem->yPos == 255));
}

bool GameLogic::IsKingDanger(int color)
{
	//获取王棋
	chessIten * pKindChessItem = &m_chessIten[(color == BLACK_CHESS) ? 0 : 1][3];

	//获取坐标
	assert(pKindChessItem->xPos != 255);
	assert(pKindChessItem->yPos != 255);

	//危险判断
	return IsChessDanger(pKindChessItem->xPos, pKindChessItem->yPos);
}

bool GameLogic::IsChessDanger(int xTargetPos, int yTargetPos)
{
	//效验变量
	assert((xTargetPos < 8) && (yTargetPos < 8));

	//获取棋子
	chessIten * pTargetChessItem = m_chessBord[xTargetPos][yTargetPos];
	if (pTargetChessItem == nullptr) return false;

	//索引定义
	int EnemyIndex = (pTargetChessItem->color == BLACK_CHESS) ? 1 : 0;

	//棋子搜索
	int xSourcePos = 255, ySourcePos = 255;
	//for (int i = 0; i<CountArray(m_chessIten[EnemyIndex]); i++)
	for (int i = 0; i < 16; i++)
	{
		//获取棋子
		chessIten * pSourceChessItem = &m_chessIten[EnemyIndex][i];
		if ((pSourceChessItem->xPos == 255) || (pSourceChessItem->yPos == 255)) continue;

		//获取坐标
		xSourcePos = pSourceChessItem->xPos;
		ySourcePos = pSourceChessItem->xPos;

		//路径搜索
		switch (pSourceChessItem->chess)
		{
		case CHESS_KING:	//王
		{
			//偏移变量
			int xExcursion = GetExcursion(xSourcePos, xTargetPos);
			int yExcursion = GetExcursion(ySourcePos, yTargetPos);

			//步法判断
			if ((xExcursion <= 1) && (yExcursion <= 1)) return true;

			break;
		}
		case CHESS_QUEEN:	//后
		{
			//偏移变量
			int xExcursion = GetExcursion(xSourcePos, xTargetPos);
			int yExcursion = GetExcursion(ySourcePos, yTargetPos);

			//步法判断
			if ((xSourcePos != xTargetPos) && (ySourcePos != yTargetPos) && (xExcursion != yExcursion)) break;

			//阻塞分析
			if (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false) return true;

			break;
		}
		case CHESS_ROOK:	//车
		{
			//步法判断
			if ((xSourcePos != xTargetPos) && (ySourcePos != yTargetPos)) break;

			//阻塞分析
			if (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false) return true;

			break;
		}
		case CHESS_ELEPHANT://象
		{
			//偏移变量
			int xExcursion = GetExcursion(xSourcePos, xTargetPos);
			int yExcursion = GetExcursion(ySourcePos, yTargetPos);

			//步法判断
			if ((xExcursion == 0) || (yExcursion == 0) || (xExcursion != yExcursion)) break;

			//阻塞分析
			if (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false) return true;

			break;
		}
		case CHESS_HORSE:	//马
		{
			//偏移变量
			int xExcursion = GetExcursion(xSourcePos, xTargetPos);
			int yExcursion = GetExcursion(ySourcePos, yTargetPos);

			//步法判断
			if ((xExcursion == 1) && (yExcursion == 2)) return true;
			if ((xExcursion == 2) && (yExcursion == 1)) return true;

			break;
		}
		case CHESS_SOLDIER:	//兵
		{
			//变量定义
			int nYStep = (pSourceChessItem->color == BLACK_CHESS) ? -1 : 1;
			int xExcursion = GetExcursion(xSourcePos, xTargetPos);

			//步法判断
			if (xExcursion == 1)
			{
				if ((ySourcePos + nYStep) == yTargetPos) return true;
				if ((ySourcePos == yTargetPos) && (pTargetChessItem->chess == CHESS_SOLDIER) && (pTargetChessItem->walkCount == 1)) return true;
			}

			break;
		}
		}
	}
	return false;
}

bool GameLogic::IsWalkLegality(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos)
{
	//效验坐标
	assert((xSourcePos < 8) && (ySourcePos < 8));
	assert((xTargetPos < 8) && (yTargetPos < 8));

	//获取棋子
	chessIten * pSourceChessItem = m_chessBord[xSourcePos][ySourcePos];
	chessIten * pTargetChessItem = m_chessBord[xTargetPos][yTargetPos];

	//效验起点
	assert(pSourceChessItem != nullptr);
	if (pSourceChessItem == nullptr) return false;

	//王车易位
	if ((pTargetChessItem != nullptr) && (pTargetChessItem->color == pSourceChessItem->color))
	{
		//步数判断
		if ((pTargetChessItem->walkCount != 0) && (pSourceChessItem->walkCount != 0)) return false;

		//王棋判断
		chessIten * pKingChessItem = nullptr;
		if (pTargetChessItem->chess == CHESS_KING) pKingChessItem = pTargetChessItem;
		else if (pSourceChessItem->chess == CHESS_KING) pKingChessItem = pTargetChessItem;
		else return false;

		//车棋判断
		chessIten * pRookChessItem = nullptr;
		if (pTargetChessItem->chess == CHESS_ROOK) pKingChessItem = pTargetChessItem;
		else if (pSourceChessItem->chess == CHESS_ROOK) pKingChessItem = pTargetChessItem;
		else return false;

		//阻挡判断
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
	}

	//走棋判断
	switch (pSourceChessItem->chess)
	{
	case CHESS_KING:	//王
	{
		//偏移变量
		int xExcursion = GetExcursion(xSourcePos, xTargetPos);
		int yExcursion = GetExcursion(ySourcePos, yTargetPos);

		//步法判断
		if ((xExcursion>1) || (yExcursion>1)) return false;

		return true;
	}
	case CHESS_QUEEN:	//后
	{
		//偏移变量
		int xExcursion = GetExcursion(xSourcePos, xTargetPos);
		int yExcursion = GetExcursion(ySourcePos, yTargetPos);

		//步法判断
		if ((xSourcePos != xTargetPos) && (ySourcePos != yTargetPos) && (xExcursion != yExcursion)) return false;

		//阻塞分析
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
	}
	case CHESS_ROOK:	//车
	{
		//步法判断
		if ((xSourcePos != xTargetPos) && (ySourcePos != yTargetPos)) return false;

		//阻塞分析
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
	}
	case CHESS_ELEPHANT://象
	{
		//偏移变量
		int xExcursion = GetExcursion(xSourcePos, xTargetPos);
		int yExcursion = GetExcursion(ySourcePos, yTargetPos);

		//步法判断
		if ((xExcursion == 0) || (yExcursion == 0) || (xExcursion != yExcursion)) return false;

		//阻塞分析
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
	}
	case CHESS_HORSE:	//马
	{
		//偏移变量
		int xExcursion = GetExcursion(xSourcePos, xTargetPos);
		int yExcursion = GetExcursion(ySourcePos, yTargetPos);

		//步法判断
		if ((xExcursion == 1) && (yExcursion == 2)) return true;
		if ((xExcursion == 2) && (yExcursion == 1)) return true;

		return false;
	}
	case CHESS_SOLDIER:	//兵
	{
		//变量定义
		int nYStep = (pSourceChessItem->color == BLACK_CHESS) ? -1 : 1;

		//直行判断
		if (xSourcePos == xTargetPos)
		{
			if (pTargetChessItem != nullptr) return false;
			if ((ySourcePos + nYStep) == yTargetPos) return true;
			if ((pSourceChessItem->walkCount == 0) && ((ySourcePos + nYStep * 2) == yTargetPos)
				&& (m_chessBord[xSourcePos][ySourcePos + nYStep] == nullptr)) {
				return true;
			} 
			else return false;	
		}

		//斜行判断
		if ((ySourcePos + nYStep) != yTargetPos) return false;
		if ((GetExcursion(xSourcePos, xTargetPos)) != 1) return false;

		//吃子判断
		if ((pTargetChessItem != nullptr) && (pTargetChessItem->color != pSourceChessItem->color)) return true;

		//过路兵判断
		int YTempPos = (pSourceChessItem->color == BLACK_CHESS) ? 3 : 4;
		if ((pTargetChessItem == nullptr) && (ySourcePos == YTempPos))
		{
			chessIten * pTempChessItem = m_itemLately[(pSourceChessItem->color == BLACK_CHESS) ? 1 : 0];
			if (pTempChessItem == nullptr) return false;
			if (pTempChessItem->walkCount != 1) return false;
			if (pTempChessItem->xPos != xTargetPos) return false;
			if (pTempChessItem->chess != CHESS_SOLDIER) return false;
			return true;
		}

		return false;
	}
	}

	assert(false);

	return false;
}

void GameLogic::printChessBord()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++) {
			auto chess = m_chessBord[i][j];
			if (chess) {
				std::cout << chess->chess << " ";
			}else{
				std::cout << 0 << " ";
			}
			
			//printf("%d ", &m_chessBord[i][j]->chess);
		}
		std::cout << endl;
	}
	std::cout << endl;
}

void GameLogic::printChessID()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++) {
			auto chess = m_chessBord[i][j];
			if (chess) {
				std::cout << chess->chessID << " ";
			}
			else {
				std::cout << 0 << " ";
			}
		}
		std::cout << endl;
	}
	std::cout << endl;
}

void GameLogic::printChessPos()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++) {
			auto chess = m_chessBord[i][j];
			if (chess) {
				std::cout << "(" << chess->xPos << "," << chess->yPos << ")" <<" ";
			}
			else {
				std::cout << "(0,0)" << " ";
			}
		}
		std::cout << endl;
	}
	std::cout << endl;
}

void GameLogic::printChessColor()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++) {
			auto chess = m_chessBord[i][j];
			if (chess) {
				std::cout << chess->color << " ";
			}
			else {
				std::cout << 0 << " ";
			}
		}
		std::cout << endl;
	}
	std::cout << endl;
}
