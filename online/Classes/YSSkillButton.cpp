#include "YSSkillButton.h"
#include "GameMenuScene.h"
#include"SelectToPlayScene.h"
#include "BaseMonster.h"
#include "GameManager.h"
#include "Yase.h"
#include "YSGameScene.h"
USING_NS_CC;


bool YSSkillButton::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//透明的背景
	auto visibleSize = Director::getInstance()->getVisibleSize();

	/*
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *bg = Sprite::create("GameScene/unVisble.png");

	bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	addChild(bg);
	*/


	//正常显示按钮
	m_normalskill = MenuItemImage::create("GameScene/NorSkill.png", "GameScene/NorSkill.png");
	m_normalskill->setScale(0.3);
	m_normalskill->setTarget(this, menu_selector(YSSkillButton::norskillClickCallBack, this));
	m_normalskill->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width - 100, visibleSize.height - 100)));

	m_SkillOne = MenuItemImage::create("GameItem/YS/SkillFirst.png", "GameItem/YS/SkillFirst.png");
	m_SkillOne->setScale(0.7);
	m_SkillOne->setTarget(this, menu_selector(YSSkillButton::skillOneClickCallBack, this));
	m_SkillOne->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 - 10), (visibleSize.height - visibleSize.height / 5 + 30))));

	m_SkillTwo = MenuItemImage::create("GameItem/YS/SkillSecond.png", "GameItem/YS/SkillSecond.png");
	m_SkillTwo->setScale(0.7);
	m_SkillTwo->setTarget(this, menu_selector(YSSkillButton::skillTwoClickCallBack, this));
	m_SkillTwo->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 + 30), (visibleSize.height - visibleSize.height / 5 - 70))));



	m_SkillThree = MenuItemImage::create("GameItem/YS/SkillThird.png", "GameItem/YS/SkillThird.png");
	m_SkillThree->setScale(0.7);
	m_SkillThree->setTarget(this, menu_selector(YSSkillButton::skillThreeClickCallBack, this));
	m_SkillThree->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 + 120), (visibleSize.height - visibleSize.height / 5 - 100))));


	m_skills = Menu::create(m_normalskill, m_SkillOne, m_SkillTwo, m_SkillThree, nullptr);
	m_skills->setPosition(Vec2::ZERO);

	addChild(m_skills, 1);

	return true;
}

void YSSkillButton::onEnter() {
	Layer::onEnter();
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/YS/animation.plist", "GameItem/YS/animation.png");
	//AnimationCache::getInstance()->addAnimationsWithFile("GameItem/YS/animation.plist");
}

int YSSkillButton::getAttact() {
	return addAttact;
}
int YSSkillButton::getMonsterExp() {
	return addMonsterExp;
}
int YSSkillButton::getTowerExpDeath() {
	return addTowerExpDeath;
}
int YSSkillButton::getTowerExpOnce() {
	return addTowerExpOnce;
}

void YSSkillButton::setAttact(int temp) {
	this->addAttact = temp;
}

void YSSkillButton::updateLevel() {
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

void YSSkillButton::updateKilled() {
	auto scene = Director::getInstance()->getRunningScene();
	BaseMonster * hero = (BaseMonster*)scene->getChildByTag(HERO);
	if (hero->getCurrHp() == 0)
	{
		GameManager::getInstance()->KILLED = GameManager::getInstance()->KILLED + 1;
	}
}

void YSSkillButton::changeAttributes() {

	auto scene = Director::getInstance()->getRunningScene();
	BaseMonster * hero = (BaseMonster*)scene->getChildByTag(HERO);

	if (hero->monsterDistance <= hero->towerDistance)
	{
		auto currHp = hero->nearestMonster->getCurrHp();

		currHp = currHp - hero->getForce() - getAttact() + hero->nearestMonster->getArmor();//受到攻击Hp减少

		if (currHp <= 0)
		{
			currHp = 0;
		}
		hero->nearestMonster->setCurrHp(currHp);

		hero->nearestMonster->getHpBar()->setPercentage((currHp / hero->nearestMonster->getMaxHp()) * 100);//更新Hp

		if (currHp == 0)
		{
			GameManager::getInstance()->KILLS = GameManager::getInstance()->KILLS + 1;
			hero->nearestMonster->death();//消失啊 vector移除啊
			GameManager::getInstance()->MONEY = GameManager::getInstance()->MONEY + hero->nearestMonster->getMoney();
		}
		//加经验区分小兵和怪？
		GameManager::getInstance()->EXPERIENCE = GameManager::getInstance()->EXPERIENCE + getMonsterExp();
		updateLevel();
	}
	else if (hero->towerDistance < hero->monsterDistance) {

		auto currHp = hero->nearestTower->getCurrHp();

		currHp = currHp - hero->getForce() - getAttact() + hero->nearestMonster->getArmor();//受到攻击Hp减少

		if (currHp <= 0)
		{
			currHp = 0;
		}
		hero->nearestTower->setCurrHp(currHp);

		hero->nearestTower->getHpBar()->setPercentage((currHp / hero->nearestMonster->getMaxHp()) * 100);//更新Hp

		if (currHp == 0)
		{
			GameManager::getInstance()->KILLS = GameManager::getInstance()->KILLS + 1;
			hero->nearestTower->death();//消失啊 vector移除啊
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

void YSSkillButton::norskillClickCallBack(Ref* obj)

{
	Scene* scene = Director::getInstance()->getRunningScene();
	Yase * hero = ((YSGameScene *)scene)->hero;
	hero->setIsAttacking(4);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/YS/animation.plist", "GameItem/YS/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto cache = AnimationCache::getInstance();
	/*auto animation = Animation::createWithSpriteFrames(vecSpriteFrame, 0.2f);
	auto animate = Animate::create(animation);
	spriteRun->runAction(RepeatForever::create(animate));*/
	auto animation1 = cache->getAnimation("SkillThreeRight");
	animation1->setLoops(1);
	auto animate1 = Animate::create(animation1);
	hero->baseSprite->runAction(animate1);

	//setAttact(0);
	//hero->setForce(hero->getForce() + getAttact());
	//scope??
	//hero->shoot();
	//hero->setForce(hero->getForce() - getAttact());
	//updateKilled();
	//changeAttributes();
	hero->setForce(219);
	hero->setArmor(121);
	hero->shoot();
	hero->setIsAttacking(0);


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
	auto call = CallFuncN::create(this, callfuncN_selector(YSSkillButton::norSkillCoolDownCallBack));
	progressTimerNor->runAction(Sequence::create(pto, call, NULL));


}



void YSSkillButton::skillOneClickCallBack(Ref* obj)

{

	Scene* scene = Director::getInstance()->getRunningScene();
	Yase * hero = ((YSGameScene *)scene)->hero;
	hero->setIsAttacking(1);
	//auto scene = Director::getInstance()->getRunningScene();
	//BaseMonster* hero = (BaseMonster *)scene->getChildByTag(Hero);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/YS/animation.plist", "GameItem/YS/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto animation2 = cache->getAnimation("SkillThreeRight");//??
	auto action2 = Animate::create(animation2);
	hero->baseSprite->runAction(Sequence::create(action2, action2->reverse(), NULL));

	//setAttact(20);
	//hero->setForce(hero->getForce() + getAttact());
	//scope??
	//hero->shoot();
	//hero->setForce(hero->getForce() - getAttact());
	//updateKilled();
	//changeAttributes();
	hero->setForce(249);
	hero->setArmor(121);
	hero->shoot();
	hero->setIsAttacking(0);

	//技能冷却
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto spProOne = Sprite::create("GameScene/stencil.png");
	spProOne->setScale(0.7);//
	progressTimerOne = ProgressTimer::create(spProOne);
	progressTimerOne->setScale(0.7);//
	progressTimerOne->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 - 10), (visibleSize.height - visibleSize.height / 5 + 30))));
	progressTimerOne->setVisible(false);
	this->addChild(progressTimerOne, 100);

	//设置按钮不可用
	m_SkillOne->setVisible(true);
	m_SkillOne->setEnabled(false);

	// 设置精灵进度条为顺时针
	progressTimerOne->setVisible(true);//
	progressTimerOne->setType(ProgressTimer::Type::RADIAL);

	//让进度条执行一个动作 显示出动画效果 逐渐覆盖半透模板形成冷却效果;这里进行计时冷却动画的实现和时间控制
	auto pto = ProgressTo::create(5, 100);
	auto call = CallFuncN::create(this, callfuncN_selector(YSSkillButton::skillOneCoolDownCallBack));
	progressTimerOne->runAction(Sequence::create(pto, call, NULL));

}

void YSSkillButton::skillTwoClickCallBack(Ref* obj)

{
	Scene* scene = Director::getInstance()->getRunningScene();
	Yase * hero = ((YSGameScene *)scene)->hero;
	hero->setIsAttacking(2);
	//auto scene = Director::getInstance()->getRunningScene();
	//BaseMonster* hero = (BaseMonster *)scene->getChildByTag(Hero);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/YS/animation.plist", "GameItem/YS/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto animation2 = cache->getAnimation("SkillThreeRight");//??
	auto action2 = Animate::create(animation2);
	hero->baseSprite->runAction(Sequence::create(action2, action2->reverse(), NULL));

	/*
	setAttact(50);
	hero->setForce(hero->getForce() + getAttact());
	//scope??
	hero->shoot();
	hero->setForce(hero->getForce() - getAttact());
	updateKilled();
	changeAttributes();
	*/
	hero->setArmor(229);
	hero->setForce(131);
	hero->shoot();
	hero->setIsAttacking(0);

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
	auto call = CallFuncN::create(this, callfuncN_selector(YSSkillButton::skillTwoCoolDownCallBack));
	progressTimerTwo->runAction(Sequence::create(pto, call, NULL));
}

void YSSkillButton::skillThreeClickCallBack(Ref* obj)

{
	Scene* scene = Director::getInstance()->getRunningScene();
	Yase * hero = ((YSGameScene *)scene)->hero;
	hero->setIsAttacking(3);
	//auto scene = Director::getInstance()->getRunningScene();
	//BaseMonster* hero = (BaseMonster *)scene->getChildByTag(Hero);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/YS/animation.plist", "GameItem/YS/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto animation2 = cache->getAnimation("SkillThreeRight");//??
	auto action2 = Animate::create(animation2);
	hero->baseSprite->runAction(Sequence::create(action2, action2->reverse(), NULL));

	hero->setForce(262);
	hero->setArmor(121);
	hero->shoot();
	hero->setIsAttacking(0);
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
	auto call = CallFuncN::create(this, callfuncN_selector(YSSkillButton::skillThreeCoolDownCallBack));
	progressTimerThree->runAction(Sequence::create(pto, call, NULL));
}


void YSSkillButton::YSNorSkill()

{
	Scene* scene = Director::getInstance()->getRunningScene();
	Yase * hero = static_cast<Yase*>((static_cast<YSGameScene *>(scene))->getChildByName("enemy"));
	//hero->setIsAttacking(4);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/YS/animation.plist", "GameItem/YS/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto cache = AnimationCache::getInstance();
	/*auto animation = Animation::createWithSpriteFrames(vecSpriteFrame, 0.2f);
	auto animate = Animate::create(animation);
	spriteRun->runAction(RepeatForever::create(animate));*/
	auto animation1 = cache->getAnimation("SkillThreeRight");
	animation1->setLoops(1);
	auto animate1 = Animate::create(animation1);
	hero->baseSprite->runAction(animate1);

	//setAttact(0);
	//hero->setForce(hero->getForce() + getAttact());
	//scope??
	//hero->shoot();
	//hero->setForce(hero->getForce() - getAttact());
	//updateKilled();
	//changeAttributes();
	hero->setForce(219);
	hero->setArmor(121);
	hero->shoot();
	//hero->setIsAttacking(0);


	//技能冷却
	/*auto visibleSize = Director::getInstance()->getVisibleSize();
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
	auto call = CallFuncN::create(this, callfuncN_selector(YSSkillButton::norSkillCoolDownCallBack));
	progressTimerNor->runAction(Sequence::create(pto, call, NULL));*/


}



void YSSkillButton::YSSkillOne()

{

	Scene* scene = Director::getInstance()->getRunningScene();
	Yase * hero = static_cast<Yase*>((static_cast<YSGameScene *>(scene))->getChildByName("enemy"));	//hero->setIsAttacking(1);
	//auto scene = Director::getInstance()->getRunningScene();
	//BaseMonster* hero = (BaseMonster *)scene->getChildByTag(Hero);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/YS/animation.plist", "GameItem/YS/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto animation2 = cache->getAnimation("SkillThreeRight");//??
	auto action2 = Animate::create(animation2);
	hero->baseSprite->runAction(Sequence::create(action2, action2->reverse(), NULL));

	//setAttact(20);
	//hero->setForce(hero->getForce() + getAttact());
	//scope??
	//hero->shoot();
	//hero->setForce(hero->getForce() - getAttact());
	//updateKilled();
	//changeAttributes();
	hero->setForce(249);
	hero->setArmor(121);
	hero->shoot();
	//hero->setIsAttacking(0);

	//技能冷却
	/*auto visibleSize = Director::getInstance()->getVisibleSize();
	auto spProOne = Sprite::create("GameScene/stencil.png");
	spProOne->setScale(0.7);//
	progressTimerOne = ProgressTimer::create(spProOne);
	progressTimerOne->setScale(0.7);//
	progressTimerOne->setPosition(Director::getInstance()->convertToGL(Vec2((visibleSize.width - visibleSize.width / 5 - 10), (visibleSize.height - visibleSize.height / 5 + 30))));
	progressTimerOne->setVisible(false);
	this->addChild(progressTimerOne, 100);

	//设置按钮不可用
	m_SkillOne->setVisible(true);
	m_SkillOne->setEnabled(false);

	// 设置精灵进度条为顺时针
	progressTimerOne->setVisible(true);//
	progressTimerOne->setType(ProgressTimer::Type::RADIAL);

	//让进度条执行一个动作 显示出动画效果 逐渐覆盖半透模板形成冷却效果;这里进行计时冷却动画的实现和时间控制
	auto pto = ProgressTo::create(5, 100);
	auto call = CallFuncN::create(this, callfuncN_selector(YSSkillButton::skillOneCoolDownCallBack));
	progressTimerOne->runAction(Sequence::create(pto, call, NULL));*/

}

void YSSkillButton::YSSkillTwo()

{
	Scene* scene = Director::getInstance()->getRunningScene();
	Yase * hero = static_cast<Yase*>((static_cast<YSGameScene *>(scene))->getChildByName("enemy"));	//hero->setIsAttacking(2);
	//auto scene = Director::getInstance()->getRunningScene();
	//BaseMonster* hero = (BaseMonster *)scene->getChildByTag(Hero);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/YS/animation.plist", "GameItem/YS/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto animation2 = cache->getAnimation("SkillThreeRight");//??
	auto action2 = Animate::create(animation2);
	hero->baseSprite->runAction(Sequence::create(action2, action2->reverse(), NULL));

	/*
	setAttact(50);
	hero->setForce(hero->getForce() + getAttact());
	//scope??
	hero->shoot();
	hero->setForce(hero->getForce() - getAttact());
	updateKilled();
	changeAttributes();
	*/
	hero->setArmor(229);
	hero->setForce(131);
	hero->shoot();
	//hero->setIsAttacking(0);

	//技能冷却
	/*auto visibleSize = Director::getInstance()->getVisibleSize();
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
	auto call = CallFuncN::create(this, callfuncN_selector(YSSkillButton::skillTwoCoolDownCallBack));
	progressTimerTwo->runAction(Sequence::create(pto, call, NULL));*/
}

void YSSkillButton::YSSkillThree()

{
	Scene* scene = Director::getInstance()->getRunningScene();
	Yase * hero = static_cast<Yase*>((static_cast<YSGameScene *>(scene))->getChildByName("enemy"));	//hero->setIsAttacking(3);
	//hero->setIsAttacking(3);
	//auto scene = Director::getInstance()->getRunningScene();
	//BaseMonster* hero = (BaseMonster *)scene->getChildByTag(Hero);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameItem/YS/animation.plist", "GameItem/YS/animation.png");
	AnimationCache::getInstance()->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile("GameItem/YS/animation.plist");
	auto animation2 = cache->getAnimation("SkillThreeRight");//??
	auto action2 = Animate::create(animation2);
	hero->baseSprite->runAction(Sequence::create(action2, action2->reverse(), NULL));

	hero->setForce(262);
	hero->setArmor(121);
	hero->shoot();
	//hero->setIsAttacking(0);
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
	/*auto visibleSize = Director::getInstance()->getVisibleSize();
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
	auto call = CallFuncN::create(this, callfuncN_selector(YSSkillButton::skillThreeCoolDownCallBack));
	progressTimerThree->runAction(Sequence::create(pto, call, NULL));*/
}



/** 技能冷却完毕回调*/

void YSSkillButton::norSkillCoolDownCallBack(Node* node)

{
	//恢复触摸
	m_normalskill->setEnabled(true);
	m_normalskill->setVisible(true);
	//隐藏
	progressTimerNor->setVisible(false);
}

void YSSkillButton::skillOneCoolDownCallBack(Node* node)

{
	//恢复触摸
	m_SkillOne->setEnabled(true);
	m_SkillOne->setVisible(true);
	//隐藏
	progressTimerOne->setVisible(false);
}

void YSSkillButton::skillTwoCoolDownCallBack(Node* node)

{
	//恢复触摸
	m_SkillTwo->setEnabled(true);
	m_SkillTwo->setVisible(true);
	//隐藏
	progressTimerTwo->setVisible(false);
}

void YSSkillButton::skillThreeCoolDownCallBack(Node* node)

{
	//恢复触摸
	m_SkillThree->setEnabled(true);
	m_SkillThree->setVisible(true);
	//隐藏
	progressTimerThree->setVisible(false);
}

