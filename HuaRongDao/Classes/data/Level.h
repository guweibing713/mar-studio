#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "cocos2d.h"
USING_NS_CC ;
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2 ;
class Level : public Ref
{
public:
	static  cocos2d::Vector<Level*> s_levelVec ;
	static const char * XML_FILE_NAME ;
	static bool initStatic();
	static bool parseData(XMLElement * pElement) ;
	static void finalizeStatic();

	static std::vector<std::string> splitString(std::string str,std::string parttern) ;
	static std::vector<int> splitInt(std::string str,std::string parttern) ;
public:
	Level();
	~Level();
	bool init(XMLElement * pElement) ;
private:
	CC_SYNTHESIZE(int, m_id ,ID);
	CC_SYNTHESIZE(std::vector<int> ,m_roleID ,RoleID);
	CC_SYNTHESIZE(std::vector<Vec2> ,m_rolePos ,RolePos);
};
#endif