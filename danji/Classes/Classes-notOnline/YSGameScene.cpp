#include "YSGameScene.h"
#include "Yase.h"
#include "YSSkillButton.h"
//#include "BaseMap.h"
#include "GameMenuScene.h"
#include "ShopScene.h"
#include "ScoreScene.h"

USING_NS_CC;


bool YSGameScene::init() {


	//��Ӽ��ܲ�
	auto Skills = YSSkillButton::create();
	//Skills->setZOrder(-1);
	addChild(Skills);
	//�˵�
	auto layer = GameMenuScene::create();
	layer->setTag(MENU);
	addChild(layer);

	auto shopLayer = ShopScene::create();
	shopLayer->setTag(SHOP);
	addChild(shopLayer);
	shopLayer->setTouchEnabled(false);
	shopLayer->setVisible(false);

	auto scoreLayer = ScoreScene::create();
	scoreLayer->setTag(SCORE);
	addChild(scoreLayer);
	scoreLayer->setVisible(false);
	scoreLayer->setTouchEnabled(false);

	hero = Yase::createMonster(2);
	hero->setTag(Ys);
	addChild(hero);
	this->scheduleUpdate();
	return true;
}


/*
Scene* YSGameScene::playGame()
{
	auto scene = new YSGameScene();
	if (scene && scene->initGame()) {
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return NULL;
}
*/

//�����Ϸ�������ĸ��ֲ�
/*bool YSGameScene::initGame()
{
	// �����͵�virtual bool init���� �����add�����
	//�������£�
	//auto GameMenu = GameMenuScene::create();
	//addChild(GameMenu);

	//��Ӽ��ܲ�
	//auto Skills = YSSkillButton::create();
	//Skills->setZOrder(-1);
	//addChild(Skills);

	//��ӵ�ͼ��
	//BaseMap* map = BaseMap::create();
	//addChild(map);

	//Yase * ys = Yase::create();
	//addChild(ys);
	
	return true;
}
*/

void YSGameScene::onEnter()
{
	Scene::onEnter();
	log("HelloWorld onEnter");

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode keyCode, Event* event)
	{
		keys[keyCode] = true;
	};

	listener->onKeyReleased = [=](cocos2d::EventKeyboard::KeyCode keyCode, Event* event)
	{
		keys[keyCode] = false;;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

bool YSGameScene::isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode)
{
	if (keys[keyCode])
	{
		return true;
	}
	else
	{
		return false;
	}
}

void YSGameScene::keyPressedDuration(cocos2d::EventKeyboard::KeyCode code)
{
	//int offsetX = 0, offsetY = 0;
	Vec2 playerPos = hero->getPosition();
	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		playerPos.x -= 0.5;// _tileMap->getTileSize().width;
		hero->setState(stateWalkLeft);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		playerPos.x += 0.5;// _tileMap->getTileSize().width;
		hero->setState(stateWalkRight);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		playerPos.y += 0.5;//_tileMap->getTileSize().height;
		hero->setState(stateWalkUp);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		playerPos.y -= 0.5;// _tileMap->getTileSize().height;
		hero->setState(stateWalkDown);
		break;
	default:
		break;
	}
	hero->setPosition(playerPos);
}

void YSGameScene::update(float delta)
{
	// Register an update function that checks to see if the CTRL key is pressed
		// and if it is displays how long, otherwise tell the user to press it
	Node::update(delta);
	auto leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW, rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW, upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW, downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	if (isKeyPressed(leftArrow))
	{
		keyPressedDuration(leftArrow);
	}
	else if (isKeyPressed(rightArrow))
	{
		keyPressedDuration(rightArrow);
	}
	else if (isKeyPressed(upArrow))
	{
		keyPressedDuration(upArrow);
	}
	else if (isKeyPressed(downArrow))
	{
		keyPressedDuration(downArrow);
	}
}
