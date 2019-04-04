#ifndef __RANKING_LIST_SCENE_H__
#define __RANKING_LIST_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <vector>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio::timeline;
using namespace std;

#include "cocos2d.h"

class RankingListScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(RankingListScene);

	Node* rootNode;

	ListView* m_pLayOut_RankingList;

	void ReadRankingList(Ref * ref);

	Node* CreatMaternalNode();

	Button* m_pBtn_Back;

	//void AddDataToVec(CMD_S_RankingList databuf);

	vector<int> vec_avta_index;
	vector<int> vec_winNum;
	vector<string> vec_name;
};

#endif // __RANKING_LIST_SCENE_H__
