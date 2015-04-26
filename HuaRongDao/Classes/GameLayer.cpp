#include "GameLayer.h"
#include "RoleSprite.h"
#include "data/Role.h"
#include "data/Level.h"  
#include "GameDefine.h" 

USING_NS_CC ;
Scene* GameLayer::createScene()
{
	auto gameSceneInstance = Scene::create() ;
	auto layer = GameLayer::create() ;
	layer->setTag(0);
	gameSceneInstance->addChild(layer, 0, "gameLayer");
	return gameSceneInstance;
}
GameLayer::GameLayer()
	:m_pLevel(nullptr)
	,m_pSeleNode(nullptr)
	,m_touchBegin(Vec2(0,0))
	,m_touchMove(Vec2(0,0))
	,m_minDis(0)
{
	m_pLevel = Level::s_levelVec.at(0);
	CC_SAFE_RETAIN(m_pLevel) ;
	m_minDis = 20.0f ;
	steps = 0;
	m_time = 0;
}

GameLayer::~GameLayer()
{
	CC_SAFE_RELEASE_NULL(m_pLevel) ;
}

//Scene* GameLayer::s2GameSceneInstance()
//{
//	//类GameLayer 不能访问自己定义的静态变量.
//	return GameLayer::game333SceneInstance;
//}

//GameLayer* GameLayer::gameLayer()
//{
//	return (GameLayer*)game333SceneInstance->getChildByName("gameLayer");
//}

void GameLayer::resetPLevel(Level* level)
{
	m_pLevel = level;
	CC_SAFE_RETAIN(m_pLevel) ;

}

bool GameLayer::init()
{
	Size size = Director::getInstance()->getWinSize();
	
	auto bgSprite = Sprite::create("main_bg0.png") ;
	bgSprite->setPosition(size.width / 2,size.height / 2) ;
	addChild(bgSprite);
	
	auto exitSprite = Sprite::create("exit.png");
	exitSprite->setPosition(size.width / 2, GAME_SCREEN_HEIGHT * 50 / 480 - exitSprite->getContentSize().height/3);
	addChild(exitSprite);

	//initMap() ;

	auto lisener = EventListenerTouchOneByOne::create() ;
	lisener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan,this) ;
	lisener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved,this) ;
	lisener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lisener,this) ;

	// 加载ttf字体  
	TTFConfig config("fonts/Marker Felt.ttf", 20);
	// 游戏时间的显示  
	auto labelTime = Label::createWithTTF(config, StringUtils::format("%ds", m_time));
	labelTime->setPosition(Vec2(GAME_SCREEN_WIDTH * 180 / 320 - labelTime->getContentSize().width / 2, labelTime->getContentSize().height ));
	labelTime->setTag(11);
	this->addChild(labelTime);

	auto lsteps = LabelAtlas::create("Steps", "num.png", 17.0f, 30.0f, '0');
	//auto lsteps = LabelAtlas::create("Steps", "num2.png", 25.7f, 50.0f, '0');
	lsteps->setPosition(Point(GAME_SCREEN_WIDTH * 260 / 320, lsteps->getContentSize().height - 5));
	lsteps->setAnchorPoint(Point(0.5, 0.5));//原来的锚点在(0,0)  
	lsteps->setTag(12);
	this->addChild(lsteps);
	lsteps->setString("0");

	schedule(schedule_selector(GameLayer::myClock), 1.0f);
	
	return true ;
}

void GameLayer::initMap()
{
	Size size = Director::getInstance()->getWinSize();

	float x = 0 ;
	float y = 0 ;

	float paddingLeft   = 10.0f ;
	float paddingBottom = 53.0f ;

	for (unsigned i = 0 ; i < m_pLevel->getRoleID().size() ;i++)
	{
		Role * pRole = Role::s_roleVec.at(m_pLevel->getRoleID().at(i)) ;
		Sprite * pp = Sprite::create(pRole->getImageName()->getCString());
		int col = m_pLevel->getRolePos().at(i).x ;
		int row = m_pLevel->getRolePos().at(i).y ;
		x = paddingLeft + col * 1.0f * 75.0f ;
		y = paddingBottom + row * 1.0f * 75.0f ;
		RoleSprite * pRoleSprite = RoleSprite::create(pRole,row,col,Rect(x,y,pp->getContentSize().width,pp->getContentSize().height),this,this->getLocalZOrder()) ;
		m_pRoleSpriteVec.pushBack(pRoleSprite) ;
	}
	updateMapData() ;
}

bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto loction = this->convertToNodeSpace(touch->getLocation()) ;
	
	for (unsigned int i = 0 ; i < m_pRoleSpriteVec.size() ; i++)
	{
		RoleSprite * pRoleSprite = m_pRoleSpriteVec.at(i) ;
		if (pRoleSprite->getBoundingBox().containsPoint(loction))
		{
			m_touchBegin = loction ;
			m_pSeleNode = pRoleSprite ;
			break;
		}
	}

	return true ;
}
void GameLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	if ((m_pSeleNode && m_pSeleNode->getNumberOfRunningActions() > 0) || !m_pSeleNode)
	{
		return ;
	}

	auto loction = this->convertToNodeSpace(touch->getLocation()) ;
	m_touchMove = loction ;
	float dis = m_touchMove.getDistance(m_touchBegin) ;
	auto var = m_touchMove - m_touchBegin ;
	auto moveByPos = Vec2(0,0) ;
	auto varColRow = Vec2(0,0) ;
	if (dis >= m_minDis)//移动
	{
		bool isMoveHor = abs(var.x) > abs(var.y) ;
		bool isMoveTop  ;
		bool isMoveRight ;
		if (isMoveHor)//水平方向移动
		{
			isMoveRight = var.x > 0 ;
			if (var.x > 0)//right
			{
				moveByPos = Vec2(75.0f,0) ;
				varColRow = Vec2(1,0) ;
			}
			else//left
			{
				moveByPos = Vec2(-75.0f,0) ;
				varColRow = Vec2(-1,0) ;
			}
		}
		else//竖直方向移动
		{
			isMoveTop = var.y > 0 ;
			if (var.y > 0)//top
			{
				moveByPos = Vec2(0,75.0f) ;
				varColRow = Vec2(0,1) ;
			}
			else//bottom
			{
				moveByPos = Vec2(0,-75.0f) ;
				varColRow = Vec2(0,-1) ;
			}
		}
		if (isCanMove(isMoveHor,isMoveHor ? isMoveRight : isMoveTop))
		{
			m_pSeleNode->setCol(m_pSeleNode->getCol() + (int)varColRow.x) ;
			m_pSeleNode->setRow(m_pSeleNode->getRow() + (int)varColRow.y) ;
			m_pSeleNode->runAction(
				MoveBy::create(0.5f,moveByPos)
				) ;
			updateMapData() ;

			// step increase 1
			steps++;

			LabelAtlas *labelSteps = (LabelAtlas *)this->getChildByTag(12);
			labelSteps->setString(StringUtils::format("%d", steps));
		}
	}
}
void GameLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_touchBegin = Vec2(0,0) ;
	m_touchMove = Vec2(0,0) ;
	m_pSeleNode = nullptr ;
}

bool GameLayer::isCanMove(bool isMoveHor,bool moveAdd)
{
	bool isMove = false;
	CCAssert(m_pSeleNode,"select node should not is null!!") ;
	int width = m_pSeleNode->getWidth() ;
	int height = m_pSeleNode->getHeight() ;
	int row = m_pSeleNode->getRow() ;
	int col = m_pSeleNode->getCol() ;
	if (isMoveHor)//水平方向
	{
		if (moveAdd)
		{
			int colTem = col + width;
			int rowTem = row ;
 			int filledNum = 0 ;
 			for (int i = 0; i < height ; i++)
 			{
				rowTem += i;
				if (m_mapState[rowTem][colTem] == 1)
				{
					filledNum++ ;
				}
 			}
			isMove = (filledNum == 0) ? true : false;
			if (m_pSeleNode->getCol() + width -1 == 3)
			{
				isMove = false ;
			}
		}
		else
		{
			int colTem = col - 1;
			int rowTem = row ;
			int filledNum = 0 ;
			for (int i = 0; i < height ; i++)
			{
				rowTem += i;
				if (m_mapState[rowTem][colTem] == 1)
				{
					filledNum++ ;
				}
			}
			isMove = (filledNum == 0) ? true : false;
			if (m_pSeleNode->getCol() == 0)
			{
				isMove = false ;
			}
		}
	}
	else//竖直方向
	{
		if (moveAdd)
		{
			int colTem = col ;
			int rowTem = row + height ;
			int filledNum = 0 ;
			for (int i = 0; i < width; i++)
			{
				colTem += i;
				if (m_mapState[rowTem][colTem] == 1)
				{
					filledNum++ ;
				}
			}
			isMove = (filledNum == 0) ? true : false;
			if (m_pSeleNode->getRow() + height -1 == 4)
			{
				isMove = false ;
			}
		}
		else
		{
			int colTem = col ;
			int rowTem = row - 1 ;
			int filledNum = 0 ;
			for (int i = 0; i < width; i++)
			{
				colTem += i;
				if (m_mapState[rowTem][colTem] == 1)
				{
					filledNum++ ;
				}
			}
			isMove = (filledNum == 0) ? true : false;
			if (m_pSeleNode->getRow() == 0)
			{
				isMove = false;
				// game over check.
				if (m_pSeleNode->getRole()->getType() == kRoleTypeBoss && m_pSeleNode->getCol() == 1)
				{
					isMove = true ;//只有曹操能从这走
					// you win the game.
				}
			}
		}
	}
	return isMove ;
}

void GameLayer::updateMapData()
{
	resetMapData() ;
	if (m_pRoleSpriteVec.size() == 10)//<需要10个角色1Boss，5将军，4兵
	{
		for (unsigned int i = 0 ;i < m_pRoleSpriteVec.size() ;i++)
		{
			RoleSprite * pRoleSprite = m_pRoleSpriteVec.at(i) ;
			int width = pRoleSprite->getWidth() ;
			int heigit = pRoleSprite->getHeight() ;
			int row = pRoleSprite->getRow() ;
			int col = pRoleSprite->getCol() ;
			for (int i = 0 ; i < heigit ; i++)
			{
				for (int j = 0 ; j < width ; j++)
				{
					m_mapState[row + i][col + j] = 1 ;
				}
			}
		}
	}
	/*CCLOG("================Test==========================") ;
	for (int i = 0; i < 5 ; i++)
	{
	const char *ppp = "" ;
	for (int j = 0; j < 4; j++)
	{
	ppp = CCString::createWithFormat("%s%d,",ppp,m_mapState[i][j])->getCString() ;
	}
	CCLOG(ppp) ;
	}*/
}
void GameLayer::resetMapData()
{
	for (int row = 0 ;row < 5 ; row++)
	{
		for (int col = 0 ; col < 4 ; col++)
		{
			m_mapState[row][col] = 0; 
		}
	}
}

void GameLayer::myClock(float dt)
{
	// 时间变化  
	++m_time;
	//// 如果时间归0，那么执行游戏结束动画，并跳到游戏结束界面  
	//if (m_time == 0)
	//{
	//	Label *labelTime = (Label *)this->getChildByTag(11);
	//	labelTime->setScale(0);

	//	// 游戏结束 动画  
	//	auto gmov = Sprite::create("pic_gameover.png");
	//	gmov->setPosition(Point(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT*1.5));
	//	this->addChild(gmov);

	//	auto action = MoveTo::create(3.0f, Point(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));

	//	gmov->runAction(action);

	//	// 跳转到游戏结束界面  
	//	return;
	//}
	// 如果还有时间，那么时间减少  
	if (m_time > 0) {
		Label *labelTime = (Label *)this->getChildByTag(11);
		labelTime->setString(getFormatTime(m_time));
		labelTime->setPosition(Vec2(GAME_SCREEN_WIDTH * 180 / 320 - labelTime->getContentSize().width / 2, labelTime->getContentSize().height));
	}
}

std::string GameLayer::getFormatTime(int t){
	std::string formatTime = "";
	int h = 0;
	int m = 0;
	int s = 0;
	h = t / 3600;
	m = t % 3600 / 60;
	s = t % 3600 % 60 % 60;
	formatTime = StringUtils::format("%dh:%dm:%ds", h, m, s);

	return formatTime;
}