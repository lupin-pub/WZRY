#ifndef  _LOGINSCENE_H__
#define  _LOGINSCENE_H__

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
class LoginScene :public cocos2d::Layer
{
private:

    cocos2d::Sprite* usernameBG;
    cocos2d::ui::TextField* usernameInput;
	cocos2d::CCSprite* m_pSprite;
    void createLoginButton();

    void initPutBox();

    void menuBackCallback(cocos2d::Ref* pSender);
    void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType event);

   // void onEnter();

	
public:

    static cocos2d::Scene * createScene();
    virtual bool init();
    CREATE_FUNC(LoginScene);

};
#endif // ! _LOGINSCENE_SCENE_H__
