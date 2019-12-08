#ifndef	CONTAINER_COMMANDS_H
#define	CONTAINER_COMMANDS_H
//-----------------------------------------------------------------------------
#include "ContainerCommand.hpp"
#include <string>
#include <vector>


//-----------------------------------------------------------------------------
class CCANCommand : public CContainerCommand
{
private :
protected :
	std::string m_HostDevice;
	std::string m_GuestDevice;
	
	std::vector< std::string > m_RuleReceive;
	std::vector< std::string > m_RuleSend;
	
public:
	// methods
	virtual bool ExecPreStartCommand();
	virtual bool ExecPostStartCommand();
	
	virtual bool ExecPreShutdownCommand();
	virtual bool ExecPostShutdownCommand();
	
	bool SetVXCANDeviceNames(std::string host, std::string guest);
	bool SetGatewayRuleReceive(std::string rule);
	bool SetGatewayRuleSend(std::string rule);
	
	
	CCANCommand();
	CCANCommand(const CCANCommand&) = delete;
	CCANCommand &operator = (const CCANCommand&) = delete;
	virtual ~CCANCommand();
};
//-----------------------------------------------------------------------------
#endif	//#ifndef	CONTAINER_COMMANDS_H
