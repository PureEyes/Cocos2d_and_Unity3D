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
	//�����Ӧ
	void onClick(Ref* ref);
	//�޸ĸ�����Ϣ
	void SendModifInfor();
	//��ȡ�û���Ϣ
	void SendReadUserInfoReq();
	//�޸�ͷ��
	void ChangeAvta(Ref* ref);
	void CreatMsgBox();
	void ReadUserInfo(Ref *ref);
	void PrintMsg(int ret);
	void ChangeSceneToLobby(Ref* ref);

public:
	Node * rootNode;
	Button * m_pBtn_Back;										//���أ��ύ
	Button * m_pBtn_Change;										//�ı�ͷ��
	Button * m_pBtn_RankingList;								//�ı�ͷ��
	Text   * m_pTxt_Name;									//����
	TextBMFont * m_pTxt_WinNum;									//ʤ��
	TextField * m_pTxt_Signature;								//ǩ��
	TextField * m_pTxt_Age;										//����

	TextBMFont * m_pTxt_W_TotalNum;								//W��Ϸ�ܳ���
	TextBMFont * m_pTxt_B_TotalNum;								//B��Ϸ�ܳ���

	TextBMFont * m_pTxt_W_TotalWinNum;							//W Win��Ϸ�ܳ���
	TextBMFont * m_pTxt_B_TotalWinNum;							//B Win��Ϸ�ܳ���

	CheckBox* m_pCB_Nan;
	CheckBox* m_pCB_Nv;

	Sprite* m_Spr_Icon;											//ͷ��
	//CMessageBox *m_pN_box;

	

public:
	int	   __avta_num ;											//ͷ������
	string __userName;											//����
	string __signature;											//ǩ��
	int    __age;												//����
	string __sex;												//�Ա�
	bool   __isMan = true;										//�Ƿ�������
	bool   __isload = false;									//�Ƿ����msgbox
	CClient* client;
};

#endif // __USERINFO_H__
