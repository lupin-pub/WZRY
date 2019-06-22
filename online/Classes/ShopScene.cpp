#include "ShopScene.h"
#include "SimpleAudioEngine.h"
#include "GameMenuScene.h"
#include "BaseMonster.h"
#include "Daji.h"
#include "Houyi.h"
#include "Yase.h"
#include "GameManager.h"
#include "DJGameScene.h"
#include "HYGameScene.h"
#include "YSGameScene.h"

USING_NS_CC;


bool ShopScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/*
	Sprite *bg = Sprite::create("GameScene/Halfvisible.png");
	bg->setScale(visibleSize.height / bg->getContentSize().height);
	bg->setPosition(Vec2(origin.x + visibleSize.width/2,
						 origin.y + visibleSize.height/ 2));
	this->addChild(bg);
	*/

	//装备类型
	//攻击
	auto labelAttact = Label::createWithSystemFont("Attact equipments", "Arial", 18);
	labelAttact->setPosition(Director::getInstance()->convertToGL(Vec2(80, 12)));
	addChild(labelAttact);
	//防御
	auto labelDefense = Label::createWithSystemFont("Defense equipments", "Arial", 18);
	labelDefense->setPosition(Director::getInstance()->convertToGL(Vec2(90, 94)));
	addChild(labelDefense);
	//移动
	auto labelMove = Label::createWithSystemFont("Move equipments", "Arial", 18);
	labelMove->setPosition(Director::getInstance()->convertToGL(Vec2(80, 176)));
	addChild(labelMove);
	//法术
	auto labelSpell = Label::createWithSystemFont("Spell equipments", "Arial", 18);
	labelSpell->setPosition(Director::getInstance()->convertToGL(Vec2(100, 340)));
	addChild(labelSpell);
	//打野
	auto labelPlayingWild = Label::createWithSystemFont("Playing Wild equipments", "Arial", 18);
	labelPlayingWild->setPosition(Director::getInstance()->convertToGL(Vec2(80, 258)));
	addChild(labelPlayingWild);

	//加钱
	/*
	auto instance = GameManager::getInstance();
	String* Money = String::createWithFormat("Money:%d", instance->MONEY);
	auto lableMoney = Label::createWithSystemFont(Money->getCString(), "Arial", 18);
	lableMoney->setPosition(Director::getInstance()->convertToGL(Vec2(120, 500)));
	addChild(lableMoney);
	*/

	//辅助 空

	//界面类型
	auto labelTitle = Label::createWithSystemFont("The Equipments of your hero", "Arial", 18);
	labelTitle->setPosition(Director::getInstance()->convertToGL(Vec2(120, 422)));
	addChild(labelTitle);

	auto labelBack = Label::createWithSystemFont("Back", "Arial", 24);
	auto BackmenuItem = MenuItemLabel::create(labelBack, CC_CALLBACK_1(ShopScene::menuBackItemCallback, this));

	BackmenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(480, 505)));
	//最下面位置
	Menu * menu = Menu::create(BackmenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	BuyFau = Label::createWithSystemFont("Failed purchase", "Arial", 24);
	BuyFau->setPosition(Director::getInstance()->convertToGL(Vec2(480,550)));
	BuyFau->setVisible(false);
	this->addChild(BuyFau);

	BuySus = Label::createWithSystemFont("Successful purchase", "Arial", 24);
	BuySus->setPosition(Director::getInstance()->convertToGL(Vec2(480, 550)));
	BuySus->setVisible(false);
	this->addChild(BuySus);

	//创建装备精灵对象 并 监听
	initAttactPicture();
	initDefensePicture();
	initMovePicture();
	initPlayingWildPicture();
	initSpellPicture();
	return true;
}

void ShopScene::menuBackItemCallback(cocos2d::Ref * ref) {

	BuyFau->setVisible(false);
	BuyFau->setVisible(false);
	//Director::getInstance()->popScene();
	auto scene = Director::getInstance()->getRunningScene();
	ShopScene * ShopLayer = (ShopScene *)scene->getChildByTag(SHOP);
	ShopLayer->setVisible(false);
	ShopLayer->setTouchEnabled(false);
	GameMenuScene * MenuLayer = (GameMenuScene *)scene->getChildByTag(MENU);
	MenuLayer->getScoreItem()->setEnabled(true);//
	MenuLayer->getHomeItem()->setEnabled(true);
}

bool ShopScene::onTouchAttactBegan(Touch * touch, Event * event) {

	//获取事件绑定的 target
	auto target = static_cast<ShopAttactIcon *>(event->getCurrentTarget());
	auto target1 = static_cast<ShopAttactIcon *>(event->getCurrentTarget())->icon;
	Vec2 locationInNode = target1->convertToNodeSpace(touch->getLocation());
	Size s = target1->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode)) {
		//连续点击时正常显示购买提示
		BuyFau->setVisible(false);
		BuySus->setVisible(false);

		//判断是否为第一次购买装备
		//清楚上一次装备所改的属性
		//改变这次属性

		auto instance = GameManager::getInstance();

		if (target->getMoney() <= instance->MONEY) {
			//购买成功
			//显示购买的装备
			target->setPosition(Director::getInstance()->convertToGL(Vec2(300, 460)));
			BuySus->setVisible(true);
			//英雄
			Scene* scene = Director::getInstance()->getRunningScene();
			int tag = scene->getTag();
			Daji * Dhero = NULL;
			Houyi * Hhero = NULL;
			Yase * Yhero = NULL;
			if (tag == Dj) {
				Dhero = ((DJGameScene *)scene)->hero;

				if (shopIconsAttact.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Dhero->setForce(Dhero->getForce() + target->getAttact());
					Dhero->setArmor(Dhero->getArmor() + target->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() + target->getAttact());
					if ((Dhero->getCurrHp() + target->getHP()) > Dhero->getMaxHp())
						Dhero->setCurrHp(Dhero->getMaxHp());
					else {
						Dhero->setCurrHp(Dhero->getCurrHp() + target->getHP());
					}

					shopIconsAttact.pushBack(target);
				}
				else {
					//非第一次购买
					ShopAttactIcon* lastIcon = shopIconsAttact.back();
					//属性清除
					Dhero->setForce(Dhero->getForce() - lastIcon->getAttact());
					Dhero->setArmor(Dhero->getArmor() - lastIcon->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() - lastIcon->getAttact());
					if ((Dhero->getCurrHp() - lastIcon->getHP()) > 0) {
						Dhero->setCurrHp(Dhero->getCurrHp() - lastIcon->getHP());
					}

					shopIconsAttact.popBack();

					//属性更改
					Dhero->setForce(Dhero->getForce() + target->getAttact());
					Dhero->setArmor(Dhero->getArmor() + target->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() + target->getAttact());
					if ((Dhero->getCurrHp() + target->getHP()) > Dhero->getMaxHp())
						Dhero->setCurrHp(Dhero->getMaxHp());
					else {
						Dhero->setCurrHp(Dhero->getCurrHp() + target->getHP());
					}

					shopIconsAttact.pushBack(target);

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

				}
			}
			else if (tag == Hy) {
				Hhero = ((HYGameScene *)scene)->hero;

				if (shopIconsAttact.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Hhero->setForce(Hhero->getForce() + target->getAttact());
					Hhero->setArmor(Hhero->getArmor() + target->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() + target->getAttact());
					if ((Hhero->getCurrHp() + target->getHP()) > Hhero->getMaxHp())
						Hhero->setCurrHp(Hhero->getMaxHp());
					else {
						Hhero->setCurrHp(Hhero->getCurrHp() + target->getHP());
					}
					shopIconsAttact.pushBack(target);
				}
				else {
					//非第一次购买
					ShopAttactIcon* lastIcon = shopIconsAttact.back();
					//属性清除
					Hhero->setForce(Hhero->getForce() - lastIcon->getAttact());
					Hhero->setArmor(Hhero->getArmor() - lastIcon->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() - lastIcon->getAttact());
					if ((Hhero->getCurrHp() - lastIcon->getHP()) > 0) {
						Hhero->setCurrHp(Hhero->getCurrHp() - lastIcon->getHP());
					}

					shopIconsAttact.popBack();

					//属性更改
					Hhero->setForce(Hhero->getForce() + target->getAttact());
					Hhero->setArmor(Hhero->getArmor() + target->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() + target->getAttact());
					if ((Hhero->getCurrHp() + target->getHP()) > Hhero->getMaxHp())
						Hhero->setCurrHp(Hhero->getMaxHp());
					else {
						Hhero->setCurrHp(Hhero->getCurrHp() + target->getHP());
					}

					shopIconsAttact.pushBack(target);

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

				}
			}
			/*
			else if (tag == Ys) {
				Yhero = ((YSGameScene *)scene)->hero;
				if (shopIconsAttact.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Yhero->setForce(Yhero->getForce() + target->getAttact());
					Yhero->setArmor(Yhero->getArmor() + target->getAttact());
					Yhero->setRunSpeed(Yhero->getRunSpeed() + target->getAttact());
					if ((Yhero->getCurrHp() + target->getHP()) > Yhero->getMaxHp())
						Yhero->setCurrHp(Yhero->getMaxHp());
					else {
						Yhero->setCurrHp(Yhero->getCurrHp() + target->getHP());
					}
					shopIconsAttact.pushBack(target);
			}
				else {
				//非第一次购买
				ShopAttactIcon* lastIcon = shopIconsAttact.back();
				//属性清除
				Yhero->setForce(Yhero->getForce() - lastIcon->getAttact());
				Yhero->setArmor(Yhero->getArmor() - lastIcon->getAttact());
				Yhero->setRunSpeed(Yhero->getRunSpeed() - lastIcon->getAttact());
				if ((Yhero->getCurrHp() - lastIcon->getHP()) > 0){
					Yhero->setCurrHp(Yhero->getCurrHp() - lastIcon->getHP());
				}

				shopIconsAttact.popBack();

				//属性更改
				Yhero->setForce(Yhero->getForce() + target->getAttact());
				Yhero->setArmor(Yhero->getArmor() + target->getAttact());
				Yhero->setRunSpeed(Yhero->getRunSpeed() + target->getAttact());
				if ((Yhero->getCurrHp() + target->getHP()) > Yhero->getMaxHp())
					Yhero->setCurrHp(Yhero->getMaxHp());
				else {
					Yhero->setCurrHp(Yhero->getCurrHp() + target->getHP());
				}

				shopIconsAttact.pushBack(target);

				//扣钱
				instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

			}
			}
			*/
		}
		else {
			//购买失败 消息提示？
			BuyFau->setVisible(true);
			return false;
		}
		return true;
	}
	return false;
}

bool ShopScene::onTouchDefenseBegan(Touch * touch, Event * event) {
	//获取事件绑定的 target
	auto target = static_cast<ShopDefenseIcon *>(event->getCurrentTarget());
	auto target1 = static_cast<ShopDefenseIcon *>(event->getCurrentTarget())->icon;
	Vec2 locationInNode = target1->convertToNodeSpace(touch->getLocation());
	Size s = target1->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode)) {
		//连续点击时正常显示购买提示
		BuyFau->setVisible(false);
		BuySus->setVisible(false);

		//判断是否为第一次购买装备
		//清楚上一次装备所改的属性
		//改变这次属性

		auto instance = GameManager::getInstance();

		if (target->getMoney() <= instance->MONEY) {
			//购买成功
			//显示购买的装备
			target->setPosition(Director::getInstance()->convertToGL(Vec2(375, 460)));
			BuySus->setVisible(true);
			//英雄
			Scene* scene = Director::getInstance()->getRunningScene();
			int tag = scene->getTag();
			Daji * Dhero = NULL;
			Houyi * Hhero = NULL;
			Yase * Yhero = NULL;
			if (tag == Dj) {
				Dhero = ((DJGameScene *)scene)->hero;

				if (shopIconsDefense.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Dhero->setForce(Dhero->getForce() + target->getAttact());
					Dhero->setArmor(Dhero->getArmor() + target->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() + target->getAttact());
					if ((Dhero->getCurrHp() + target->getHP()) > Dhero->getMaxHp())
						Dhero->setCurrHp(Dhero->getMaxHp());
					else {
						Dhero->setCurrHp(Dhero->getCurrHp() + target->getHP());
					}

					shopIconsDefense.pushBack(target);
				}
				else {
					//非第一次购买
					ShopDefenseIcon* lastIcon = shopIconsDefense.back();
					//属性清除
					Dhero->setForce(Dhero->getForce() - lastIcon->getAttact());
					Dhero->setArmor(Dhero->getArmor() - lastIcon->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() - lastIcon->getAttact());
					if ((Dhero->getCurrHp() - lastIcon->getHP()) > 0) {
						Dhero->setCurrHp(Dhero->getCurrHp() - lastIcon->getHP());
					}

					shopIconsDefense.popBack();

					//属性更改
					Dhero->setForce(Dhero->getForce() + target->getAttact());
					Dhero->setArmor(Dhero->getArmor() + target->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() + target->getAttact());
					if ((Dhero->getCurrHp() + target->getHP()) > Dhero->getMaxHp())
						Dhero->setCurrHp(Dhero->getMaxHp());
					else {
						Dhero->setCurrHp(Dhero->getCurrHp() + target->getHP());
					}

					shopIconsDefense.pushBack(target);

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

				}
			}
			else if (tag == Hy) {
				Hhero = ((HYGameScene *)scene)->hero;

				if (shopIconsDefense.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Hhero->setForce(Hhero->getForce() + target->getAttact());
					Hhero->setArmor(Hhero->getArmor() + target->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() + target->getAttact());
					if ((Hhero->getCurrHp() + target->getHP()) > Hhero->getMaxHp())
						Hhero->setCurrHp(Hhero->getMaxHp());
					else {
						Hhero->setCurrHp(Hhero->getCurrHp() + target->getHP());
					}
					shopIconsDefense.pushBack(target);
				}
				else {
					//非第一次购买
					ShopDefenseIcon* lastIcon = shopIconsDefense.back();
					//属性清除
					Hhero->setForce(Hhero->getForce() - lastIcon->getAttact());
					Hhero->setArmor(Hhero->getArmor() - lastIcon->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() - lastIcon->getAttact());
					if ((Hhero->getCurrHp() - lastIcon->getHP()) > 0) {
						Hhero->setCurrHp(Hhero->getCurrHp() - lastIcon->getHP());
					}

					shopIconsDefense.popBack();

					//属性更改
					Hhero->setForce(Hhero->getForce() + target->getAttact());
					Hhero->setArmor(Hhero->getArmor() + target->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() + target->getAttact());
					if ((Hhero->getCurrHp() + target->getHP()) > Hhero->getMaxHp())
						Hhero->setCurrHp(Hhero->getMaxHp());
					else {
						Hhero->setCurrHp(Hhero->getCurrHp() + target->getHP());
					}

					shopIconsDefense.pushBack(target);

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

				}
			}
			/*
			else if (tag == Ys) {
				Yhero = ((YSGameScene *)scene)->hero;
				if (shopIconsDefense.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Yhero->setForce(Yhero->getForce() + target->getAttact());
					Yhero->setArmor(Yhero->getArmor() + target->getAttact());
					Yhero->setRunSpeed(Yhero->getRunSpeed() + target->getAttact());
					if ((Yhero->getCurrHp() + target->getHP()) > Yhero->getMaxHp())
						Yhero->setCurrHp(Yhero->getMaxHp());
					else {
						Yhero->setCurrHp(Yhero->getCurrHp() + target->getHP());
					}
					shopIconsDefense.pushBack(target);
			}
				else {
				//非第一次购买
				ShopDefenseIcon* lastIcon = shopIconsDefense.back();
				//属性清除
				Yhero->setForce(Yhero->getForce() - lastIcon->getAttact());
				Yhero->setArmor(Yhero->getArmor() - lastIcon->getAttact());
				Yhero->setRunSpeed(Yhero->getRunSpeed() - lastIcon->getAttact());
				if ((Yhero->getCurrHp() - lastIcon->getHP()) > 0){
					Yhero->setCurrHp(Yhero->getCurrHp() - lastIcon->getHP());
				}

				shopIconsDefense.popBack();

				//属性更改
				Yhero->setForce(Yhero->getForce() + target->getAttact());
				Yhero->setArmor(Yhero->getArmor() + target->getAttact());
				Yhero->setRunSpeed(Yhero->getRunSpeed() + target->getAttact());
				if ((Yhero->getCurrHp() + target->getHP()) > Yhero->getMaxHp())
					Yhero->setCurrHp(Yhero->getMaxHp());
				else {
					Yhero->setCurrHp(Yhero->getCurrHp() + target->getHP());
				}

				shopIconsDefense.pushBack(target);

				//扣钱
				instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

			}
			}
			*/
		}
		else {
			//购买失败 消息提示？
			BuyFau->setVisible(true);
			return false;
		}
		return true;
	}
	return false;
}

bool ShopScene::onTouchMoveBegan(Touch * touch, Event * event) {
	//获取事件绑定的 target
	auto target = static_cast<ShopMoveIcon *>(event->getCurrentTarget());
	auto target1 = static_cast<ShopMoveIcon *>(event->getCurrentTarget())->icon;
	Vec2 locationInNode = target1->convertToNodeSpace(touch->getLocation());
	Size s = target1->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	//单击范围判断检测
	if (rect.containsPoint(locationInNode)) {
		//连续点击时正常显示购买提示
		BuyFau->setVisible(false);
		BuySus->setVisible(false);

		//判断是否为第一次购买装备
		//清楚上一次装备所改的属性
		//改变这次属性

		auto instance = GameManager::getInstance();

		if (target->getMoney() <= instance->MONEY) {
			//购买成功
			//显示购买的装备
			target->setPosition(Director::getInstance()->convertToGL(Vec2(450, 460)));
			BuySus->setVisible(true);
			//英雄
			Scene* scene = Director::getInstance()->getRunningScene();
			int tag = scene->getTag();
			Daji * Dhero = NULL;
			Houyi * Hhero = NULL;
			Yase * Yhero = NULL;
			if (tag == Dj) {
				Dhero = ((DJGameScene *)scene)->hero;

				if (shopIconsMove.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Dhero->setForce(Dhero->getForce() + target->getAttact());
					Dhero->setArmor(Dhero->getArmor() + target->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() + target->getAttact());
					if ((Dhero->getCurrHp() + target->getHP()) > Dhero->getMaxHp())
						Dhero->setCurrHp(Dhero->getMaxHp());
					else {
						Dhero->setCurrHp(Dhero->getCurrHp() + target->getHP());
					}

					shopIconsMove.pushBack(target);
				}
				else {
					//非第一次购买
					ShopMoveIcon* lastIcon = shopIconsMove.back();
					//属性清除
					Dhero->setForce(Dhero->getForce() - lastIcon->getAttact());
					Dhero->setArmor(Dhero->getArmor() - lastIcon->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() - lastIcon->getAttact());
					if ((Dhero->getCurrHp() - lastIcon->getHP()) > 0) {
						Dhero->setCurrHp(Dhero->getCurrHp() - lastIcon->getHP());
					}

					shopIconsMove.popBack();

					//属性更改
					Dhero->setForce(Dhero->getForce() + target->getAttact());
					Dhero->setArmor(Dhero->getArmor() + target->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() + target->getAttact());
					if ((Dhero->getCurrHp() + target->getHP()) > Dhero->getMaxHp())
						Dhero->setCurrHp(Dhero->getMaxHp());
					else {
						Dhero->setCurrHp(Dhero->getCurrHp() + target->getHP());
					}

					shopIconsMove.pushBack(target);

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

				}
			}
			else if (tag == Hy) {
				Hhero = ((HYGameScene *)scene)->hero;

				if (shopIconsMove.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Hhero->setForce(Hhero->getForce() + target->getAttact());
					Hhero->setArmor(Hhero->getArmor() + target->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() + target->getAttact());
					if ((Hhero->getCurrHp() + target->getHP()) > Hhero->getMaxHp())
						Hhero->setCurrHp(Hhero->getMaxHp());
					else {
						Hhero->setCurrHp(Hhero->getCurrHp() + target->getHP());
					}
					shopIconsMove.pushBack(target);
				}
				else {
					//非第一次购买
					ShopMoveIcon* lastIcon = shopIconsMove.back();
					//属性清除
					Hhero->setForce(Hhero->getForce() - lastIcon->getAttact());
					Hhero->setArmor(Hhero->getArmor() - lastIcon->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() - lastIcon->getAttact());
					if ((Hhero->getCurrHp() - lastIcon->getHP()) > 0) {
						Hhero->setCurrHp(Hhero->getCurrHp() - lastIcon->getHP());
					}

					shopIconsMove.popBack();

					//属性更改
					Hhero->setForce(Hhero->getForce() + target->getAttact());
					Hhero->setArmor(Hhero->getArmor() + target->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() + target->getAttact());
					if ((Hhero->getCurrHp() + target->getHP()) > Hhero->getMaxHp())
						Hhero->setCurrHp(Hhero->getMaxHp());
					else {
						Hhero->setCurrHp(Hhero->getCurrHp() + target->getHP());
					}

					shopIconsMove.pushBack(target);

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

				}
			}
			/*
			else if (tag == Ys) {
				Yhero = ((YSGameScene *)scene)->hero;
				if (shopIconsMove.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Yhero->setForce(Yhero->getForce() + target->getAttact());
					Yhero->setArmor(Yhero->getArmor() + target->getAttact());
					Yhero->setRunSpeed(Yhero->getRunSpeed() + target->getAttact());
					if ((Yhero->getCurrHp() + target->getHP()) > Yhero->getMaxHp())
						Yhero->setCurrHp(Yhero->getMaxHp());
					else {
						Yhero->setCurrHp(Yhero->getCurrHp() + target->getHP());
					}
					shopIconsMove.pushBack(target);
			}
				else {
				//非第一次购买
				ShopMoveIcon* lastIcon = shopIconsMove.back();
				//属性清除
				Yhero->setForce(Yhero->getForce() - lastIcon->getAttact());
				Yhero->setArmor(Yhero->getArmor() - lastIcon->getAttact());
				Yhero->setRunSpeed(Yhero->getRunSpeed() - lastIcon->getAttact());
				if ((Yhero->getCurrHp() - lastIcon->getHP()) > 0){
					Yhero->setCurrHp(Yhero->getCurrHp() - lastIcon->getHP());
				}

				shopIconsMove.popBack();

				//属性更改
				Yhero->setForce(Yhero->getForce() + target->getAttact());
				Yhero->setArmor(Yhero->getArmor() + target->getAttact());
				Yhero->setRunSpeed(Yhero->getRunSpeed() + target->getAttact());
				if ((Yhero->getCurrHp() + target->getHP()) > Yhero->getMaxHp())
					Yhero->setCurrHp(Yhero->getMaxHp());
				else {
					Yhero->setCurrHp(Yhero->getCurrHp() + target->getHP());
				}

				shopIconsMove.pushBack(target);

				//扣钱
				instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

			}
			}
			*/
		}
		else {
			//购买失败 消息提示？
			BuyFau->setVisible(true);
			return false;
		}
		return true;
	}
	return false;
}

bool ShopScene::onTouchSpellBegan(Touch * touch, Event * event) {
	//获取事件绑定的 target
	auto target = static_cast<ShopSpellIcon *>(event->getCurrentTarget());
	auto target1 = static_cast<ShopSpellIcon *>(event->getCurrentTarget())->icon;
	Vec2 locationInNode = target1->convertToNodeSpace(touch->getLocation());
	Size s = target1->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode)) {
		//连续点击时正常显示购买提示
		BuyFau->setVisible(false);
		BuySus->setVisible(false);

		//判断是否为第一次购买装备
		//清楚上一次装备所改的属性
		//改变这次属性

		auto instance = GameManager::getInstance();

		if (target->getMoney() <= instance->MONEY) {
			//购买成功
			//显示购买的装备
			target->setPosition(Director::getInstance()->convertToGL(Vec2(600, 460)));
			BuySus->setVisible(true);
			//英雄
			Scene* scene = Director::getInstance()->getRunningScene();
			int tag = scene->getTag();
			Daji * Dhero = NULL;
			Houyi * Hhero = NULL;
			Yase * Yhero = NULL;
			if (tag == Dj) {
				Dhero = ((DJGameScene *)scene)->hero;

				if (shopIconsSpell.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Dhero->setForce(Dhero->getForce() + target->getAttact());
					Dhero->setArmor(Dhero->getArmor() + target->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() + target->getAttact());
					if ((Dhero->getCurrHp() + target->getHP()) > Dhero->getMaxHp())
						Dhero->setCurrHp(Dhero->getMaxHp());
					else {
						Dhero->setCurrHp(Dhero->getCurrHp() + target->getHP());
					}

					shopIconsSpell.pushBack(target);
				}
				else {
					//非第一次购买
					ShopSpellIcon* lastIcon = shopIconsSpell.back();
					//属性清除
					Dhero->setForce(Dhero->getForce() - lastIcon->getAttact());
					Dhero->setArmor(Dhero->getArmor() - lastIcon->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() - lastIcon->getAttact());
					if ((Dhero->getCurrHp() - lastIcon->getHP()) > 0) {
						Dhero->setCurrHp(Dhero->getCurrHp() - lastIcon->getHP());
					}

					shopIconsSpell.popBack();

					//属性更改
					Dhero->setForce(Dhero->getForce() + target->getAttact());
					Dhero->setArmor(Dhero->getArmor() + target->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() + target->getAttact());
					if ((Dhero->getCurrHp() + target->getHP()) > Dhero->getMaxHp())
						Dhero->setCurrHp(Dhero->getMaxHp());
					else {
						Dhero->setCurrHp(Dhero->getCurrHp() + target->getHP());
					}

					shopIconsSpell.pushBack(target);

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

				}
			}
			else if (tag == Hy) {
				Hhero = ((HYGameScene *)scene)->hero;

				if (shopIconsSpell.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Hhero->setForce(Hhero->getForce() + target->getAttact());
					Hhero->setArmor(Hhero->getArmor() + target->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() + target->getAttact());
					if ((Hhero->getCurrHp() + target->getHP()) > Hhero->getMaxHp())
						Hhero->setCurrHp(Hhero->getMaxHp());
					else {
						Hhero->setCurrHp(Hhero->getCurrHp() + target->getHP());
					}
					shopIconsSpell.pushBack(target);
				}
				else {
					//非第一次购买
					ShopSpellIcon* lastIcon = shopIconsSpell.back();
					//属性清除
					Hhero->setForce(Hhero->getForce() - lastIcon->getAttact());
					Hhero->setArmor(Hhero->getArmor() - lastIcon->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() - lastIcon->getAttact());
					if ((Hhero->getCurrHp() - lastIcon->getHP()) > 0) {
						Hhero->setCurrHp(Hhero->getCurrHp() - lastIcon->getHP());
					}

					shopIconsSpell.popBack();

					//属性更改
					Hhero->setForce(Hhero->getForce() + target->getAttact());
					Hhero->setArmor(Hhero->getArmor() + target->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() + target->getAttact());
					if ((Hhero->getCurrHp() + target->getHP()) > Hhero->getMaxHp())
						Hhero->setCurrHp(Hhero->getMaxHp());
					else {
						Hhero->setCurrHp(Hhero->getCurrHp() + target->getHP());
					}

					shopIconsSpell.pushBack(target);

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

				}
			}
			/*
			else if (tag == Ys) {
				Yhero = ((YSGameScene *)scene)->hero;
				if (shopIconsSpell.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Yhero->setForce(Yhero->getForce() + target->getAttact());
					Yhero->setArmor(Yhero->getArmor() + target->getAttact());
					Yhero->setRunSpeed(Yhero->getRunSpeed() + target->getAttact());
					if ((Yhero->getCurrHp() + target->getHP()) > Yhero->getMaxHp())
						Yhero->setCurrHp(Yhero->getMaxHp());
					else {
						Yhero->setCurrHp(Yhero->getCurrHp() + target->getHP());
					}
					shopIconsSpell.pushBack(target);
			}
				else {
				//非第一次购买
				ShopSpellIcon* lastIcon = shopIconsSpell.back();
				//属性清除
				Yhero->setForce(Yhero->getForce() - lastIcon->getAttact());
				Yhero->setArmor(Yhero->getArmor() - lastIcon->getAttact());
				Yhero->setRunSpeed(Yhero->getRunSpeed() - lastIcon->getAttact());
				if ((Yhero->getCurrHp() - lastIcon->getHP()) > 0){
					Yhero->setCurrHp(Yhero->getCurrHp() - lastIcon->getHP());
				}

				shopIconsSpell.popBack();

				//属性更改
				Yhero->setForce(Yhero->getForce() + target->getAttact());
				Yhero->setArmor(Yhero->getArmor() + target->getAttact());
				Yhero->setRunSpeed(Yhero->getRunSpeed() + target->getAttact());
				if ((Yhero->getCurrHp() + target->getHP()) > Yhero->getMaxHp())
					Yhero->setCurrHp(Yhero->getMaxHp());
				else {
					Yhero->setCurrHp(Yhero->getCurrHp() + target->getHP());
				}

				shopIconsSpell.pushBack(target);

				//扣钱
				instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

			}
			}
			*/
		}
		else {
			//购买失败 消息提示？
			BuyFau->setVisible(true);
			return false;
		}
		return true;
	}
	return false;
}

bool ShopScene::onTouchPlayingWildBegan(Touch * touch, Event * event) {
	//获取事件绑定的 target
	auto target = static_cast<ShopPlayingWildIcon *>(event->getCurrentTarget());
	auto target1 = static_cast<ShopPlayingWildIcon *>(event->getCurrentTarget())->icon;
	Vec2 locationInNode = target1->convertToNodeSpace(touch->getLocation());
	Size s = target1->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	//单击范围判断检测
	if (rect.containsPoint(locationInNode)) {
		//连续点击时正常显示购买提示
		BuyFau->setVisible(false);
		BuySus->setVisible(false);

		//判断是否为第一次购买装备
		//清楚上一次装备所改的属性
		//改变这次属性

		auto instance = GameManager::getInstance();

		if (target->getMoney() <= instance->MONEY) {
			//购买成功
			//显示购买的装备
			target->setPosition(Director::getInstance()->convertToGL(Vec2(525, 460)));
			BuySus->setVisible(true);
			//英雄
			Scene* scene = Director::getInstance()->getRunningScene();
			int tag = scene->getTag();
			Daji * Dhero = NULL;
			Houyi * Hhero = NULL;
			Yase * Yhero = NULL;
			if (tag == Dj) {
				Dhero = ((DJGameScene *)scene)->hero;

				if (shopIconsPlayingWild.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Dhero->setForce(Dhero->getForce() + target->getAttact());
					Dhero->setArmor(Dhero->getArmor() + target->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() + target->getAttact());
					if ((Dhero->getCurrHp() + target->getHP()) > Dhero->getMaxHp())
						Dhero->setCurrHp(Dhero->getMaxHp());
					else {
						Dhero->setCurrHp(Dhero->getCurrHp() + target->getHP());
					}

					shopIconsPlayingWild.pushBack(target);
				}
				else {
					//非第一次购买
					ShopPlayingWildIcon* lastIcon = shopIconsPlayingWild.back();
					//属性清除
					Dhero->setForce(Dhero->getForce() - lastIcon->getAttact());
					Dhero->setArmor(Dhero->getArmor() - lastIcon->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() - lastIcon->getAttact());
					if ((Dhero->getCurrHp() - lastIcon->getHP()) > 0) {
						Dhero->setCurrHp(Dhero->getCurrHp() - lastIcon->getHP());
					}

					shopIconsPlayingWild.popBack();

					//属性更改
					Dhero->setForce(Dhero->getForce() + target->getAttact());
					Dhero->setArmor(Dhero->getArmor() + target->getAttact());
					Dhero->setRunSpeed(Dhero->getRunSpeed() + target->getAttact());
					if ((Dhero->getCurrHp() + target->getHP()) > Dhero->getMaxHp())
						Dhero->setCurrHp(Dhero->getMaxHp());
					else {
						Dhero->setCurrHp(Dhero->getCurrHp() + target->getHP());
					}

					shopIconsPlayingWild.pushBack(target);

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

				}
			}
			else if (tag == Hy) {
				Hhero = ((HYGameScene *)scene)->hero;

				if (shopIconsPlayingWild.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Hhero->setForce(Hhero->getForce() + target->getAttact());
					Hhero->setArmor(Hhero->getArmor() + target->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() + target->getAttact());
					if ((Hhero->getCurrHp() + target->getHP()) > Hhero->getMaxHp())
						Hhero->setCurrHp(Hhero->getMaxHp());
					else {
						Hhero->setCurrHp(Hhero->getCurrHp() + target->getHP());
					}
					shopIconsPlayingWild.pushBack(target);
				}
				else {
					//非第一次购买
					ShopPlayingWildIcon* lastIcon = shopIconsPlayingWild.back();
					//属性清除
					Hhero->setForce(Hhero->getForce() - lastIcon->getAttact());
					Hhero->setArmor(Hhero->getArmor() - lastIcon->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() - lastIcon->getAttact());
					if ((Hhero->getCurrHp() - lastIcon->getHP()) > 0) {
						Hhero->setCurrHp(Hhero->getCurrHp() - lastIcon->getHP());
					}

					shopIconsPlayingWild.popBack();

					//属性更改
					Hhero->setForce(Hhero->getForce() + target->getAttact());
					Hhero->setArmor(Hhero->getArmor() + target->getAttact());
					Hhero->setRunSpeed(Hhero->getRunSpeed() + target->getAttact());
					if ((Hhero->getCurrHp() + target->getHP()) > Hhero->getMaxHp())
						Hhero->setCurrHp(Hhero->getMaxHp());
					else {
						Hhero->setCurrHp(Hhero->getCurrHp() + target->getHP());
					}

					shopIconsPlayingWild.pushBack(target);

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

				}
			}
			/*
			else if (tag == Ys) {
				Yhero = ((YSGameScene *)scene)->hero;
				if (shopIconsPlayingWild.empty()) {
					//第一次购买

					//移动装备的位置？

					//扣钱
					instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

					//修改英雄属性
					Yhero->setForce(Yhero->getForce() + target->getAttact());
					Yhero->setArmor(Yhero->getArmor() + target->getAttact());
					Yhero->setRunSpeed(Yhero->getRunSpeed() + target->getAttact());
					if ((Yhero->getCurrHp() + target->getHP()) > Yhero->getMaxHp())
						Yhero->setCurrHp(Yhero->getMaxHp());
					else {
						Yhero->setCurrHp(Yhero->getCurrHp() + target->getHP());
					}
					shopIconsPlayingWild.pushBack(target);
			}
				else {
				//非第一次购买
				ShopPlayingWildIcon* lastIcon = shopIconsPlayingWild.back();
				//属性清除
				Yhero->setForce(Yhero->getForce() - lastIcon->getAttact());
				Yhero->setArmor(Yhero->getArmor() - lastIcon->getAttact());
				Yhero->setRunSpeed(Yhero->getRunSpeed() - lastIcon->getAttact());
				if ((Yhero->getCurrHp() - lastIcon->getHP()) > 0){
					Yhero->setCurrHp(Yhero->getCurrHp() - lastIcon->getHP());
				}

				shopIconsPlayingWild.popBack();

				//属性更改
				Yhero->setForce(Yhero->getForce() + target->getAttact());
				Yhero->setArmor(Yhero->getArmor() + target->getAttact());
				Yhero->setRunSpeed(Yhero->getRunSpeed() + target->getAttact());
				if ((Yhero->getCurrHp() + target->getHP()) > Yhero->getMaxHp())
					Yhero->setCurrHp(Yhero->getMaxHp());
				else {
					Yhero->setCurrHp(Yhero->getCurrHp() + target->getHP());
				}

				shopIconsPlayingWild.pushBack(target);

				//扣钱
				instance->MONEY = GameManager::getInstance()->MONEY - target->getMoney();

			}
			}
			*/
		}
		else {
			//购买失败 消息提示？
			BuyFau->setVisible(true);
			return false;
		}
		return true;
	}
	return false;
}

void ShopScene::initAttactPicture()
{
	auto shopIcon1 = ShopAttactIcon::createAttactIcon(1);
	shopIcon1->setPosition(Director::getInstance()->convertToGL(Vec2(300, 45)));  //30x
	shopIcon1->setAttact(20);
	shopIcon1->setMoney(250);
	addChild(shopIcon1);
	//shopIconsAttact.pushBack(shopIcon1);
	auto label11 = Label::createWithSystemFont("Money:250", "Arial", 8);  //8是字体大小
	auto label12 = Label::createWithSystemFont("Physical attack: +20", "Arial", 8);
	label11->setPosition(Director::getInstance()->convertToGL(Vec2(300, 65)));  //要改
	label12->setPosition(Director::getInstance()->convertToGL(Vec2(300, 75)));
	addChild(label11);
	addChild(label12);

	auto shopIcon2 = ShopAttactIcon::createAttactIcon(2);
	shopIcon2->setPosition(Director::getInstance()->convertToGL(Vec2(375, 45)));//两个间隔横向4
	shopIcon2->setSpeed(10);//攻击速度10%
	shopIcon2->setMoney(116);
	addChild(shopIcon2);
	auto label21 = Label::createWithSystemFont("Money:290", "Arial", 8);
	auto label22 = Label::createWithSystemFont("Attact Speed: +10%", "Arial", 8);
	label21->setPosition(Director::getInstance()->convertToGL(Vec2(375, 65)));
	label22->setPosition(Director::getInstance()->convertToGL(Vec2(375, 75)));
	addChild(label21);
	addChild(label22);

	auto shopIcon3 = ShopAttactIcon::createAttactIcon(3);
	shopIcon3->setPosition(Director::getInstance()->convertToGL(Vec2(450, 45)));
	shopIcon3->setAttact(30);
	shopIcon3->setMoney(128);
	addChild(shopIcon3);
	auto label31 = Label::createWithSystemFont("Money:320", "Arial", 8);
	auto label32 = Label::createWithSystemFont("Physical attack: +30", "Arial", 8);
	label31->setPosition(Director::getInstance()->convertToGL(Vec2(450, 65)));
	label32->setPosition(Director::getInstance()->convertToGL(Vec2(450, 75)));
	addChild(label31);
	addChild(label32);

	auto shopIcon4 = ShopAttactIcon::createAttactIcon(4);
	shopIcon4->setPosition(Director::getInstance()->convertToGL(Vec2(525, 45)));
	shopIcon4->setAttact(35);
	shopIcon4->setMoney(164);
	addChild(shopIcon4);
	auto label41 = Label::createWithSystemFont("Money:410", "Arial", 8);
	auto label42 = Label::createWithSystemFont("Physical attack: +35", "Arial", 8);
	label41->setPosition(Director::getInstance()->convertToGL(Vec2(525, 65)));
	label42->setPosition(Director::getInstance()->convertToGL(Vec2(525, 75)));
	addChild(label41);
	addChild(label42);

	auto shopIcon5 = ShopAttactIcon::createAttactIcon(5);
	shopIcon5->setPosition(Director::getInstance()->convertToGL(Vec2(600, 45)));
	shopIcon5->setAttact(40);
	shopIcon5->setMoney(180);
	addChild(shopIcon5);
	auto label51 = Label::createWithSystemFont("Money:450", "Arial", 8);
	auto label52 = Label::createWithSystemFont("Physical attack: +40", "Arial", 8);
	label51->setPosition(Director::getInstance()->convertToGL(Vec2(600, 65)));
	label52->setPosition(Director::getInstance()->convertToGL(Vec2(600, 75)));
	addChild(label51);
	addChild(label52);

	auto shopIcon6 = ShopAttactIcon::createAttactIcon(6);
	shopIcon6->setPosition(Director::getInstance()->convertToGL(Vec2(675, 45)));
	shopIcon6->setAttact(60);
	shopIcon6->setMoney(220);
	addChild(shopIcon6);
	auto label61 = Label::createWithSystemFont("Money:550", "Arial", 8);
	auto label62 = Label::createWithSystemFont("Physical attack: +60", "Arial", 8);
	label61->setPosition(Director::getInstance()->convertToGL(Vec2(675, 65)));
	label62->setPosition(Director::getInstance()->convertToGL(Vec2(675, 75)));
	addChild(label61);
	addChild(label62);

	auto shopIcon7 = ShopAttactIcon::createAttactIcon(7);
	shopIcon7->setPosition(Director::getInstance()->convertToGL(Vec2(750, 45)));
	shopIcon7->setAttact(80);
	shopIcon7->setMoney(364);
	addChild(shopIcon7);
	auto label71 = Label::createWithSystemFont("Money:910", "Arial", 8);
	auto label72 = Label::createWithSystemFont("Physical attack: +80", "Arial", 8);
	label71->setPosition(Director::getInstance()->convertToGL(Vec2(750, 65)));
	label72->setPosition(Director::getInstance()->convertToGL(Vec2(750, 75)));
	addChild(label71);
	addChild(label72);

	auto shopIcon8 = ShopAttactIcon::createAttactIcon(8);
	shopIcon8->setPosition(Director::getInstance()->convertToGL(Vec2(825, 45)));
	shopIcon8->setAttact(40);
	shopIcon8->setHP(300);
	shopIcon8->setMoney(316);
	addChild(shopIcon8);
	auto label81 = Label::createWithSystemFont("Money:790", "Arial", 8);
	auto label82 = Label::createWithSystemFont("Physical attack: +40", "Arial", 8);
	auto label83 = Label::createWithSystemFont("HP:+300", "Arial", 8);
	label81->setPosition(Director::getInstance()->convertToGL(Vec2(825, 65)));
	label82->setPosition(Director::getInstance()->convertToGL(Vec2(825, 75)));
	label83->setPosition(Director::getInstance()->convertToGL(Vec2(825, 85)));
	addChild(label81);
	addChild(label82);
	addChild(label83);

	auto shopIcon9 = ShopAttactIcon::createAttactIcon(9);
	shopIcon9->setPosition(Director::getInstance()->convertToGL(Vec2(900, 45)));
	shopIcon9->setAttact(50);
	shopIcon9->setSpeed(20);//20%
	shopIcon9->setMoney(356);
	addChild(shopIcon9);
	auto label91 = Label::createWithSystemFont("Money:890", "Arial", 8);
	auto label92 = Label::createWithSystemFont("Physical attack: +50", "Arial", 8);
	auto label93 = Label::createWithSystemFont("Attact Speed: +20%", "Arial", 8);
	label91->setPosition(Director::getInstance()->convertToGL(Vec2(900, 65)));
	label92->setPosition(Director::getInstance()->convertToGL(Vec2(900, 75)));
	label93->setPosition(Director::getInstance()->convertToGL(Vec2(900, 85)));
	addChild(label91);
	addChild(label92);
	addChild(label93);

	auto shopIcon10 = ShopAttactIcon::createAttactIcon(10);
	shopIcon10->setPosition(Director::getInstance()->convertToGL(Vec2(975, 45)));
	shopIcon10->setAttact(45);
	shopIcon10->setHP(400);//冷却缩减
	shopIcon10->setMoney(432);
	addChild(shopIcon10);
	auto label101 = Label::createWithSystemFont("Money:1080", "Arial", 8);
	auto label102 = Label::createWithSystemFont("Physical attack: +45", "Arial", 8);
	auto label103 = Label::createWithSystemFont("HP:+400", "Arial", 8);
	label101->setPosition(Director::getInstance()->convertToGL(Vec2(975, 65)));
	label102->setPosition(Director::getInstance()->convertToGL(Vec2(975, 75)));
	label103->setPosition(Director::getInstance()->convertToGL(Vec2(975, 85)));
	addChild(label101);
	addChild(label102);
	addChild(label103);

	auto buttonListener = EventListenerTouchOneByOne::create();
	buttonListener->setSwallowTouches(true);
	buttonListener->onTouchBegan = CC_CALLBACK_2(ShopScene::onTouchAttactBegan, this);
	EventDispatcher* _eventDispatcher = Director::getInstance()->getEventDispatcher();

	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener, shopIcon1);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon2);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon3);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon4);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon5);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon6);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon7);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon8);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon9);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon10);
}


void ShopScene::initDefensePicture()
{
	auto shopIcon1 = ShopDefenseIcon::createDefenseIcon(1);
	shopIcon1->setPosition(Director::getInstance()->convertToGL(Vec2(300, 125)));
	shopIcon1->setHP(300);
	shopIcon1->setMoney(120);
	addChild(shopIcon1);
	//shopIconsAttact.pushBack(shopIcon1);
	auto label11 = Label::createWithSystemFont("Money:300", "Arial", 8);
	auto label12 = Label::createWithSystemFont("HP: +300", "Arial", 8);
	label11->setPosition(Director::getInstance()->convertToGL(Vec2(300, 145)));
	label12->setPosition(Director::getInstance()->convertToGL(Vec2(300, 155)));
	addChild(label11);
	addChild(label12);

	auto shopIcon2 = ShopDefenseIcon::createDefenseIcon(2);
	shopIcon2->setPosition(Director::getInstance()->convertToGL(Vec2(375, 125)));
	shopIcon1->setHP(200);
	shopIcon2->setMoney(220);
	addChild(shopIcon2);
	auto label21 = Label::createWithSystemFont("Money:220", "Arial", 8);
	auto label22 = Label::createWithSystemFont("HP: +200", "Arial", 8);
	label21->setPosition(Director::getInstance()->convertToGL(Vec2(375, 145)));
	label22->setPosition(Director::getInstance()->convertToGL(Vec2(375, 155)));
	addChild(label21);
	addChild(label22);

	///披风跳过3

	auto shopIcon4 = ShopDefenseIcon::createDefenseIcon(4);
	shopIcon4->setPosition(Director::getInstance()->convertToGL(Vec2(450, 125)));
	shopIcon4->setHP(60);
	shopIcon4->setMoney(140);
	addChild(shopIcon4);
	auto label41 = Label::createWithSystemFont("Money:140", "Arial", 8);
	auto label42 = Label::createWithSystemFont("HP: +60", "Arial", 8);
	label41->setPosition(Director::getInstance()->convertToGL(Vec2(450, 145)));
	label42->setPosition(Director::getInstance()->convertToGL(Vec2(450, 155)));
	addChild(label41);
	addChild(label42);

	auto shopIcon5 = ShopDefenseIcon::createDefenseIcon(5);
	shopIcon5->setPosition(Director::getInstance()->convertToGL(Vec2(525, 125)));
	shopIcon5->setHP(1000);
	shopIcon5->setMoney(360);
	addChild(shopIcon5);
	auto label51 = Label::createWithSystemFont("Money:900", "Arial", 8);
	auto label52 = Label::createWithSystemFont("HP: +1000", "Arial", 8);
	label51->setPosition(Director::getInstance()->convertToGL(Vec2(525, 145)));
	label52->setPosition(Director::getInstance()->convertToGL(Vec2(525, 155)));
	addChild(label51);
	addChild(label52);

	auto shopIcon6 = ShopDefenseIcon::createDefenseIcon(6);
	shopIcon6->setPosition(Director::getInstance()->convertToGL(Vec2(600, 125)));
	shopIcon6->setHP(700);
	shopIcon6->setMoney(360);
	shopIcon6->setAttact(240);
	addChild(shopIcon6);
	auto label61 = Label::createWithSystemFont("Money:900", "Arial", 8);
	auto label62 = Label::createWithSystemFont("Physical attack: +240", "Arial", 8);
	auto label63 = Label::createWithSystemFont("HP: +700", "Arial", 8);
	label61->setPosition(Director::getInstance()->convertToGL(Vec2(600, 145)));
	label62->setPosition(Director::getInstance()->convertToGL(Vec2(600, 155)));
	label63->setPosition(Director::getInstance()->convertToGL(Vec2(600, 165)));
	addChild(label61);
	addChild(label62);
	addChild(label63);

	auto shopIcon7 = ShopDefenseIcon::createDefenseIcon(7);
	shopIcon7->setPosition(Director::getInstance()->convertToGL(Vec2(675, 125)));
	shopIcon7->setAttact(240);
	shopIcon7->setMoney(408);
	shopIcon6->setHP(820);
	addChild(shopIcon7);
	auto label71 = Label::createWithSystemFont("Money:1020", "Arial", 8);
	auto label72 = Label::createWithSystemFont("Physical attack: +240", "Arial", 8);
	auto label73 = Label::createWithSystemFont("HP: +820", "Arial", 8);
	label71->setPosition(Director::getInstance()->convertToGL(Vec2(675, 145)));
	label72->setPosition(Director::getInstance()->convertToGL(Vec2(675, 155)));
	label73->setPosition(Director::getInstance()->convertToGL(Vec2(675, 165)));
	addChild(label71);
	addChild(label72);
	addChild(label73);

	auto shopIcon8 = ShopDefenseIcon::createDefenseIcon(8);
	shopIcon8->setPosition(Director::getInstance()->convertToGL(Vec2(750, 125)));
	shopIcon8->setAttact(400);
	shopIcon8->setHP(300);
	shopIcon8->setMoney(360);
	addChild(shopIcon8);
	auto label81 = Label::createWithSystemFont("Money:900", "Arial", 8);
	auto label82 = Label::createWithSystemFont("Physical attack: +400", "Arial", 8);
	auto label83 = Label::createWithSystemFont("HP:+300", "Arial", 8);
	label81->setPosition(Director::getInstance()->convertToGL(Vec2(750, 145)));
	label82->setPosition(Director::getInstance()->convertToGL(Vec2(750, 155)));
	label83->setPosition(Director::getInstance()->convertToGL(Vec2(750, 165)));
	addChild(label81);
	addChild(label82);
	addChild(label83);

	auto shopIcon9 = ShopDefenseIcon::createDefenseIcon(9);
	shopIcon9->setPosition(Director::getInstance()->convertToGL(Vec2(825, 125)));
	shopIcon9->setHP(210);
	shopIcon9->setSpeed(20);//20%
	shopIcon9->setMoney(292);
	addChild(shopIcon9);
	auto label91 = Label::createWithSystemFont("Money:730", "Arial", 8);
	auto label92 = Label::createWithSystemFont("HP:+210", "Arial", 8);
	auto label93 = Label::createWithSystemFont("Attact Speed: +20%", "Arial", 8);
	label91->setPosition(Director::getInstance()->convertToGL(Vec2(825, 145)));
	label92->setPosition(Director::getInstance()->convertToGL(Vec2(825, 155)));
	label93->setPosition(Director::getInstance()->convertToGL(Vec2(825, 165)));
	addChild(label91);
	addChild(label92);
	addChild(label93);

	auto shopIcon10 = ShopDefenseIcon::createDefenseIcon(10);
	shopIcon10->setPosition(Director::getInstance()->convertToGL(Vec2(900, 125)));
	shopIcon10->setAttact(250);
	shopIcon10->setHP(900);//冷却缩减
	shopIcon10->setMoney(736);
	addChild(shopIcon10);
	auto label101 = Label::createWithSystemFont("Money:1840", "Arial", 8);
	auto label102 = Label::createWithSystemFont("Physical attack: +250", "Arial", 8);
	auto label103 = Label::createWithSystemFont("HP:+900", "Arial", 8);
	label101->setPosition(Director::getInstance()->convertToGL(Vec2(900, 145)));
	label102->setPosition(Director::getInstance()->convertToGL(Vec2(900, 155)));
	label103->setPosition(Director::getInstance()->convertToGL(Vec2(900, 165)));
	addChild(label101);
	addChild(label102);
	addChild(label103);

	auto shopIcon11 = ShopDefenseIcon::createDefenseIcon(11);
	shopIcon11->setPosition(Director::getInstance()->convertToGL(Vec2(975, 125)));
	shopIcon11->setHP(2000);//冷却缩减
	shopIcon11->setMoney(828);
	addChild(shopIcon11);
	auto label111 = Label::createWithSystemFont("Money:2070", "Arial", 8);
	auto label112 = Label::createWithSystemFont("HP:+2000", "Arial", 8);
	label111->setPosition(Director::getInstance()->convertToGL(Vec2(975, 145)));
	label112->setPosition(Director::getInstance()->convertToGL(Vec2(975, 155)));
	addChild(label111);
	addChild(label112);

	auto buttonListener = EventListenerTouchOneByOne::create();
	buttonListener->setSwallowTouches(true);
	buttonListener->onTouchBegan = CC_CALLBACK_2(ShopScene::onTouchDefenseBegan, this);
	EventDispatcher* _eventDispatcher = Director::getInstance()->getEventDispatcher();

	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener, shopIcon1);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon2);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon3);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon4);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon5);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon6);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon7);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon8);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon9);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon10);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon11);
}

void ShopScene::initMovePicture()
{
	auto shopIcon1 = ShopMoveIcon::createMoveIcon(1);
	shopIcon1->setPosition(Director::getInstance()->convertToGL(Vec2(300, 205)));
	shopIcon1->setSpeed(30);
	shopIcon1->setMoney(250);
	addChild(shopIcon1);
	//shopIconsAttact.pushBack(shopIcon1);
	auto label11 = Label::createWithSystemFont("Money:250", "Arial", 8);
	auto label12 = Label::createWithSystemFont("Move Speed: +30", "Arial", 8);
	label11->setPosition(Director::getInstance()->convertToGL(Vec2(300, 225)));
	label12->setPosition(Director::getInstance()->convertToGL(Vec2(300, 235)));
	addChild(label11);
	addChild(label12);

	auto shopIcon2 = ShopMoveIcon::createMoveIcon(2);
	shopIcon2->setPosition(Director::getInstance()->convertToGL(Vec2(375, 205)));
	shopIcon2->setSpeed(60);
	shopIcon2->setHP(110);//防御??
	shopIcon2->setMoney(116);
	addChild(shopIcon2);
	auto label21 = Label::createWithSystemFont("Money:70", "Arial", 8);
	auto label22 = Label::createWithSystemFont("Attact Speed: +60%", "Arial", 8);
	//auto label22 = Label::createWithSystemFont("Attact Speed:+60", "Arial", 8);
	auto label23 = Label::createWithSystemFont("HP: +110%", "Arial", 8);   //要加%才不模糊
	//auto label23 = Label::createWithSystemFont("HP: +110", "Arial", 8);
	label21->setPosition(Director::getInstance()->convertToGL(Vec2(375, 225)));
	label22->setPosition(Director::getInstance()->convertToGL(Vec2(375, 235)));
	label23->setPosition(Director::getInstance()->convertToGL(Vec2(375, 245)));
	addChild(label21);
	addChild(label22);
	addChild(label23);

	auto shopIcon3 = ShopMoveIcon::createMoveIcon(3);
	shopIcon3->setPosition(Director::getInstance()->convertToGL(Vec2(450, 205)));
	shopIcon3->setSpeed(60);
	shopIcon3->setAttact(110);
	shopIcon3->setMoney(116);
	addChild(shopIcon3);
	auto label31 = Label::createWithSystemFont("Money:710", "Arial", 8);
	auto label32 = Label::createWithSystemFont("Physical attack: +110%", "Arial", 8);  //要加%才不模糊
	//auto label32 = Label::createWithSystemFont("Physical attack: +110", "Arial", 8);
	auto label33 = Label::createWithSystemFont("Attact Speed: +60%", "Arial", 8);
	//auto label33 = Label::createWithSystemFont("Attact Speed: +60", "Arial", 8);
	label31->setPosition(Director::getInstance()->convertToGL(Vec2(450, 225)));
	label32->setPosition(Director::getInstance()->convertToGL(Vec2(450, 235)));
	label33->setPosition(Director::getInstance()->convertToGL(Vec2(450, 245)));
	addChild(label31);
	addChild(label32);
	addChild(label33);

	auto shopIcon4 = ShopMoveIcon::createMoveIcon(4);
	shopIcon4->setPosition(Director::getInstance()->convertToGL(Vec2(525, 205)));
	shopIcon4->setSpeed(60);
	shopIcon4->setAttact(75);
	shopIcon4->setHP(35);//回蓝
	shopIcon4->setMoney(116);
	addChild(shopIcon4);
	auto label41 = Label::createWithSystemFont("Money:710", "Arial", 8);
	auto label42 = Label::createWithSystemFont("Physical attack: +75", "Arial", 8);
	auto label43 = Label::createWithSystemFont("Attact Speed: +60%", "Arial", 8);
	//auto label43 = Label::createWithSystemFont("Attact Speed: +60", "Arial", 8);
	auto label44 = Label::createWithSystemFont("HP:+35", "Arial", 8);
	label41->setPosition(Director::getInstance()->convertToGL(Vec2(525, 225)));
	label42->setPosition(Director::getInstance()->convertToGL(Vec2(525, 235)));
	label43->setPosition(Director::getInstance()->convertToGL(Vec2(525, 245)));
	label44->setPosition(Director::getInstance()->convertToGL(Vec2(525, 255)));
	addChild(label41);
	addChild(label42);
	addChild(label43);
	addChild(label44);

	auto shopIcon5 = ShopMoveIcon::createMoveIcon(5);
	shopIcon5->setPosition(Director::getInstance()->convertToGL(Vec2(600, 205)));
	shopIcon5->setSpeed(60);
	shopIcon5->setAttact(60);
	shopIcon5->setHP(50);
	shopIcon5->setMoney(116);
	addChild(shopIcon5);
	auto label51 = Label::createWithSystemFont("Money:710", "Arial", 8);
	auto label52 = Label::createWithSystemFont("Physical attack: +60", "Arial", 8);
	//auto label53 = Label::createWithSystemFont("Attact Speed: +60", "Arial", 8);
	auto label53 = Label::createWithSystemFont("Attact Speed: +60%", "Arial", 8);
	auto label54 = Label::createWithSystemFont("HP:+50", "Arial", 8);
	label51->setPosition(Director::getInstance()->convertToGL(Vec2(600, 225)));
	label52->setPosition(Director::getInstance()->convertToGL(Vec2(600, 235)));
	label53->setPosition(Director::getInstance()->convertToGL(Vec2(600, 245)));
	label54->setPosition(Director::getInstance()->convertToGL(Vec2(600, 255)));
	addChild(label51);
	addChild(label52);
	addChild(label53);
	addChild(label54);

	auto shopIcon6 = ShopMoveIcon::createMoveIcon(6);
	shopIcon6->setPosition(Director::getInstance()->convertToGL(Vec2(675, 205)));
	shopIcon6->setSpeed(90);// 60 + 30%
	shopIcon6->setMoney(116);
	addChild(shopIcon6);
	auto label61 = Label::createWithSystemFont("Money:710", "Arial", 8);
	auto label62 = Label::createWithSystemFont("Attact Speed: +30%", "Arial", 8);
	auto label63 = Label::createWithSystemFont("Move Speed: +60", "Arial", 8);
	label61->setPosition(Director::getInstance()->convertToGL(Vec2(675, 225)));
	label62->setPosition(Director::getInstance()->convertToGL(Vec2(675, 235)));
	label63->setPosition(Director::getInstance()->convertToGL(Vec2(675, 245)));
	addChild(label61);
	addChild(label62);
	addChild(label63);

	auto shopIcon7 = ShopMoveIcon::createMoveIcon(7);
	shopIcon7->setPosition(Director::getInstance()->convertToGL(Vec2(750, 205)));
	shopIcon7->setSpeed(120);
	shopIcon7->setMoney(212);
	addChild(shopIcon7);
	auto label71 = Label::createWithSystemFont("Money:530", "Arial", 8);
	auto label72 = Label::createWithSystemFont("Move Speed:+120", "Arial", 8);
	label71->setPosition(Director::getInstance()->convertToGL(Vec2(750, 225)));
	label72->setPosition(Director::getInstance()->convertToGL(Vec2(750, 235)));
	addChild(label71);
	addChild(label72);

	auto buttonListener = EventListenerTouchOneByOne::create();
	buttonListener->setSwallowTouches(true);
	buttonListener->onTouchBegan = CC_CALLBACK_2(ShopScene::onTouchMoveBegan, this);
	EventDispatcher* _eventDispatcher = Director::getInstance()->getEventDispatcher();

	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener, shopIcon1);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon2);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon3);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon4);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon5);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon6);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon7);

}

void ShopScene::initPlayingWildPicture()
{
	auto shopIcon1 = ShopPlayingWildIcon::createPlayingWildIcon(1);
	shopIcon1->setPosition(Director::getInstance()->convertToGL(Vec2(300, 285)));
	shopIcon1->setAttact(80);//50%??
	shopIcon1->setMoney(250);
	addChild(shopIcon1);
	//shopIconsAttact.pushBack(shopIcon1);
	auto label11 = Label::createWithSystemFont("Money:250", "Arial", 8);
	auto label12 = Label::createWithSystemFont("Physical attack: +50", "Arial", 8);
	label11->setPosition(Director::getInstance()->convertToGL(Vec2(300, 305)));
	label12->setPosition(Director::getInstance()->convertToGL(Vec2(300, 315)));
	addChild(label11);
	addChild(label12);

	auto shopIcon2 = ShopPlayingWildIcon::createPlayingWildIcon(2);
	shopIcon2->setPosition(Director::getInstance()->convertToGL(Vec2(375, 285)));
	shopIcon2->setSpeed(60);
	shopIcon2->setAttact(100);
	shopIcon2->setMoney(300);
	addChild(shopIcon2);
	auto label21 = Label::createWithSystemFont("Money:750", "Arial", 8);
	auto label22 = Label::createWithSystemFont("Attact Speed: +60%", "Arial", 8);
	auto label23 = Label::createWithSystemFont("Physical attack: +60", "Arial", 8);
	label21->setPosition(Director::getInstance()->convertToGL(Vec2(375, 305)));
	label22->setPosition(Director::getInstance()->convertToGL(Vec2(375, 315)));
	label23->setPosition(Director::getInstance()->convertToGL(Vec2(375, 325)));
	addChild(label21);
	addChild(label22);
	addChild(label23);

	auto shopIcon3 = ShopPlayingWildIcon::createPlayingWildIcon(3);
	shopIcon3->setPosition(Director::getInstance()->convertToGL(Vec2(450, 285)));
	shopIcon3->setHP(60);
	shopIcon3->setAttact(120);
	shopIcon3->setMoney(450);
	addChild(shopIcon3);
	auto label31 = Label::createWithSystemFont("Money:750", "Arial", 8);
	auto label32 = Label::createWithSystemFont("Physical attack: +120", "Arial", 8);
	auto label33 = Label::createWithSystemFont("HP: +60", "Arial", 8);
	label31->setPosition(Director::getInstance()->convertToGL(Vec2(450, 305)));
	label32->setPosition(Director::getInstance()->convertToGL(Vec2(450, 315)));
	label33->setPosition(Director::getInstance()->convertToGL(Vec2(450, 325)));
	addChild(label31);
	addChild(label32);
	addChild(label33);

	auto shopIcon4 = ShopPlayingWildIcon::createPlayingWildIcon(4);
	shopIcon4->setPosition(Director::getInstance()->convertToGL(Vec2(525, 285)));
	shopIcon4->setSpeed(20);
	shopIcon4->setAttact(120);
	shopIcon4->setHP(35);//回蓝
	shopIcon4->setMoney(300);
	addChild(shopIcon4);
	auto label41 = Label::createWithSystemFont("Money:750", "Arial", 8);
	auto label42 = Label::createWithSystemFont("Physical attack: +120", "Arial", 8);
	auto label43 = Label::createWithSystemFont("Attact Speed: +20%", "Arial", 8);
	auto label44 = Label::createWithSystemFont("HP:+35", "Arial", 8);
	label41->setPosition(Director::getInstance()->convertToGL(Vec2(525, 305)));
	label42->setPosition(Director::getInstance()->convertToGL(Vec2(525, 315)));
	label43->setPosition(Director::getInstance()->convertToGL(Vec2(525, 325)));
	label44->setPosition(Director::getInstance()->convertToGL(Vec2(525, 335)));
	addChild(label41);
	addChild(label42);
	addChild(label43);
	addChild(label44);

	auto shopIcon5 = ShopPlayingWildIcon::createPlayingWildIcon(5);
	shopIcon5->setPosition(Director::getInstance()->convertToGL(Vec2(600, 285)));
	shopIcon5->setAttact(500);//法力+400
	shopIcon5->setMoney(864);
	addChild(shopIcon5);
	auto label51 = Label::createWithSystemFont("Money:2160", "Arial", 8);
	auto label52 = Label::createWithSystemFont("Physical attack: +500", "Arial", 8);
	label51->setPosition(Director::getInstance()->convertToGL(Vec2(600, 305)));
	label52->setPosition(Director::getInstance()->convertToGL(Vec2(600, 315)));
	addChild(label51);
	addChild(label52);

	auto shopIcon6 = ShopPlayingWildIcon::createPlayingWildIcon(6);
	shopIcon6->setPosition(Director::getInstance()->convertToGL(Vec2(675, 285)));
	shopIcon6->setHP(240);// 防御
	shopIcon6->setMoney(720);
	addChild(shopIcon6);
	auto label61 = Label::createWithSystemFont("Money:1800", "Arial", 8);
	auto label62 = Label::createWithSystemFont("HP: +240", "Arial", 8);
	label61->setPosition(Director::getInstance()->convertToGL(Vec2(675, 305)));
	label62->setPosition(Director::getInstance()->convertToGL(Vec2(675, 315)));
	addChild(label61);
	addChild(label62);

	//7略

	auto shopIcon8 = ShopPlayingWildIcon::createPlayingWildIcon(7);
	shopIcon8->setPosition(Director::getInstance()->convertToGL(Vec2(750, 285)));
	shopIcon8->setAttact(240);
	shopIcon8->setSpeed(230);//攻击速度 移速？23% 8 + 15
	shopIcon8->setMoney(864);
	addChild(shopIcon8);
	auto label81 = Label::createWithSystemFont("Money:216", "Arial", 8);
	auto label82 = Label::createWithSystemFont("Move Speed:+230", "Arial", 8);
	auto label83 = Label::createWithSystemFont("Physical attack: +500", "Arial", 8);
	label81->setPosition(Director::getInstance()->convertToGL(Vec2(750, 305)));
	label82->setPosition(Director::getInstance()->convertToGL(Vec2(750, 315)));
	label83->setPosition(Director::getInstance()->convertToGL(Vec2(750, 325)));
	addChild(label81);
	addChild(label82);
	addChild(label83);

	auto buttonListener = EventListenerTouchOneByOne::create();
	buttonListener->setSwallowTouches(true);

	buttonListener->onTouchBegan = CC_CALLBACK_2(ShopScene::onTouchPlayingWildBegan, this);
	EventDispatcher* _eventDispatcher = Director::getInstance()->getEventDispatcher();

	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener, shopIcon1);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon2);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon3);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon4);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon5);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon6);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon7);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon8);

}

void ShopScene::initSpellPicture()
{
	
	auto shopIcon1 = ShopSpellIcon::createSpellIcon(1);
	shopIcon1->setPosition(Director::getInstance()->convertToGL(Vec2(300, 365)));
	shopIcon1->setAttact(20);
	shopIcon1->setMoney(250);
	addChild(shopIcon1);
	auto label11 = Label::createWithSystemFont("Money:250", "Arial", 8);
	auto label12 = Label::createWithSystemFont("Physical attack: +20", "Arial", 8);
	label11->setPosition(Director::getInstance()->convertToGL(Vec2(300, 385)));
	label12->setPosition(Director::getInstance()->convertToGL(Vec2(300, 395)));
	addChild(label11);
	addChild(label12);
	//shopIconsAttact.pushBack(shopIcon1);

	auto shopIcon2 = ShopSpellIcon::createSpellIcon(2);
	shopIcon2->setPosition(Director::getInstance()->convertToGL(Vec2(375, 365)));
	shopIcon2->setSpeed(20);//攻击速度10%
	shopIcon2->setMoney(140);
	addChild(shopIcon2);
	auto label21 = Label::createWithSystemFont("Money:140", "Arial", 8);
	auto label22 = Label::createWithSystemFont("Attact Speed: +20%", "Arial", 8);
	label21->setPosition(Director::getInstance()->convertToGL(Vec2(375, 385)));
	label22->setPosition(Director::getInstance()->convertToGL(Vec2(375, 395)));
	addChild(label21);
	addChild(label22);

	auto shopIcon3 = ShopSpellIcon::createSpellIcon(3);
	shopIcon3->setPosition(Director::getInstance()->convertToGL(Vec2(450, 365)));
	shopIcon3->setAttact(40);
	shopIcon3->setMoney(180);
	addChild(shopIcon3);
	auto label31 = Label::createWithSystemFont("Money:180", "Arial", 8);
	auto label32 = Label::createWithSystemFont("Physical attack: +40", "Arial", 8);
	label31->setPosition(Director::getInstance()->convertToGL(Vec2(450, 385)));
	label32->setPosition(Director::getInstance()->convertToGL(Vec2(450, 395)));
	addChild(label31);
	addChild(label32);

	auto shopIcon4 = ShopSpellIcon::createSpellIcon(4);
	shopIcon4->setPosition(Director::getInstance()->convertToGL(Vec2(525, 365)));
	shopIcon4->setAttact(60);
	shopIcon4->setMoney(220);
	addChild(shopIcon4);
	auto label41 = Label::createWithSystemFont("Money:220", "Arial", 8);
	auto label42 = Label::createWithSystemFont("Physical attack: +60", "Arial", 8);
	label41->setPosition(Director::getInstance()->convertToGL(Vec2(525, 385)));
	label42->setPosition(Director::getInstance()->convertToGL(Vec2(525, 395)));
	addChild(label41);
	addChild(label42);

	auto shopIcon5 = ShopSpellIcon::createSpellIcon(5);
	shopIcon5->setPosition(Director::getInstance()->convertToGL(Vec2(600, 365)));
	shopIcon5->setSpeed(60);
	shopIcon5->setMoney(220);
	addChild(shopIcon5);
	auto label51 = Label::createWithSystemFont("Money:200", "Arial", 8);
	auto label52 = Label::createWithSystemFont("Attact Speed: +60%", "Arial", 8);
	label51->setPosition(Director::getInstance()->convertToGL(Vec2(600, 385)));
	label52->setPosition(Director::getInstance()->convertToGL(Vec2(600, 395)));
	addChild(label51);
	addChild(label52);

	auto buttonListener = EventListenerTouchOneByOne::create();
	buttonListener->setSwallowTouches(true);


	buttonListener->onTouchBegan = CC_CALLBACK_2(ShopScene::onTouchSpellBegan, this);
	EventDispatcher* _eventDispatcher = Director::getInstance()->getEventDispatcher();

	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener, shopIcon1);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon2);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon3);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon4);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener->clone(), shopIcon5);
}