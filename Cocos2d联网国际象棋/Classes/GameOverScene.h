#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio::timeline;

#pragma execution_character_set("utf-8")

class GameOverScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);

	void onClick(Ref * ref);

	void ReadUserInfo(Ref* ref);
public:
	Node * rootNode;

	Sprite* m_pSpr_ShowWin;										//

	TextBMFont * m_pTxt_W_TotalNum;								//W��Ϸ�ܳ���
	TextBMFont * m_pTxt_B_TotalNum;								//B��Ϸ�ܳ���
	TextBMFont * m_pTxt_W_TotalWinNum;							//W Win��Ϸ�ܳ���
	TextBMFont * m_pTxt_B_TotalWinNum;							//B Win��Ϸ�ܳ���

	Button*  m_pBtn_RankingList;								//���а�
	Button*  m_pBtn_Back;										//����
};

#endif // __GAMEOVER_SCENE_H__
