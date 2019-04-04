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
	Button* m_pBtnQiuHe;												//求和
	Button* m_pBtnCapitulate;
	Button* m_pBtnQuit;
	Button* m_pBtnChongLia;												//悔棋

	///user1
	Text* m_pTxtUserName1;
	TextBMFont* m_pTxtWinNum1;												//赢场
	TextBMFont* m_pTxtLoseNum1;												//输场
																			///user2
	Text* m_pTxtUserName2;
	TextBMFont* m_pTxtWinNum2;
	TextBMFont* m_pTxtLoseNum2;

	//spr
	Sprite* m_pIconPlayer1;												//玩家1头像
	Sprite* m_pIconPlayer2;												//玩家1头像
	Text* m_pTxtRoomID;													//房间号

																		///杂项
	Sprite* m_pSpr_ChessBord;											//棋盘
	Sprite* m_pSpr_SelectFram;											//棋子选择框
	Node* m_pN_Hellp;													//帮助节点
	Button* m_pBtn_BackToGame;											//返回游戏场景
	Button* m_pBtn_Ready;
	Node* m_pN_Chess;
private:
	Chess * _chess[32];													//棋子容器
	vector<Step*> _step;												//记录当前走棋
	//Step * _step;														//记录当前走棋
	int m_nUser;														//标记玩家
	int m_nCurrUser;													//标记当前下棋玩家
	int m_nChessColor;													//棋子颜色
	int m_nCurrChessID = -1;											//标记当前点击棋子id
	int m_nSwitchChess = 0;												//升变棋子
	CClient* client;
	EventListenerTouchOneByOne* listener;

	bool gameOver = true;														//游戏结束
	bool gameStart = false;														//游戏开始
	enMoveType m_mMoveType;														//走棋类型
	vector<Button*> btn_vec;													//升变棋子容器
	//GameLogic* gameLogic;

public:
	void QuitRoom(Ref* ref);
	void ReadRivalInfo(Ref* ref);
	void onExit();
	void PrintMsg(int ret, int inxex);
public:
	//初始化视图
	void initView();
	//点击响应
	void onClick(Ref* ref);
	//改变棋盘
	void ChangeChessBord(Ref* ref);
	//发送已准备消息
	void SendReadyMsg();
	//发送移动棋子消息
	void SendMoveChessMsg(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess);
	//发送认输事件
	void SendGiveUpMsg();
	//发送求和事件
	void SendPeaceMsg();
	//准备棋盘棋子
	void ReadyBoard(int color);
	//开始游戏
	void StartGame(Ref* ref);
	//重新开始游戏
	void RestartGame();
	//void StartGame();
	//移动棋子
	void MoveChessRival(Ref* ref);
	void MoveChessSelf(Ref* ref);
	//游戏结束
	void GameOver(Ref* ref);
	//和棋请求
	void UserPeaceReq(Ref* ref);
	//he棋请求答应
	void UserPeaceAnswer(Ref* ref);
	//
	void ChangeChess(Step* step);

private:
	//坐标转换
	inline Point CoordinateConversion(int _x, int _y);
	//回调
	void onActionComplete(Node * node, int moveID, int killID);
	//获取棋子在数组_chess[]的下标
	int GetChessIdByChessInChessBoradPoint(int _x, int _y);
	//获取棋子在棋盘上的坐标，将棋盘坐标转换为屏幕坐标
	Point GetChessPosInBoard(int _px, int _py);
	//判断点击位置是否在棋盘上
	bool JudgeClickIsInChessBoard(Point clickPoint, int &_x, int &_y);
	//显示选择棋子框
	void ShowSelectChessFrme(int _id);
	//移动棋子到点击位置
	void MoveChessToClickPoint(int moveID, int killID, int _x, int _y);
	//触摸响应函数
	bool TouchBegan(Touch * touch, Event * unused_event);
	//
	void AddChessToStep(int moveid, int killid, int xFrom, int yFrom, int xTo, int yTo);
	//简单判断棋子移动
	bool IsWalkLegality(int sourceChessID, int targetChessID, int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos);
	//判断兵的升变
	enMoveType JudgeSoldierUp(int sourceChessID);
	//获取偏移量
	inline int GetExcursion(int sourcePos, int targetPos){
		return (sourcePos>targetPos) ? (sourcePos - targetPos) : (targetPos - sourcePos);
	}
	//阻塞分析
	inline bool ObstructAnalyse(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos);
};

#endif // __HELLOWORLD_SCENE_H__