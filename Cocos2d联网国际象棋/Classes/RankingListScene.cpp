#include "RankingListScene.h"
#include "CClient.h"
#include "LobbyScene.h"

Scene* RankingListScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = RankingListScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance danji
bool RankingListScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }

	rootNode = CSLoader::createNode("RankingListScene.csb");

	auto msg = CClient::getInstance()->MsgCreateReadRankingList();
	CClient::getInstance()->SendData(msg);

	m_pLayOut_RankingList = (ListView*)rootNode->getChildByName("Node_RankingList");

	m_pBtn_Back = (Button*)rootNode->getChildByName("Btn_Back");
	m_pBtn_Back->addClickEventListener([=](Ref* ref) {
		Scene* lobby = LobbyScene::createScene();
		TransitionFade* fadeScene = TransitionFade::create(1.2f, lobby, Color3B::BLACK);
		Director::getInstance()->replaceScene(fadeScene);
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_READ_RANKINGLIST_NOTIFICATION);
		this->stopAllActions();
	});

	__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RankingListScene::ReadRankingList), SUB_C_READ_RANKINGLIST_NOTIFICATION, nullptr);

	this->addChild(rootNode);

    return true;
}

void RankingListScene::ReadRankingList(Ref * ref)
{
	CMD_S_RankingList data;
	data = CClient::getInstance()->MsgReadRankingList(CClient::getInstance()->_msgQueue[0]);

	vec_avta_index.push_back(data.avta_index0);
	vec_avta_index.push_back(data.avta_index1);
	vec_avta_index.push_back(data.avta_index2);
	vec_avta_index.push_back(data.avta_index3);
	vec_avta_index.push_back(data.avta_index4);

	vec_winNum.push_back(data.winNum0);
	vec_winNum.push_back(data.winNum1);
	vec_winNum.push_back(data.winNum2);
	vec_winNum.push_back(data.winNum3);
	vec_winNum.push_back(data.winNum4);

	vec_name.push_back(data.name0);
	vec_name.push_back(data.name1);
	vec_name.push_back(data.name2);
	vec_name.push_back(data.name3);
	vec_name.push_back(data.name4);

	Sprite* topSpr = Sprite::create("res/menu/starlan.png");
	topSpr->setScale(0.43, 0.25);
	topSpr->setPosition(160, 365);


	/*Text* ranking = Text::create();
	ranking->setString("排名");
	ranking->setFontSize(18);
	ranking->setPosition(Point(60, 280));
	this->addChild(ranking);
	Text* name = Text::create();
	name->setString("名字");
	name->setFontSize(18);
	name->setPosition(Point(160, 280));
	this->addChild(name);
	Text* winNum = Text::create();
	winNum->setString("胜场");
	winNum->setFontSize(18);
	winNum->setPosition(Point(260, 280));
	this->addChild(winNum);
	this->addChild(topSpr);*/

	for (int i = 0; i < 5; i++) {
		Node* maternNode = CreatMaternalNode();
		Sprite* avtaSpr = (Sprite*)maternNode->getChildByTag(44);
		avtaSpr->setTexture(Sprite::create(StringUtils::format("res/menu/icon/icon%d.png", vec_avta_index[i]))->getTexture());
		Text* nameText = (Text*)maternNode->getChildByTag(55);
		nameText->setString(vec_name[i]);
		if (CClient::getInstance()->__userName == vec_name[i]) nameText->setTextColor(Color4B::RED);
		Text* winNum = (Text*)maternNode->getChildByTag(66);
		winNum->setString(to_string(vec_winNum[i]));
		Text* rank_text = (Text*)maternNode->getChildByTag(77);
		rank_text->setString(to_string(i + 1));
		maternNode->setPosition(Point(260, 220 - i * 55));

		if (i < 3) {
			Sprite* sprRank = Sprite::create(StringUtils::format("res/menu/starli_%d.png", i));
			sprRank->setScale(0.15f);
			sprRank->setPosition(avtaSpr->getPosition() + Point(20, 20));
			maternNode->addChild(sprRank);
		}
		m_pLayOut_RankingList->addChild(maternNode);
	}

	auto callFunc = CallFunc::create([&]() {
		__NotificationCenter::getInstance()->removeObserver(this, SUB_C_READ_RANKINGLIST_NOTIFICATION);
	});

	this->runAction(Sequence::create(DelayTime::create(2), callFunc, nullptr));
}

Node * RankingListScene::CreatMaternalNode()
{
	Node* maternalNode = Node::create();

	Sprite* celSpr = Sprite::create("res/menu/bar-drakgary80ah-x152.png");
	celSpr->setPosition(Point(-130, 70));
	celSpr->setScale(0.32f, 0.40f);
	maternalNode->addChild(celSpr);
	Sprite* avtaSpr = Sprite::create("res/menu/icon/icon6.png");
	avtaSpr->setTag(44);
	avtaSpr->setScale(0.25f);
	avtaSpr->setPosition(Point(-235, 70));
	maternalNode->addChild(avtaSpr);

	Text* ranking = Text::create();
	ranking->setString("排名");
	ranking->setFontSize(18);
	ranking->setPosition(Point(-180, 80));
	maternalNode->addChild(ranking);
	Text* name = Text::create();
	name->setString("名字");
	name->setFontSize(18);
	name->setPosition(Point(-110, 80));
	maternalNode->addChild(name);
	Text* winNum = Text::create();
	winNum->setString("胜场");
	winNum->setFontSize(18);
	winNum->setPosition(Point(-30, 80));
	maternalNode->addChild(winNum);

	
	Text* name_text = Text::create();
	name_text->setString("mingzi");
	name_text->setFontSize(18);
	name_text->setPosition(Point(-110, 60));
	name_text->setTag(55);
	maternalNode->addChild(name_text);
	Text* winNum_text = Text::create();
	winNum_text->setPosition(Point(-30, 60));
	winNum_text->setTag(66);
	winNum_text->setString("winNum");
	winNum_text->setFontSize(18);
	maternalNode->addChild(winNum_text);
	Text* rank_text = Text::create();
	rank_text->setString("rank");
	rank_text->setFontSize(18);
	rank_text->setPosition(Point(-180, 60));
	rank_text->setTag(77);
	maternalNode->addChild(rank_text);

	maternalNode->setPosition(Point(260, 240));
	return maternalNode;
}