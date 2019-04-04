#include "GameScene.h"

#include <assert.h>
#include "LobbyScene.h"
#include "CMessageBox.h"
#include "GameOverScene.h"
#include"AudioEngine.h"

using namespace cocos2d::experimental;;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	rootNode = CSLoader::createNode("GameScene.csb");

	addChild(rootNode);

	client = CClient::getInstance();

	//gameLogic = new GameLogic();

	initView();

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = (CC_CALLBACK_2(GameScene::TouchBegan, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	__NotificationCenter::getInstance()->addObserver
	(this, CC_CALLFUNCO_SELECTOR(GameScene::ReadRivalInfo), SUB_C_DATA_LOAD_RIVAL_NOTIFICATION, nullptr);

	__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameScene::QuitRoom), SUB_C_QUIT_ROOM_NOTIFICATION, nullptr);

	__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameScene::MoveChessRival), SUB_S_MOVE_CHESS_RIVAL_NOTIFICATION, nullptr);

	__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameScene::StartGame), SUB_S_GAME_START_NOTIFICATION, nullptr);

	__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameScene::MoveChessSelf), SUB_S_MOVE_CHESS_SELF_NOTIFICATION, nullptr);

	__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameScene::GameOver), SUB_S_GAME_END_NOTIFICATION, nullptr);

	__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameScene::UserPeaceReq), SUB_S_PEACE_REQ_NOTIFICATION, nullptr);

	__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameScene::UserPeaceAnswer), SUB_S_PEACE_ANSWER_NOTIFICATION, nullptr);

	//发送读取
	int recvMsgType = client->getCurrRecvMsgType();
	if (recvMsgType == SUB_C_START_MATCH) {
		auto msg = client->MsgCreateReadRivalInfo();
		client->SendData(msg);
	}


	auto callFunc = CallFunc::create([&]() {
		//StartGame(reg);
		m_pBtn_Ready->setVisible(true);
	});

	this->runAction(Sequence::create(DelayTime::create(2), callFunc, nullptr));

	return true;
}

void GameScene::QuitRoom(Ref * ref)
{
	int ret = client->getRespond();
	if (ret == QUIT_ROOM_SUCCEED) {
		auto lobbyScene = LobbyScene::createScene();
		TransitionFade* fadeScene = TransitionFade::create(1.2f, lobbyScene, Color3B::BLACK);
		Director::getInstance()->replaceScene(fadeScene);
		/*__NotificationCenter::getInstance()->removeObserver(this, SUB_C_QUIT_ROOM_NOTIFICATION);
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_DATA_LOAD_RIVAL_NOTIFICATION);
		__NotificationCenter::getInstance()->removeObserver(this, SUB_S_MOVE_CHESS_RIVAL_NOTIFICATION);
		__NotificationCenter::getInstance()->removeObserver(this, SUB_S_GAME_END_NOTIFICATION);*/
		__NotificationCenter::getInstance()->removeAllObservers(this);
		this->stopAllActions();
	}
}

void GameScene::ReadRivalInfo(Ref * ref)
{
	CMD_C_DATALOAD data;
	data = client->MsgReadUserData(client->_msgQueue[0]);

	string name = StringUtils::format("%s", data.name);

	int __totalPlayNum = data.W_TotalNum + data.B_TotalNum;
	int __totalWinNum = data.W_TotalWinNum + data.B_TotalWinNum;
	int __totalLoseNum = (__totalPlayNum)-__totalWinNum;

	string w1 = StringUtils::format("%d", __totalWinNum);
	string w2 = StringUtils::format("%d", __totalLoseNum);

	int ret = client->getInstance()->__avta_index;
	auto fileName = StringUtils::format("menu/icon/icon%d.png", ret);
	if (ret < 0 || ret > 6) {
		fileName = "menu/icon/icon5.png";
	}
	auto newTexture = Sprite::create(fileName);
	m_pIconPlayer2->setTexture(newTexture->getTexture());
	m_pTxtUserName2->setText(name);
	m_pTxtWinNum2->setText(w1);
	m_pTxtLoseNum2->setText(w2);

	auto callFunc = CallFunc::create([&]() {
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_DATA_LOAD_RIVAL_NOTIFICATION);
	});
	auto seq = Sequence::create(DelayTime::create(1.5f), callFunc, nullptr);
	this->runAction(seq);
}

void GameScene::onExit()
{
	//delete(gameLogic);
	//gameLogic = nullptr;
}

void GameScene::PrintMsg(int ret, int inxex)
{
	string msg;
	switch (ret)
	{
	case PEACE_SUCCEED:
		msg = "对方同意和棋!";
		break;
	case PEACE_FAILED:
		msg = "对方拒绝求和!";
		break;
	case REGRET_SUCCEED:
		msg = "对方同意悔棋!";
		break;
	case REGRET_FAILED:
		msg = "对方拒绝悔棋!";
		break;
	case GAMEOVER_NORMAL:
		if (client->_isWin == false) {
			msg = "王被杀,游戏结束!";
			int effAudioID = AudioEngine::play2d("res/audio/lose.mp3", false, client->_AudioVolume);
		} else {
			msg = "你战胜了对手!";
			int effAudioID = AudioEngine::play2d("res/audio/win.mp3", false, client->_AudioVolume);
		}
		break;
	case GAMEOVER_GIVEUP:
		if (client->_isWin == false) msg = "我方认输,游戏结束!";
		else msg = "敌方认输,游戏结束!";
		break;
	case GAMEOVER_USER_LEFT:
		msg = "对方退出,游戏结束!";
		break;
	default:
		break;
	}
	CMessageBox *box = new CMessageBox();
	if (inxex == 1) box->CreatCMessageBoxB(msg.c_str());
	if (inxex == 2) box->CreatCMessageBoxC(msg.c_str());
	box->setPosition(Point(160, 360));
	box->setLocalZOrder(100);
	this->addChild(box);
}
void GameScene::initView()
{
	m_pBtnList = (Node*)rootNode->getChildByName("btn_List");
	m_pN_UserInfo = (Node*)rootNode->getChildByName("Node_UserInfo");
	m_pN_User1_Info = (Sprite*)m_pN_UserInfo->getChildByName("user1_Info");
	m_pN_User2_Info = (Sprite*)m_pN_UserInfo->getChildByName("user2_Info");
	m_pN_Spr = (Node*)rootNode->getChildByName("Node_Spr");
	//m_pN_Chess = (Layout*)rootNode->getChildByName("Node_Chess");
	//m_pN_Chess->setAnchorPoint(ccp(0.5, 0.5));
	m_pN_Chess = Node::create();
	m_pN_Chess->setAnchorPoint(Point(0.5, 0.5));
	this->addChild(m_pN_Chess);

	///btn
	m_pBtnChangeChessBord = (Button*)m_pBtnList->getChildByName("btn_Change_ChessBord");
	m_pBtnChangeChessBord->addClickEventListener(CC_CALLBACK_1(GameScene::onClick, this));
	m_pBtnRule = (Button*)m_pBtnList->getChildByName("btn_Rule");
	m_pBtnRule->addClickEventListener(CC_CALLBACK_1(GameScene::onClick, this));
	m_pBtnAudio = (Button*)m_pBtnList->getChildByName("btn_Audio");
	m_pBtnAudio->addClickEventListener(CC_CALLBACK_1(GameScene::onClick, this));
	m_pBtnAudio->setBrightStyle((client->_OpenAudio == true) ? BrightStyle::NORMAL : BrightStyle::HIGHLIGHT);
	m_pBtnQuit = (Button*)m_pBtnList->getChildByName("btn_Quit");
	m_pBtnQuit->addClickEventListener(CC_CALLBACK_1(GameScene::onClick, this));
	m_pBtnCapitulate = (Button*)m_pBtnList->getChildByName("btn_Capitulate");
	m_pBtnCapitulate->addClickEventListener(CC_CALLBACK_1(GameScene::onClick, this));
	m_pBtnQiuHe = (Button*)m_pBtnList->getChildByName("btn_QiuHe");
	m_pBtnQiuHe->addClickEventListener(CC_CALLBACK_1(GameScene::onClick, this));
	m_pBtnChongLia = (Button*)m_pBtnList->getChildByName("btn_ChongLia");
	m_pBtnChongLia->addClickEventListener(CC_CALLBACK_1(GameScene::onClick, this));

	//spr
	int ret = client->getInstance()->__avta_index;
	auto fileName = StringUtils::format("menu/icon/icon%d.png", ret);
	if (ret < 0 || ret > 6) {
		fileName = "menu/icon/icon5.png";
	}
	m_pIconPlayer1 = (Sprite*)m_pN_Spr->getChildByName("icon_Player1");
	auto newTexture = Sprite::create(fileName);
	m_pIconPlayer1->setTexture(newTexture->getTexture());

	m_pIconPlayer2 = (Sprite*)m_pN_Spr->getChildByName("icon_Player2");
	//m_pIconPlayer2->setRotation(180);

	///user1
	m_pTxtUserName1 = (Text*)m_pN_User1_Info->getChildByName("user_Name1");
	m_pTxtWinNum1 = (TextBMFont*)m_pN_User1_Info->getChildByName("txt_WinNum1");
	m_pTxtLoseNum1 = (TextBMFont*)m_pN_User1_Info->getChildByName("txt_LoseNum1");
	m_pTxtUserName1->setString(client->__userName);
	m_pTxtWinNum1->setString(StringUtils::format("%d", client->__totalWinNum));
	m_pTxtLoseNum1->setString(StringUtils::format("%d", client->__totalLoseNum));
	//m_pN_User1_Info->setRotation(180);
	//m_pTxtLoseNum1->setString("1");
	///user2
	m_pTxtUserName2 = (Text*)m_pN_User2_Info->getChildByName("user_Name2");
	m_pTxtUserName2->setString("user_name");
	m_pTxtWinNum2 = (TextBMFont*)m_pN_User2_Info->getChildByName("txt_WinNum2");
	m_pTxtLoseNum2 = (TextBMFont*)m_pN_User2_Info->getChildByName("txt_LoseNum2");
	///杂项
	m_pSpr_ChessBord = (Sprite*)rootNode->getChildByName("spr_ChessBord");
	m_pSpr_SelectFram = Sprite::create("menu/select.png");
	m_pSpr_SelectFram->setScale(0.5);
	m_pSpr_SelectFram->setVisible(false);
	rootNode->addChild(m_pSpr_SelectFram);

	m_pN_Hellp = CSLoader::createNode("Node_Help.csb");
	m_pN_Hellp->setVisible(false);
	m_pBtn_BackToGame = (Button*)m_pN_Hellp->getChildByName("btn_back");
	m_pBtn_BackToGame->addClickEventListener(CC_CALLBACK_1(GameScene::onClick, this));
	m_pN_Hellp->setPosition(Point(160, 240));
	m_pN_Hellp->setLocalZOrder(2000);
	rootNode->addChild(m_pN_Hellp);

	m_pBtn_Ready = Button::create("res/menu/btn_queding.png");
	m_pBtn_Ready->addClickEventListener(CC_CALLBACK_1(GameScene::onClick, this));
	m_pBtn_Ready->setVisible(false);
	m_pBtn_Ready->setPosition(Point(160, 240));
	m_pBtn_Ready->setScale(0.7);
	rootNode->addChild(m_pBtn_Ready);

	m_mMoveType = enMoveType_Error;
}

void GameScene::onClick(Ref * ref)
{
	int effAudioID = AudioEngine::play2d("res/audio/button.mp3", false, client->_AudioVolume);
	if (ref == m_pBtnChangeChessBord) {
		__NotificationCenter::getInstance()->addObserver
		(this, CC_CALLFUNCO_SELECTOR(GameScene::ChangeChessBord), CHESSBORD_CHANGE_NOTIFICATION, nullptr);
		CMessageBox* box = new CMessageBox();
		box->CreatCMessageBoxD();
		box->setPosition(Point(160, 240));
		//box->setScale(1.7, 1);
		this->addChild(box);
	}
	else if (ref == m_pBtnRule) {
		m_pN_Hellp->setVisible(true);
		m_pN_Chess->setVisible(false);
	}
	else if (ref == m_pBtnAudio) {
		(client->_AudioVolume == 1.0) ? client->_AudioVolume = 0 : client->_AudioVolume = 1.0;
		(client->_OpenAudio == true) ? client->_OpenAudio = false : client->_OpenAudio = true;
		m_pBtnAudio->setBrightStyle((client->_OpenAudio == true) ? BrightStyle::NORMAL : BrightStyle::HIGHLIGHT);
	}
	else if (ref == m_pBtnQuit) {
		const char* msg = client->MsgCreateQuitRoom();
		client->SendData(msg);
	}
	else if (ref == m_pBtnCapitulate) {
		if (this->gameOver == true) return;
		SendGiveUpMsg();
	}
	else if (ref == m_pBtnQiuHe) {
		if (this->gameOver == true) return;
		SendPeaceMsg();
	}
	else if (ref == m_pBtn_Ready) {
		SendReadyMsg();
		m_pBtn_Ready->setVisible(false);
	}
	else if (ref == m_pBtn_BackToGame) {
		m_pN_Hellp->setVisible(false);
		PageView *pg = (PageView*)m_pN_Hellp->getChildByName("PageView_2");
		pg->setCurPageIndex(0);
		m_pN_Chess->setVisible(true);
	}
	else if (ref == m_pBtnChongLia) {
		//RestartGame();
	}
}

void GameScene::ChangeChessBord(Ref * ref)
{
	int chessbordIndex = CClient::getInstance()->__chessBord_Index;
	auto fileName = StringUtils::format("menu/ChessBord/Bord_%d.png", chessbordIndex);
	auto newSpr = Sprite::create(fileName);
	if (m_pSpr_ChessBord != nullptr) m_pSpr_ChessBord->setTexture(newSpr->getTexture());
	auto callFunc = CallFunc::create([&]() {
		__NotificationCenter::getInstance()->removeObserver(this, CHESSBORD_CHANGE_NOTIFICATION);
	});
	auto seq = Sequence::create(DelayTime::create(0.2f), callFunc, nullptr);
	this->runAction(seq);
}

void GameScene::SendReadyMsg()
{
	auto msg = client->MsgCreateReady();
	client->SendData(msg);
}

void GameScene::SendMoveChessMsg(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos, int switchChess)
{
	//Point pSource = CoordinateConversion(xSourcePos, ySourcePos);
	//Point pTarget = CoordinateConversion(xTargetPos, yTargetPos);
	const char* msg = client->MsgCreateMoveChess(xSourcePos, ySourcePos, xTargetPos, yTargetPos, switchChess);
	client->SendData(msg);
}

void GameScene::SendGiveUpMsg()
{
	const char* msg = client->MsgCreateGiveUp();
	client->SendData(msg);
	this->gameOver = true;
}

void GameScene::SendPeaceMsg()
{
	const char* msg = client->MsgCreatePeace();
	client->SendData(msg);
}

void GameScene::ReadyBoard(int color)
{
Size size = Director::getInstance()->getWinSize();
for (size_t i = 0; i < CHESS_NUM; i++)
{
	_chess[i] = Chess::create(i, color);

	//设置棋子的初始位置为随机位置
	_chess[i]->setPosition(Point(CCRANDOM_0_1() * size.width, CCRANDOM_0_1() * size.height));

	_chess[i]->setVisible(false);

	m_pN_Chess->addChild(_chess[i]);
}
}

void GameScene::StartGame(Ref* ref)
{
	int ret = client->getRespond();
	m_nChessColor = ret + 1;
	m_nUser = ret;
	m_nCurrUser = WHITE_PLAYER;

	ReadyBoard(m_nUser);

	//因为国际象棋摆棋的特殊，在这里更改王和后的纹理和棋子类型
	auto kingTexture = Sprite::create("res/menu/Chess/one/chess_1_1.png")->getTexture();
	auto queenTexture = Sprite::create("res/menu/Chess/one/chess_1_2.png")->getTexture();
	if (m_nUser == BLACK_PLAYER) {
		_chess[19]->setTexture(queenTexture);
		_chess[20]->setTexture(kingTexture);
	}
	else {
		_chess[19]->setTexture(queenTexture);
		_chess[20]->setTexture(kingTexture);
	}
	_chess[19]->setType(2);
	_chess[20]->setType(1);
	//---------------------------------------------------

	for (size_t i = 0; i < CHESS_NUM; i++)
	{
		_chess[i]->setVisible(true);
		_chess[i]->setZOrder(10);
		CCMoveTo* move = CCMoveTo::create(0.6f, GetChessPosInBoard(_chess[i]->getX(), _chess[i]->getY()));
		_chess[i]->runAction(move);
	}

	auto callFunc = CallFunc::create([&]() {
		__NotificationCenter::getInstance()->removeObserver(this, SUB_S_GAME_START_NOTIFICATION);
	});
	auto seq = Sequence::create(DelayTime::create(5.0f), callFunc, nullptr);
	this->runAction(seq);

	this->gameOver = false;
	this->gameStart = true;
}

void GameScene::MoveChessRival(Ref * ref)
{
	CMD_S_MoveChess data = client->MsgMoveChess(client->_msgQueue[0]);

	Point sPos, tPos;

	if (m_nUser == WHITE_PLAYER) {
		sPos = Point(data.xSourcePos, data.ySourcePos);
		tPos = Point(data.xTargetPos, data.yTargetPos);
	}
	else {
		sPos = CoordinateConversion(data.xSourcePos, data.ySourcePos);
		tPos = CoordinateConversion(data.xTargetPos, data.yTargetPos);
	}

	int moveChessID = GetChessIdByChessInChessBoradPoint(sPos.x, sPos.y);///WHITE_CHESS   BLACK_CHESS
	int killChessID = GetChessIdByChessInChessBoradPoint(tPos.x, tPos.y);

	m_nCurrUser = data.currentUser;

	log("%d", moveChessID);
	_chess[moveChessID]->setX(tPos.x);

	_chess[moveChessID]->setY(tPos.y);


	MoveTo* moveTo = MoveTo::create(0.5f, GetChessPosInBoard(tPos.x, tPos.y));

	CallFuncN* actionCallback = CallFuncN::create(CC_CALLBACK_1(GameScene::onActionComplete, this, moveChessID, killChessID));

	Sequence* seq = Sequence::create(moveTo, actionCallback, NULL);

	_chess[moveChessID]->runAction(seq);

	//兵是升变
	if (_chess[moveChessID]->getType() == CHESS_SOLDIER && data.switchChess != 0) {
		int chessColor = (m_nChessColor == BLACK_CHESS) ? WHITE_CHESS : BLACK_CHESS;
		auto newTexture = Sprite::create(StringUtils::format("res/menu/Chess/one/chess_%d_%d.png", chessColor, data.switchChess))->getTexture();
		_chess[moveChessID]->setTexture(newTexture);
		_chess[moveChessID]->setType(data.switchChess);
		m_nSwitchChess = 0;
	}
	//吃过路兵
	int yTempPos = (_chess[moveChessID]->getRed() == BLACK_CHESS) ? 3 : 4;
	if ((killChessID == -1) && (_chess[moveChessID]->getType() == CHESS_SOLDIER)) {
		/*Point point;
		if (pSourceChessItem->getRed() == BLACK_CHESS) point = CoordinateConversion(xTargetPos, yTempPos);
		else point = ccp(xTargetPos, yTempPos);*/
		int chessID = GetChessIdByChessInChessBoradPoint(tPos.x, sPos.y);
		if (chessID == -1) return ;
		if (_chess[chessID]->getWalkCount() != 1) return ;
		if (_chess[chessID]->getType() != CHESS_SOLDIER) return ;
		_chess[chessID]->setVisible(false);
		_chess[chessID]->setDead(true);
		//m_mMoveType = enMoveType_EatPassingSoldier;
		//return true;
	}


	m_nCurrChessID = -1;
	m_pSpr_SelectFram->setVisible(false);
	//(m_nCurrUser == WHITE_PLAYER) ? (m_nCurrUser = BLACK_PLAYER) : (m_nCurrUser = WHITE_PLAYER);
}

void GameScene::MoveChessSelf(Ref * ref)
{
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = (CC_CALLBACK_2(GameScene::TouchBegan, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	int res = client->getRespond();
	if (res == MOVECHESS_FAILED) return;
	//根据走棋类型变换棋谱
	
	//Step* step = _step;
	_chess[_step[0]->_moveid]->setX(_step[0]->_xTo);

	_chess[_step[0]->_moveid]->setY(_step[0]->_yTo);

	MoveTo* moveTo = MoveTo::create(0.5f, GetChessPosInBoard(_step[0]->_xTo, _step[0]->_yTo));

	CallFuncN* actionCallback = CallFuncN::create(CC_CALLBACK_1(GameScene::onActionComplete, this, _step[0]->_moveid, _step[0]->_killid));

	Sequence* seq = Sequence::create(moveTo, actionCallback, NULL);

	_chess[_step[0]->_moveid]->runAction(seq);

	ChangeChess(_step[0]);

	m_nCurrChessID = -1;
	m_pSpr_SelectFram->setVisible(false);
	(m_nCurrUser == 0) ? (m_nCurrUser = 1) : (m_nCurrUser = 0);
}

void GameScene::GameOver(Ref * ref)
{
	log("END!");
	int ret = client->getRespond();
	CMD_S_GameEnd data = client->MsgGameOver(client->_msgQueue[0]);
	(data.wWinUser == m_nUser) ? (client->_isWin = false) : (client->_isWin = true);
	PrintMsg(ret, 2);
	this->gameOver = true;
	this->gameStart = false;
	
	auto callFunc = CallFunc::create([&]() {
		__NotificationCenter::getInstance()->removeAllObservers(this);

		auto scene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(scene);
	});
	
	this->runAction(Sequence::create(DelayTime::create(3.0f), callFunc, nullptr));
}

void GameScene::UserPeaceReq(Ref * ref)
{
	Sprite* __rootNode = Sprite::create("res/menu/tuijian_1.png");
	__rootNode->setScale(0.6);
	Button* m_pBtn_OK = Button::create("res/menu/btn_ok.png");
	m_pBtn_OK->setScale(0.7);
	m_pBtn_OK->setPosition(Point(110, 55));
	Button* m_pBtn_Quxiao = Button::create("res/menu/btn_quxiao.png");
	m_pBtn_Quxiao->setScale(0.7);
	m_pBtn_Quxiao->setPosition(Point(290, 55));

	Text* txt = Text::create();
	txt->setText("对方发起求和!");
	txt->setPosition(Point(200, 110));
	txt->setScale(3);
	__rootNode->addChild(txt);
	__rootNode->addChild(m_pBtn_OK);
	__rootNode->addChild(m_pBtn_Quxiao);
	__rootNode->setPosition(Point(160, 240));
	this->addChild(__rootNode);

	auto func = [=](Ref* ref) {
		if (ref == m_pBtn_OK) {
			CMD_C_PeaceAnswer data;
			data.cbApprove = PEACE_SUCCEED;
			const char* msg = CClient::getInstance()->MsgCreatePeaceAnswer(data);
			CClient::getInstance()->SendData(msg);
			__rootNode->setVisible(false);
		}
		else {
			CMD_C_PeaceAnswer data;
			data.cbApprove = PEACE_FAILED;
			const char* msg = CClient::getInstance()->MsgCreatePeaceAnswer(data);
			CClient::getInstance()->SendData(msg);
			__rootNode->setVisible(false);
		}
	};
	m_pBtn_OK->addClickEventListener(func);
	m_pBtn_Quxiao->addClickEventListener(func);
}

void GameScene::UserPeaceAnswer(Ref * ref)
{
	int ret = client->getRespond();
	PrintMsg(ret, 1);
	if (ret == PEACE_SUCCEED) {
		this->gameOver = true;
		this->gameStart = false;
		auto callFunc = CallFunc::create([&]() {
			__NotificationCenter::getInstance()->removeObserver(this, SUB_S_PEACE_ANSWER_NOTIFICATION);
			__NotificationCenter::getInstance()->removeObserver(this, SUB_S_PEACE_REQ_NOTIFICATION);
		});
		auto seq = Sequence::create(DelayTime::create(1.5f), callFunc, nullptr);
		this->runAction(seq);
	}
}

void GameScene::ChangeChess(Step* step)
{
	Chess* chess = _chess[step->_moveid];
	if (m_mMoveType == enMoveType_SoldierUp) {
		auto newTexture = Sprite::create(StringUtils::format("res/menu/Chess/one/chess_%d_%d.png", m_nChessColor, m_nSwitchChess))->getTexture();
		chess->setTexture(newTexture);
		chess->setType(m_nSwitchChess);
		m_nSwitchChess = 0;
	}
	else if (m_mMoveType == enMoveType_Normal) {

	}
	else if (m_mMoveType == enMoveType_EatPassingSoldier) {
		int yTempPos = (chess->getRed() == BLACK_CHESS) ? 3 : 4;
		Point point;
		/*if (chess->getRed() == BLACK_CHESS) point = CoordinateConversion(chess->getX(), chess->getY());
		else point = ccp(chess->getX(), chess->getY());*/
		int chessId = GetChessIdByChessInChessBoradPoint(chess->getX(), chess->getY()-1);
		_chess[chessId]->setVisible(false);
		_chess[chessId]->setDead(true);
	}
	else return;

}

Point GameScene::GetChessPosInBoard(int _px, int _py)
{
	int _x = _px * CHESS_DIAMETER;
	int _y = _py * CHESS_DIAMETER;
	return Point(_x, _y) + Point(CHESS_OFFSET_X, CHESS_OFFSET_Y);
}

int GameScene::GetChessIdByChessInChessBoradPoint(int xTargetPos, int yTargetPos)
{
	Chess* chess;
	for (size_t i = 0; i < CHESS_NUM; i++) {
		chess = _chess[i];
		if (chess->getX() == xTargetPos && chess->getY() == yTargetPos && !chess->getDead()) return chess->getID();
	}
	return -1;
}

bool GameScene::JudgeClickIsInChessBoard(Point clickPoint, int & _x, int & _y)
{
	for (_x = 0; _x <= CHESS_COL; _x++)
	{
		for (_y = 0; _y <= CHESS_ROW; _y++)
		{
			Point theXYPostion = GetChessPosInBoard(_x, _y);
			float distance = clickPoint.getDistance(theXYPostion);
			if (distance < CHESS_DIAMETER / 2) return true;
		}
	}
	return false;
}


void GameScene::ShowSelectChessFrme(int _id)
{
	if (_id == -1) return;
	if (m_nCurrUser != m_nUser) return;
	m_nCurrChessID = _id;
	if (m_nChessColor != _chess[m_nCurrChessID]->getRed()) {
		m_nCurrChessID = -1;
		return;
	}
	m_mMoveType = JudgeSoldierUp(m_nCurrChessID);
	m_pSpr_SelectFram->setVisible(true);
	m_pSpr_SelectFram->setPosition(_chess[m_nCurrChessID]->getPosition());
}

void GameScene::MoveChessToClickPoint(int moveID, int killID, int _x, int _y)
{
	//判断点击棋子和目标棋子是否同颜色
	if (killID != -1 && _chess[moveID]->getRed() == _chess[killID]->getRed()) {
		ShowSelectChessFrme(killID);
		return;
	}
	//坐标转换
	///黑棋和白棋公用一个坐标，左下角为(0,0),在摆黑棋时将其y坐标减了7,
	///要在白棋方得到黑棋的正确坐标，需要将黑棋y坐标减7，得到黑棋的正确坐标.
	Point sPos, tPos;
	if (m_nUser == WHITE_PLAYER) {
		sPos = Point(_chess[m_nCurrChessID]->getX(), _chess[m_nCurrChessID]->getY());
		tPos = Point(_x, _y);
	}
	else {
		sPos = CoordinateConversion(_chess[m_nCurrChessID]->getX(), _chess[m_nCurrChessID]->getY());
		tPos = CoordinateConversion(_x, _y);
	}

	bool cameMove = IsWalkLegality(moveID, killID, sPos.x, sPos.y, tPos.x, tPos.y);
	if (cameMove == false) { 
		AudioEngine::play2d("res/audio/unablemoves.mp3", false, client->_AudioVolume);
		return;
	};
	AudioEngine::play2d("res/audio/move.mp3", false, client->_AudioVolume);
	if ((moveID < 32 || moveID == -1) && (killID < 32 || killID == -1)) {
		AddChessToStep(moveID, killID, sPos.x, sPos.y, _x, _y);
		//(m_nCurrUser == 1) ? (m_nCurrUser = 2) : (m_nCurrUser = 1);
		SendMoveChessMsg(sPos.x, sPos.y, tPos.x, tPos.y, m_nSwitchChess);

		_eventDispatcher->removeEventListener(listener);
	}
}

bool GameScene::TouchBegan(Touch * touch, Event * unused_event)
{
	if (this->gameStart == false) return false;
	if (this->gameOver == true) return false;

	Point clickPoint = touch->getLocation();
	//保存触摸点的棋盘坐标
	int x = 0, y = 0;

	//判断点击位置是否在棋盘上
	if (!JudgeClickIsInChessBoard(clickPoint, x, y)) return false;

	Point tPos = CoordinateConversion(x, y);
	log("(%d,%d)", (int)tPos.x, (int)tPos.y);
	
	//log("(%d,%d)",_chess[m_nCurrChessID]->getX(), _chess[m_nCurrChessID]->getY());

	//通过触摸点在棋盘中的坐标获取选中的棋子的id
	int clickid = GetChessIdByChessInChessBoradPoint(x, y);

	//Chess* che = gameLogic->_chess[clickid];
	if (clickid != -1 && m_nChessColor == _chess[clickid]->getRed() && m_nCurrUser == m_nUser) AudioEngine::play2d("res/audio/down.mp3", false, client->_AudioVolume);

	log("%d", clickid);
	log("%d", _chess[clickid]->getType());

	//m_nCurrChessID = clickid;
	//if (m_nChessColor != _chess[clickid]->getRed()) return false;
	//if (m_nCurrChessID != -1 && m_nUser == m_nCurrUser) {
	if (m_nCurrChessID != -1 && m_nChessColor == _chess[m_nCurrChessID]->getRed() && m_nUser == m_nCurrUser) {
		if ((m_nCurrChessID < 32 || m_nCurrChessID == -1) && (clickid < 32 || clickid == -1))
			MoveChessToClickPoint(m_nCurrChessID, clickid, x, y);
	}else{
		ShowSelectChessFrme(clickid);
	}

	return true;
}

void GameScene::AddChessToStep(int moveid, int killid, int xFrom, int yFrom, int xTo, int yTo)
{
	_step.clear();
	//delete _step[0];
	assert(moveid<31 && moveid>-1);
	Step* step = Step::create(moveid, killid, xFrom, yFrom, xTo, yTo);
	_step.push_back(step);
}

bool GameScene::IsWalkLegality(int sourceChessID, int targetChessID, int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos)
{
	//效验坐标
	Chess * pSourceChessItem = _chess[sourceChessID];
	assert((xSourcePos < 8) && (ySourcePos < 8));
	assert((xTargetPos < 8) && (yTargetPos < 8));

	int cType = pSourceChessItem->getType();
	//偏移变量
	int xExcursion = GetExcursion(xSourcePos, xTargetPos);
	int yExcursion = GetExcursion(ySourcePos, yTargetPos);

	//王车易位
	if ((targetChessID != -1) && (_chess[targetChessID]->getRed() == pSourceChessItem->getRed()))
	{
		Chess * pTargetChessItem = _chess[targetChessID];
		//步数判断
		if ((pTargetChessItem->getWalkCount() != 0) && (pSourceChessItem->getWalkCount() != 0)) return false;

		//王棋判断
		Chess * pKingChessItem = nullptr;
		if (pTargetChessItem->getType() == CHESS_KING) pKingChessItem = pTargetChessItem;
		else if (pSourceChessItem->getType() == CHESS_KING) pKingChessItem = pTargetChessItem;
		else return false;

		//车棋判断
		Chess * pRookChessItem = nullptr;
		if (pTargetChessItem->getType() == CHESS_ROOK) pKingChessItem = pTargetChessItem;
		else if (pSourceChessItem->getType() == CHESS_ROOK) pKingChessItem = pTargetChessItem;
		else return false;

		m_mMoveType = enMoveType_KingRook;
		//阻挡判断
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
	}


	switch (cType)
	{
	case CHESS_SOLDIER:
		//变量定义
		int nYStep;
		nYStep = (pSourceChessItem->getRed() == BLACK_CHESS) ? -1 : 1;;
		//士兵不能往后走
		if (pSourceChessItem->getRed() == BLACK_CHESS) {
			if (ySourcePos < yTargetPos) return false;
		}else {
			if (ySourcePos > yTargetPos) return false;
		}
		//直行判断
		if (xSourcePos == xTargetPos)
		{
			if (targetChessID != -1) return false;
			if ((ySourcePos + nYStep) == yTargetPos) return true;
			if ((pSourceChessItem->getWalkCount() == 0) && ((ySourcePos + nYStep * 2) == yTargetPos)) {
				return true;
			}
			else return false;
		}
		//斜行判断
		if ((ySourcePos + nYStep) != yTargetPos) return false;
		if ((GetExcursion(xSourcePos, xTargetPos)) != 1) return false;
		//吃子判断
		if ((targetChessID != -1) && (_chess[targetChessID]->getRed() != pSourceChessItem->getRed())) return true;
		//过路兵判断
		int yTempPos;
		yTempPos = (pSourceChessItem->getRed() == BLACK_CHESS) ? 3 : 4;
		if ((targetChessID == -1) && (ySourcePos == yTempPos)) {
			Point point;
			if (pSourceChessItem->getRed() == BLACK_CHESS) point = CoordinateConversion(xTargetPos, yTempPos);
			else point = Point(xTargetPos, yTempPos);
			int chessID = GetChessIdByChessInChessBoradPoint(point.x, point.y);
			if (chessID == -1) return false;
			if (_chess[chessID]->getWalkCount() != 1) return false;
			if (_chess[chessID]->getX() != point.x) return false;
			if (_chess[chessID]->getType() != CHESS_SOLDIER) return false;
			m_mMoveType = enMoveType_EatPassingSoldier;
			return true;
		}
		break;

	case CHESS_ROOK:
		//步法判断
		if ((xSourcePos != xTargetPos) && (ySourcePos != yTargetPos)) return false;

		//阻塞分析
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);

		break;
	case CHESS_HORSE:
		//步法判断
		if ((xExcursion == 1) && (yExcursion == 2)) return true;
		if ((xExcursion == 2) && (yExcursion == 1)) return true;

		return false;
		break;
	case CHESS_ELEPHANT:
		//步法判断
		if ((xExcursion == 0) || (yExcursion == 0) || (xExcursion != yExcursion)) return false;

		//阻塞分析
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
		break;
	case CHESS_QUEEN:
		//步法判断
		if ((xSourcePos != xTargetPos) && (ySourcePos != yTargetPos) && (xExcursion != yExcursion)) return false;

		//阻塞分析
		return (ObstructAnalyse(xSourcePos, ySourcePos, xTargetPos, yTargetPos) == false);
		break;
	case CHESS_KING:
		//步法判断
		if ((xExcursion>1) || (yExcursion>1)) return false;

		return true;
		break;
	}
}

enMoveType GameScene::JudgeSoldierUp(int sourceChessID)
{
	//获取棋子
	Chess * pSourceChessItem = _chess[sourceChessID];
	//效验坐标
	//assert((pSourceChessItem->getX()<8) && (pSourceChessItem->getY()<8));
	enMoveType MoveChessType = enMoveType_Normal;
	//兵的升变
	if ((pSourceChessItem->getType() == CHESS_SOLDIER) && (pSourceChessItem->getWalkCount() > 1) && ((pSourceChessItem->getY() == 1) || (pSourceChessItem->getY() == 6))) {

		CMessageBox *box = new CMessageBox();
		box->CreatCMessageBoxC("请选择升变棋子!");
		box->setPosition(Point(160, 360));
		this->addChild(box);

		MoveChessType = enMoveType_SoldierUp;
		Layout *lay = Layout::create();
		lay->setBackGroundImage("res/menu/box_tongyong.png");
		lay->setBackGroundImageScale9Enabled(true);
		lay->setContentSize(Size(190, 140));
		lay->setAnchorPoint(Point(0.5f, 0.5f));
		//lay->setScale(0.7);
		lay->setPosition(Point(160, 240));
		this->addChild(lay);

		
		auto func = [=](Ref* ref) {
			for (int i = 0; i < btn_vec.size(); i++) {
				if (ref == btn_vec[i]) {
					m_nSwitchChess = i + 2;
					break;
				}
			}
			lay->setVisible(false);
			btn_vec.clear();
		};

		Button* btn_close = Button::create("res/menu/button/btn_close.png");
		btn_close->addClickEventListener([=](Ref* ref) {
			lay->setVisible(false);
			btn_vec.clear();
		});
		btn_close->setScale(0.4);
		btn_close->setPosition(Point(180, 130));
		lay->addChild(btn_close);

		for (int i = 2; i < 6; i++) {
			Button* btn = Button::create(StringUtils::format("res/menu/Chess/one/chess_%d_%d.png", m_nChessColor, i));
			btn->addClickEventListener(func);
			btn->setPosition(Point((i % 2 == 0) ? 45 : 130, (i <= 3) ? 100 : 40));
			btn->setScale(0.8f);
			btn_vec.push_back(btn);
			lay->addChild(btn);
		}
	}
	return MoveChessType;
}


inline bool GameScene::ObstructAnalyse(int xSourcePos, int ySourcePos, int xTargetPos, int yTargetPos)
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
	} while (GetChessIdByChessInChessBoradPoint(xSourcePos, ySourcePos) == -1);

	return true;
}

inline Point GameScene::CoordinateConversion(int _x, int _y)
{
	return Point(7, 7) - Point(_x, _y);
	//return ccp(_x, 7 - _y);
}

void GameScene::onActionComplete(Node * node, int moveID, int killID)
{
	//_chess[moveID]->setZOrder(_chess[moveID]->getZOrder() + 1);

	_chess[moveID]->setWalkCount(_chess[moveID]->getWalkCount() + 1);

	if (killID != -1)
	{
		_chess[killID]->setDead(true);

		_chess[killID]->setVisible(false);

		if (_chess[killID]->getType() == CHESS_KING)
		{
			//reStartGame();
		}
	}
}