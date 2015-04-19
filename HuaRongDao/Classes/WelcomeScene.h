#ifndef __WELCOME_SCENE_H__  
#define __WELCOME_SCENE_H__  

#include "cocos2d.h"  

class WelcomeScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuStartCallback(Ref* pSender);
	CREATE_FUNC(WelcomeScene);

	CC_SYNTHESIZE(int, m_level, GameLevel);//ËùÑ¡¹Ø¿¨
};

#endif // __WELCOME_SCENE_H__