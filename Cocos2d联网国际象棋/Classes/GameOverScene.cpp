#include "GameOverScene.h"
#include "LobbyScene.h"
#include "CClient.h"
#include "RankingListScene.h"

Scene* GameOverScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOverScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	//FileUtils::getInstance()->addSearchPath("res/menu");
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance danji
bool GameOverScene::init()
{
	if (!Layer::init()){
		return false;
	}

	rootNode = CSLoader::createNode("GameOverScene.csb");
	this->addChild(rootNode);
	//rootNode->setVisible(true);

	m_pSpr_ShowWin = (Sprite*)rootNode->getChildByName("Spr_ShowWin");
	auto newTexture = Sprite::create("res/menu/text-youlost.png")->getTexture();
	if (CClient::getInstance()->_isWin == false) m_pSpr_ShowWin->setTexture(newTexture);

	m_pBtn_RankingList = (Button*)rootNode->getChildByName("Btn_RankingList");
	m_pBtn_RankingList->addClickEventListener(CC_CALLBACK_1(GameOverScene::onClick, this));
	m_pBtn_Back = (Button*)rootNode->getChildByName("Btn_Back");
	m_pBtn_Back->addClickEventListener(CC_CALLBACK_1(GameOverScene::onClick, this));

	m_pTxt_W_TotalNum = (TextBMFont*)rootNode->getChildByName("text_W_TotalNum");
	m_pTxt_B_TotalNum = (TextBMFont*)rootNode->getChildByName("text_B_TotalNum");

	m_pTxt_W_TotalWinNum = (TextBMFont*)rootNode->getChildByName("text_W_TotalWinNum");
	m_pTxt_B_TotalWinNum = (TextBMFont*)rootNode->getChildByName("text_B_TotalWinNum");

	CClient::getInstance()->SendData(CClient::getInstance()->MsgCreateReadUserInfo());

	__NotificationCenter::getInstance()->addObserver
	(this, CC_CALLFUNCO_SELECTOR(GameOverScene::ReadUserInfo), SUB_C_DATA_LOAD_SELF_NOTIFICATION, nullptr);

	return true;
}

void GameOverScene::onClick(Ref * ref)
{
	if (ref == m_pBtn_Back ) {
		Scene* lobby = LobbyScene::createScene();
		TransitionFade* fadeScene = TransitionFade::create(1.2f, lobby, Color3B::BLACK);
		Director::getInstance()->replaceScene(fadeScene);
		CClient::getInstance()->_isWin = false;
	}
	else if (ref == m_pBtn_RankingList) {
		Scene* rank = RankingListScene::createScene();
		TransitionFade* fadeScene = TransitionFade::create(1.2f, rank, Color3B::BLACK);
		Director::getInstance()->replaceScene(fadeScene);
	}
}

void GameOverScene::ReadUserInfo(Ref * ref)
{
	CMD_C_DATALOAD data;
	data = CClient::getInstance()->MsgReadUserData(CClient::getInstance()->_msgQueue[0]);

	string sex = StringUtils::format("%s", data.sex);
	string sign = StringUtils::format("%s", data.signature);
	string age = StringUtils::format("%d", data.age);
	string w1 = StringUtils::format("%d", data.W_TotalNum);
	string w2 = StringUtils::format("%d", data.W_TotalWinNum);
	string b1 = StringUtils::format("%d", data.B_TotalNum);
	string b2 = StringUtils::format("%d", data.B_TotalWinNum);
	//Êý¾Ý
	m_pTxt_W_TotalNum->setString(w1);
	m_pTxt_W_TotalWinNum->setString(w2);
	m_pTxt_B_TotalNum->setString(b1);
	m_pTxt_B_TotalWinNum->setString(b2);

	auto callFunc = CallFunc::create([&]() {
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_DATA_LOAD_SELF_NOTIFICATION);
	});
	auto seq = Sequence::create(DelayTime::create(3.0f), callFunc, nullptr);
	this->runAction(seq);
}
