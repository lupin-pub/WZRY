#include "GlobalManager.h"
#include "Crystal.h"
#include "BaseTower.h"
#include "BaseMonster.h"
#include "LongSoldier.h"
#include "ShortSoldier.h"
#include "EndScene.h"

USING_NS_CC;

Crystal* Crystal::createCrystal(Point point, int color)
{
	auto tower = new Crystal();
	if (tower&&tower->initTower(color))
	{
		tower->setPosition(point);
		tower->autorelease();
		return tower;
	}
	CC_SAFE_DELETE(tower);
	return NULL;
}

bool Crystal::initTower(int color)
{
	if (!BaseTower::init(color))
	{
		return false;
	}
	BaseTower::onEnter();
	this->BaseTower::setScope(TileCoord*2);
	this->BaseTower::setTowerColor(color);
	if (BaseTower::getTowerColor() == 1)
	{
		towerBase = Sprite::create("RedCrystal.png");
	}
	else
		towerBase = Sprite::create("BlueCrystal.png");
	float _scale = TileCoord * 2 / towerBase->getContentSize().height;
	towerBase->setScale(0.16);
	//spriteBullet = Sprite::create("");
	this->setForce(350);
	this->setMaxHp(9000);
	this->setCurrHp(9000);
	this->setArmor(20);
	this->setMoney(70);
	this->setScope(45);
	this->setHpPercentage(100);
	//���ʵ�� λ�� ��ɫ ��Щ��Ҫ��ôŪ��

	//towerBase->addChild(spriteBullet);
	addChild(towerBase);
	createAndSetHpBar();
	schedule(schedule_selector(BaseTower::shoot), 1.0f);
	return true;
}

void Crystal::death()
{
	//if (getMonsterColor() == 1)
	//{
		//if (GameManager::getInstance()->monsterRedVector.contains(this))
			//GameManager::getInstance()->monsterRedVector.eraseObject(this);
	//}
	//else if (getMonsterColor() == 2)
	//{
		//if (GameManager::getInstance()->monsterBlueVector.contains(this))
			//GameManager::getInstance()->monsterBlueVector.eraseObject(this);//��vector�Ƴ�
	//}
		//setCurrHp(0);
	setState(StateDeath);
	//hpBgSprite->setVisible(false);�����������𣿣�������
	//this->setVisible(false);
	this->stopAllActions();
	unscheduleUpdate();
	//auto decal_blood = Sprite::createWithSpriteFrameName("decal_blood_0001.png");
	//decal_blood->setPosition(Point(baseSprite->getContentSize().width / 2, -baseSprite->getContentSize().height / 4));
	//baseSprite->addChild(decal_blood);
	//GameManager::getInstance()->MONEY = GameManager::getInstance()->MONEY + getMoney();  ������������Ӣ�۵�shoot����������ã���������
	//baseSprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "death")));
	this->runAction(Sequence::create(FadeOut::create(1.0f)
		, CallFuncN::create(CC_CALLBACK_0(BaseTower::setVisible, this, false))
		, NULL));
	//�жϺ��� �ж��ķ�ʤ��
	//��Ϸ����
	//}
	//!!!
	Director::getInstance()->pushScene(TransitionFade::create(1, EndScene::createScene(getTowerColor())));
	
}