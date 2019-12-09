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
	
	virtual bool ExecPreStopCommand() = 0;
	virtual bool ExecPostStopCommand() = 0;

	
	CContainerCommand(){}
	CContainerCommand(const CContainerCommand&) = delete;
	CContainerCommand &operator = (const CContainerCommand&) = delete;
	virtual ~CContainerCommand(){}
};
//-----------------------------------------------------------------------------
#endif	//#ifndef	CONTAINER_COMMAND_H

