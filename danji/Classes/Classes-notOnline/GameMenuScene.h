#ifndef _GAMEMENUSCENE_H_
#define _GAMEMENUSCENE_H_

#pragma once
#include "cocos2d.h"

#include"ui/UIButton.h"
#include"SelectToPlayScene.h"

USING_NS_CC;

//class ShopScene;
//class ScoreScene;

typedef enum
{
	SHOP = 1,
	SCORE = 2,
	MENU,
	Hero,
	Dj,
	Hy,
	Ys,
	MAP,
}LayerTypes;

class GameMenuScene : public cocos2d::Layer
{

public:

	//static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameMenuScene);

	MenuItemImage *  getShopItem() { return ShopItem; };
	MenuItemImage *  getScoreItem() { return ScoreItem; };
	MenuItemImage *  getHomeItem() { return HomeItem; };


private:

	MenuItemImage * ShopItem;
	MenuItemImage * ScoreItem;
	MenuItemImage * HomeItem;

	//进度条 
	ProgressTimer * progressTimerHome;
	//进度条精灵图片
	//Sprite * spProHome;

	void menuHomeCallback(cocos2d::Ref* pSender);
	// 冷却完毕回调
	void HomeCoolDownCallBack(Node* node);


	// 查看战绩
	void menuScoreCallback(cocos2d::Ref* pSender);

	// 金钱图标 查看装备 并 购买
	void menuShopCallback(cocos2d::Ref* pSender);

	//创建退出按钮
	void createCloseButton();
	//创建菜单项
	void createItem();
};

#endif
