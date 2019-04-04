#ifndef __USERINFO_H__
#define __USERINFO_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "CClient.h"

#include <iostream>

using namespace cocos2d;
using namespace cocostudio::timeline;
using namespace cocos2d::ui;
using namespace std;
#pragma execution_character_set("utf-8")

class UserInfo : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(UserInfo);

	void initView();
	//点击相应
	void onClick(Ref* ref);
	//修改个人信息
	void SendModifInfor();
	//读取用户信息
	void SendReadUserInfoReq();
	//修改头像
	void ChangeAvta(Ref* ref);
	void CreatMsgBox();
	void ReadUserInfo(Ref *ref);
	void PrintMsg(int ret);
	void ChangeSceneToLobby(Ref* ref);

public:
	Node * rootNode;
	Button * m_pBtn_Back;										//返回，提交
	Button * m_pBtn_Change;										//改变头像
	Button * m_pBtn_RankingList;								//改变头像
	Text   * m_pTxt_Name;									//名字
	TextBMFont * m_pTxt_WinNum;									//胜率
	TextField * m_pTxt_Signature;								//签名
	TextField * m_pTxt_Age;										//年龄

	TextBMFont * m_pTxt_W_TotalNum;								//W游戏总场次
	TextBMFont * m_pTxt_B_TotalNum;								//B游戏总场次

	TextBMFont * m_pTxt_W_TotalWinNum;							//W Win游戏总场次
	TextBMFont * m_pTxt_B_TotalWinNum;							//B Win游戏总场次

	CheckBox* m_pCB_Nan;
	CheckBox* m_pCB_Nv;

	Sprite* m_Spr_Icon;											//头像
	//CMessageBox *m_pN_box;

	

public:
	int	   __avta_num ;											//头像索引
	string __userName;											//名字
	string __signature;											//签名
	int    __age;												//年龄
	string __sex;												//性别
	bool   __isMan = true;										//是否是男性
	bool   __isload = false;									//是否加载msgbox
	CClient* client;
};

#endif // __USERINFO_H__
