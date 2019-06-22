#include "GlobalManager.h"
#include "BaseMonster.h"
#include "GameManager.h"
#include "BaseMap.h"
#include "Houyi.h"

USING_NS_CC;

Houyi* Houyi::createMonster(int color)//std::vector<Point> points, std::vector<std::vector<std::vector<Point>>> sonpath)
{
	auto _monster = new Houyi();
	//auto monster = new Houyi();
	if (_monster && _monster->init())
	{
		_monster->setMonsterColor(color);
		if (_monster->getMonsterColor() == 2)
		{
			_monster->setPosition(200, 130);
			GameManager::getInstance()->monsterBlueVector.pushBack(_monster);
		}
		else
		{
			_monster->setPosition(780, 480);
			GameManager::getInstance()->monsterRedVector.pushBack(_monster);
		}
		//_monster->setPosition(point);
		_monster->setMonsterType(HOUYI);
		//monster->setPointsVector(points);
		_monster->setMaxHp(3182);
		_monster->setCurrHp(3182);
		_monster->setMoney(200);//打死一个英雄加多少钱？
		//monster->sonPath = sonpath;
		_monster->setForce(180);//攻击值
		_monster->setArmor(66);//防御值
		_monster->setScope(55);//远程
		//monster->setAttackByTower(true);
		//monster->setAttackBySoldier(true);//唔
		_monster->setRunSpeed(20);
		//_monster->runNextPoint();
		_monster->autorelease();
		return _monster;
	}
	CC_SAFE_DELETE(_monster);
	return NULL;
}


void Houyi::removeBullet()//攻击！  保证打中
{
	//bool isMissed = true;
	auto instance = GameManager::getInstance();

	if (currMinMonster <= currMinTower) //改currMinTower和towerDistance
	{
		auto currHp = nearestMonster->getCurrHp();
		auto currExperience = GameManager::getInstance()->EXPERIENCE;
		currHp = currHp - this->getForce() + nearestMonster->getArmor();//受到攻击Hp减少
		if (getMonsterColor() == 2)
		{
			currExperience = currExperience + 5;
		}

		if (currHp <= 0)
		{
			currHp = 0;
		}
		nearestMonster->setCurrHp(currHp);

		nearestMonster->getHpBar()->setPercentage((currHp / nearestMonster->getMaxHp()) * 100);//更新Hp

		if (currHp == 0)
		{
			nearestMonster->death();//消失啊 vector移除啊
			if (getMonsterColor() == 2)
			{
				GameManager::getInstance()->MONEY = GameManager::getInstance()->MONEY + nearestMonster->getMoney();
				currExperience = currExperience + nearestMonster->getExperience();
				if (nearestMonster->getMonsterType() == DAJI || nearestMonster->getMonsterType() == YASE || nearestMonster->getMonsterType() == HOUYI)
				{
					GameManager::getInstance()->KILLS = GameManager::getInstance()->KILLS + 1;
				}
			}
			else if (getMonsterColor() == 1)
			{
				if (nearestMonster->getMonsterType() == DAJI || nearestMonster->getMonsterType() == YASE || nearestMonster->getMonsterType() == HOUYI)
				{
					GameManager::getInstance()->KILLED = GameManager::getInstance()->KILLED + 1;
				}
			}

		}
		if (currExperience > 350)
		{
			GameManager::getInstance()->LEVEL = GameManager::getInstance()->LEVEL + 1;
			GameManager::getInstance()->EXPERIENCE = currExperience - 350;
		}
		//spriteBullet->removeFromParent();
	}
	else if (currMinTower < currMinMonster)   //改currMinTower和towerDistance
	{
		auto currHp = nearestTower->getCurrHp();
		auto currExperience = GameManager::getInstance()->EXPERIENCE;

		currHp = currHp - this->getForce() + nearestTower->getArmor();//受到攻击Hp减少
		if (getMonsterColor() == 2)
		{
			currExperience = currExperience + 5;
		}

		if (currHp <= 0)
		{
			currHp = 0;
		}
		nearestTower->setCurrHp(currHp);

		nearestTower->getHpBar()->setPercentage((currHp / nearestTower->getMaxHp()) * 100);//更新Hp//改nearestMonster和nearestTower

		if (currHp == 0)
		{
			nearestTower->death();//消失啊 vector移除啊
			if (getMonsterColor() == 2)
			{
				GameManager::getInstance()->MONEY = GameManager::getInstance()->MONEY + nearestTower->getMoney();
				currExperience = currExperience + nearestTower->getExperience();
			}
		}
		if (currExperience > 350)
		{
			GameManager::getInstance()->LEVEL = GameManager::getInstance()->LEVEL + 1;
			GameManager::getInstance()->EXPERIENCE = currExperience - 350;
		}
		//spriteBullet->removeFromParent();
	}

}

void Houyi::shoot()//必须点攻击才攻击 everytime
{
	auto instance = GameManager::getInstance();//
	checkNearestMonster();
	checkNearestTower();
	//char temp1[20];
	//sprintf(temp1,"")
	if (currMinMonster <= towerDistance && nearestMonster != NULL/*&& nearestMonster->getCurrHp() > 0*/)
	{
		spriteBullet = Sprite::create("Bullet.png");
		float _scale = (TileCoord / 2) / spriteBullet->getContentSize().height;
		spriteBullet->setScale(0.04);
		addChild(spriteBullet);
		//auto currBullet = ArrowTowerBullet();//生成弓箭
		Point shootVector = nearestMonster->getPosition() - this->getPosition();//两点间 留下了geParent
		//auto pVectr = currBullet->getPosition() - shootVector;//不知道干嘛
		auto moveByAction = MoveBy::create(0.5f, shootVector);//我觉得是moveby啊
		
		spriteBullet->runAction(Sequence::create(moveByAction, FadeOut::create(1 / 30), NULL));//, CallFuncN::create(CC_CALLBACK_0(BaseTower::removeBullet, this)), NULL));//BulletAction沿直线飞行 removeBullet判断击中并子弹消失
		Houyi::removeBullet();
	}
	else if (currMinTower < currMinMonster&&nearestTower != NULL)
	{
		spriteBullet = Sprite::create("Bullet.png");
		float _scale = (TileCoord / 2) / spriteBullet->getContentSize().height;
		spriteBullet->setScale(0.04);
		addChild(spriteBullet);
		//auto currBullet = ArrowTowerBullet();//生成弓箭
		Point shootVector = nearestTower->getPosition() - this->getPosition();//两点间 留下了geParent
		//auto pVectr = currBullet->getPosition() - shootVector;//不知道干嘛
		auto moveByAction = MoveBy::create(0.5f, shootVector);//我觉得是moveby啊
		//currBullet->runAction(moveToAction);
		//currBullet->shoot;//removeBullet!!
		//currBullet->setPosition();//塔顶上
		//currBullet = NULL;
		
		spriteBullet->runAction(Sequence::create(moveByAction, FadeOut::create(1 / 30), NULL));//, CallFuncN::create(CC_CALLBACK_0(BaseTower::removeBullet, this)), NULL));//BulletAction沿直线飞行 removeBullet判断击中并子弹消失
		Houyi::removeBullet();
	}
}

bool Houyi::init()
{
	if (!BaseMonster::init())
	{
		return false;
	}
	//setMonsterType(BOSS_CANIBAL);
	//setName("Boss_Canibal_");
	BaseMonster::onEnter();
	baseSprite = Sprite::create("HYup.png");
	float theScale = 2 * TileCoord / baseSprite->getContentSize().height;
	baseSprite->setScale(0.5);
	//baseSprite->setPosition(Vec2(0, 240));
	addChild(baseSprite);

	createAndSetHpBar();
	lastState = stateNone;
	schedule(schedule_selector(BaseMonster::hpSchedule), 1.0f);
	

	//scheduleUpdate();
	//schedule(schedule_selector(Boss_Canibal::beforeSkill), 50.0f, 2, 20.0f);
	//setListener();
	//skillTag = 0;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_tileMap = TMXTiledMap::create("map/MyMap2.tmx");//随手加了一个auto程序就崩了啊 真的千万别乱改 请一五一十地抄代码！！！
	_tileMap->setAnchorPoint(Point(0.5, 0.5));
	_tileMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	auto size = _tileMap->getContentSize();
	float scaleX = visibleSize.width / size.width;
	float scaleY = visibleSize.height / size.height;
	float scale = scaleX > scaleY ? scaleX : scaleY;
	_tileMap->setScale(scale);
	_tileMap->setVisible(false);

	_collidable = _tileMap->getLayer("collidable");
	addChild(_tileMap);
	this->scheduleUpdate();//for键盘

	return true;
}

void Houyi::onEnter()
{
	//Layer::onEnter();
	//log("HelloWorld onEnter");

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

cocos2d::Vec2 Houyi::tileCoordFromPosition(cocos2d::Vec2 pos)
{
	int x = pos.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y) / _tileMap->getTileSize().height;
	return cocos2d::Vec2(x, y);
}

void Houyi::setPlayerPosition(Vec2 position)
{
	Vec2 tileCoord = this->tileCoordFromPosition(position);//像素点坐标->瓦片坐标
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
	this->setPosition(position);
}

bool Houyi::isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode)
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

void Houyi::keyPressedDuration(cocos2d::EventKeyboard::KeyCode code)
{
	//int offsetX = 0, offsetY = 0;
	Vec2 playerPos = this->getPosition();
	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		playerPos.x -= 0.5;// _tileMap->getTileSize().width;
		this->setState(stateWalkLeft);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		playerPos.x += 0.5;// _tileMap->getTileSize().width;
		this->setState(stateWalkRight);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		playerPos.y += 0.5;//_tileMap->getTileSize().height;
		this->setState(stateWalkUp);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		playerPos.y -= 0.5;// _tileMap->getTileSize().height;
		this->setState(stateWalkDown);
		break;
	default:
		break;
	}
	this->setPlayerPosition(playerPos);
}

void Houyi::update(float delta)
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

void Houyi::death()
{

	/*setState(stateDeath);
	hpBgSprite->setVisible(false);

	this->stopAllActions();
	unscheduleUpdate();

	this->runAction(Sequence::create(FadeOut::create(1.0f)
		, CallFuncN::create(CC_CALLBACK_0(BaseMonster::setVisible, this, false))
		, NULL));
	this->scheduleOnce(schedule_selector(BaseMonster::relive), 15.0f);//15秒后复活*/
	relive();

}