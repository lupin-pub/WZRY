#ifndef __SELECTETOPLAYSCENE_H__
#define __SELECTETOPLAYSCENE_H__

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIButton.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "BaseMap.h"

class YSSkillButton;

typedef enum
{
	DJ = 100,
	HY,
	YS,
	HERO,
 } HeroTypes;

class SelectToPlayScene :public cocos2d::Layer
{
public:
	BaseMap* map;
    static cocos2d::Scene * createScene();
    virtual bool init();
    //void touchEvent(Ref * pSender, cocos2d::ui::Widget::TouchEventType type);
	void touchDJEvent(Ref * pSender, cocos2d::ui::Widget::TouchEventType type);
	void touchHYEvent(Ref * pSender, cocos2d::ui::Widget::TouchEventType type);
	void touchYSEvent(Ref * pSender, cocos2d::ui::Widget::TouchEventType type);
    CREATE_FUNC(SelectToPlayScene);
};
#endif 
