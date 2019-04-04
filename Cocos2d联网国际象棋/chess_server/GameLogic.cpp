#include "stdafx.h"
#include "GameLogic.h"


GameLogic::GameLogic()
{
	//���̱���
	//��������
	memset(m_itemLately, 0, sizeof(m_itemLately));

	//���̱���
	memset(m_chessIten, 0, sizeof(m_chessIten));
	memset(m_wStepCount, 0, sizeof(m_wStepCount));
	memset(m_chessBord, 0, sizeof(m_chessBord));
}

GameLogic::~GameLogic()
{
}

void GameLogic::ResetChessBorad()
{
	//��������
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
			//��������
			yPos = (user == 0) ? 7 : 0;
			m_chessIten[user][nIndex].yPos = yPos;
			m_chessIten[user][nIndex].xPos = nIndex;
			m_chessIten[user][nIndex].chessID = nIndex;
			m_chessIten[user][nIndex].chess = ChessInfo[nIndex];
			m_chessIten[user][nIndex].color = BLACK_CHESS + user;
			m_chessBord[nIndex][yPos] = &m_chessIten[user][nIndex];

			//��������
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
	//��������
	memset(m_chessBord, 0, sizeof(m_chessBord));
	memset(m_itemLately, 0, sizeof(m_itemLately));

	//��������
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
	//Ч�����
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
	//Ч������
	assert((xSourcePos != xTargetPos) || (ySourcePos != yTargetPos));

	//��������
	int nXStep = 0, nYStep = 0;
	if (xTargetPos != xSourcePos) {
		nXStep = (xTargetPos > xSourcePos) ? 1 : -1;
	}
	if (yTargetPos != ySourcePos) {
		nYStep = (yTargetPos > ySourcePos) ? 1 : -1;
	}

	//��������
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
	//Ч�����
	assert(wStepCount <= (int)m_chessStepInfo.size());

	//��������
	chessManual * pChessManual = nullptr;
	int nManualCount = m_chessStepInfo.size();

	//�������
	for (int i = 0; i<wStepCount; i++)
	{
		//��������
		pChessManual = &m_chessStepInfo[nManualCount - i - 1];

		//��ȡ����
		chessIten * pSourceChessItem = &m_chessIten[GetColorIndex(pChessManual->sourceColor)][pChessManual->sourceID];
		chessIten * pTargetChessItem = (pChessManual->uniteChess != NO_CHESS) ? pTargetChessItem = 
			&m_chessIten[GetColorIndex(pChessManual->uniteColor)][pChessManual->uniteID] : nullptr;

		//Ч������
		assert(pSourceChessItem->xPos != 255);
		assert(pSourceChessItem->yPos != 255);

		//���ò���
		m_wStepCount[GetColorIndex(pChessManual->sourceColor)]--;

		//����Դ��
		pSourceChessItem->walkCount--;
		pSourceChessItem->chess = pChessManual->sourceChess;
		pSourceChessItem->xPos = pChessManual->xSourceChessPos;
		pSourceChessItem->yPos = pChessManual->ySourceChessPos;

		//��������
		if (pTargetChessItem != nullptr)
		{
			pTargetChessItem->xPos = pChessManual->xUniteChessPos;
			pTargetChessItem->yPos = pChessManual->yUniteChessPos;
			if (pSourceChessItem->color == pTargetChessItem->color) pTargetChessItem->walkCount--;
		}
	}

	//ɾ������
	//auto step = m_chessStepInfo.begin();
	//m_chessStepInfo.RemoveAt(nManualCount - wStepCount, wStepCount);
	m_chessStepInfo.pop_back();

	//��������
	UpdateChessBorad();

	return true;
}

enMoveType GameLogic::MoveChess(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess)
{
	//Ч������
	assert((xSourcePos<8) && (ySourcePos<8));
	assert((xTargetPos<8) && (yTargetPos<8));

	//��ȡ����
	chessIten * pSourceChessItem = m_chessBord[xSourcePos][ySourcePos];
	chessIten * pTargetChessItem = m_chessBord[xTargetPos][yTargetPos];

	//���ױ���
	chessManual ChessManual;
	memset(&ChessManual, 0, sizeof(ChessManual));
	ChessManual.xSourceChessPos = xSourcePos;
	ChessManual.ySourceChessPos = ySourcePos;
	ChessManual.xTargetChessPos = xTargetPos;
	ChessManual.yTargetChessPos = yTargetPos;
	ChessManual.sourceID = pSourceChessItem->chessID;
	ChessManual.sourceColor = pSourceChessItem->color;
	ChessManual.sourceChess = pSourceChessItem->chess;

	//��������
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

	//������λ
	if ((pTargetChessItem != nullptr) && (pTargetChessItem->color == pSourceChessItem->color))
	{
		//��ȡ����
		chessIten * pKingChessItem = (pTargetChessItem->chess == CHESS_KING) ? pTargetChessItem : pSourceChessItem;
		chessIten * pRookChessItem = (pTargetChessItem->chess == CHESS_ROOK) ? pTargetChessItem : pSourceChessItem;

		//λ�ö���
		int xKingPos = pKingChessItem->xPos;
		int xRookPos = pRookChessItem->xPos;
		int xKingExcursion = (xKingPos>xRookPos) ? -2 : 2;
		int xRookExcursion = (xKingPos>xRookPos) ? 1 : -1;

		//��¼����
		ChessManual.uniteID = pTargetChessItem->chessID;
		ChessManual.uniteColor = pTargetChessItem->color;
		ChessManual.uniteChess = pTargetChessItem->chess;
		ChessManual.xUniteChessPos = pTargetChessItem->yPos;
		ChessManual.yUniteChessPos = pTargetChessItem->yPos;
		m_chessStepInfo.push_back(ChessManual);

		//��������
		pKingChessItem->walkCount++;
		pRookChessItem->walkCount++;
		pKingChessItem->xPos += xKingExcursion;
		pRookChessItem->xPos = pKingChessItem->xPos + xRookExcursion;

		//��������
		m_chessBord[xKingPos][pKingChessItem->yPos] = nullptr;
		m_chessBord[xRookPos][pRookChessItem->yPos] = nullptr;
		m_chessBord[pKingChessItem->yPos][pKingChessItem->yPos] = pKingChessItem;
		m_chessBord[pRookChessItem->xPos][pRookChessItem->yPos] = pRookChessItem;

		return enMoveType_KingRook;
	}

	//�����ж�
	if (pTargetChessItem != nullptr)
	{
		//��¼����
		ChessManual.uniteID = pTargetChessItem->chessID;
		ChessManual.uniteColor = pTargetChessItem->color;
		ChessManual.uniteChess = pTargetChessItem->chess;
		ChessManual.xUniteChessPos = pTargetChessItem->xPos;
		ChessManual.yUniteChessPos = pTargetChessItem->yPos;

		//��������
		pTargetChessItem->xPos = 255;
		pTargetChessItem->yPos = 255;
		m_chessBord[xTargetPos][yTargetPos] = nullptr;
	}

	//��·���ж�
	if ((pTargetChessItem == nullptr) && (pSourceChessItem->chess == CHESS_SOLDIER) && (xSourcePos != xTargetPos))
	{
		//��ȡ����
		chessIten * pEnemyChessItem = m_chessBord[xTargetPos][ySourcePos];
		assert(pEnemyChessItem != nullptr);

		//��¼����
		ChessManual.uniteID = pEnemyChessItem->chessID;
		ChessManual.uniteColor = pEnemyChessItem->color;
		ChessManual.uniteChess = pEnemyChessItem->chess;
		ChessManual.xUniteChessPos = pEnemyChessItem->xPos;
		ChessManual.yUniteChessPos = pEnemyChessItem->yPos;

		//��������
		pEnemyChessItem->yPos = 255;
		pEnemyChessItem->yPos = 255;
		m_chessBord[xTargetPos][ySourcePos] = nullptr;
	}

	//��¼����
	m_chessStepInfo.push_back(ChessManual);
	m_wStepCount[pSourceChessItem->color - 1]++;
	m_itemLately[pSourceChessItem->color - 1] = pSourceChessItem;

	//�ƶ�����
	pSourceChessItem->walkCount++;
	pSourceChessItem->xPos = xTargetPos;
	pSourceChessItem->yPos = yTargetPos;
	m_chessBord[xSourcePos][ySourcePos] = nullptr;
	m_chessBord[xTargetPos][yTargetPos] = pSourceChessItem;

	return MoveChessType;
}

bool GameLogic::IsGameFinish(int color)
{
	//��ȡ����
	chessIten * pKindChessItem = &m_chessIten[(color == BLACK_CHESS) ? 0 : 1][3];

	//�����ж�
	return ((pKindChessItem->xPos == 255) && (pKindChessItem->yPos == 255));
}

bool GameLogic::IsKingDanger(int color)
{
	//��ȡ����
	chessIten * pKindChessItem = &m_chessIten[(color == BLACK_CHESS) ? 0 : 1][3];

	//��ȡ����
	assert(pKindChessItem->xPos != 255);
	assert(pKindChessItem->yPos != 255);

	//Σ���ж�
	return IsChessDanger(pKindChessItem->xPos, pKindChessItem->yPos);
}

bool GameLogic::IsChessDanger(int xTargetPos, int yTargetPos)
{
	//Ч�����
	assert((xTargetPos < 8) && (yTargetPos < 8));

	//��ȡ����
	chessIten * pTargetChessItem = m_chessBord[xTargetPos][yTargetPos];
	if (pTargetChessItem == nullptr) return false;

	//��������
	int EnemyIndex = (pTargetChessItem->color == BLACK_CHESS) ? 1 : 0;

	//��������
	int xSourcePos = 255, ySourcePos = 255;
	//for (int i = 0; i<CountArray(m_chessIten[EnemyIndex]); i++)
	for (int i = 0; i < 16; i++)
	{
		//��ȡ����
		chessIten * pSourceChessItem = &m_chessIten[EnemyIndex][i];
		if ((pSourceChessItem->xPos == 255) || (pSourceChessItem->yPos == 255)) continue;

		//��ȡ����
		xSourcePos = pSourceChessItem->xPos;
		ySourcePos = pSourceChessItem->xPos;

		//·������
		switch (pSourceChessItem->chess)
		{
		case CHESS_KING:	//��
		{
			//ƫ�Ʊ���
			int xExcursion = GetExcursion(xSourcePos, xTargetPos);
			int yExcursion = GetExcursion(ySourcePos, yTargetPos);

			//�����ж�
			if ((xExcursion <= 1) && (yExcursion <= 1)) return true;

			break;
		}
		case CHESS_QUEEN:	//��
		{
			//ƫ�Ʊ���
			int xExcursion = GetExcursion(xSourcePos, xTargetPos);
			int yExcursion = GetExcursion(ySourcePos, yTargetPos);

			//�����ж�
			if ((xSourcePos != xTargetPos) && (ySourcePos != yTargetPos) && (xExcursion != yExcursion)) break;

			//��������
			if (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false) return true;

			break;
		}
		case CHESS_ROOK:	//��
		{
			//�����ж�
			if ((xSourcePos != xTargetPos) && (ySourcePos != yTargetPos)) break;

			//��������
			if (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false) return true;

			break;
		}
		case CHESS_ELEPHANT://��
		{
			//ƫ�Ʊ���
			int xExcursion = GetExcursion(xSourcePos, xTargetPos);
			int yExcursion = GetExcursion(ySourcePos, yTargetPos);

			//�����ж�
			if ((xExcursion == 0) || (yExcursion == 0) || (xExcursion != yExcursion)) break;

			//��������
			if (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false) return true;

			break;
		}
		case CHESS_HORSE:	//��
		{
			//ƫ�Ʊ���
			int xExcursion = GetExcursion(xSourcePos, xTargetPos);
			int yExcursion = GetExcursion(ySourcePos, yTargetPos);

			//�����ж�
			if ((xExcursion == 1) && (yExcursion == 2)) return true;
			if ((xExcursion == 2) && (yExcursion == 1)) return true;

			break;
		}
		case CHESS_SOLDIER:	//��
		{
			//��������
			int nYStep = (pSourceChessItem->color == BLACK_CHESS) ? -1 : 1;
			int xExcursion = GetExcursion(xSourcePos, xTargetPos);

			//�����ж�
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
	//Ч������
	assert((xSourcePos < 8) && (ySourcePos < 8));
	assert((xTargetPos < 8) && (yTargetPos < 8));

	//��ȡ����
	chessIten * pSourceChessItem = m_chessBord[xSourcePos][ySourcePos];
	chessIten * pTargetChessItem = m_chessBord[xTargetPos][yTargetPos];

	//Ч�����
	assert(pSourceChessItem != nullptr);
	if (pSourceChessItem == nullptr) return false;

	//������λ
	if ((pTargetChessItem != nullptr) && (pTargetChessItem->color == pSourceChessItem->color))
	{
		//�����ж�
		if ((pTargetChessItem->walkCount != 0) && (pSourceChessItem->walkCount != 0)) return false;

		//�����ж�
		chessIten * pKingChessItem = nullptr;
		if (pTargetChessItem->chess == CHESS_KING) pKingChessItem = pTargetChessItem;
		else if (pSourceChessItem->chess == CHESS_KING) pKingChessItem = pTargetChessItem;
		else return false;

		//�����ж�
		chessIten * pRookChessItem = nullptr;
		if (pTargetChessItem->chess == CHESS_ROOK) pKingChessItem = pTargetChessItem;
		else if (pSourceChessItem->chess == CHESS_ROOK) pKingChessItem = pTargetChessItem;
		else return false;

		//�赲�ж�
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
	}

	//�����ж�
	switch (pSourceChessItem->chess)
	{
	case CHESS_KING:	//��
	{
		//ƫ�Ʊ���
		int xExcursion = GetExcursion(xSourcePos, xTargetPos);
		int yExcursion = GetExcursion(ySourcePos, yTargetPos);

		//�����ж�
		if ((xExcursion>1) || (yExcursion>1)) return false;

		return true;
	}
	case CHESS_QUEEN:	//��
	{
		//ƫ�Ʊ���
		int xExcursion = GetExcursion(xSourcePos, xTargetPos);
		int yExcursion = GetExcursion(ySourcePos, yTargetPos);

		//�����ж�
		if ((xSourcePos != xTargetPos) && (ySourcePos != yTargetPos) && (xExcursion != yExcursion)) return false;

		//��������
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
	}
	case CHESS_ROOK:	//��
	{
		//�����ж�
		if ((xSourcePos != xTargetPos) && (ySourcePos != yTargetPos)) return false;

		//��������
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
	}
	case CHESS_ELEPHANT://��
	{
		//ƫ�Ʊ���
		int xExcursion = GetExcursion(xSourcePos, xTargetPos);
		int yExcursion = GetExcursion(ySourcePos, yTargetPos);

		//�����ж�
		if ((xExcursion == 0) || (yExcursion == 0) || (xExcursion != yExcursion)) return false;

		//��������
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
	}
	case CHESS_HORSE:	//��
	{
		//ƫ�Ʊ���
		int xExcursion = GetExcursion(xSourcePos, xTargetPos);
		int yExcursion = GetExcursion(ySourcePos, yTargetPos);

		//�����ж�
		if ((xExcursion == 1) && (yExcursion == 2)) return true;
		if ((xExcursion == 2) && (yExcursion == 1)) return true;

		return false;
	}
	case CHESS_SOLDIER:	//��
	{
		//��������
		int nYStep = (pSourceChessItem->color == BLACK_CHESS) ? -1 : 1;

		//ֱ���ж�
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

		//б���ж�
		if ((ySourcePos + nYStep) != yTargetPos) return false;
		if ((GetExcursion(xSourcePos, xTargetPos)) != 1) return false;

		//�����ж�
		if ((pTargetChessItem != nullptr) && (pTargetChessItem->color != pSourceChessItem->color)) return true;

		//��·���ж�
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
