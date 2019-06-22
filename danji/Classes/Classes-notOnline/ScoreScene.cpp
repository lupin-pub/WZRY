#include "ScoreScene.h"
#include "GameMenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameMenuScene.h"
#include "GameManager.h"
#include "DJGameScene.h"
#include "YSGameScene.h"
#include "HYGameScene.h"
#include "GameManager.h"
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

void ScoreScene::onEnter() {
	Layer::onEnter();
	auto size = Director::getInstance()->getWinSize();
	auto textKills = cocos2d::ui::Text::create();
	auto textKilled = cocos2d::ui::Text::create();
	auto textLevel = cocos2d::ui::Text::create();
	auto textMoney = cocos2d::ui::Text::create();
	textKills->setText("0");
	textKilled->setText("0");
	textLevel->setText("0");
	textMoney->setText("0");
	textKills->setFontSize(20);
	textKilled->setFontSize(20);
	textLevel->setFontSize(20);
	textMoney->setFontSize(20);
	textKills->setTag(300);
	textKilled->setTag(301);
	textLevel ->setTag(302);
	textMoney->setTag(303);
	textMoney->setPosition(Director::getInstance()->convertToGL(Vec2(480, 160)));
	textKills->setPosition(Director::getInstance()->convertToGL(Vec2(480, 232)));
	textKilled->setPosition(Director::getInstance()->convertToGL(Vec2(480, 302)));
	textLevel->setPosition(Director::getInstance()->convertToGL(Vec2(480, 372)));
	addChild(textMoney);
	addChild(textKills);
	addChild(textKilled);
	addChild(textLevel);
	schedule(schedule_selector(ScoreScene::schedulerCallback1),1);
}
void ScoreScene::schedulerCallback1(float dt) {
	auto textKills = dynamic_cast<cocos2d::ui::Text*>(getChildByTag(300));
	//int value = atoi(textKills->getStringValue().c_str());
	int kills = GameManager::getInstance()->KILLS;
	textKills->setText(std::to_string(kills));

	auto textKilled = dynamic_cast<cocos2d::ui::Text*>(getChildByTag(301));
	//int value = atoi(textKills->getStringValue().c_str());
	int killed = GameManager::getInstance()->KILLED;
	textKilled->setText(std::to_string(killed));

	auto textLevel = dynamic_cast<cocos2d::ui::Text*>(getChildByTag(302));
	//int value = atoi(textKills->getStringValue().c_str());
	int level = GameManager::getInstance() ->LEVEL;
	textLevel->setText(std::to_string(level));

	auto textMoney = dynamic_cast<cocos2d::ui::Text*>(getChildByTag(303));
	//int value = atoi(textKills->getStringValue().c_str());
	float money = GameManager::getInstance()->MONEY;
	textMoney->setText(std::to_string(money));
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
	
	auto lableKills = Label::createWithSystemFont("Number of kills:", "Arial", 30);
	lableKills->setPosition(Director::getInstance()->convertToGL(Vec2(480, 190)));
	addChild(lableKills);

	auto lableKilled = Label::createWithSystemFont("Killed times:", "Arial", 30);
	lableKilled->setPosition(Director::getInstance()->convertToGL(Vec2(480, 260)));
	addChild(lableKilled);

	auto lableLevel = Label::createWithSystemFont("The level of the hero is:", "Arial", 30);
	lableLevel->setPosition(Director::getInstance()->convertToGL(Vec2(480,330)));
	addChild(lableLevel);

	auto lableMoney = Label::createWithSystemFont("Money:", "Arial", 30);
	lableMoney->setPosition(Director::getInstance()->convertToGL(Vec2(480, 130)));
	addChild(lableMoney);
 
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
























