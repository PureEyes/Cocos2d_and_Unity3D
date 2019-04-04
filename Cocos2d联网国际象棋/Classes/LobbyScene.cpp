#include "LobbyScene.h"
#include "GameScene.h"
#include "LoginScene.h"
#include "CMessageBox.h"
#include"AudioEngine.h"

using namespace cocos2d::experimental;;

Scene* LobbyScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LobbyScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LobbyScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    rootNode = CSLoader::createNode("LobbyScene.csb");

	client = CClient::getInstance();

	initView();

    addChild(rootNode);

	__NotificationCenter::getInstance()->addObserver
	(this, CC_CALLFUNCO_SELECTOR(LobbyScene::changeSceneToGame), SUB_C_CREAT_ROOM_NOTIFICATION, nullptr);

	__NotificationCenter::getInstance()->addObserver
	(this, CC_CALLFUNCO_SELECTOR(LobbyScene::changeSceneToGame), SUB_C_START_MATCH_NOTIFICATION, nullptr);

	__NotificationCenter::getInstance()->addObserver
	(this, CC_CALLFUNCO_SELECTOR(LobbyScene::ReadUserInfo), SUB_C_DATA_LOAD_SELF_NOTIFICATION, nullptr);


	/*auto callFunc = CallFunc::create([&]() {
		auto msg = client->MsgCreateReadRoomList();
		client->SendData(msg);
	});
	this->runAction(Sequence::create(DelayTime::create(2), callFunc, nullptr));*/

    return true;
}

void LobbyScene::initView()
{
	m_pN_BtnList = (Node*)rootNode->getChildByName("btn_list");
	m_pN_Setting = (Node*)rootNode->getChildByName("Node_Setting");
	m_pN_Top = (Node*)rootNode->getChildByName("spr_top");

	m_pBtn_Setting = (Button*)m_pN_BtnList->getChildByName("btn_Setting");	
	m_pBtn_Setting->addClickEventListener(CC_CALLBACK_1(LobbyScene::onClick, this));
	m_pBtn_CreatRoom = (Button*)m_pN_BtnList->getChildByName("btn_CreatRoom");
	m_pBtn_CreatRoom->addClickEventListener(CC_CALLBACK_1(LobbyScene::onClick, this));
	m_pBtn_StartMatch = (Button*)m_pN_BtnList->getChildByName("btn_StartMatch");
	m_pBtn_StartMatch->addClickEventListener(CC_CALLBACK_1(LobbyScene::onClick, this));

	m_pBtn_Rule = (Button*)m_pN_Setting->getChildByName("btn_Rule");
	m_pBtn_Rule->addClickEventListener(CC_CALLBACK_1(LobbyScene::onClick, this));
	m_pBtn_Audio = (Button*)m_pN_Setting->getChildByName("btn_Audio");
	m_pBtn_Audio->addClickEventListener(CC_CALLBACK_1(LobbyScene::onClick, this));
	m_pBtn_Audio->setBrightStyle((client->_OpenAudio == true) ? BrightStyle::NORMAL : BrightStyle::HIGHLIGHT);
	m_pBtn_Quit = (Button*)m_pN_Setting->getChildByName("btn_Quit");
	m_pBtn_Quit->addClickEventListener(CC_CALLBACK_1(LobbyScene::onClick, this));

	m_pBtn_Icon = (Button*)m_pN_Top->getChildByName("btn_Icon");
	
	///
	int ret = client->getInstance()->__avta_index;
	auto fileName = StringUtils::format("menu/icon/icon%d.png", ret);
	if (ret < 0 || ret > 6) {
		fileName = "menu/icon/icon5.png";
	}
	m_pBtn_Icon->loadTextureNormal(fileName);
	m_pBtn_Icon->loadTexturePressed(fileName);
	///
	m_pBtn_Icon->addClickEventListener(CC_CALLBACK_1(LobbyScene::onClick, this));

	m_pUserName = (TextBMFont*)m_pN_Top->getChildByName("txt_Name");
	m_pUserName->setString(client->__userName);

	m_pN_Hellp = CSLoader::createNode("Node_Help.csb");
	m_pN_Hellp->setVisible(false);
	m_pBtn_BackToGame = (Button*)m_pN_Hellp->getChildByName("btn_back");
	m_pBtn_BackToGame->addClickEventListener(CC_CALLBACK_1(LobbyScene::onClick, this));
	m_pN_Hellp->setPosition(Point(160, 240));
	m_pN_Hellp->setLocalZOrder(20);
	rootNode->addChild(m_pN_Hellp);

	auto msg = client->MsgCreateReadUserInfo();
	client->SendData(msg);
}

void LobbyScene::onClick(Ref * ref)
{
	int effAudioID = AudioEngine::play2d("res/audio/button.mp3", false, client->_AudioVolume);
	if (ref == m_pBtn_Setting) {
		auto callFunc = CallFunc::create([&]() {
			m_pBtn_Setting->setEnabled(true);
		});
		if (isVisibleSetting) {
			m_pBtn_Setting->setEnabled(false);
			auto moveto = MoveTo::create(0.3, Point(-150, 0));
			auto seq = Sequence::create(moveto, callFunc, nullptr);
			m_pN_Setting->runAction(seq);
			isVisibleSetting = false;	
		}
		else {
			m_pBtn_Setting->setEnabled(false);
			auto moveto = MoveTo::create(0.4, Point(-0, 0));
			auto seq = Sequence::create(moveto, callFunc, nullptr);
			m_pN_Setting->runAction(seq);
			isVisibleSetting = true;
		}
		
	}
	else if (ref == m_pBtn_CreatRoom) {
		const char* msg = client->MsgCreateRoom();
		client->SendData(msg);
	}
	else if (ref == m_pBtn_StartMatch) {
		const char* msg = client->MsgCreateMatch();
		client->SendData(msg);
	}
	else if (ref == m_pBtn_Rule) {
		m_pN_Hellp->setVisible(true);
	}
	else if (ref == m_pBtn_BackToGame) {
		m_pN_Hellp->setVisible(false);
		PageView *pg = (PageView*)m_pN_Hellp->getChildByName("PageView_2");
		pg->setCurPageIndex(0);
	}
	else if (ref == m_pBtn_Audio) {
		(client->_AudioVolume == 1.0) ? client->_AudioVolume = 0 : client->_AudioVolume = 1.0;
		(client->_OpenAudio == true) ? client->_OpenAudio = false : client->_OpenAudio = true;
		m_pBtn_Audio->setBrightStyle((client->_OpenAudio == true) ? BrightStyle::NORMAL : BrightStyle::HIGHLIGHT);
	}
	else if (ref == m_pBtn_Quit) {
		quitLobby();
	}
	else if (ref == m_pBtn_Icon) {
		auto userInfo = UserInfo::createScene();
		TransitionFade* fadeScene = TransitionFade::create(1.2f, userInfo, Color3B::BLACK);
		Director::getInstance()->replaceScene(fadeScene);
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_CREAT_ROOM_NOTIFICATION);
		Director::getInstance()->replaceScene(fadeScene);
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_START_MATCH_NOTIFICATION);
		//SendReadUserInfoReq();
	}
}

void LobbyScene::changeSceneToGame(Ref *ref)
{
	int ret = client->getRespond();
	PrintfMsg(ret);
	if (ret == MATCH_SUCCEED || ret == CREAT_ROOM_SUCCEED) {
		auto gameScene = GameScene::createScene();
		TransitionFade* fadeScene = TransitionFade::create(1.2f, gameScene, Color3B::BLACK);
		Director::getInstance()->replaceScene(fadeScene);
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_CREAT_ROOM_NOTIFICATION);
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_START_MATCH_NOTIFICATION);
	}
}

void LobbyScene::ReadUserInfo(Ref *ref)
{
	auto data = client->MsgReadUserData(client->_msgQueue[0]);
	
	int ret = data.avta_index;
	auto fileName = StringUtils::format("menu/icon/icon%d.png", ret);
	if (ret < 0 || ret > 6) {
		fileName = "menu/icon/icon5.png";
	}
	m_pBtn_Icon->loadTextureNormal(fileName);
	m_pBtn_Icon->loadTexturePressed(fileName);

	auto callFunc = CallFunc::create([&]() {
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_DATA_LOAD_SELF_NOTIFICATION);
	});
	auto seq = Sequence::create(DelayTime::create(0.2f), callFunc, nullptr);
	this->runAction(seq);
	log("red user data finish!");
}

void LobbyScene::quitLobby()
{
	CClient::getInstance()->Quit();
	auto loginScene = LoginScene::createScene();
	TransitionFade* fadeScene = TransitionFade::create(1.2f, loginScene, Color3B::BLACK);
	Director::getInstance()->replaceScene(fadeScene);
}

void LobbyScene::LobbyScene::PrintfMsg(int ret)
{
	string msg;
	switch (ret)
	{
	case MATCH_SUCCEED:
		msg = "对局找到!";
		break;
	case CREAT_ROOM_SUCCEED:
		msg = "创建成功!";
		break;
	case CREAT_ROOM_FAILED:
		msg = "创建失败!";
		break;
	case MATCH_FAILED:
		msg = "已加入匹配队列,请耐心等待!";
		break;
	default:
		break;
	}
	CMessageBox *box = new CMessageBox();
	box->CreatCMessageBoxB(msg.c_str());
	box->setPosition(Point(160, 360));
	box->setLocalZOrder(100);
	this->addChild(box);
}