#include "Role.h"

const char * Role::XML_FILE_NAME = "roles.xml" ;
Vector<Role*> Role::s_roleVec ;
bool Role::initStatic()
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

bool Role::parseData(XMLElement * pElement)
{
	s_roleVec.clear() ;
	XMLElement * child = pElement->FirstChildElement() ;
	for (;child;child = child->NextSiblingElement())
	{
		if (strcmp(child->Value(),"role") == 0)
		{
			Role * pRol = new Role() ;
			if (!pRol->init(child))
			{
				CC_SAFE_RELEASE_NULL(pRol); 
				return false ;
			}
			s_roleVec.pushBack(pRol) ;
			pRol->release() ;
		}
	}

	return true ;
}

void Role::finalizeStatic()
{
	s_roleVec.clear() ;
}
Role::Role()
	:m_id(-1)
	,m_pImageName(nullptr)
	,m_type(kRoleTypeNone)
	,m_width(0)
	,m_height(0)
{

}

Role::~Role()
{
	CC_SAFE_RELEASE_NULL(m_pImageName) ;
}

bool Role::init(XMLElement * pElement)
{
	const char * pId = pElement->Attribute("id") ;
	IF_NULL_RETURN_FALSE(pId) ;
	m_id = atoi(pId) ;

	const char * pImageName = pElement->Attribute("image_name") ;
	IF_NULL_RETURN_FALSE(pImageName) ;
	m_pImageName = new __String(pImageName) ;

	const char* pType = pElement->Attribute("type") ;
	IF_NULL_RETURN_FALSE(pType);
	m_type = getTypeByChar(pType) ;

	const char * pWidth = pElement->Attribute("width") ;
	IF_NULL_RETURN_FALSE(pWidth);
	m_width = atoi(pWidth); 
	
	const char * pHeight = pElement->Attribute("height") ;
	IF_NULL_RETURN_FALSE(pHeight);
	m_height = atoi(pHeight); 


	log("Role:%d----%d-----%d",m_id,m_width,m_height);
	return true;
}

RoleType Role::getTypeByChar(const char * pType)
{
	if (strcmp("boss",pType) == 0)
	{
		return kRoleTypeBoss ;
	}
	else if (strcmp("soldier",pType) == 0)
	{
		return kRoleTypeSoldier ;
	}
	else if (strcmp("general_ver",pType) == 0)
	{
		return kRoleTypeArmyGeneralVertical;
	}
	else if (strcmp("general_hor",pType) == 0)
	{
		return kRoleTypeArmyGeneralHorizontal;
	}

	return kRoleTypeNone ;
}