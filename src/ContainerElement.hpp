#ifndef	CONTAINER_ELEMENT_H
#define	CONTAINER_ELEMENT_H
//-----------------------------------------------------------------------------
#include "ContainerBase.hpp"
#include "ContainerCommand.hpp"
#include <string>
#include <json/json.h>
#include <lxc/lxccontainer.h>

//-----------------------------------------------------------------------------
class CContainerElement : public CContainerBase
{
private :
protected :
	bool m_Valid;
	bool m_autoboot;
	struct lxc_container *m_pLxcContainer;
	
	std::string m_JsonString;
	Json::Value m_JsonValue;
	std::string m_GuestName;
	std::string m_CofigFileNameWithPath;
	std::vector< CContainerCommand* > m_Commands;
	
	bool GeneratConfig(std::string basepath);
	bool RunPreStartProcess();
	bool RunPostStartProcess();
	bool RunPreStopProcess();
	bool RunPostStopProcess();
	
	void HeaderOut(std::ofstream &ofs);
	void BasicConfigOut(std::ofstream &ofs, Json::Value &json);
	void ResourceConfigOut(std::ofstream &ofs, Json::Value &json);
	void MountConfigOut(std::ofstream &ofs, Json::Value &json);
	void NetworkConfigOut(std::ofstream &ofs, Json::Value &json);
	void ICCConfigOut(std::ofstream &ofs, Json::Value &json);
	
	
public:
	// set methods
	virtual bool SetJsonString(std::string &jsonstring);
	
	// get methods
	virtual bool GetName(std::string &name);
	virtual bool GetJsonValue(Json::Value &jsonvalue);
	
	// methods
	virtual bool SetupContainer();
	virtual bool StartContainer();
	virtual bool StopContainer();

	
	
	CContainerElement();
	CContainerElement(const CContainerElement&) = delete;
	CContainerElement &operator = (const CContainerElement&) = delete;
	virtual ~CContainerElement();

	//debug
	void DebugPrintJson();

};
//-----------------------------------------------------------------------------
#endif	//#ifndef	CONTAINER_ELEMENT_H
