#include "RoleSprite.h"
#include "data/Role.h"
USING_NS_CC ;
RoleSprite * RoleSprite::create(Role * pRole ,int row,int col,const cocos2d::Rect & bounds,cocos2d::Node * pParent ,int zOrder)
{
	auto pRet = new RoleSprite(pRole,row,col);

	pRet->setContentSize(bounds.size) ;
	pRet->setPosition(bounds.origin) ;
	pRet->setAnchorPoint(Vec2(0,0)) ;
	if (pRet->init())
	{
		pParent->addChild(pRet,zOrder) ;
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		return nullptr ;
	}

	return pRet ;
}

RoleSprite::RoleSprite(Role * role,int row,int col)
	:m_pRole(nullptr)
	,m_row(0)
	,m_col(0)
	,m_width(0)
	,m_height(0)
{
	m_pRole = role ;
	CC_SAFE_RETAIN(m_pRole) ; 
	m_row = row ;
	m_col = col ;

	m_width  = m_pRole->getWidth() ;
	m_height = m_pRole->getHeight() ;
}

RoleSprite::~RoleSprite()
{
	CC_SAFE_RELEASE_NULL(m_pRole);
}

bool RoleSprite::init()
{
	Sprite* pSkin = Sprite::create(m_pRole->getImageName()->getCString()) ;
	pSkin->setPosition(this->getBoundingBox().size.width / 2,this->getBoundingBox().size.height / 2) ;
	addChild(pSkin) ;
	return true;
}