#ifndef __LOBBY_SCENE_H__
#define __LOBBY_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LoginScene.h"
#include "UserInfo.h"
#include "CClient.h"


USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio::timeline;
#pragma execution_character_set("utf-8")

class LobbyScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(LobbyScene);

	void initView();

	void onClick(Ref* ref);
	
public:
	Node * rootNode;
	Node * m_pN_BtnList;
	Button* m_pBtn_CreatRoom;
	Button* m_pBtn_StartMatch;
	Button* m_pBtn_Setting;

	Node * m_pN_Setting;
	Button* m_pBtn_Rule;
	Button* m_pBtn_Audio;
	Button* m_pBtn_Quit;

	Node* m_pN_Top;
	Button* m_pBtn_Icon;
	TextBMFont* m_pUserName;

	CClient* client;

	Node* m_pN_Hellp;													//帮助节点
	Button* m_pBtn_BackToGame;											//返回游戏场景

	bool voice;															//声音设置

public:
	//进入游戏场景
	void changeSceneToGame(Ref *ref);
	//读取个人信息消息
	void ReadUserInfo(Ref *ref);
	//退出大厅，重新登陆
	void quitLobby();		
	//打印消息
	void PrintfMsg(int ret);
private:
	bool isVisibleSetting = true;									//是否显示按钮列表

};

#endif // __HELLOWORLD_SCENE_H__
