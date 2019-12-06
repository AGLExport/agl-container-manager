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
	
#if 0	
	// method
	bool CreateGuestConfig(std::string guest);
	
	
	void HeaderOut(std::ofstream &ofs);
	void BasicConfigOut(std::ofstream &ofs, Json::Value &json);
	void MountConfigOut(std::ofstream &ofs, Json::Value &json);
	void NetworkConfigOut(std::ofstream &ofs, Json::Value &json);
	void ICCConfigOut(std::ofstream &ofs, Json::Value &json);
	
	bool GetDevNum(std::string node, dev_t &dev);
	bool CreateCANDevName(std::string &host, std::string &guest);
#endif
	
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
