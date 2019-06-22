#ifndef _SHOPMOVEICON_H_
#define _SHOPMOVEICON_H_
#pragma once
#include "cocos2d.h"

USING_NS_CC;
class ShopMoveIcon : public Sprite
{
public:
	Sprite* icon;
	int type;//???

	virtual bool initMoveIcon(int tpye);
	static ShopMoveIcon* createMoveIcon(int type);

	virtual int getAttact();
	virtual int getMoney();
	virtual int getHP();
	virtual int getSpeed();
	virtual void setAttact(int temp);
	virtual void setMoney(int temp);
	virtual void setHP(int temp);
	virtual void setSpeed(int temp);

private:

	//打野装备类型
	int addAttact = 0;
	int addHP = 0;
	int addSpeed = 0;
	int Money = 0;
	CREATE_FUNC(ShopMoveIcon);

	//void setGlowing();
	//void setNotGlowing();
	//Sprite* glow;
};
#endif