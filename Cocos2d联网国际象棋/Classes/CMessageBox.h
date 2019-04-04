#ifndef __CMESSAGE_BOX_H__
#define __CMESSAGE_BOX_H__

#include <vector>
#include "cocos2d.h"
#include <iostream>
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;
#pragma execution_character_set("utf-8")

class CMessageBox :public Node
{
public:
	~CMessageBox();
	CMessageBox();
	//ͷ��ѡ���
	void CreatCMessageBoxA();
	void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
	//��Ϣ��ʾ��
	void CreatCMessageBoxB(string msg);
	//������Ϣ��
	void CreatCMessageBoxC(string msg);
	//����������Ϣ��
	void CreatCMessageBoxD();
	//��Ϸ�������
	void CreatCMessageBoxE(bool isWin);

	void update(float dt);

	//virtual void onEnter();
	virtual void onExit();

public:
	//inline int getIconIndex() { return icon_index; };
	vector<Button*> __icon_list_;
	bool isLeave = false;										//�Ƿ���ͷ�����
	int loopNum;
	int msgBoxType;

private:
	/*int icon_index;
	int __curr_icon_index;*/
};

#endif