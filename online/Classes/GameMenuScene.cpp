#include "ShopScene.h"
#include "ScoreScene.h"
#include "GameMenuScene.h"
#include "BaseMonster.h"
#include "DJGameScene.h"
#include "HYGameScene.h"
#include "YSGameScene.h"
#include "Yase.h"
USING_NS_CC;


bool GameMenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}


	// �ı���ʾ��
	//addChild(createText());

	//�õ�Ӣ������ String name =  GameMenuScene::getHero();
	//setHero();

	createCloseButton();
	createItem();

	return true;
}

void GameMenuScene::createItem() {

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//��ֹ����������ť�������
	ShopItem = MenuItemImage::create(
		"GameScene/Money.jpg",
		"GameScene/Money.jpg",
		CC_CALLBACK_1(GameMenuScene::menuShopCallback, this));
	ShopItem->setScale(0.2);
	ShopItem->setPosition(Director::getInstance()->convertToGL(Vec2(50, visibleSize.height / 4 + 20)));

	ScoreItem = MenuItemImage::create(
		"GameScene/Score.png",
		"GameScene/Score.png",
		CC_CALLBACK_1(GameMenuScene::menuScoreCallback, this));
	ScoreItem->setScale(0.4);
	ScoreItem->setPosition(Director::getInstance()->convertToGL(Vec2(60, visibleSize.height / 4 - 60)));

	HomeItem = MenuItemImage::create(
		"GameScene/Home.png",
		"GameScene/Home.png",
		CC_CALLBACK_1(GameMenuScene::menuHomeCallback, this));
	HomeItem->setScale(0.35);
	HomeItem->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 3 - 200), visibleSize.height - 60)));

	auto menu = Menu::create(ShopItem, ScoreItem, HomeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}



void GameMenuScene::createCloseButton()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto closeButton = ui::Button::create("GameScene/Close.jpg", "GameScene/Close.jpg");
	// ���Ͻ�
	closeButton->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width - 50, 20)));
	closeButton->setScale(0.2);
	closeButton->setOpacity(233);

	closeButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
		if (type != ui::Widget::TouchEventType::ENDED) return;
		Director::getInstance()->end();
	});
	this->addChild(closeButton);
}


void GameMenuScene::menuScoreCallback(cocos2d::Ref* pSender) {
	//Director::getInstance()->pushScene(TransitionFade::create(1, ScoreScene::create()));
	auto scene = Director::getInstance()->getRunningScene();
	ScoreScene * ScoreLayer = (ScoreScene *)scene->getChildByTag(SCORE);
	ScoreLayer->setVisible(true);
	ScoreLayer->setTouchEnabled(true);
	ShopItem->setEnabled(false);
	HomeItem->setEnabled(false);
}

void GameMenuScene::menuShopCallback(cocos2d::Ref* pSender) {
	//Director::getInstance()->pushScene(TransitionFade::create(1, ShopScene::create()));
	auto scene = Director::getInstance()->getRunningScene();
	ShopScene * ShopLayer = (ShopScene *)scene->getChildByTag(SHOP);
	ShopLayer->setVisible(true);
	ShopLayer->setTouchEnabled(true);
	ScoreItem->setEnabled(false);
	HomeItem->setEnabled(false);
}

void GameMenuScene::menuHomeCallback(cocos2d::Ref* pSender) {

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto spProHome = Sprite::create("GameScene/stencil.png");
	spProHome->setScale(0.6);//
	progressTimerHome = ProgressTimer::create(spProHome);
	progressTimerHome->setScale(0.6);//
	progressTimerHome->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 3 - 205), visibleSize.height - 70)));
	progressTimerHome->setVisible(false);
	this->addChild(progressTimerHome, 100);

	//���ð�ť������
	//HomeItem->setVisible(true);
	HomeItem->setEnabled(false);

	// ���þ��������Ϊ˳ʱ��
	progressTimerHome->setVisible(true);
	progressTimerHome->setType(ProgressTimer::Type::RADIAL);

	//�ý�����ִ��һ������ ��ʾ������Ч�� �𽥸��ǰ�͸ģ���γ���ȴЧ��;������м�ʱ��ȴ������ʵ�ֺ�ʱ�����
	auto pto = ProgressTo::create(5, 100);
	auto call = CallFuncN::create(this, callfuncN_selector(GameMenuScene::HomeCoolDownCallBack));
	progressTimerHome->runAction(Sequence::create(pto, call, NULL));
}

void GameMenuScene::HomeCoolDownCallBack(Node* node)
{
	Scene* scene = Director::getInstance()->getRunningScene();
	int tag = scene->getTag();

	//sprite1->setVisible(false);
	//�ָ�����
	HomeItem->setEnabled(true);
	//����
	progressTimerHome->setVisible(false);

	Daji * Dhero = NULL;
	Houyi * Hhero = NULL;
	Yase * Yhero = NULL;
	if (tag == Dj) {
		Dhero = ((DJGameScene *)scene)->hero;
		Dhero->Home();
	}
	else if (tag == Hy) {
		Hhero = ((HYGameScene *)scene)->hero;
		Hhero->Home();
	}
	else if (tag == Ys) {
		Yhero = ((YSGameScene *)scene)->hero;
		Yhero->Home();
	}
}




