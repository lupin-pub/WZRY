#ifndef _ENDSCENE_H_
#define _ENDSCENE_H_

#pragma once

#include "cocos2d.h"

class EndScene : public cocos2d::Layer
{
private:
	cocos2d::Sprite* createBGImage(int );
	void menuExitCallback(cocos2d::Ref* pSender);

public:
	static cocos2d::Scene* createScene(int );
	static Layer* createEnd(int color);
	virtual bool initEnd(int );
	CREATE_FUNC(EndScene);
};

#endif 

