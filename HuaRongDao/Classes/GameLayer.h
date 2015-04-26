#ifndef _GAME_LAYER_H_
#define _GAME_LAYER_H_

#include "cocos2d.h"

class Level ;
class RoleSprite ;
class GameLayer : public cocos2d::Layer
{
public:
	 static cocos2d::Scene* createScene();
	 CREATE_FUNC(GameLayer);
public:
	GameLayer();
	~GameLayer();
	virtual bool init();

	//@biref 初始化地图
	void initMap() ;
	//@biref 更新m_mapState
	void updateMapData() ;
	void resetMapData();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) ;
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) ;
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) ;

	//@biref 是否可以移动，参数：要移动的方向,是往加的方向移动还是往减的方向移动
	bool isCanMove(bool isMoveHor,bool moveAdd);

	void myClock(float dt);
	std::string getFormatTime(int t);
	static GameLayer* gameLayer();
	static cocos2d::Scene* s2GameSceneInstance();
	void resetPLevel(Level* level);

	void menuStartCallback(Ref* pSender);
private:
	//Level * m_pLevel;//<关卡数据的引用
	cocos2d::Vector<RoleSprite*> m_pRoleSpriteVec ;//<存储角色精灵
	cocos2d::Vec2 m_touchBegin ;//<开始点击的位置
	cocos2d::Vec2 m_touchMove ;//<移动过程中的点
	float m_minDis ;//<最小移动距离，>=他的时候才判断是否移动
	RoleSprite * m_pSeleNode ;//<选中的角色
	int m_mapState[5][4] ;//<5行4列，0是空，1是已填充

	int steps; 
	int m_time;
	CC_SYNTHESIZE(Level *, m_pLevel, PLevel);//所选关卡

	static cocos2d::Scene* game333SceneInstance;

};
#endif