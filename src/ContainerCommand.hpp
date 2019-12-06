#ifndef	CONTAINER_COMMAND_H
#define	CONTAINER_COMMAND_H
//-----------------------------------------------------------------------------
#include "ContainerBase.hpp"


//-----------------------------------------------------------------------------
class CContainerCommand : public CContainerBase
{
private :
protected :
	
	
public:
	// methods
	virtual bool ExecPreStartCommand() = 0;
	virtual bool ExecPostStartCommand() = 0;
	
	virtual bool ExecPreShutdownCommand() = 0;
	virtual bool ExecPostShutdownCommand() = 0;

	
	CContainerCommand(){}
	CContainerCommand(const CContainerCommand&) = delete;
	CContainerCommand &operator = (const CContainerCommand&) = delete;
	virtual ~CContainerCommand(){}
};
//-----------------------------------------------------------------------------
#endif	//#ifndef	CONTAINER_COMMAND_H

