#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LobbyScene.h"
#include <iostream>
#include "CClient.h"
//#include "mod/Observer.h"
//#include "mod/Observer.h"

using namespace std;

using namespace cocos2d::ui;
USING_NS_CC;
#pragma execution_character_set("utf-8")

class LoginScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(LoginScene);

	//virtual void update();

	void initView();

	void onClick(Ref* ref);

	void loginGame();

	void changeSceneToLobby(Ref* ref);

	void PrintMsg(int ret);


public:
	Node * rootNode;

	Button * m_pBtn_Login;
	Button *m_pBtn_Register;
	Button *m_pBtn_Ok;
	Button *m_pBtn_No;
	Node*	m_Node_Login;

	TextField* m_pTxt_Name;
	TextField* m_pTxt_Passwd;

	CClient* client;
	bool isLogin;
};

#endif // __HELLOWORLD_SCENE_H__
