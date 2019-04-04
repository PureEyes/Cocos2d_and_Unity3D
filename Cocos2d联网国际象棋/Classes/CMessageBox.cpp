#include "CMessageBox.h"
#include "def.h"
#include "CClient.h"
#include "UserInfo.h"

CMessageBox::CMessageBox()
{
}


CMessageBox::~CMessageBox()
{
}

void CMessageBox::CreatCMessageBoxA()
{
	msgBoxType = 1;
	Layout *lay = new Layout();
	lay->setBackGroundImage("menu/btn_ChoosePiece.png");
	loopNum = 6;
	for (int i = 0; i < loopNum; i++) {
		Button* btn = Button::create(StringUtils::format("menu/icon/icon%d.png", i));
			btn->setScale(0.2, 0.25);
		if (i < 3) {
			btn->setPosition(Point(-40 + i * 40, 25));
		}
		else {
			btn->setPosition(Point(-40 + (i - 3) * 40, -25));
		}
		__icon_list_.push_back(btn);
		btn->setName(StringUtils::format("btn_%d", i));
		lay->addChild(btn);
		btn->addTouchEventListener(CC_CALLBACK_2(CMessageBox::touchEvent, this));
	}
	this->addChild(lay);

	CCDirector::sharedDirector()->getScheduler()
		->scheduleSelector(schedule_selector(CMessageBox::update), this, 1.f, false);
}

void CMessageBox::CreatCMessageBoxB(string msg)
{
	msgBoxType = 2;
	Sprite* __rootNode = Sprite::create("res/menu/bar-drakgary80ah-x152.png");
	__rootNode->setScale(0.3);
	Text* __msgText = Text::create();
	__msgText->setText(msg.c_str());
	__msgText->setScale(5);
	__msgText->setPosition(Point(420, 60));
	__msgText->setOpacity(155);
	__rootNode->addChild(__msgText);
	auto callFunc = CallFunc::create([&]() {
		this->setVisible(false);
		this->release();
	});

	auto moveTo = MoveTo::create(1.1f, Point(0, 80));
	auto fadeOut = FadeOut::create(1.1f);
	auto spawn = Spawn::create(moveTo, fadeOut, nullptr);
	
	auto seq = Sequence::create(spawn, callFunc, nullptr);
	__rootNode->runAction(seq);
	this->addChild(__rootNode);
}
//滚动消息框
void CMessageBox::CreatCMessageBoxC(string msg)
{
	msgBoxType = 3;
	Sprite* __rootNode = Sprite::create("res/menu/bar-drakgary80ah-x152.png");
	__rootNode->setScale(0.4, 0.3);
	Text* __msgText = Text::create();
	__msgText->setText(msg.c_str());
	__msgText->setScale(5);
	__msgText->setPosition(Point(1000, 60));
	__msgText->setOpacity(195);
	__rootNode->addChild(__msgText);

	auto moveTo = MoveTo::create(3.0f, Point(-700, 60));
	auto callFunc = CallFunc::create([&]() {
		this->setVisible(false);
		this->autorelease();
	});
	auto seq = Sequence::create(moveTo, callFunc, nullptr);
	__msgText->runAction(seq);

	this->addChild(__rootNode);
}

void CMessageBox::CreatCMessageBoxD()
{
	msgBoxType = 4;
	Layout *layout = Layout::create();
	layout->setBackGroundImage("menu/box_tongyong.png");
	layout->setBackGroundImageScale9Enabled(true);
	layout->setAnchorPoint(Point(0.5f, 0.5f));
	layout->setContentSize(Size(260, 300));

	loopNum = 5;
	int j = 0;
	for (int i = 0; i < loopNum; i++) {
		Button* btn = Button::create(StringUtils::format("menu/ChessBord/Bord_%d.png", i));
		//(i % 2 != 0) ? (j += 1) : (j = j);
		btn->setScale(0.15f);

		if (i < 2) {
			btn->setPosition(Point(i * 120 + 70, 240));
		}
		if (i >= 2 && i < 4) {
			btn->setPosition(Point((i-2) * 120 + 70, 150));
		}
		if (i >= 4) {
			btn->setPosition(Point((i-4) * 120 + 70, 60));
		}
		layout->addChild(btn);
		__icon_list_.push_back(btn);
		btn->addTouchEventListener(CC_CALLBACK_2(CMessageBox::touchEvent, this));
	}
	this->addChild(layout);

	CCDirector::sharedDirector()->getScheduler()
		->scheduleSelector(schedule_selector(CMessageBox::update), this, 1.f, false);
}

void CMessageBox::CreatCMessageBoxE(bool isWin)
{
	msgBoxType = 5;
	Layout *layout = Layout::create();
	layout->setBackGroundImage("menu/bg8.png");
	layout->setBackGroundImageScale9Enabled(true);
	layout->setAnchorPoint(Point(0.5f, 0.5f));
	layout->setContentSize(Size(260, 130));
	Sprite* winSpr = Sprite::create("menu/text-youwin.png");
	winSpr->setPosition(Point(150, 120));
	winSpr->setScale(0.5);
	if (isWin == false) winSpr->setTexture(Sprite::create("menu/text-youlost.png")->getTexture());
	layout->addChild(winSpr);

	auto callFunc = [&](Ref* ref) {

	};

	Button* btn_back = Button::create("res/menu/button/icon-options-soundon-return.png");
	btn_back->addClickEventListener(callFunc);
	btn_back->setScale(0.6);
	btn_back->setPosition(Point(50, 40));
	layout->addChild(btn_back);
	Button* btn_rangeList = Button::create("res/menu/button/icon-options-language-x64.png");
	btn_rangeList->addClickEventListener(callFunc);
	btn_rangeList->setScale(0.6);
	btn_rangeList->setPosition(Point(100, 40));
	layout->addChild(btn_rangeList);
	this->addChild(layout);
}


void CMessageBox::update(float dt)
{
	if (isLeave == true) {
		if (msgBoxType == 1) {
			__NotificationCenter::getInstance()->postNotification(AVTA_CHANGE_NOTIFICATION);
		}
		else if (msgBoxType == 4) {
			__NotificationCenter::getInstance()->postNotification(CHESSBORD_CHANGE_NOTIFICATION);
		}
		
	}
	//log("1");
}

void CMessageBox::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:					//按钮按下的
		for (int i = 0; i < loopNum; i++) {
			Button* btn = __icon_list_.at(i);
			if (ref == btn) {
				if (msgBoxType == 1) {
					CClient::getInstance()->__avta_index = i;
				}
				else if (msgBoxType == 4) {
					CClient::getInstance()->__chessBord_Index = i;
				}
				//CClient::getInstance()->__avta_index = i;
				log("%s->%d->size%d", btn->getName().c_str(), i, __icon_list_.size());
				break;
			}
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:					//按钮结束按下的
		isLeave = true;
		this->setVisible(false);
		auto callFunc = CallFunc::create([&]() {
			this->unschedule(schedule_selector(CMessageBox::update));
			__icon_list_.clear();
			this->release();
		});
		auto seq = Sequence::create(DelayTime::create(1.0f), callFunc, nullptr);
		this->runAction(seq);
		break;
	}
}

void CMessageBox::onExit()
{
	this->unschedule(schedule_selector(CMessageBox::update));
}