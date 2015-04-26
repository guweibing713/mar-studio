#include "WelcomeScene.h"  
#include "GameDefine.h" 
#include "GameLayer.h" 
#include "AboutScene.h"
#include "data/Level.h"  

USING_NS_CC;

Scene* WelcomeScene::createScene()  {
	auto scene = Scene::create();
	auto layer = WelcomeScene::create();
	scene->addChild(layer);
	return scene;
}

// 欢迎界面 初始化函数  
bool WelcomeScene::init()   {
	// 先初始化父类，不成功返回false  
	if (!Layer::init())    {
		return false;
	}

	m_level = 0;
	// TexturePacker 制作所有资源到一张图中,加载资源,提供给调用.
	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Huarongdao2.plist", "Huarongdao2.png");

	// 添加背景图片  
	auto sprite = Sprite::create("menu_bg.png");
	sprite->setPosition(Point(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
	this->addChild(sprite);

	// 添加开始按钮  
	auto startItem = MenuItemImage::create(
		"menuword/menuword_01.png",
		"menuword/menuword_back_01.png",
		CC_CALLBACK_1(WelcomeScene::menuStartCallback, this));

	startItem->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 6 + startItem->getContentSize().height * 4));

	// 添加选项按钮  
	auto optionItem = MenuItemImage::create(
		"menuword/menuword_02.png",
		"menuword/menuword_back_02.png",
		CC_CALLBACK_1(WelcomeScene::menuOptionCallback, this));

	optionItem->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 6 + startItem->getContentSize().height * 3));

	// 添加排行榜按钮  
	auto ranklistItem = MenuItemImage::create(
		"menuword/menuword_03.png",
		"menuword/menuword_back_03.png",
		CC_CALLBACK_1(WelcomeScene::menuRankListCallback, this));

	ranklistItem->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 6 + startItem->getContentSize().height * 2));

	// 添加游戏说明按钮  
	auto instructionItem = MenuItemImage::create(
		"menuword/menuword_04.png",
		"menuword/menuword_back_04.png",
		CC_CALLBACK_1(WelcomeScene::menuInstructionCallback, this));

	instructionItem->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 6 + startItem->getContentSize().height * 1));

	// 添加关于游戏按钮  
	auto aboutItem = MenuItemImage::create(
		"menuword_05.png",
		"menuword_back_05.png",
		CC_CALLBACK_1(WelcomeScene::menuAboutCallback, this));

	aboutItem->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 6));


	auto menu = Menu::create(startItem, optionItem, ranklistItem, instructionItem, aboutItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void WelcomeScene::menuStartCallback(Ref* pSender)
{
	// 跳转到游戏界面  
	// create a scene. it's an autorelease object
	TransitionScene * reScene = NULL;
	float t = 1.2f;
	auto scene = GameLayer::createScene();

	//AppDelegate已经初始化配置Level了.
	//Level::initStatic();
	//((GameLayer*)scene->getChildByTag(0))->setPLevel(Level::s_levelVec.at(1));
	
	// m_pLevel引用+1,不然关闭游戏会报错.
	//CC_SAFE_RETAIN(((GameLayer*)scene->getChildByTag(0))->getPLevel());
	
	//(引用增1放到GameLayer内. 增加setPLevel2方法设置当前Level,引用增1)
	//((GameLayer*)scene->getChildByTag(0))->resetPLevel(Level::s_levelVec.at(2));
	GameLayer::gameLayer()->resetPLevel(Level::s_levelVec.at(1));
	// 初始化地图挪到外面后 背景图绘制比角色晚,因此Role被覆盖了.
	// 采用resetPLevel之后,恢复之前的setPLevel方法,背景不会覆盖Role了.????
	// 原因:点击下面的其他方法menuOptionCallback 进入到GameLayer,而这些方法没有initMap设置Level.
	
	// 初始化地图
	((GameLayer*)scene->getChildByTag(0))->initMap();

	//    CCTransitionFade
	//    作用：创建一个逐渐过渡到目标颜色的切换动画
	//    参数1：过渡动作的持续时间
	//    参数2：切换的目标场景的对象
	//    参数3：目标颜色
	//reScene = CCTransitionFade::create(t, scene, ccc3(255, 0, 0));

	//    CCTransitionSlideInR
	//    作用：创建一个从右侧推入并顶出旧场景的过渡动画
	//    参数1：过渡动作的持续时间
	//    参数2：切换的目标场景的对象
	reScene  =TransitionSlideInR::create(t, scene);
	// run
	//CCDirector::sharedDirector()->replaceScene(scene);
	Director::getInstance()->replaceScene(scene);
}

void WelcomeScene::menuOptionCallback(Ref* pSender)
{
	TransitionScene * reScene = NULL;
	float t = 1.2f;
	auto scene = GameLayer::createScene();
	reScene = TransitionSlideInR::create(t, scene);
	Director::getInstance()->replaceScene(reScene);
}

void WelcomeScene::menuRankListCallback(Ref* pSender)
{
	TransitionScene * reScene = NULL;
	float t = 1.2f;
	auto scene = GameLayer::createScene();
	reScene = TransitionSlideInR::create(t, scene);
	Director::getInstance()->replaceScene(reScene);
}

void WelcomeScene::menuInstructionCallback(Ref* pSender)
{
	TransitionScene * reScene = NULL;
	float t = 1.2f;
	auto scene = GameLayer::createScene();
	reScene = TransitionSlideInR::create(t, scene);
	Director::getInstance()->replaceScene(reScene);
}

void WelcomeScene::menuAboutCallback(Ref* pSender)
{
	TransitionScene * reScene = NULL;
	float t = 1.2f;
	auto scene = ABOUT::createScene();
	reScene = TransitionSlideInR::create(t, scene);
	Director::getInstance()->replaceScene(reScene);
}