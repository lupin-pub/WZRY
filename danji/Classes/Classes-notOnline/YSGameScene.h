#ifndef _YSGAMESCENE_H_
#define _YSGAMESCENE_H_
#pragma once
#include "cocos2d.h"
#include "Yase.h"

USING_NS_CC;

class Yase;

class YSGameScene : public Scene
{
public:
	virtual void update(float dt);
	virtual void onEnter();
	virtual bool isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);
	virtual void keyPressedDuration(cocos2d::EventKeyboard::KeyCode code);
	virtual bool init();


	//virtual bool init();
	CREATE_FUNC(YSGameScene);
	Yase* hero;

private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
};

#endif

