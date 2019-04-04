#include "LoginScene.h"
#include "CMessageBox.h"
#include"AudioEngine.h"

using namespace cocos2d::experimental;;

using namespace cocostudio::timeline;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    rootNode = CSLoader::createNode("LoginScene.csb");
	
	client = CClient::getInstance();

	client->Connect("127.0.0.1", 6601);
	
    addChild(rootNode);
	
	initView();

	__NotificationCenter::getInstance()->addObserver
	(this, CC_CALLFUNCO_SELECTOR(LoginScene::changeSceneToLobby), SUB_C_LOGIN_REQ_NOTIFICATION, nullptr);
	
	__NotificationCenter::getInstance()->addObserver
	(this, CC_CALLFUNCO_SELECTOR(LoginScene::changeSceneToLobby), SUB_C_REGISTER_REQ_NOTIFICATION, nullptr);


    return true;
}

void LoginScene::initView()
{
	m_Node_Login = CSLoader::createNode("Node_Login.csb");
	m_Node_Login->setPosition(Point(160, 240));
	m_Node_Login->setScale(1.5, 1.3);
	m_Node_Login->setVisible(false);

	m_pBtn_Login = (Button*)rootNode->getChildByName("btn_Login");
	m_pBtn_Login->addClickEventListener(CC_CALLBACK_1(LoginScene::onClick, this));

	m_pBtn_Register = (Button*)rootNode->getChildByName("btn_Register");
	m_pBtn_Register->addClickEventListener(CC_CALLBACK_1(LoginScene::onClick, this));

	m_pBtn_Ok = (Button*)m_Node_Login->getChildByName("btn_Ok");
	m_pBtn_Ok->addClickEventListener(CC_CALLBACK_1(LoginScene::onClick, this));

	m_pBtn_No = (Button*)m_Node_Login->getChildByName("btn_No");
	m_pBtn_No->addClickEventListener(CC_CALLBACK_1(LoginScene::onClick, this));

	m_pTxt_Name = (TextField*)m_Node_Login->getChildByName("TextField_Name");
	m_pTxt_Passwd = (TextField*)m_Node_Login->getChildByName("TextField_Passwd");

	//AudioEngine::preload("res/audio/button.wav");

	addChild(m_Node_Login);
}

void LoginScene::onClick(Ref * ref)
{
	int effAudioID = AudioEngine::play2d("res/audio/button.mp3", false, client->_AudioVolume);
	log("%d", effAudioID);
	if (ref == m_pBtn_Login) {
		isLogin = true;
		m_Node_Login->setVisible(true);
	}
	else if (ref == m_pBtn_Register) {
		isLogin = false;
		m_Node_Login->setVisible(true);
	}
	else if (ref == m_pBtn_Ok) {
		loginGame();
	}
	else if (ref == m_pBtn_No) {
		m_Node_Login->setVisible(false);
		
	}
}

void LoginScene::loginGame()
{
	string name = m_pTxt_Name->getString();
	client->__userName = name;
	string passwd = m_pTxt_Passwd->getString();

	if (name.empty() || passwd.empty()) {
		CMessageBox *box = new CMessageBox();
		box->CreatCMessageBoxB("用户名或密码为空！");
		box->setPosition(Point(160, 360));
		box->setLocalZOrder(100);
		rootNode->addChild(box);
		/*CMessageBox *box = new CMessageBox();
		box->CreatCMessageBoxC("哈哈哈哈哈哈哈哈哈！！！！！");
		box->setPosition(ccp(160, 360));
		rootNode->addChild(box);*/
		return;
	}

	const char* msg;
	if (isLogin == true) {
		msg = client->MsgCreateLogin(name.c_str(), passwd.c_str());
	}else{
		msg = client->MsgCreateRegister(name.c_str(), passwd.c_str());
	}
	client->SendData(msg);
}

void LoginScene::changeSceneToLobby(Ref* ref )
{
	//获取服务器返回消息
	//client->RespondFromServerMsg(client->_msgQueue[0]);
	int ret = client->getRespond();
	if (ret == LOGIN_SUCCEED) {
		Scene* lobbyScene = LobbyScene::createScene();
		TransitionFade* fadeScene = TransitionFade::create(1.2f, lobbyScene, Color3B::BLACK);
		Director::getInstance()->replaceScene(fadeScene);
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_LOGIN_REQ_NOTIFICATION);
		m_Node_Login->setVisible(false);

		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_REGISTER_REQ_NOTIFICATION);
		m_Node_Login->setVisible(false);
	}
	else if (ret == REGISTER_SUCCEED) {
		m_Node_Login->setVisible(false);
	}
	PrintMsg(ret);
}

void LoginScene::PrintMsg(int ret)
{
	CMessageBox *box = new CMessageBox();
	string msg;

	switch (ret)
	{
	case ERROR_LOGIN_NAME_NULL:
		msg = "用户名不存在!";
		break;
	case ERROR_LOGIN_PASSWD_NOTMATCH:
		msg = "密码不匹配!";
		break;
	case LOGIN_SUCCEED:
		msg = "登陆成功!";
		break;
	case ERROR_REGISTER_NAME_NULL:
		msg = "用户名为空!";
		break;
	case ERROR_REGISTER_NAME_EXIST:
		msg = "用户已存在!";
		break;
	case REGISTER_SUCCEED:
		msg = "注册成功!";
		break;
	default:
		msg = "网络错误!";
		break;
	}

	box->CreatCMessageBoxB(msg.c_str());
	box->setPosition(Point(160, 360));
	box->setLocalZOrder(1000);
	rootNode->addChild(box);
}
