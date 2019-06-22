#ifndef _STARTSCENE_H_
#define _STARTSCENE_H_

#pragma once

#include "cocos2d.h"

class StartScene : public cocos2d::Layer
{
private:
    cocos2d::Menu* createText();
    cocos2d::Sprite* createBGImage();
    void menuExitCallback(cocos2d::Ref* pSender);
    void menuPlayCallback(cocos2d::Ref* pSender);
   // void menuHelpCallback(cocos2d::Ref * pSender);
    void menuSettingsCallback(cocos2d::Ref * pSender);
    
    void onEnter();
    void onExit();
 
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(StartScene);                
};

#endif 

