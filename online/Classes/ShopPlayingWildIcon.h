#ifndef _SHOPPLAYINGWILDICON_H_
#define _SHOPPLAYINGWILDICON_H_
#pragma once
#include "cocos2d.h"

USING_NS_CC;
class ShopPlayingWildIcon : public Sprite
{
public:
	Sprite* icon;

	virtual bool initPalyingWildIcon(int tpye);
	static ShopPlayingWildIcon* createPlayingWildIcon(int type);
	virtual int getAttact();
	virtual int getMoney();
	virtual int getHP();
	virtual int getSpeed();
	virtual void setAttact(int temp);
	virtual void setMoney(int temp);
	virtual void setHP(int temp);
	virtual void setSpeed(int temp);
private:
	//��Ұװ������
	int type;
	int addAttact;
	int addHP;
	int addSpeed;
	int Money;

	CREATE_FUNC(ShopPlayingWildIcon);

	//void setGlowing();
	//void setNotGlowing();
	//Sprite* glow;
};
#endif