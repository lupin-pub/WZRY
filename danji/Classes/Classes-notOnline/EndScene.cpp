#include "EndScene.h"
#include "DJGameScene.h"
#include "YSGameScene.h"
#include "HYGameScene.h"
#include "Crystal.h"
#include "BaseTower.h"
//#include "Audio/GameAudio.h"

USING_NS_CC;


Scene* EndScene::createScene(int color)
{
	//scene is an auto release object 
	auto scene = Scene::create();

	//layer is an auto release object
	auto layer = EndScene::createEnd(color);

	//add layer as a child to scene
	scene->addChild(layer);

	//return the scene
	return scene;
}

Layer* EndScene::createEnd(int color)
{
	auto endScene = new EndScene();

	if (endScene && endScene->initEnd(color))
	{
		endScene->autorelease();
		return endScene;
	}
	CC_SAFE_DELETE(endScene);
	return NULL;
}

bool EndScene::initEnd(int color)
{
	addChild(createBGImage(color));
	
	auto _labelExit = Label::createWithSystemFont("Exit", "Arial", 30);
	auto _menuItemLabelExit = MenuItemLabel::create(_labelExit, CC_CALLBACK_1(EndScene::menuExitCallback, this));
	auto _menu = Menu::create(_menuItemLabelExit, nullptr);
	//设置Menu位置属性
	_menu->setPosition(Vec2::ZERO);
	addChild(_menu);

	return true;
}


void  EndScene::menuExitCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}



cocos2d::Sprite* EndScene::createBGImage(int color) {

	//我方赢了
	if (color = 1){
		auto size = Director::getInstance()->getVisibleSize();
		auto bgImage = Sprite::create("GameItem/Victory.jpg");
		bgImage->setScale(1.25);
		bgImage->setPosition(size.width / 2, size.height / 2);
		return bgImage;
	}
	
	//我方输了
	else {
		auto size = Director::getInstance()->getVisibleSize();
		auto bgImage = Sprite::create("GameItem/Defeat.jpeg");
		bgImage->setScale(1.25);
		bgImage->setPosition(size.width / 2, size.height / 2);
		return bgImage;
	}
}
