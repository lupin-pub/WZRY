#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SettingsScene : public cocos2d::Layer
{
private:

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(SettingsScene);
};

#endif // _SETTINGS_SCENE_H__