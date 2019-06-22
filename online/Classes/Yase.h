#ifndef _YASE_H_
#define _YASE_H_

#include "BaseMonster.h"
//#include "cocos2d.h"

class Yase : public BaseMonster
{
	Yase* _monster;

public:
	virtual bool init() override;
	virtual void onEnter();
	virtual bool isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);
	virtual void keyPressedDuration(cocos2d::EventKeyboard::KeyCode code);
	void setPlayerPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 position);

	void update(float delta) override;
	virtual void shoot();
	virtual void removeBullet();

	static Yase* createMonster(int color);

	//virtual bool init() override;
	//static Yase* createMonster(std::vector<Point> points, std::vector<std::vector<std::vector<Point>>> sonPath);
	//void getHurt();
	void death();
	//void explosion();
	//void beforeSkill(float dt);
	//void skill();
	//void afterSkill();
	std::vector<std::vector<std::vector<Point>>> sonPath;
	//void addSons();
	//Sprite* cloud;
	//void frozen();
	//Point location;
	//void changePositionLeft();
	//void changePositionRight();
	int skillTag;
	cocos2d::TMXTiledMap * _tileMap;
	cocos2d::TMXLayer * _collidable;

private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;

};

#endif
