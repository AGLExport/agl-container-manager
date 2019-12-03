#ifndef	CONTAINER_RUNTIME_H
#define	CONTAINER_RUNTIME_H
//-----------------------------------------------------------------------------
#include "ContainerConfig.hpp"
#include <string>


//-----------------------------------------------------------------------------
class IContainerRuntime : public CContainerBase
{
private :
protected :
	
	
	
public:
	virtual bool GetGuestList(std::vector< std::string > &guestlist) = 0;
	
	virtual bool ExecGuestContainer(std::string guest) = 0;
	
	
	IContainerRuntime(){}
	IContainerRuntime(const IContainerRuntime&) = delete;
	IContainerRuntime &operator = (const IContainerRuntime&) = delete;
	virtual ~IContainerRuntime(){}
};
//-----------------------------------------------------------------------------
#endif	//#ifndef	CONTAINER_RUNTIME_H
