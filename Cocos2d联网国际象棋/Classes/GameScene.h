#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "CClient.h"
#include "GameLogic.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace cocostudio::timeline;
using namespace cocos2d::ui;
using namespace std;
#pragma execution_character_set("utf-8")

class GameScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

public:
	Node* rootNode;
	Node* m_pBtnList;
	Node* m_pN_UserInfo;
	Sprite* m_pN_User1_Info;
	Sprite* m_pN_User2_Info;
	Node* m_pN_Spr;

	///btn
	Button* m_pBtnChangeChessBord;
	Button* m_pBtnRule;
	Button* m_pBtnAudio;
	Button* m_pBtnQiuHe;												//���
	Button* m_pBtnCapitulate;
	Button* m_pBtnQuit;
	Button* m_pBtnChongLia;												//����

	///user1
	Text* m_pTxtUserName1;
	TextBMFont* m_pTxtWinNum1;												//Ӯ��
	TextBMFont* m_pTxtLoseNum1;												//�䳡
																			///user2
	Text* m_pTxtUserName2;
	TextBMFont* m_pTxtWinNum2;
	TextBMFont* m_pTxtLoseNum2;

	//spr
	Sprite* m_pIconPlayer1;												//���1ͷ��
	Sprite* m_pIconPlayer2;												//���1ͷ��
	Text* m_pTxtRoomID;													//�����

																		///����
	Sprite* m_pSpr_ChessBord;											//����
	Sprite* m_pSpr_SelectFram;											//����ѡ���
	Node* m_pN_Hellp;													//�����ڵ�
	Button* m_pBtn_BackToGame;											//������Ϸ����
	Button* m_pBtn_Ready;
	Node* m_pN_Chess;
private:
	Chess * _chess[32];													//��������
	vector<Step*> _step;												//��¼��ǰ����
	//Step * _step;														//��¼��ǰ����
	int m_nUser;														//������
	int m_nCurrUser;													//��ǵ�ǰ�������
	int m_nChessColor;													//������ɫ
	int m_nCurrChessID = -1;											//��ǵ�ǰ�������id
	int m_nSwitchChess = 0;												//��������
	CClient* client;
	EventListenerTouchOneByOne* listener;

	bool gameOver = true;														//��Ϸ����
	bool gameStart = false;														//��Ϸ��ʼ
	enMoveType m_mMoveType;														//��������
	vector<Button*> btn_vec;													//������������
	//GameLogic* gameLogic;

public:
	void QuitRoom(Ref* ref);
	void ReadRivalInfo(Ref* ref);
	void onExit();
	void PrintMsg(int ret, int inxex);
public:
	//��ʼ����ͼ
	void initView();
	//�����Ӧ
	void onClick(Ref* ref);
	//�ı�����
	void ChangeChessBord(Ref* ref);
	//������׼����Ϣ
	void SendReadyMsg();
	//�����ƶ�������Ϣ
	void SendMoveChessMsg(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess);
	//���������¼�
	void SendGiveUpMsg();
	//��������¼�
	void SendPeaceMsg();
	//׼����������
	void ReadyBoard(int color);
	//��ʼ��Ϸ
	void StartGame(Ref* ref);
	//���¿�ʼ��Ϸ
	void RestartGame();
	//void StartGame();
	//�ƶ�����
	void MoveChessRival(Ref* ref);
	void MoveChessSelf(Ref* ref);
	//��Ϸ����
	void GameOver(Ref* ref);
	//��������
	void UserPeaceReq(Ref* ref);
	//he�������Ӧ
	void UserPeaceAnswer(Ref* ref);
	//
	void ChangeChess(Step* step);

private:
	//����ת��
	inline Point CoordinateConversion(int _x, int _y);
	//�ص�
	void onActionComplete(Node * node, int moveID, int killID);
	//��ȡ����������_chess[]���±�
	int GetChessIdByChessInChessBoradPoint(int _x, int _y);
	//��ȡ�����������ϵ����꣬����������ת��Ϊ��Ļ����
	Point GetChessPosInBoard(int _px, int _py);
	//�жϵ��λ���Ƿ���������
	bool JudgeClickIsInChessBoard(Point clickPoint, int &_x, int &_y);
	//��ʾѡ�����ӿ�
	void ShowSelectChessFrme(int _id);
	//�ƶ����ӵ����λ��
	void MoveChessToClickPoint(int moveID, int killID, int _x, int _y);
	//������Ӧ����
	bool TouchBegan(Touch * touch, Event * unused_event);
	//
	void AddChessToStep(int moveid, int killid, int xFrom, int yFrom, int xTo, int yTo);
	//���ж������ƶ�
	bool IsWalkLegality(int sourceChessID, int targetChessID, int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos);
	//�жϱ�������
	enMoveType JudgeSoldierUp(int sourceChessID);
	//��ȡƫ����
	inline int GetExcursion(int sourcePos, int targetPos){
		return (sourcePos>targetPos) ? (sourcePos - targetPos) : (targetPos - sourcePos);
	}
	//��������
	inline bool ObstructAnalyse(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos);
};

#endif // __HELLOWORLD_SCENE_H__