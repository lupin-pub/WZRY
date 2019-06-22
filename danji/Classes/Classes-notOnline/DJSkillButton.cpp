#include "DJSkillButton.h"
#include "GameMenuScene.h"
#include"SelectToPlayScene.h"
#include "BaseMonster.h"
#include "GameManager.h"
#include "Daji.h"
#include "DJGameScene.h"

USING_NS_CC;



void DJSkillButton::onEnter() {
	Layer::onEnter();
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/DJ/animation.plist", "GameItem/DJ/animation.png");
	//AnimationCache::getInstance()->addAnimationsWithFile("GameItem/DJ/animation.plist");
}

bool DJSkillButton::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//透明的背景
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	

	//正常显示按钮
	m_normalskill = MenuItemImage::create("GameScene/NorSkill.png", "GameScene/NorSkill.png");
	m_normalskill->setScale(0.3);
	m_normalskill->setTarget(this, menu_selector(DJSkillButton::norskillClickCallBack, this));
	m_normalskill->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width - 100, visibleSize.height - 100)));

	m_SkillOne = MenuItemImage::create("GameItem/DJ/SkillFirst.png", "GameItem/DJ/SkillFirst.png");
	m_SkillOne->setScale(0.7);
	m_SkillOne->setTarget(this, menu_selector(DJSkillButton::skillOneClickCallBack, this));
	m_SkillOne->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 - 10), (visibleSize.height - visibleSize.height / 5 + 30))));

	m_SkillTwo = MenuItemImage::create("GameItem/DJ/SkillSecond.png", "GameItem/DJ/SkillSecond.png");
	m_SkillTwo->setScale(0.7);
	m_SkillTwo->setTarget(this, menu_selector(DJSkillButton::skillTwoClickCallBack, this));
	m_SkillTwo->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 + 30), (visibleSize.height - visibleSize.height / 5 - 70))));

	m_SkillThree = MenuItemImage::create("GameItem/DJ/SkillThird.png", "GameItem/DJ/SkillThird.png");
	m_SkillThree->setScale(0.7);
	m_SkillThree->setTarget(this, menu_selector(DJSkillButton::skillThreeClickCallBack, this));
	m_SkillThree->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 + 120), (visibleSize.height - visibleSize.height / 5 - 100))));


	m_skills = Menu::create(m_normalskill, m_SkillOne, m_SkillTwo, m_SkillThree, nullptr);
	m_skills->setPosition(Vec2::ZERO);

	addChild(m_skills, 1);

	

	return true;
}



int DJSkillButton::getAttact() {
	return addAttact;
}
int DJSkillButton::getMonsterExp() {
	return addMonsterExp;
}
int DJSkillButton::getTowerExpDeath() {
	return addTowerExpDeath;
}
int DJSkillButton::getTowerExpOnce() {
	return addTowerExpOnce;
}

void DJSkillButton::setAttact(int temp) {
	this->addAttact = temp;
}

void DJSkillButton::updateLevel() {
	auto instance = GameManager::getInstance();
	if (instance->EXPERIENCE >= instance->MAXEXPONE && instance->LEVEL < 5) {
		instance->LEVEL = instance->LEVEL + 1;
		instance->EXPERIENCE = 0;
	}
	if (instance->EXPERIENCE >= instance->MAXEXPTWO && instance->LEVEL >= 5) {
		instance->LEVEL = instance->LEVEL + 1;
		instance->EXPERIENCE = 0;
	}
}

void DJSkillButton::updateKilled() {
	Scene* scene = Director::getInstance()->getRunningScene();
	Daji * hero = ((DJGameScene *)scene)->hero;
	if (hero -> getCurrHp() == 0)
	{
		GameManager::getInstance()->KILLED = GameManager::getInstance()->KILLED + 1;
	}
}

void DJSkillButton::changeAttributes() {

	Scene* scene = Director::getInstance()->getRunningScene();
	Daji * hero = ((DJGameScene *)scene)->hero;

	hero -> checkNearestMonster();
	hero -> checkNearestTower();
	if (hero -> monsterDistance <=hero -> towerDistance)
	{
		auto currHp = hero->nearestMonster->getCurrHp();

		currHp = currHp - hero->getForce()- getAttact() + hero->nearestMonster->getArmor();//受到攻击Hp减少

		if (currHp <= 0)
		{
			currHp = 0;
		}
		if (currHp == 0)
		{
			GameManager::getInstance()->KILLS = GameManager::getInstance()->KILLS + 1;
		}
		//加经验区分小兵和怪？
		GameManager::getInstance()->EXPERIENCE = GameManager::getInstance()->EXPERIENCE + getMonsterExp();
		updateLevel();
	}
	else if (hero->towerDistance < hero->monsterDistance){
	
		auto currHp = hero->nearestTower->getCurrHp();

	   currHp = currHp - hero->getForce() - getAttact() + hero->nearestTower->getArmor();//受到攻击Hp减少

		if (currHp <= 0)
		{
			currHp = 0;
		}
		if (currHp == 0)
		{
			GameManager::getInstance()->KILLS = GameManager::getInstance()->KILLS + 1;
			GameManager::getInstance()->MONEY = GameManager::getInstance()->MONEY + hero->nearestTower->getMoney();
			GameManager::getInstance()->EXPERIENCE = GameManager::getInstance()->EXPERIENCE + getTowerExpDeath();
			updateLevel();
		}
		else {
			GameManager::getInstance()->EXPERIENCE = GameManager::getInstance()->EXPERIENCE + getTowerExpOnce();
			updateLevel();
		}
	}	
}

// 技能按钮点击回调 
void DJSkillButton::norskillClickCallBack(Ref* obj)

{
	
	Scene* scene = Director::getInstance()->getRunningScene();
	Daji * hero = ((DJGameScene *)scene) -> hero;
	//shoot函数
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/DJ/animation.plist", "GameItem/DJ/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/DJ/animation.plist");
	auto cache = AnimationCache::getInstance();
	/*auto animation = Animation::createWithSpriteFrames(vecSpriteFrame, 0.2f);
	auto animate = Animate::create(animation);
	spriteRun->runAction(RepeatForever::create(animate));*/
	auto animation1 = cache->getAnimation("SkillThreeRight");
	animation1->setLoops(1);
	auto animate1 = Animate::create(animation1);
	hero->baseSprite->runAction(animate1);

	hero->setForce(170);
	hero->setArmor(83);
	hero->shoot();
	//setAttact(0);
	//hero->setForce(hero->getForce() + getAttact());
	//scope??
	//hero->shoot();
	//hero->setForce(hero->getForce() - getAttact());
	//updateKilled();
	//changeAttributes();
	//技能冷却
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto spProNor = Sprite::create("GameScene/stencil.png");
	spProNor->setScale(1.2);//
	progressTimerNor = ProgressTimer::create(spProNor);
	progressTimerNor->setScale(1.2);//
	progressTimerNor->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width - 100, visibleSize.height - 100)));
	progressTimerNor->setVisible(false);
	this->addChild(progressTimerNor, 100);

	//设置按钮不可用
	m_normalskill->setVisible(true);
	m_normalskill->setEnabled(false);

	// 设置精灵进度条为顺时针
	progressTimerNor->setVisible(true);//
	progressTimerNor->setType(ProgressTimer::Type::RADIAL);

	//让进度条执行一个动作 显示出动画效果 逐渐覆盖半透模板形成冷却效果;这里进行计时冷却动画的实现和时间控制
	auto pto = ProgressTo::create(1, 100);
	auto call = CallFuncN::create(this, callfuncN_selector(DJSkillButton::norSkillCoolDownCallBack));
	progressTimerNor->runAction(Sequence::create(pto, call, NULL));
}

void DJSkillButton::skillOneClickCallBack(Ref* obj)

{
	Scene* scene = Director::getInstance()->getRunningScene();
	Daji * hero = ((DJGameScene *)scene)->hero;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/DJ/animation.plist", "GameItem/DJ/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/DJ/animation.plist");
	auto cache = AnimationCache::getInstance();
	/*auto animation = Animation::createWithSpriteFrames(vecSpriteFrame, 0.2f);
	auto animate = Animate::create(animation);
	spriteRun->runAction(RepeatForever::create(animate));*/
	auto animation1 = cache->getAnimation("SkillThreeRight");
	animation1->setLoops(1);
	auto animate1 = Animate::create(animation1);
	hero->baseSprite->runAction(animate1);
	
	hero->setForce(210);
	hero->setArmor(83);
	hero->shoot();
	//setAttact(20);
	//hero->setForce(hero->getForce() + getAttact());
	//scope??
	//hero->shoot();
	//hero->setForce(hero->getForce() - getAttact());
	//updateKilled();
	//changeAttributes();

	//设置按钮不可用

	//技能冷却
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto spProOne = Sprite::create("GameScene/stencil.png");
	spProOne->setScale(0.7);//
	progressTimerOne = ProgressTimer::create(spProOne);
	progressTimerOne->setScale(0.7);//
	progressTimerOne->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 - 10), (visibleSize.height - visibleSize.height / 5 + 30))));
	progressTimerOne->setVisible(false);
	this->addChild(progressTimerOne, 100);
	m_SkillOne->setVisible(true);
	m_SkillOne->setEnabled(false);
	
	// 设置精灵进度条为顺时针
	progressTimerOne->setVisible(true);//
	progressTimerOne->setType(ProgressTimer::Type::RADIAL);

	//让进度条执行一个动作 显示出动画效果 逐渐覆盖半透模板形成冷却效果;这里进行计时冷却动画的实现和时间控制
	auto pto = ProgressTo::create(10, 100);
	auto call = CallFuncN::create(this, callfuncN_selector(DJSkillButton::skillOneCoolDownCallBack));
	progressTimerOne->runAction(Sequence::create(pto, call, NULL));
}

void DJSkillButton::skillTwoClickCallBack(Ref* obj)
{    
	Scene* scene = Director::getInstance()->getRunningScene();
	Daji * hero = ((DJGameScene *)scene)->hero;

	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/DJ/animation.plist", "GameItem/DJ/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/DJ/animation.plist");
	auto cache = AnimationCache::getInstance();
	/*auto animation = Animation::createWithSpriteFrames(vecSpriteFrame, 0.2f);
	auto animate = Animate::create(animation);
	spriteRun->runAction(RepeatForever::create(animate));*/
	auto animation1 = cache->getAnimation("SkillThreeRight");
	animation1->setLoops(1);
	auto animate1 = Animate::create(animation1);
	hero->baseSprite->runAction(animate1);
	
	hero->setForce(170);
	hero->setArmor(103);
	hero->shoot();
	/*
	setAttact(50);
	hero->setForce(hero->getForce() + getAttact());
	//scope??
	hero->shoot();
	hero->setForce(hero->getForce() - getAttact());
	updateKilled();
	changeAttributes();
	*/

	//技能冷却
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto spProTwo = Sprite::create("GameScene/stencil.png");
	spProTwo->setScale(0.7);//
	progressTimerTwo = ProgressTimer::create(spProTwo);
	progressTimerTwo->setScale(0.7);//
	progressTimerTwo->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 + 30), (visibleSize.height - visibleSize.height / 5 - 70))));
	progressTimerTwo->setVisible(false);
	this->addChild(progressTimerTwo, 100);
	//设置按钮不可用
	m_SkillTwo->setVisible(true);
	m_SkillTwo->setEnabled(false);

	// 设置精灵进度条为顺时针
	progressTimerTwo->setVisible(true);
	progressTimerTwo->setType(ProgressTimer::Type::RADIAL);

	//让进度条执行一个动作 显示出动画效果 逐渐覆盖半透模板形成冷却效果;这里进行计时冷却动画的实现和时间控制
	auto pto = ProgressTo::create(5, 100);
	auto call = CallFuncN::create(this, callfuncN_selector(DJSkillButton::skillTwoCoolDownCallBack));
	progressTimerTwo->runAction(Sequence::create(pto, call, NULL));
}

void DJSkillButton::skillThreeClickCallBack(Ref* obj)

{
	// 攻击函数
	Scene* scene = Director::getInstance()->getRunningScene();
	Daji * hero = ((DJGameScene *)scene)->hero;
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/DJ/animation.plist", "GameItem/DJ/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/DJ/animation.plist");
	auto cache = AnimationCache::getInstance();
	/*auto animation = Animation::createWithSpriteFrames(vecSpriteFrame, 0.2f);
	auto animate = Animate::create(animation);
	spriteRun->runAction(RepeatForever::create(animate));*/
	auto animation1 = cache->getAnimation("SkillThreeRight");
	animation1->setLoops(1);
	auto animate1 = Animate::create(animation1);
	hero->baseSprite->runAction(animate1);
	
	hero->setForce(230);
	hero->setArmor(83);
	hero->shoot();
	/*
	setAttact(80);
	hero->setForce(hero->getForce() + getAttact());
	//scope??
	hero->shoot();
	hero->setForce(hero->getForce() - getAttact());
	updateKilled();
	changeAttributes();
	*/

	//技能冷却 
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto spProThr = Sprite::create("GameScene/stencil.png");
	spProThr->setScale(0.7);//
	progressTimerThree = ProgressTimer::create(spProThr);
	progressTimerThree->setScale(0.7);//
	progressTimerThree->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 + 120), (visibleSize.height - visibleSize.height / 5 - 100))));
	progressTimerThree->setVisible(false);
	this->addChild(progressTimerThree, 100);

	//设置按钮不可用
	m_SkillThree->setVisible(true);
	m_SkillThree->setEnabled(false);

	// 设置精灵进度条为顺时针
	progressTimerThree->setVisible(true);
	progressTimerThree->setType(ProgressTimer::Type::RADIAL);
	//让进度条执行一个动作 显示出动画效果 逐渐覆盖半透模板形成冷却效果;这里进行计时冷却动画的实现和时间控制
	auto pto = ProgressTo::create(2, 100);
	auto call = CallFuncN::create(this, callfuncN_selector(DJSkillButton::skillThreeCoolDownCallBack));
	progressTimerThree->runAction(Sequence::create(pto, call, NULL));
}

/** 技能冷却完毕回调*/


void DJSkillButton::norSkillCoolDownCallBack(Node* node)

{
	//恢复触摸
	m_normalskill->setEnabled(true);
	m_normalskill->setVisible(true);
	//隐藏
	progressTimerNor->setVisible(false);
}

void DJSkillButton::skillOneCoolDownCallBack(Node* node)

{
	//sprite1->setVisible(false);

	//恢复触摸
	m_SkillOne->setEnabled(true);
	//隐藏
	progressTimerOne->setVisible(false);
}

void DJSkillButton::skillTwoCoolDownCallBack(Node* node)

{
	//恢复触摸
	m_SkillTwo->setEnabled(true);
	//隐藏
	progressTimerTwo->setVisible(false);
}

void DJSkillButton::skillThreeCoolDownCallBack(Node* node)

{
	//恢复触摸
	m_SkillThree->setEnabled(true);
	//隐藏
	progressTimerThree->setVisible(false);
}