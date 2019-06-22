#ifndef __HYGAMESCENE_H__
#define __HYGAMESCENE_H__

#pragma once
#include "cocos2d.h"
#include "Houyi.h"

USING_NS_CC;

class Houyi;



class HYGameScene : public Scene
{
public:
	virtual void update(float dt);
	virtual void onEnter();
	virtual bool isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);
	virtual void keyPressedDuration(cocos2d::EventKeyboard::KeyCode code);
	virtual bool init();
	//void setPlayerPosition(cocos2d::Vec2 position);
	//cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 position);

	CREATE_FUNC(HYGameScene);
	Houyi* hero;
	//cocos2d::TMXTiledMap * _tileMap;
	//cocos2d::TMXLayer * _collidable;

private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
};

#endif

