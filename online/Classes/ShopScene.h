#ifndef _SHOPSCENE_H_
#define _SHOPSCENE_H_
#pragma once
#include "cocos2d.h"
#include "ShopAttactIcon.h"
#include "ShopMoveIcon.h"
#include "ShopSpellIcon.h"
#include "ShopDefenseIcon.h"
#include "ShopPlayingWildIcon.h"

USING_NS_CC;

class ShopScene : public cocos2d::Layer
{

public:

	//����ʧ������
	Label * BuyFau;
	Label * BuySus;


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

	int count[3];//������Ʒ������

	// ���� ���� �ƶ� ���� ��Ұ/  װ�������͸��Ե�����
	const int countAttact = 22;
	const int countDefense = 21;
	const int countMove = 7;
	const int countSpell = 23;
	const int countPlayingWild = 8;

	
	//�浱��װ�� ���������������� �����һ�εĸ��� ������һ�εĸ���
	Vector<ShopAttactIcon*> shopIconsAttact;
	Vector<ShopDefenseIcon*> shopIconsDefense;
	Vector<ShopMoveIcon*> shopIconsMove;
	Vector<ShopSpellIcon*> shopIconsSpell;
	Vector<ShopPlayingWildIcon*> shopIconsPlayingWild;


	//�����װ��������
	/*
	Vector<ShopAttactIcon*> shopIconsAttact;
	Vector<ShopDefenseIcon*> shopIconsDefense;
	Vector<ShopMoveIcon*> shopIconsMove;
	Vector<ShopSpellIcon*> shopIconsSpell;
	Vector<ShopPlayingWildIcon*> shopIconsPlayingWild;
	*/

	// implement the "static create()" method manually
	CREATE_FUNC(ShopScene);
};

#endif