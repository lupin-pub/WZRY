#include "DJGameScene.h"
#include "Daji.h"
#include "DJSkillButton.h"
#include "GameMenuScene.h"
#include "ShopScene.h"
#include "ScoreScene.h"
#include "BaseMap.h"
#include "cocos2d.h"

USING_NS_CC;

bool DJGameScene::init() {


	hero = Daji::createMonster(2);//?
	hero->setTag(Dj);
	addChild(hero);
	this->scheduleUpdate();

	//添加技能层
	auto Skills = DJSkillButton::create();
	//Skills->setZOrder(-1);
	addChild(Skills);
	//菜单
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

	return true;
}

void DJGameScene::onEnter()
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

Vec2 DJGameScene::tileCoordFromPosition(Vec2 pos)
{
	int x = pos.x /tiledMap->getTileSize().width;
	int y = ((tiledMap->getMapSize().height * tiledMap->getTileSize().height) - pos.y) / tiledMap->getTileSize().height;
	return Vec2(x, y);
}

void DJGameScene::setPlayerPosition(Vec2 position)
{
	scene = Director::getInstance()->getRunningScene();
	map = (BaseMap *)scene->getChildByTag(MAP);
	tiledMap = map->_tileMap;
	_collidable = tiledMap->getLayer("collidable");

	Vec2 tileCoord = tileCoordFromPosition(position);//像素点坐标->瓦片坐标
	int tileGid = _collidable->getTileGIDAt(tileCoord);//获得瓦片的GID

	if (tileGid > 0)
	{
		/*Value prop = _tileMap->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();

		std::string collision = propValueMap["Collidable"].asString();

		if (collision == "true")
		{
			return;
		}*/
		return;
	}
	//auto moveTo = MoveTo::create(0.3f, position);
	//_player->runAction(moveTo);
	hero->setPosition(position);
}

bool DJGameScene::isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode)
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

void DJGameScene::keyPressedDuration(cocos2d::EventKeyboard::KeyCode code)
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
	hero->setPlayerPosition(playerPos);
}

void DJGameScene::update(float delta)
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