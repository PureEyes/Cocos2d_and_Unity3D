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

	Node* m_pN_Hellp;													//�����ڵ�
	Button* m_pBtn_BackToGame;											//������Ϸ����

	bool voice;															//��������

public:
	//������Ϸ����
	void changeSceneToGame(Ref *ref);
	//��ȡ������Ϣ��Ϣ
	void ReadUserInfo(Ref *ref);
	//�˳����������µ�½
	void quitLobby();		
	//��ӡ��Ϣ
	void PrintfMsg(int ret);
private:
	bool isVisibleSetting = true;									//�Ƿ���ʾ��ť�б�

};

#endif // __HELLOWORLD_SCENE_H__
