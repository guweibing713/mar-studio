#include "Level.h"

#define  IF_NULL_RETURN_FALSE(_x_) if(_x_ == nullptr) return false 

const char * Level::XML_FILE_NAME = "level.xml" ;
Vector<Level*> Level::s_levelVec ;
bool Level::initStatic()
{
	std::string filePath = FileUtils::getInstance()->fullPathForFilename(XML_FILE_NAME) ;

	tinyxml2::XMLDocument pDoc;

	FileUtils::getInstance()->setPopupNotify(false) ;
	ssize_t fileSize = 0 ;
	std::string data = FileUtils::getInstance()->getStringFromFile(filePath.c_str()); 
	FileUtils::getInstance()->setPopupNotify(true) ;

	pDoc.Parse(data.c_str()) ;

	XMLElement * pEle = pDoc.RootElement() ;

	return parseData(pEle) ;
}

bool Level::parseData(XMLElement * pElement)
{
	s_levelVec.clear() ;
	XMLElement * child = pElement->FirstChildElement() ;
	for (;child;child = child->NextSiblingElement())
	{
		if (strcmp(child->Value(),"level") == 0)
		{
			Level * pRol = new Level() ;
			if (!pRol->init(child))
			{
				CC_SAFE_RELEASE_NULL(pRol); 
				return false ;
			}
			s_levelVec.pushBack(pRol) ;
			pRol->release() ;
		}
	}

	return true ;
}
void Level::finalizeStatic()
{
	s_levelVec.clear() ;
}

std::vector<std::string> Level::splitString(std::string str,std::string parttern)
{
	std::string::size_type pos;
	std::vector<std::string> result ;
	str+=parttern;
	unsigned int size=str.size();
	for(unsigned int i=0; i<size; i++)
	{
		pos=str.find(parttern,i);
		if(pos<size)
		{
			std::string s=str.substr(i,pos-i);
			result.push_back(s);
			i=pos+parttern.size()-1;
		}
	}
	return result;
}

std::vector<int> Level::splitInt(std::string str,std::string parttern)
{
	std::vector<std::string> strVec = splitString(str,parttern);
	std::vector<int> result ;

	std::vector<std::string>::iterator iter;
	for ( iter = strVec.begin() ; iter != strVec.end() ; iter++ )
	{
		int value = atoi((*iter).c_str());
		result.push_back(value) ;
	}

	return result ;
}
Level::Level()
	:m_id(-1)
{

}

Level::~Level()
{

}

bool Level::init(XMLElement * pElement)
{
	IF_NULL_RETURN_FALSE(pElement->Attribute("id")) ;
	const char * pId =  pElement->Attribute("id") ;
	m_id = atoi(pId) ;

	IF_NULL_RETURN_FALSE(pElement->Attribute("role_id")) ;
	m_roleID = splitInt(pElement->Attribute("role_id"),",") ;
	std::vector<int> rowVec ;
	IF_NULL_RETURN_FALSE(pElement->Attribute("row")) ;
	rowVec = splitInt(pElement->Attribute("row"),",") ;
	std::vector<int> colVec ;
	IF_NULL_RETURN_FALSE(pElement->Attribute("col")) ;
	colVec = splitInt(pElement->Attribute("col"),",") ;

	for(unsigned int i = 0; i < m_roleID.size() ; i++)
	{
		auto pos = Vec2(colVec.at(i),rowVec.at(i)) ;
		m_rolePos.push_back(pos) ;
	}

	CCLOG("ID:%d",m_id) ;
	return true ;
}