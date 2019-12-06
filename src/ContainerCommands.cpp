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
	std::string test = std::string("cat /cross/container-dev/agl-container-manager/container/ivi.json");
	SyncExecCommand(test);
}
//-----------------------------------------------------------------------------
bool CCANCommand::ExecPostStartCommand()
{
	
}
//-----------------------------------------------------------------------------
bool CCANCommand::ExecPreShutdownCommand()
{
	
}
//-----------------------------------------------------------------------------
bool CCANCommand::ExecPostShutdownCommand()
{
	
}
//-----------------------------------------------------------------------------
bool CCANCommand::SetVXCANDeviceNames(std::string host, std::string guest)
{
	this->m_HostDevice = host;
	this->m_GuestDevice = guest;
	
	return true;
}
//-----------------------------------------------------------------------------



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
	pstr = (char*)::malloc(length);
	if (pstr == NULL) return -1;
	
	::memset(args,0,sizeof(args));
	::memcpy(pstr, command.c_str(), length);
	
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

