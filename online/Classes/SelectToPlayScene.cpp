#include "GameMenuScene.h"
#include "StartScene.h"
#include "SelectToPlayScene.h" 
#include "SimpleAudioEngine.h"
#include "BaseMap.h"
#include "DJGameScene.h"
#include "HYGameScene.h"
#include "YSGameScene.h"
#include "webclient.h"
#include "GameManager.h"
USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocostudio;
using namespace cocos2d::extension;

/*to do */
/*the case of the players*/
/*and StartPaly*/

MyClient client;

Scene* SelectToPlayScene::createScene()
{
	//scene is an auto release object 
	auto scene = Scene::create();

	//layer is an auto release object
	auto layer = SelectToPlayScene::create();

	//add layer as a child to scene
	scene->addChild(layer);

	//return the scene
	return scene;
}

bool SelectToPlayScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto size = Director::getInstance()->getWinSize();

	auto  visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto bgImage = Sprite::create("StartMenu/bgSelect.jpg");
	bgImage->setScale(1); //1.25
	//bgImage->setLocalZOrder(-1);
	bgImage->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(bgImage);

	auto text = Text::create();
	text->setText("Select a Hero to play the game!");
	text->setFontName("Marker Felt");
	text->setFontSize(38);
	//text -> setColor(Color3B(255, 168, 176));
	text->setPosition(Vec2(400, 420));
	addChild(text);


	auto DJbutton = Button::create("StartMenu/DJ-normal.png", "StartMenu/DJ-selected.png", "StartMenu/DJ-selected.png");
	DJbutton->setTouchEnabled(true);
	DJbutton->setScale9Enabled(true);
	//DJbutton->setTitleText("DJ");
	//DJbutton->setPosition(Point(size.width / 2, size.height / 2 - 140));
	DJbutton->setPosition(Vec2(380, 310));
	DJbutton->setTag(Dj);
	DJbutton->addTouchEventListener(this, toucheventselector(SelectToPlayScene::touchDJEvent));

	addChild(DJbutton);



	auto HYbutton = Button::create();
	HYbutton->setTouchEnabled(true);
	HYbutton->setScale9Enabled(true);
	HYbutton->loadTextures("StartMenu/HY-normal.png", "StartMenu/HY-selected.png", "StartMenu/HY-selected.png");
	HYbutton->setTag(Hy);
	HYbutton->setPosition(Vec2(280, 310));
	//HYbutton -> setPosition

	//HYbutton -> setScale(1);

	HYbutton->addTouchEventListener(this, toucheventselector(SelectToPlayScene::touchHYEvent));
	addChild(HYbutton);



	auto YSbutton = Button::create();
	YSbutton->setTouchEnabled(true);
	YSbutton->setScale9Enabled(true);
	YSbutton->loadTextures("StartMenu/YS-normal.png", "StartMenu/YS-selected.png", "StartMenu/YS-selected.png");
	YSbutton->setTag(Ys);
	YSbutton->addTouchEventListener(this, toucheventselector(SelectToPlayScene::touchYSEvent));
	YSbutton->setPosition(Vec2(480, 310));
	this->addChild(YSbutton);;


	return true;
}


void SelectToPlayScene::touchDJEvent(Ref * pSender, cocos2d::ui::Widget::TouchEventType type) {


	if (type == Widget::TouchEventType::ENDED) {
		int tag = ((Button*)pSender)->getTag();
	
		auto scene = DJGameScene::create();
		scene->setTag(Dj);
		//auto scene = GameScene::create();
		//scene->setTag(tag);
		//scene->select();
		client.init();
		while (true)
		{
			if (client.GameConnect())
				break;
		}
		//auto scene = GameScene::create();
		//scene->setTag(tag);
		//scene->select();
		client.MyHeroSelect(2);//
		auto gm = GameManager::getInstance();
		gm->MyType = 2;
		while (true)
		{
			client.EnemyHeroSelect();
			if (gm->EnemyType != 0)
			{
				break;
			}
		}
		if (gm->EnemyType == 1)
		{
			Houyi * enemy = Houyi::createMonster(1);
			enemy->setName("enemy");
			enemy->setZOrder(200);
			scene->addChild(enemy);
		}
		if (gm->EnemyType == 2)
		{
			Daji * enemy = Daji::createMonster(1);
			enemy->setName("enemy");
			enemy->setZOrder(200);
			scene->addChild(enemy);
		}
		if (gm->EnemyType == 3)
		{
			Yase * enemy = Yase::createMonster(1);
			enemy->setName("enemy");
			enemy->setZOrder(200);
			scene->addChild(enemy);
		}
	   //添加地图 塔 和小兵
		auto  map = BaseMap::createBaseMap();
		scene->addChild(map);
		map->setTag(MAP);
		map->setZOrder(-1);
		auto reScene = TransitionSlideInR::create(1.0f, scene);
		Director::getInstance()->replaceScene(reScene);
		this->schedule(schedule_selector(DJGameScene::callweb));
	}

	/*
	else if (type == Widget::TouchEventType::ENDED){
		HYbuttonv->setVisible(false);
		YSbuttonv->setVisible(false);
		DJbuttonv->setVisible(false);
	}
	*/
}

void SelectToPlayScene::touchHYEvent(Ref * pSender, cocos2d::ui::Widget::TouchEventType type) {


	if (type == Widget::TouchEventType::ENDED) {
		int tag = ((Button*)pSender)->getTag();

		auto scene = HYGameScene::create();
		//scene->setTag(tag);
		//scene->select();
		scene->setTag(Hy);
		client.init();
		while (true)
		{
			if (client.GameConnect())
				break;
		}
		client.MyHeroSelect(1);/////////
		auto gm = GameManager::getInstance();
		gm->MyType = 1;
		while (true)
		{
			client.EnemyHeroSelect();
			if (gm->EnemyType != 0)
			{
				break;
			}
		}
		if (gm->EnemyType == 1)
		{
			Houyi * hero = Houyi::createMonster(1);
			hero->setName("enemy");
			addChild(hero);
		}
		if (gm->EnemyType == 2)
		{
			Daji * hero = Daji::createMonster(1);
			hero->setName("enemy");
			addChild(hero);
		}
		if (gm->EnemyType == 3)
		{
			Yase * hero = Yase::createMonster(1);
			hero->setName("enemy");
			addChild(hero);
		}
		//添加地图 塔 和小兵
		auto  map = BaseMap::createBaseMap();
		scene->addChild(map);
		map->setTag(MAP);
		map->setZOrder(-1);
	
		auto reScene = TransitionSlideInR::create(1.0f, scene);
		Director::getInstance()->replaceScene(reScene);
		this->schedule(schedule_selector(HYGameScene::callweb));
	}
}

void SelectToPlayScene::touchYSEvent(Ref * pSender, cocos2d::ui::Widget::TouchEventType type) {

	//auto visibleSize = Director::getInstance()->getVisibleSize();

	if (type == Widget::TouchEventType::ENDED) {
		int tag = ((Button*)pSender)->getTag();

		auto scene = YSGameScene::create();
		//auto scene = GameScene::create();
		//scene->setTag(tag);
		scene->setTag(Ys);
		client.init();
		while (true)
		{
			if (client.GameConnect())
				break;
		}
		client.MyHeroSelect(3);/////////
		auto gm = GameManager::getInstance();
		gm->MyType = 3;
		while (true)
		{
			client.EnemyHeroSelect();
			if (gm->EnemyType != 0)
			{
				break;
			}
		}
		if (gm->EnemyType == 1)
		{
			Houyi * hero = Houyi::createMonster(1);
			hero->setName("enemy");
			addChild(hero);
		}
		if (gm->EnemyType == 2)
		{
			Daji * hero = Daji::createMonster(1);
			hero->setName("enemy");
			addChild(hero);
		}
		if (gm->EnemyType == 3)
		{
			Yase * hero = Yase::createMonster(1);
			hero->setName("enemy");
			addChild(hero);
		}
		//添加地图 塔 和小兵
		auto  map = BaseMap::createBaseMap();
		scene->addChild(map);
		map->setTag(MAP);
		map->setZOrder(-1);

		auto reScene = TransitionSlideInR::create(1.0f, scene);
		Director::getInstance()->replaceScene(reScene);
		this->schedule(schedule_selector(YSGameScene::callweb));
	}

	/*
	else if (type == Widget::TouchEventType::ENDED){
		HYbuttonv->setVisible(false);
		YSbuttonv->setVisible(false);
		DJbuttonv->setVisible(false);
	}
	*/
}














