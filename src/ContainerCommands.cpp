#include "ContainerCommands.hpp"
#include <string>
#include <iostream>
#include <fstream>

#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>


static const char hostcandev[] = "vcan0";
//-----------------------------------------------------------------------------
int SyncExecCommand(std::string command);


//-----------------------------------------------------------------------------
CCANCommand::CCANCommand()
{
	
}
//-----------------------------------------------------------------------------
CCANCommand::~CCANCommand()
{
	
}
//-----------------------------------------------------------------------------
bool CCANCommand::ExecPreStartCommand()
{
	std::string hostdev = std::string(hostcandev);
	std::string command;
	bool result = false;
	int num, ret = -1;
	
	//create device and host side up
	command = std::string("ip link add ") + this->m_HostDevice + std::string(" type vxcan peer name ") + this->m_GuestDevice;
	std::cout << "command: " << command << std::endl;
	ret = SyncExecCommand(command);
	
	if (ret == 0)
	{
		command = std::string("ip link set ") + this->m_HostDevice + std::string(" up");
		std::cout << "command: " << command << std::endl;
		ret = SyncExecCommand(command);
		
		if (ret == 0)
		{
			result = true;
			
			//set route
			num = this->m_RuleReceive.size();
			for(int i=0;i<num;i++)
			{
				command = std::string("cangw -A ") + std::string("-s ") + hostdev + std::string(" -d ") + this->m_HostDevice + std::string(" ") + m_RuleReceive[i];
				std::cout << "command: " << command << std::endl;
				ret = SyncExecCommand(command);
			}
			
			num = this->m_RuleSend.size();
			for(int i=0;i<num;i++)
			{
				command = std::string("cangw -A ") + std::string("-s ") + this->m_HostDevice + std::string(" -d ") + hostdev + std::string(" ") + m_RuleReceive[i];
				std::cout << "command: " << command << std::endl;
				ret = SyncExecCommand(command);
			}
		}
	}

	return result;
}
//-----------------------------------------------------------------------------
bool CCANCommand::ExecPostStartCommand()
{
	return true;
}
//-----------------------------------------------------------------------------
bool CCANCommand::ExecPreStopCommand()
{
	return true;
}
//-----------------------------------------------------------------------------
bool CCANCommand::ExecPostStopCommand()
{
	std::string command;
	int ret = -1;
	
	//create device and host side up
	command = std::string("ip link del ") + this->m_HostDevice;
	std::cout << "command: " << command << std::endl;
	ret = SyncExecCommand(command);
	
	return true;
}
//-----------------------------------------------------------------------------
bool CCANCommand::SetVXCANDeviceNames(std::string host, std::string guest)
{
	this->m_HostDevice = host;
	this->m_GuestDevice = guest;
	
	return true;
}
//-----------------------------------------------------------------------------
bool CCANCommand::SetGatewayRuleReceive(std::string rule)
{
	int num = this->m_RuleReceive.size();
	this->m_RuleReceive.resize(num+1);
	
	this->m_RuleReceive[num] = rule;
	
	return true;
}
//-----------------------------------------------------------------------------
bool CCANCommand::SetGatewayRuleSend(std::string rule)
{
	int num = this->m_RuleSend.size();
	this->m_RuleSend.resize(num+1);
	
	this->m_RuleSend[num] = rule;
	
	return true;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------


int SyncExecCommand(std::string command)
{
	pid_t pid,ret;
	char *pstr,*args[32];
	int length, num;
	int status;
	int result = -1;
	
	length = command.length();
	pstr = (char*)::malloc(length+1);
	if (pstr == NULL) return -1;
	
	::memset(args,0,sizeof(args));
	::memcpy(pstr, command.c_str(), length);
	pstr[length] = '\0';
	
	args[0] = &pstr[0];
	
	for (int i=1,num=1;i < length && num < 31;i++)
	{
		if (pstr[i] == '\0') break;
		
		if (pstr[i-1] == ' ')
		{
			pstr[i-1] = '\0';
			args[num] = &pstr[i];
			num++;
		}
	}
	
	pid = fork();
	
	if (pid >= 0)
	{
		if (pid == 0)
		{
			//child process
			execvp(args[0], args);
			
			_exit(-1);
		}
		
		if (pid > 0)
		{
			ret = waitpid(pid, &status, 0);
			
			if (ret >= 0 )
			{
				if ( WIFEXITED(status) )
				{
					result = WEXITSTATUS(status);
				}
			}
		}
	}
	
	return result;
}

