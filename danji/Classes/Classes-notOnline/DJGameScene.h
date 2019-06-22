#ifndef __DJGAMESCENE_H__
#define __DJGAMESCENE_H__

#pragma once
#include "cocos2d.h"
#include "Daji.h"
#include "BaseMap.h"

USING_NS_CC;

class Daji;



class DJGameScene : public Scene
{
public:
	Scene* scene;
	BaseMap* map;
	cocos2d::TMXTiledMap* tiledMap;
	cocos2d::TMXLayer * _collidable;
	virtual void update(float dt);
	virtual void onEnter();
	virtual bool isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);
	virtual void keyPressedDuration(cocos2d::EventKeyboard::KeyCode code);
	void setPlayerPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 position);
	virtual bool init();
	//void setPlayerPosition(cocos2d::Vec2 position);
	//cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 position);

	CREATE_FUNC(DJGameScene);
	Daji* hero;
	//cocos2d::TMXTiledMap * _tileMap;
	//cocos2d::TMXLayer * _collidable;

private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
};

#endif

