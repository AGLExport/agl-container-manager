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
	
	
public:
	virtual bool GetGuestList(std::vector< std::string > &guestlist);
	
	virtual bool ExecGuestContainer(std::string guest);
	virtual bool ShoutdownGuestContainer(std::string guest);
	
	
	CLXCRuntime();
	CLXCRuntime(const CLXCRuntime&) = delete;
	CLXCRuntime &operator = (const CLXCRuntime&) = delete;
	virtual ~CLXCRuntime();

};
//-----------------------------------------------------------------------------
#endif	//#ifndef	LXC_RUNTIME_H
