#include "LoginScene.h"
#include "StartScene.h"
#include <string>
#include <stdlib.h>
USING_NS_CC;

using namespace std;
Scene* LoginScene::createScene()
{
    auto scene = Scene::create();

    auto layer = LoginScene::create();

    scene->addChild(layer);

    return scene;
}

bool LoginScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

	auto size = Director::getInstance()->getVisibleSize();
	auto m_pSprite = Sprite::create("StartMenu/backgroundLogin.jpeg");
	m_pSprite->setScale(1.25);
	m_pSprite->setLocalZOrder(-1);
	m_pSprite->setPosition(size.width / 2, size.height / 2);
	addChild(m_pSprite);
   
    createLoginButton();

    initPutBox();

    return true;
}

void LoginScene::createLoginButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto loginButton = ui::Button::create("StartMenu/LoginButtonNormal.png", "StartMenu/LoginButtonSelected.png");
    loginButton->setTitleText("Login");
    loginButton->setTitleFontSize(32);
    loginButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.35f));
    loginButton->setOpacity(233);
	loginButton->setScale(1.0f);

    loginButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
    {
        if (type != ui::Widget::TouchEventType::ENDED) return;
        auto username = usernameInput->getString();
        if (username.empty())
        {
            MessageBox("Username can't be empty", "Alert");
        }
        else
        {
            Director::getInstance()->replaceScene(TransitionFade::create(1.2f, StartScene::createScene()));
        }
    });
    addChild(loginButton);
	LabelTTF *label = LabelTTF::create();
	label->setString("Please Enter your Name");
	label->setFontSize(20);
	label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(label);
}

void LoginScene::initPutBox()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    usernameBG = Sprite::create("StartMenu/input_normal.png");
    usernameBG->setScale(1.5f);
    usernameBG->setOpacity(200);

    usernameInput = ui::TextField::create("Input Username Here", "fonts/OpenSans-Regular.ttf", 24);
    usernameInput->setString(UserDefault::getInstance()->getStringForKey("username", ""));
    usernameInput->setColor(Color3B::GREEN);
    usernameInput->setCursorChar('|');
    usernameInput->setCursorEnabled(true);
    usernameInput->setMaxLength(15);
    usernameInput->setMaxLengthEnabled(true);
    usernameInput->setPosition(usernameBG->getContentSize() / 2);
    usernameInput->addEventListener(CC_CALLBACK_2(LoginScene::textFieldEvent, this));

    usernameBG->addChild(usernameInput);

    usernameBG->setScale(1.5);
    usernameBG->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height / 2 + usernameBG->getContentSize().height * 2));

    addChild(usernameBG);

}

void LoginScene::textFieldEvent(Ref* sender, ui::TextField::EventType event)
{
    switch (event) {
    case ui::TextField::EventType::ATTACH_WITH_IME:
        usernameBG->setTexture("StartMenu/input_active.png");
        break;
    case ui::TextField::EventType::DETACH_WITH_IME:
        usernameBG->setTexture("StartMenu/input_normal.png");
        break;
    }
}

void LoginScene::menuBackCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->popScene();
}
