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

	//������ 
	ProgressTimer * progressTimerHome;
	//����������ͼƬ
	//Sprite * spProHome;

	void menuHomeCallback(cocos2d::Ref* pSender);
	// ��ȴ��ϻص�
	void HomeCoolDownCallBack(Node* node);


	// �鿴ս��
	void menuScoreCallback(cocos2d::Ref* pSender);

	// ��Ǯͼ�� �鿴װ�� �� ����
	void menuShopCallback(cocos2d::Ref* pSender);

	//�����˳���ť
	void createCloseButton();
	//�����˵���
	void createItem();
};

#endif
