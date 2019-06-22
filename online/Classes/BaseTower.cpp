#include "GlobalManager.h"
#include "BaseTower.h"
#include "Crystal.h"
//#include "SimpleAudioEngine.h"
//#include "BaseMap.h"
#include "GameManager.h"
#include "BaseMonster.h"
#include "LongSoldier.h"
#include "ShortSoldier.h"
#include "Daji.h"
#include "Houyi.h"
#include "Yase.h"

USING_NS_CC;


BaseTower* BaseTower::createTower(Point point, int TowerColor)
{
	auto tower = new BaseTower();
	if (tower&&tower->init(TowerColor))
	{
		tower->setPosition(point);
		
		tower->autorelease();
		
		return tower;
	}
	CC_SAFE_DELETE(tower);
	return NULL;
}

bool BaseTower::init(int TowerColor)//创建塔对象 要先set颜色才能init
{
	if (!Sprite::init())
	{
		return false;
	}
	//Sprite::onEnter();
	setTowerColor(TowerColor);
	if (getTowerColor() == 1)//red
	{
		towerBase = Sprite::create("RedDefense.png");
	}
	else
		towerBase = Sprite::create("BlueDefense.png");

	addChild(towerBase);
	float _scale = TileCoord * 2 / towerBase->getContentSize().height;
	towerBase->setScale(0.08);

	
	//
	this->setForce(320);
	this->setMaxHp(6000);
	this->setCurrHp(6000); //600
	this->setArmor(20);
	this->setMoney(70);
	this->setExperience(85);
	this->setScope(80);
	this->setHpPercentage(100);

	//spriteBullet = Sprite::create("");
	
	//多个实例 位置 这些都要怎么弄啊

	//towerBase->addChild(spriteBullet);
	createAndSetHpBar();
	//ischedule(schedule_selector(BaseTower::shoot), 1.0f);

	return true;

}

void BaseTower::checkNearestMonster()
{
	auto instance = GameManager::getInstance();
	if (this->getTowerColor() == 1)
	{
		auto monsterBlueVector = instance->monsterBlueVector;

		auto currMinDistant = this->scope;


		BaseMonster *monsterTemp = NULL;
		for (int i = 0; i < monsterBlueVector.size(); i++)
		{
			auto monster = monsterBlueVector.at(i);
			double distance = this->getPosition().getDistance(monster->getPosition());

			if (distance < currMinDistant&&monster->getState()!=7)//在塔的范围内且可以被攻击
			{
				currMinDistant = distance;//攻击最近怪物
				monsterTemp = monster;//目前攻击的怪物
			}
		}
		nearestMonster = monsterTemp;
	}
	else
	{
		auto monsterRedVector = instance->monsterRedVector;


		auto currMinDistant = this->scope;


		BaseMonster *monsterTemp = NULL;
		for (int i = 0; i < monsterRedVector.size(); i++)
		{
			auto monster = monsterRedVector.at(i);
			double distance = this->getPosition().getDistance(monster->getPosition());

			if (distance < currMinDistant&&monster->getState()!=7)//在塔的范围内且可以被攻击
			{
				currMinDistant = distance;//攻击最近怪物
				monsterTemp = monster;//目前攻击的怪物
			}
		}
		nearestMonster = monsterTemp;
	}
}

/*void BaseTower::onEnter()
{

}*/

/*void SimpleArrowTower::buildingAnimation()
{
	auto building = Sprite::create();
	auto constructing = Sprite::createWithSpriteFrameName("BlueDefense.png");

}*/

void BaseTower::shoot(float dt)//每隔多久shoot一次 0.7  调度器一般在init或onEnter
{
	auto instance = GameManager::getInstance();//
	checkNearestMonster();
	//char temp1[20];
	//sprintf(temp1,"")
	if (nearestMonster != NULL /*&& nearestMonster->getCurrHp() > 0*/)
	{
		spriteBullet = Sprite::create("Bullet.png");
		float _scale = (TileCoord / 2) / spriteBullet->getContentSize().height;
		spriteBullet->setScale(0.04);
		addChild(spriteBullet);
		//auto currBullet = ArrowTowerBullet();//生成弓箭
		Point shootVector = nearestMonster->getPosition() - this->getPosition();//两点间 留下了geParent
		//auto pVectr = currBullet->getPosition() - shootVector;//不知道干嘛
		auto moveByAction = MoveBy::create(0.5f, shootVector);//我觉得是moveby啊
		//currBullet->runAction(moveToAction);
		//currBullet->shoot;//removeBullet!!
		//currBullet->setPosition();//塔顶上
		//currBullet = NULL;
		//spriteBullet->runAction(moveToAction);//, CallFuncN::create(CC_CALLBACK_0(BaseTower::removeBullet, this)), NULL));//BulletAction沿直线飞行 removeBullet判断击中并子弹消失
		spriteBullet->runAction(Sequence::create(moveByAction, FadeOut::create(1 / 30),NULL));
		BaseTower::removeBullet();
	}
}


void BaseTower::removeBullet()//攻击！  保证打中
{
	//bool isMissed = true;
	auto instance = GameManager::getInstance();

	auto currHp = nearestMonster->getCurrHp();

	currHp = currHp - this->getForce() + nearestMonster->getArmor();//受到攻击Hp减少

	if (currHp <= 0)
	{
		currHp = 0;
	}
	nearestMonster->setCurrHp(currHp);

	nearestMonster->getHpBar()->setPercentage((currHp / nearestMonster->getMaxHp()) * 100);//更新Hp

	if (currHp == 0)
	{
		nearestMonster->death();//消失啊 vector移除啊
	}
	if (currHp == 0 && getTowerColor() == 1)
	{
		if (nearestMonster->getMonsterType() == HOUYI || nearestMonster->getMonsterType() == YASE || nearestMonster->getMonsterType() == DAJI)
		{
			GameManager::getInstance()->KILLED += 1;
		}
	}
}
		

void BaseTower::death()
{
	//if (getMonsterColor() == 1)
	//{
		//if (GameManager::getInstance()->monsterRedVector.contains(this))
			//GameManager::getInstance()->monsterRedVector.eraseObject(this);
	//}
	//else if (getMonsterColor() == 2)
	//{
		//if (GameManager::getInstance()->monsterBlueVector.contains(this))
			//GameManager::getInstance()->monsterBlueVector.eraseObject(this);//从vector移除
	//}
		//setCurrHp(0);
	setState(StateDeath);
	//hpBgSprite->setVisible(false);背景框做了吗？？？？？
	//this->setVisible(false);
	this->stopAllActions();
	unscheduleUpdate();//shoot
	//auto decal_blood = Sprite::createWithSpriteFrameName("decal_blood_0001.png");
	//decal_blood->setPosition(Point(baseSprite->getContentSize().width / 2, -baseSprite->getContentSize().height / 4));
	//baseSprite->addChild(decal_blood);
	//GameManager::getInstance()->MONEY = GameManager::getInstance()->MONEY + getMoney();  ！！！！！在英雄的shoot函数里会有用！！！！！
	//baseSprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "death")));
	this->runAction(Sequence::create(FadeOut::create(1.0f)
		, CallFuncN::create(CC_CALLBACK_0(BaseTower::setVisible, this, false))
		, NULL));
	//}
}

void BaseTower::createAndSetHpBar()
{
	hpBgSprite = Sprite::create("blood2.png");//好东西！之前见过的！！！哪见的不记得了嘤嘤嘤
	hpBgSprite->setScale(0.18);
	hpBgSprite->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height/4*1.2));
	this->addChild(hpBgSprite);

	hpBar = ProgressTimer::create(Sprite::create("blood1.png"));
	hpBar->setType(ProgressTimer::Type::BAR);
	hpBar->setMidpoint(Point(0, 0.5f));//开始点 左中？
	hpBar->setBarChangeRate(Point(1, 0));//水平方向展开
	hpBar->setPercentage(hpPercentage);
	hpBar->setPosition(Point(hpBgSprite->getContentSize().width / 2, hpBgSprite->getContentSize().height / 2));
	//hpBar->setTag(666);  //加了个标签 
	hpBgSprite->addChild(hpBar);
}

void BaseTower::onEnter()
{
	Sprite::onEnter();
	schedule(schedule_selector(BaseTower::shoot), 1.0f);

}