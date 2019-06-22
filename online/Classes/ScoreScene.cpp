#include "ScoreScene.h"
#include "GameMenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameMenuScene.h"
#include "GameManager.h"
#include "DJGameScene.h"
#include "YSGameScene.h"
#include "HYGameScene.h"
USING_NS_CC;

Scene* ScoreScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ScoreScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ScoreScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
	/*
	Sprite *bg = Sprite::create("GameScene/Halfvisible.png");
	bg->setScale(visibleSize.height / bg->getContentSize().height);
	bg->setPosition(Vec2(origin.x + visibleSize.width/ 2,
		origin.y + visibleSize.height / 2));
	this->addChild(bg);
	*/
	

    auto menu = Menu::create( );
    menu->setPosition(Vec2::ZERO);

	auto lableTitle = Label::createWithSystemFont("The Scores of your hero", "Arial", 30);
	lableTitle->setPosition(Director::getInstance()->convertToGL(Vec2(480, 82)));
	addChild(lableTitle);
    //顶上位置
    
	auto instance = GameManager::getInstance();
	/*
	String* killsHERO = String::createWithFormat("Number of killing heros:%d", instance -> HEROKILLS);
	auto lableKillHERO = Label::createWithSystemFont(killsHERO->getCString(), "Arial", 30);
	lableKillHERO->setPosition(Director::getInstance()->convertToGL(Vec2(480, 152)));
	addChild(lableKillHERO);
	*/
	String* kills = String::createWithFormat("Number of kills:%d", instance->KILLS);
	auto lableKills = Label::createWithSystemFont(kills->getCString(), "Arial", 30);
	lableKills->setPosition(Director::getInstance()->convertToGL(Vec2(480, 232)));
	addChild(lableKills);

	String* killed = String::createWithFormat("Killed times:%d", instance->KILLED);
	auto lableKilled = Label::createWithSystemFont(killed->getCString(), "Arial", 30);
	lableKilled->setPosition(Director::getInstance()->convertToGL(Vec2(480, 302)));
	addChild(lableKilled);

	//等级
	String* level = String::createWithFormat("The level of the hero is:%d", instance->LEVEL);
	auto lableLevel = Label::createWithSystemFont(level->getCString(), "Arial", 30);
	lableLevel->setPosition(Director::getInstance()->convertToGL(Vec2(480,372)));
	addChild(lableLevel);

 
	auto labelBack = Label::createWithSystemFont("Back", "Arial", 30);

    auto BackmenuItem = MenuItemLabel::create(labelBack ,CC_CALLBACK_1(ScoreScene::menuBackItemCallback,this));
    //最下面位置
	BackmenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(480, 470)));
    menu -> addChild(BackmenuItem);
    this->addChild(menu);
     
    return true;
}


void ScoreScene::menuBackItemCallback(cocos2d::Ref * ref){
   // Director::getInstance() -> popScene();
	auto scene = Director::getInstance()->getRunningScene();
	ScoreScene * ScoreLayer = (ScoreScene *)scene->getChildByTag(SCORE);
	ScoreLayer->setVisible(false);//?
	ScoreLayer->setTouchEnabled(false);
	GameMenuScene * MenuLayer = (GameMenuScene *)scene->getChildByTag(MENU);
	MenuLayer->getShopItem()->setEnabled(true);
	MenuLayer->getHomeItem()->setEnabled(true);
}
























