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
		monster->setMoney(48);//�ٶ�һ�� ���֪��
		//monster->sonPath = sonpath;
		monster->setForce(133);//����ֵ
		monster->setArmor(38);//����ֵ
		monster->setScope(45);//Զ��
		//monster->setAttackByTower(true);
		//monster->setAttackBySoldier(true);//��
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

	//if (getCurrHp() != getMaxHp()) //��Ƿȱ�öడ ����type���Ķ����İ�  whatever��˵��
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

void paoCar::shoot(float dt)//ÿ�����shootһ�� 0.5
{
	auto instance = GameManager::getInstance();//
	checkNearestMonster();//dt��ͣ����
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
			//cocos2d::Action* moveByAction = MoveBy::create(1.0f, Point(16, 10.1));//����
			//baseSprite->runAction(moveByAction);//����ɻ� ����ǰ�߰ɣ�
		}
		else
		{
			auto playerpos = this->getPosition();
			playerpos = Vec2(playerpos.x - 7.55, playerpos.y - 5.0);
			this->setPosition(playerpos);
			//cocos2d::Action* moveByAction = MoveBy::create(1.0f, Point(-15.1, -10.0));//����
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
		//auto currBullet = ArrowTowerBullet();//���ɹ���
		Point shootVector = nearestMonster->getPosition() - this->getPosition();//����� ɾ��geParent ��Ҫ�����ڵ�ͼ�������ɶ
		//auto pVectr = currBullet->getPosition() - shootVector;//��֪������
		auto moveByAction = MoveBy::create(1.0f, shootVector);//�Ҿ�����moveby��
		//currBullet->runAction(moveToAction);
		//currBullet->shoot;//removeBullet!!
		//currBullet->setPosition();//������
		//currBullet = NULL;
		//ʿ��ֻ�ֺ�����ͬ���򲥷Ŷ����������Ҷ�������

		if (getMonsterColor() == 2)
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/Blue/GunCar/animation.plist", "GameItem/Blue/GunCar/animation.png");
			AnimationCache::getInstance()->addAnimationsWithFile("GameItem/Blue/GunCar/animation.plist");
			auto cache = AnimationCache::getInstance();
			auto animation1 = cache->getAnimation("AttactUp");
			animation1->setLoops(1);
			auto animate1 = Animate::create(animation1);
			baseSprite->runAction(animate1);
			// ֡����ֹͣ������
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
			// ֡����ֹͣ������
			AnimationCache::destroyInstance();
			SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
		}


		//spriteBullet->runAction(moveToAction);//, CallFuncN::create(CC_CALLBACK_0(BaseTower::removeBullet, this)), NULL));//BulletAction��ֱ�߷��� removeBullet�жϻ��в��ӵ���ʧ
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
