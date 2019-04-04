#include "UserInfo.h"
#include "LobbyScene.h"
#include <assert.h>
#include "CMessageBox.h"
#include "RankingListScene.h"

Scene* UserInfo::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UserInfo::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool UserInfo::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    rootNode = CSLoader::createNode("Node_ModifInfor.csb");
	rootNode->setPosition(Point(160, 240));

	client = CClient::getInstance();

	initView();
	//读取信息
	SendReadUserInfoReq();
	//注册消息
	__NotificationCenter::getInstance()->addObserver
	(this, CC_CALLFUNCO_SELECTOR(UserInfo::ReadUserInfo), SUB_C_DATA_LOAD_SELF_NOTIFICATION, nullptr);

	__NotificationCenter::getInstance()->addObserver
	(this, CC_CALLFUNCO_SELECTOR(UserInfo::ChangeSceneToLobby), SUB_C_MODIF_INFO_NOTIFICATION, nullptr);

    addChild(rootNode);

    return true;
}

void UserInfo::initView()
{
	m_pBtn_Back = (Button*)rootNode->getChildByName("spr_Info")->getChildByName("btn_Back");
	m_pBtn_Back->addClickEventListener(CC_CALLBACK_1(UserInfo::onClick, this));
	m_pBtn_Back->setEnabled(false);
	m_pBtn_Change = (Button*)rootNode->getChildByName("btn_Change");
	m_pBtn_Change->addClickEventListener(CC_CALLBACK_1(UserInfo::onClick, this));
	m_pBtn_RankingList = (Button*)rootNode->getChildByName("Btn_RankingList");
	m_pBtn_RankingList->addClickEventListener(CC_CALLBACK_1(UserInfo::onClick, this));


	m_pTxt_Name = (Text*)rootNode->getChildByName("TextField_Name");
	m_pTxt_Signature = (TextField*)rootNode->getChildByName("TextField_Signature");

	m_pTxt_Name->setString(client->__userName);
	m_pTxt_Age = (TextField*)rootNode->getChildByName("TextField_Age");

	m_pTxt_WinNum = (TextBMFont*)rootNode->getChildByName("text_WinNum");

	m_pTxt_W_TotalNum = (TextBMFont*)rootNode->getChildByName("text_W_TotalNum");
	m_pTxt_B_TotalNum = (TextBMFont*)rootNode->getChildByName("text_B_TotalNum");

	m_pTxt_W_TotalWinNum = (TextBMFont*)rootNode->getChildByName("text_W_TotalWinNum");
	m_pTxt_B_TotalWinNum = (TextBMFont*)rootNode->getChildByName("text_B_TotalWinNum");

	m_pCB_Nan = (CheckBox*)rootNode->getChildByName("CheckBox_Nan");
	m_pCB_Nan->setEnabled(false);
	m_pCB_Nv = (CheckBox*)rootNode->getChildByName("CheckBox_Nv");
	m_pCB_Nv->setEnabled(false);
	m_pCB_Nan->addClickEventListener(CC_CALLBACK_1(UserInfo::onClick, this));
	m_pCB_Nv->addClickEventListener(CC_CALLBACK_1(UserInfo::onClick, this));

	m_Spr_Icon = (Sprite*)rootNode->getChildByName("icon_avta");
}

void UserInfo::onClick(Ref * ref)
{
	if (ref == m_pBtn_Back) {
		//提交修改请求
		SendModifInfor();
	}
	else if (ref == m_pBtn_Change) {
		__NotificationCenter::getInstance()->addObserver
		(this, CC_CALLFUNCO_SELECTOR(UserInfo::ChangeAvta), AVTA_CHANGE_NOTIFICATION, nullptr);

		CMessageBox* box = new CMessageBox();
		box->CreatCMessageBoxA();
		box->setPosition(Point(0, 50));
		box->setScale(1.7, 1);
		rootNode->addChild(box);
		//if (__isload == false) 
		//__isload = true;
		//m_pN_box->setVisible(true);
	}
	else if (ref == m_pCB_Nan) {
		__isMan = true;
		if (__isMan == true) {
			m_pCB_Nv->setSelectedState(false);
		}
	}
	else if (ref == m_pCB_Nv) {
		__isMan = false;
		if (__isMan == false) {
			m_pCB_Nan->setSelectedState(false);
		}
	}
	else if (ref == m_pBtn_RankingList) {
		auto rankListScene = RankingListScene::createScene();
		TransitionFade* fadeScene = TransitionFade::create(1.2f, rankListScene, Color3B::BLACK);
		Director::getInstance()->replaceScene(fadeScene);
	}
}

void UserInfo::SendModifInfor()
{
	__userName = m_pTxt_Name->getString();
	__signature = m_pTxt_Signature->getString();
	__age = atoi(m_pTxt_Age->getString().c_str());
	(__isMan == true) ? (__sex = "男") : (__sex = "女");
	CMD_C_MODIFINFO data;
	strcpy(data.sex, __sex.c_str());
	strcpy(data.signature, __signature.c_str());
	strcpy(data.name, __userName.c_str());
	data.age = __age;
	data.avta_index = __avta_num;
	const char* msg = client->MsgCreateModifUserInfo(data);
	client->SendData(msg);
	client->getInstance()->__avta_index = __avta_num;
}

void UserInfo::SendReadUserInfoReq()
{
	const char* msg = client->MsgCreateReadUserInfo();
	client->SendData(msg);
}

void UserInfo::ChangeAvta(Ref * ref)
{
	__avta_num = CClient::getInstance()->__avta_index;
	auto fileName = StringUtils::format("menu/icon/icon%d.png", __avta_num);
	auto newSpr = Sprite::create(fileName);
	m_Spr_Icon->setTexture(newSpr->getTexture());
	auto callFunc = CallFunc::create([&]() {
		__NotificationCenter::getInstance()->removeObserver(this, AVTA_CHANGE_NOTIFICATION);
	});
	auto seq = Sequence::create(DelayTime::create(0.2f), callFunc, nullptr);
	this->runAction(seq);
}

void UserInfo::CreatMsgBox()
{
	//m_pN_box->setVisible(true);
	//m_pN_box->CreatCMessageBoxA();
	//m_pN_box->setPosition(ccp(0, 50));
	//m_pN_box->setScale(1.7, 1);
}

void UserInfo::ReadUserInfo(Ref *ref)
{
	CMD_C_DATALOAD data;
	data = client->MsgReadUserData(client->_msgQueue[0]);

	string sex = StringUtils::format("%s", data.sex);
	string sign = StringUtils::format("%s", data.signature);
	string age = StringUtils::format("%d", data.age);
	string w1 = StringUtils::format("%d", data.W_TotalNum);
	string w2 = StringUtils::format("%d", data.W_TotalWinNum);
	string b1 = StringUtils::format("%d", data.B_TotalNum);
	string b2 = StringUtils::format("%d", data.B_TotalWinNum);
	//胜率
	float totalWinNum = data.W_TotalWinNum + data.B_TotalWinNum;
	float totalNum = data.W_TotalNum + data.B_TotalNum;
	float ret = totalWinNum/totalNum;
	string winRet = StringUtils::format("%.3f", ret);//text_WinNum
	m_pTxt_WinNum->setString(winRet);
	//年龄
	m_pTxt_Age->setString(age);
	m_pTxt_Signature->setString(sign.c_str());
	//数据
	m_pTxt_W_TotalNum->setString(w1);
	m_pTxt_W_TotalWinNum->setString(w2);
	m_pTxt_B_TotalNum->setString(b1);
	m_pTxt_B_TotalWinNum->setString(b2);
	//头像
	auto fileName = StringUtils::format("menu/icon/icon%d.png", data.avta_index);
	if (data.avta_index < 0 || data.avta_index > 6) {
		fileName = "menu/icon/icon6.png";
	}
	__avta_num = data.avta_index;
	client->getInstance()->__avta_index = data.avta_index;
	auto newSpr = Sprite::create(fileName);
	m_Spr_Icon->setTexture(newSpr->getTexture());
	//性别
	(sex == "男") ? (m_pCB_Nan->setSelectedState(true), m_pCB_Nv->setSelectedState(false)) : (m_pCB_Nan->setSelectedState(false), m_pCB_Nv->setSelectedState(true));

	//读取个人信息返回值
	int rev = client->getRespond();
	PrintMsg(rev);
	__NotificationCenter::getInstance()->removeObserver(this, SUB_C_DATA_LOAD_SELF_NOTIFICATION);

	m_pBtn_Back->setEnabled(true);
	m_pCB_Nan->setEnabled(true);
	m_pCB_Nv->setEnabled(true);

	////注册修改信息消息
	/*__NotificationCenter::getInstance()->addObserver
	(this, CC_CALLFUNCO_SELECTOR(UserInfo::ChangeSceneToLobby), REQUEST_FINISH_NOTIFICATION, nullptr);*/
}

void UserInfo::PrintMsg(int ret)
{
	CMessageBox *box = new CMessageBox();
	string msg;
	switch (ret)
	{
	case ERROR_MODIF_PASSWD_NULL:
		msg = "密码名为空!";
		break;
	case MODIF_SUCCEED:
		msg = "修改成功!";
		break;
	case DATALOAD_FAILED:
		msg = "读取信息失败!";
		break;
	case DATALOAD_SUCCEED:
		msg = "读取信息成功!";
		break;
	}

	box->CreatCMessageBoxB(msg.c_str());
	box->setPosition(Point(160, 360));
	box->setLocalZOrder(1000);
	box->setVisible(true);
	this->addChild(box);
}

void UserInfo::ChangeSceneToLobby(Ref * ref)
{
	int ret = client->getRespond();
	PrintMsg(ret);
	auto lobbyScene = LobbyScene::createScene();
	TransitionFade* fadeScene = TransitionFade::create(1.2f, lobbyScene, Color3B::BLACK);
	Director::getInstance()->replaceScene(fadeScene);
	__NotificationCenter::getInstance()->removeObserver(this, SUB_C_DATA_LOAD_SELF_NOTIFICATION);
	__NotificationCenter::getInstance()->removeObserver(this, SUB_C_MODIF_INFO_NOTIFICATION);
}