#include "GlobalManager.h"
#include "BaseMonster.h"
#include "GameManager.h"
//#include "BaseMap.h"
#include "paoCar.h"
#include "LongSoldier.h"
#include "ShortSoldier.h"
#include "BaseTower.h"
#include "Crystal.h"

USING_NS_CC;

paoCar* paoCar::createMonster(Point point, int color)//std::vector<Point> points, std::vector<std::vector<std::vector<Point>>> sonpath)
{
	auto monster = new paoCar();
	if (monster && monster->initMonster(color))
	{
		monster->setMonsterType(PAO_CAR);
		monster->setPosition(point);
		monster->setMaxHp(2070);
		monster->setCurrHp(2070);
		monster->setMoney(48);//百度一下 你就知道
		//monster->sonPath = sonpath;
		monster->setForce(133);//攻击值
		monster->setArmor(38);//防御值
		monster->setScope(45);//远程
		//monster->setAttackByTower(true);
		//monster->setAttackBySoldier(true);//唔
		monster->setRunSpeed(15);
		//monster->runNextPoint();
		monster->autorelease();



		return monster;
	}
	CC_SAFE_DELETE(monster);
	return NULL;
}

bool paoCar::initMonster(int color)
{
	if (!BaseMonster::init())
	{
		return false;
	}
	//setMonsterType(BOSS_CANIBAL);
	//setName("Boss_Canibal_");
	BaseMonster::onEnter();
	this->BaseMonster::setMonsterColor(color);
	if (BaseMonster::getMonsterColor() == 1)
	{
		baseSprite = Sprite::create("RedMagic.png");
	}
	else if (BaseMonster::getMonsterColor() == 2)
	{
		baseSprite = Sprite::create("BlueMagic.png");
	}
	//float theScale = 2 * TileCoord / baseSprite->getContentSize().height;
	baseSprite->setScale(0.4);
	//baseSprite->setPosition(Point(BlueTown.x,BlueTown.y));
	addChild(baseSprite);

	//if (getCurrHp() != getMaxHp()) //这欠缺好多啊 他的type从哪儿来的啊  whatever再说吧
	//{

	//}
	createAndSetHpBar();
	lastState = stateNone;
	//schedule(schedule_selector(BaseMonster::shoot), 0.5f);
	schedule(schedule_selector(BaseMonster::shoot), 1.0f);
	return true;
}

/*void LongSoldier::onEnter() {
	BaseMonster::onEnter();
	schedule(schedule_selector(BaseMonster::shoot), 0.5f);
}*/

void paoCar::shoot(float dt)//每隔多久shoot一次 0.5
{
	auto instance = GameManager::getInstance();//
	checkNearestMonster();//dt不停调用
	checkNearestTower();
	//char temp1[20];
	//sprintf(temp1,"")
	if (nearestMonster == NULL && nearestTower == NULL)
	{
		if (getMonsterColor() == 2)
		{
			auto playerpos = this->getPosition();
			playerpos = Vec2(playerpos.x + 8, playerpos.y + 5.05);
			this->setPosition(playerpos);
			//cocos2d::Action* moveByAction = MoveBy::create(1.0f, Point(16, 10.1));//行吗
			//baseSprite->runAction(moveByAction);//无物可击 就向前走吧！
		}
		else
		{
			auto playerpos = this->getPosition();
			playerpos = Vec2(playerpos.x - 7.55, playerpos.y - 5.0);
			this->setPosition(playerpos);
			//cocos2d::Action* moveByAction = MoveBy::create(1.0f, Point(-15.1, -10.0));//行吗
			//baseSprite->runAction(moveByAction);
		}
	}
	else if (currMinMonster <= currMinTower && nearestMonster != NULL /*&& nearestMonster->getCurrHp() > 0*/)
	{
		spriteBullet = Sprite::create("Bullet.png");
		float _scale = (TileCoord / 2) / spriteBullet->getContentSize().height;
		spriteBullet->setScale(0.04);
		baseSprite->addChild(spriteBullet);
		spriteBullet->setPosition(Point(baseSprite->getContentSize().width / 2, baseSprite->getContentSize().height / 2 * 1.2));
		//auto currBullet = ArrowTowerBullet();//生成弓箭
		Point shootVector = nearestMonster->getPosition() - this->getPosition();//两点间 删了geParent 主要看你在地图上添加了啥
		//auto pVectr = currBullet->getPosition() - shootVector;//不知道干嘛
		auto moveByAction = MoveBy::create(1.0f, shootVector);//我觉得是moveby啊
		//currBullet->runAction(moveToAction);
		//currBullet->shoot;//removeBullet!!
		//currBullet->setPosition();//塔顶上
		//currBullet = NULL;
		//士兵只分红蓝向不同方向播放动作！！！弦儿！！！

		if (getMonsterColor() == 2)
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/Blue/GunCar/animation.plist", "GameItem/Blue/GunCar/animation.png");
			AnimationCache::getInstance()->addAnimationsWithFile("GameItem/Blue/GunCar/animation.plist");
			auto cache = AnimationCache::getInstance();
			auto animation1 = cache->getAnimation("AttactUp");
			animation1->setLoops(1);
			auto animate1 = Animate::create(animation1);
			baseSprite->runAction(animate1);
			// 帧动画停止？？？
			AnimationCache::destroyInstance();
			SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
		}
		else
		{

			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/Red/GunCar/animation.plist", "GameItem/Red/GunCar/animation.png");
			AnimationCache::getInstance()->addAnimationsWithFile("GameItem/Red/GunCar/animation.plist");
			auto cache = AnimationCache::getInstance();
			auto animation1 = cache->getAnimation("AttactDown");
			animation1->setLoops(1);
			auto animate1 = Animate::create(animation1);
			baseSprite->runAction(animate1);
			// 帧动画停止？？？
			AnimationCache::destroyInstance();
			SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
		}


		//spriteBullet->runAction(moveToAction);//, CallFuncN::create(CC_CALLBACK_0(BaseTower::removeBullet, this)), NULL));//BulletAction沿直线飞行 removeBullet判断击中并子弹消失
		BaseMonster::removeBullet();
	}
	else if (currMinTower < currMinMonster&&nearestTower != NULL)
	{
		spriteBullet = Sprite::create("Bullet.png");
		float _scale = (TileCoord / 2) / spriteBullet->getContentSize().height;
		spriteBullet->setScale(0.04);
		spriteBullet->setPosition(Point(baseSprite->getContentSize().width / 2, baseSprite->getContentSize().height / 2 * 1.2));
		baseSprite->addChild(spriteBullet);
		Point shootVector = nearestTower->getPosition() - this->getPosition();
		auto moveByAction = MoveBy::create(1.0f, shootVector);

		if (getMonsterColor() == 2)
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/Blue/GunCar/animation.plist", "GameItem/Blue/GunCar/animation.png");
			AnimationCache::getInstance()->addAnimationsWithFile("GameItem/Blue/GunCar/animation.plist");
			auto cache = AnimationCache::getInstance();
			auto animation1 = cache->getAnimation("AttactUp");
			animation1->setLoops(1);
			auto animate1 = Animate::create(animation1);
			baseSprite->runAction(animate1);
			AnimationCache::destroyInstance();
			SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
		}
		else
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/Red/GunCar/animation.plist", "GameItem/Blue/GunCar/animation.png");
			AnimationCache::getInstance()->addAnimationsWithFile("GameItem/Red/GunCar/animation.plist");
			auto cache = AnimationCache::getInstance();
			auto animation1 = cache->getAnimation("AttactDown");
			animation1->setLoops(1);
			auto animate1 = Animate::create(animation1);
			baseSprite->runAction(animate1);
			AnimationCache::destroyInstance();
			SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
		}

		//auto sequenceAction = Sequence::create(moveToAction, BaseMonster::removeBullet);
		spriteBullet->runAction(Sequence::create(moveByAction, FadeOut::create(1 / 30), CallFuncN::create(CC_CALLBACK_0(BaseMonster::setVisible, this, false)), NULL));

		//spriteBullet->runAction(moveToAction);
		BaseMonster::removeBullet();
	}
}
