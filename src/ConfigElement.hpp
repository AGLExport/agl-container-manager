#ifndef	CONTAINER_ELEMENT_H
#define	CONTAINER_ELEMENT_H
//-----------------------------------------------------------------------------
#include "ContainerBase.hpp"
#include <string>
#include <json/json.h>


//-----------------------------------------------------------------------------
class CConfigElement : public CContainerBase
{
private :
protected :
	bool m_Valid;
	std::string m_JsonString;
	Json::Value m_JsonValue;
	
	
	
public:
	// set methods
	bool SetJsonString(std::string &jsonstring);
	
	// get methods
	bool GetName(std::string &name);
	bool GetJsonValue(Json::Value &jsonvalue);
	
	
	CConfigElement();
	CConfigElement(const CConfigElement&) = delete;
	CConfigElement &operator = (const CConfigElement&) = delete;
	virtual ~CConfigElement();

	//debug
	void DebugPrintJson();

};
//-----------------------------------------------------------------------------
#endif	//#ifndef CONTAINER_CONFIG_H
