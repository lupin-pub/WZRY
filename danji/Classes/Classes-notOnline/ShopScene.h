#ifndef _SHOPSCENE_H_
#define _SHOPSCENE_H_
#pragma once
#include "cocos2d.h"
#include "ShopAttactIcon.h"
#include "ShopMoveIcon.h"
#include "ShopSpellIcon.h"
#include "ShopDefenseIcon.h"
#include "ShopPlayingWildIcon.h"
#include <vector>

USING_NS_CC;

class ShopScene : public cocos2d::Layer
{

public:

	//购买失败提醒
	Label * BuyFau;
	Label * BuySus;
	Label * lableMoney;


	virtual bool init();
	void initAttactPicture();
	void initDefensePicture();
	void initMovePicture();
	void initPlayingWildPicture();
	void initSpellPicture();

	bool onTouchAttactBegan(Touch * touch, Event * event);
	bool onTouchPlayingWildBegan(Touch * touch, Event * event);
	bool onTouchSpellBegan(Touch * touch, Event * event);
	bool onTouchMoveBegan(Touch * touch, Event * event);
	bool onTouchDefenseBegan(Touch * touch, Event * event);


	// a selector callback
	void menuBackItemCallback(cocos2d::Ref* pSender);

	int count[3];//购买用品的数量

	// 攻击 防御 移动 法术 打野/  装备分类型各自的数量
	const int countAttact = 22;
	const int countDefense = 21;
	const int countMove = 7;
	const int countSpell = 23;
	const int countPlayingWild = 8;

	
	//存当下装备 用来更改人物属性 清除上一次的更改 加上这一次的更改
	Vector<ShopAttactIcon*> shopIconsAttact;
	Vector<ShopDefenseIcon*> shopIconsDefense;
	Vector<ShopMoveIcon*> shopIconsMove;
	Vector<ShopSpellIcon*> shopIconsSpell;
	Vector<ShopPlayingWildIcon*> shopIconsPlayingWild;


	//存各自装备的数组
	/*
	Vector<ShopAttactIcon*> shopIconsAttact;
	Vector<ShopDefenseIcon*> shopIconsDefense;
	Vector<ShopMoveIcon*> shopIconsMove;
	Vector<ShopSpellIcon*> shopIconsSpell;
	Vector<ShopPlayingWildIcon*> shopIconsPlayingWild;
	*/

	// implement the "static create()" method manually
	CREATE_FUNC(ShopScene);

private:
	std::vector<float> iconY{45, 125, 205, 285, 365};
	std::vector<float> iconX{ 300, 375, 450, 525, 600, 675, 750, 825, 900, 975 };
};

#endif