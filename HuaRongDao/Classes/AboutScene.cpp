#include "AboutScene.h"
#include "WelcomeScene.h"

USING_NS_CC;

Scene* ABOUT::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ABOUT::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ABOUT::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto backItem = MenuItemImage::create(
                                           "back.png",
                                           "backed.png",
                                           CC_CALLBACK_1(ABOUT::menuBackCallback, this));
    
	backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width/2 ,
                                origin.y + backItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // add "ABOUT" splash screen"
    auto sprite = Sprite::create("about.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    return true;
}

void ABOUT::menuBackCallback(Ref* pSender)
{

	TransitionScene * reScene = NULL;
	float t = 1.2f;
	auto scene = WelcomeScene::createScene();
	reScene = TransitionSlideInL::create(t, scene);
	Director::getInstance()->replaceScene(reScene);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//    return;
//#endif
//
//    Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
}
