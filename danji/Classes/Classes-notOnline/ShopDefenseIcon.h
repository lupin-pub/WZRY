#ifndef _SHOPDEFENSEICON_H_
#define _SHOPDEFENSEICON_H_
#pragma once
#include "cocos2d.h"

USING_NS_CC;
class ShopDefenseIcon : public Sprite
{
public:
	virtual bool initDefenseIcon(int tpye);
	static ShopDefenseIcon* createDefenseIcon(int type);

	CREATE_FUNC(ShopDefenseIcon);

	virtual int getAttact();
	virtual int getMoney();
	virtual int getHP();
	virtual int getSpeed();
	virtual void setAttact(int temp);
	virtual void setMoney(int temp);
	virtual void setHP(int temp);
	virtual void setSpeed(int temp);

	int type;
	Sprite* icon;

private:
	//攻击装备类型
	int addAttact;
	int addHP;
	int addSpeed;
	int Money;

	

	//void setGlowing();
	//void setNotGlowing();
	//Sprite* glow;
};

#endif


