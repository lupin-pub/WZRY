#include "GlobalManager.h"
#include "BaseMonster.h"
#include "GameManager.h"
#include "BaseMap.h"
#include "Daji.h"
//#include "GlobalManager.h"

USING_NS_CC;

Daji* Daji::createMonster(int color)//Point point)// std::vector<Point> points, std::vector<std::vector<std::vector<Point>>> sonpath)
{
	auto _monster = new Daji();
	if (_monster && _monster->init())
	{
		//_monster->setPosition(point);
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
		_monster->setMonsterType(DAJI);
		_monster->setMaxHp(3245);
		_monster->setCurrHp(3245);  //3245
		_monster->setMoney(200);//����һ��Ӣ�ۼӶ���Ǯ��
		_monster->setExperience(45);
		//monster->sonPath = sonpath;
		_monster->setForce(170);//����ֵ
		_monster->setArmor(83);//����ֵ
		_monster->setScope(75);//Զ��
		//monster->setAttackByTower(true);
		//monster->setAttackBySoldier(true);//��
		_monster->setRunSpeed(20);
		//_monster->runNextPoint()
		_monster->autorelease();
		return _monster;
	}
	CC_SAFE_DELETE(_monster);
	return NULL;
}

/*void BaseMonster::removeBullet()//������ �̳в��Ķ� ��д˭�����֣�
{
	bool isMissed = true;
	auto instance = GameManager::getInstance();
	auto spriteBullet = Sprite::create("");
	addChild(spriteBullet);

	//bullet���ھ�������
	auto bulletRect = Rect(spriteBullet->getPositionX() + this->getPositionX() - spriteBullet->getContentSize().width / 2,
		spriteBullet->getPositionY() + this->getPositionY() - spriteBullet->getContentSize().height / 2,
		spriteBullet->getContentSize().width,//�Բ����� ��Ҳ��֪��
		spriteBullet->getContentSize().height);//�;����˾Ͳ�������������
	if (this->getMonsterColor() == 1)//red
	{
		auto monsterBlueVector = instance->monsterBlueVector;

		for (int j = 0; j < monsterBlueVector.size(); j++)
		{
			auto monster = monsterBlueVector.at(j);
			auto monsterRect = monster->baseSprite->getBoundingBox();
			if (monsterRect.intersectsRect(bulletRect) && monster->getState() != 7)
			{
				auto currHp = monster->getCurrHp();

				currHp = currHp - this->getForce();//�ܵ�����Hp����

				if (currHp <= 0)
				{
					currHp = 0;
				}
				monster->setCurrHp(currHp);

				monster->getHpBar()->setPersentage((currHp / monster->getMaxHp()) * 100);//����Hp
				//monster->getHurt();     Wow!Ѫ�ȳ��棡
				isMissed = false;//����
				if (currHp == 0)
				{
					monster->death();//��ʧ�� vector�Ƴ���
					GameManager::getInstance()->MONEY = GameManager::getInstance()->MONEY + monster->getMoney();
				}
				break;
			}
		}
	}
	else
	{
		auto monsterRedVector = instance->monsterRedVector;//ɶ��˼��

		for (int j = 0; j < monsterRedVector.size(); j++)
		{
			auto monster = monsterRedVector.at(j);
			auto monsterRect = monster->baseSprite->getBoundingBox();
			if (monsterRect.intersectsRect(bulletRect) && monster->getState() != 7)
			{
				auto currHp = monster->getCurrHp();

				currHp = currHp - this->getForce();//�ܵ�����Hp����

				if (currHp <= 0)
				{
					currHp = 0;
				}
				monster->setCurrHp(currHp);

				monster->getHpBar()->setPersentage((currHp / monster->getMaxHp()) * 100);//����Hp
				//monster->getHurt();     Wow!Ѫ�ȳ��棡
				isMissed = false;//����
				if (currHp == 0)
				{
					monster->death();
					GameManager::getInstance()->MONEY = GameManager::getInstance()->MONEY + monster->getMoney();
				}
				break;
			}
		}
	}
	//���ܻ������ж���ʧ
	this->removeFromParent();
	/*if (isMissed)
	{
		//��δ����
	}
	else
	{
		this->removeFromParent();
	}

}*/

void Daji::removeBullet()//������  ��֤����
{
	//bool isMissed = true;
	auto instance = GameManager::getInstance();

	if (currMinMonster <= currMinTower) //��currMinTower��towerDistance
	{
		auto currHp = nearestMonster->getCurrHp();
		auto currExperience = GameManager::getInstance()->EXPERIENCE;
		currHp = currHp - this->getForce() + nearestMonster->getArmor();//�ܵ�����Hp����
		if (getMonsterColor() == 2)
		{
			currExperience =currExperience+ 5;
		}

		if (currHp <= 0)
		{
			currHp = 0;
		}
		nearestMonster->setCurrHp(currHp);

		nearestMonster->getHpBar()->setPercentage((currHp / nearestMonster->getMaxHp()) * 100);//����Hp

		if (currHp == 0)
		{
			nearestMonster->death();//��ʧ�� vector�Ƴ���
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
	else if (currMinTower < currMinMonster)   //��currMinTower��towerDistance
	{
		auto currHp = nearestTower->getCurrHp();
		auto currExperience = GameManager::getInstance()->EXPERIENCE;

		currHp = currHp - this->getForce() + nearestTower->getArmor();//�ܵ�����Hp����
		if (getMonsterColor() == 2)
		{
			currExperience = currExperience + 5;
		}

		if (currHp <= 0)
		{
			currHp = 0;
		}
		nearestTower->setCurrHp(currHp);

		nearestTower->getHpBar()->setPercentage((currHp / nearestTower->getMaxHp()) * 100);//����Hp//��nearestMonster��nearestTower

		if (currHp == 0)
		{
			nearestTower->death();//��ʧ�� vector�Ƴ���
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


void Daji::shoot()//����㹥���Ź��� everytime
{
	auto instance = GameManager::getInstance();//
	checkNearestMonster();
	checkNearestTower();
	

	

	if (currMinMonster <= towerDistance && nearestMonster != NULL /*&& nearestMonster->getCurrHp() > 0*/)
	{

		spriteBullet = Sprite::create("Bullet.png");
		float _scale = (TileCoord / 2) / spriteBullet->getContentSize().height;
		spriteBullet->setScale(0.04);
		addChild(spriteBullet);
		//auto currBullet = ArrowTowerBullet();//���ɹ���
		Point shootVector = nearestMonster->getPosition() - this->getPosition();//����� ������geParent
		//auto pVectr = currBullet->getPosition() - shootVector;//��֪������
		auto moveByAction = MoveBy::create(0.5f, shootVector);

	
		spriteBullet->runAction(Sequence::create(moveByAction, FadeOut::create(1 / 30), NULL));//, CallFuncN::create(CC_CALLBACK_0(BaseTower::removeBullet, this)), NULL));//BulletAction��ֱ�߷��� removeBullet�жϻ��в��ӵ���ʧ
		Daji::removeBullet();
	}
	else if (currMinTower < currMinMonster&&nearestTower != NULL)
	{
		spriteBullet = Sprite::create("Bullet.png");
		float _scale = (TileCoord / 2) / spriteBullet->getContentSize().height;
		spriteBullet->setScale(0.04);
		addChild(spriteBullet);
		//auto currBullet = ArrowTowerBullet();//���ɹ���
		Point shootVector = nearestTower->getPosition() - this->getPosition();//����� ������geParent
		//auto pVectr = currBullet->getPosition() - shootVector;//��֪������
		auto moveByAction = MoveBy::create(0.5f, shootVector);//�Ҿ�����moveby��
		//currBullet->runAction(moveToAction);
		//currBullet->shoot;//removeBullet!!
		//currBullet->setPosition();//������
		//currBullet = NULL;
		/*if (getState() == stateWalkRight)
		{
			/*
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/DJ/animation.plist", "GameItem/DJ/animation.png");
			AnimationCache::getInstance()->addAnimationsWithFile("GameItem/DJ/animation.plist");
			auto cache = AnimationCache::getInstance();
			auto animation1 = cache->getAnimation("SkillThreeRight");
			animation1->setLoops(1);
			auto animate1 = Animate::create(animation1);
			baseSprite->runAction(animate1);
			
		}
		else if (getState() == stateWalkLeft)
		{
			/*
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/DJ/animation.plist", "GameItem/DJ/animation.png");
			AnimationCache::getInstance()->addAnimationsWithFile("GameItem/DJ/animation.plist");
			auto cache = AnimationCache::getInstance();
			auto animation1 = cache->getAnimation("SkillThreeLeft");
			animation1->setLoops(1);
			auto animate1 = Animate::create(animation1);
			baseSprite->runAction(animate1);
			
		}
		else if (getState() == stateWalkUp)
		{
			/*
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/DJ/animation.plist", "GameItem/DJ/animation.png");
			AnimationCache::getInstance()->addAnimationsWithFile("GameItem/DJ/animation.plist");
			auto cache = AnimationCache::getInstance();
			auto animation1 = cache->getAnimation("SkillThreeUp");
			animation1->setLoops(1);
			auto animate1 = Animate::create(animation1);
			baseSprite->runAction(animate1);
			
		}
		else if (getState() == stateWalkDown)
		{
			/*
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/DJ/animation.plist", "GameItem/DJ/animation.png");
			AnimationCache::getInstance()->addAnimationsWithFile("GameItem/DJ/animation.plist");
			auto cache = AnimationCache::getInstance();
			auto animation1 = cache->getAnimation("SkillThreeDown");
			animation1->setLoops(1);
			auto animate1 = Animate::create(animation1);
			baseSprite->runAction(animate1);
			
		}*/
		spriteBullet->runAction(Sequence::create(moveByAction, FadeOut::create(1 / 30), NULL));//, CallFuncN::create(CC_CALLBACK_0(BaseTower::removeBullet, this)), NULL));//BulletAction��ֱ�߷��� removeBullet�жϻ��в��ӵ���ʧ
		Daji::removeBullet();
	}
}

bool Daji::init()
{
	if (!BaseMonster::init())
	{
		return false;
	}
	//setMonsterType(BOSS_CANIBAL);
	//setName("Boss_Canibal_");
	BaseMonster::onEnter();
	baseSprite = Sprite::create("DJup.png");
	float theScale = 2 * TileCoord / baseSprite->getContentSize().height;
	baseSprite->setScale(0.5);
	
	addChild(baseSprite);

	//if (getCurrHp() != getMaxHp()) //��Ƿȱ�öడ ����type���Ķ����İ�  whatever��˵��
	//{

	//}

	createAndSetHpBar();
	lastState = stateNone;
	schedule(schedule_selector(BaseMonster::hpSchedule), 1.0f);

	//schedule(schedule_selector(Boss_Canibal::beforeSkill), 50.0f, 2, 20.0f);
	//setListener();
	//skillTag = 0;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_tileMap = TMXTiledMap::create("map/MyMap2.tmx");//���ּ���һ��auto����ͱ��˰� ���ǧ����Ҹ� ��һ��һʮ�س����룡����
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
	this->scheduleUpdate();//for����

	return true;
}

void Daji::onEnter()
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

cocos2d::Vec2 Daji::tileCoordFromPosition(cocos2d::Vec2 pos)
{
	int x = pos.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y) / _tileMap->getTileSize().height;
	return cocos2d::Vec2(x, y);
}

void Daji::setPlayerPosition(Vec2 position)
{
	Vec2 tileCoord = this->tileCoordFromPosition(position);//���ص�����->��Ƭ����
	int tileGid = _collidable->getTileGIDAt(tileCoord);//�����Ƭ��GID

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

bool Daji::isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode)
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

void Daji::keyPressedDuration(cocos2d::EventKeyboard::KeyCode code)
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

void Daji::update(float delta)
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

void Daji::death()
{

	//setState(stateDeath);
	//hpBgSprite->setVisible(false);

	//this->stopAllActions();
	//unscheduleUpdate();//���� Ѫ��  

	//this->runAction(Sequence::create(FadeOut::create(1.0f)
		//, CallFuncN::create(CC_CALLBACK_0(BaseMonster::setVisible, this, false))
		//, NULL));
	//baseSprite->setVisible(false);
	//this->scheduleOnce(schedule_selector(BaseMonster::relive), 2.0f);//15��󸴻�  �ǵøĻ�����
	relive();

}