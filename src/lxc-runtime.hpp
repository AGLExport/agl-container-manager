#ifndef	LXC_RUNTIME_H
#define	LXC_RUNTIME_H
//-----------------------------------------------------------------------------
#include "ContainerConfig.hpp"
#include "container-runtime.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

//-----------------------------------------------------------------------------
class CLXCRuntime : public IContainerRuntime
{
private :
protected :
	CContainerConfig m_ContainerConfig;
	
	bool CreateGuestConfig(std::string guest);
	
	void HeaderOut(std::ofstream &ofs);
	void BasicConfigOut(std::ofstream &ofs, Json::Value &json);
	void FsMountConfigOut(std::ofstream &ofs, Json::Value &json);
	void DevMountConfigOut(std::ofstream &ofs, Json::Value &json);
	void NetworkConfigOut(std::ofstream &ofs, Json::Value &json);
	void ICCConfigOut(std::ofstream &ofs, Json::Value &json);
	
public:
	virtual bool GetGuestList(std::vector< std::string > &guestlist);
	
	virtual bool ExecGuestContainer(std::string guest);
	
	
	
	CLXCRuntime();
	CLXCRuntime(const CLXCRuntime&) = delete;
	CLXCRuntime &operator = (const CLXCRuntime&) = delete;
	virtual ~CLXCRuntime();

};
//-----------------------------------------------------------------------------
#endif	//#ifndef	LXC_RUNTIME_H
