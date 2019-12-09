#include <stdio.h>

#include "ContainerConfig.hpp"
#include "lxc-runtime.hpp"

#include <iostream>

#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <sys/types.h>
#include <sys/signalfd.h>

//---------------------------------------------------------------------------------------------------------------
int signal_setup(void)
{
	int fd = -1;
	sigset_t mask;
	
	// signal mask
	::sigemptyset(&mask);
	::sigaddset(&mask, SIGPIPE);
	::sigaddset(&mask, SIGTERM);
	
	::pthread_sigmask(SIG_BLOCK, &mask, NULL);
	
	//setup signal fd
	::sigemptyset(&mask);
	::sigaddset(&mask, SIGTERM);
	
	// create signal fd
	fd = ::signalfd(-1, &mask, SFD_CLOEXEC | SFD_NONBLOCK);
	
	return fd;
}
//---------------------------------------------------------------------------------------------------------------
int main (int argc, char *argv[]) 
{
	int sigfd = -1, ret = -1;
	struct pollfd pfds[1];
	struct signalfd_siginfo fdsi;
	std::vector< std::string > vstr;
	
	sigfd = signal_setup();
	
	// setup poll
	::memset(pfds,0,sizeof(pfds));
	pfds[0].fd = sigfd;
	pfds[0].events = POLLIN;
	pfds[0].revents = 0;
	
	CLXCRuntime *pLXC = new CLXCRuntime();
	
	pLXC->GetGuestList(vstr);
	
	for(int i=0;i<vstr.size();i++)
	{
		std::cout << "exec: " << vstr[i] << std::endl;
		pLXC->ExecGuestContainer(vstr[i]);
	}
	
	
	do
	{
		ret = poll(pfds, sizeof(pfds) / sizeof(struct pollfd), -1);
		
		if (ret == 1)
		{
			ret = read(pfds[0].fd, &fdsi, sizeof(struct signalfd_siginfo));
			if (ret == sizeof(struct signalfd_siginfo))
			{
				if ( fdsi.ssi_signo == SIGTERM)
				{
					break;
				}
			}
			
		}
		else 
		{
			//EINTR,EFAULT
			//EINVAL,ENOMEM
			if ((errno == EINVAL) || (errno == ENOMEM))
			{
				struct timespec wait;
				wait.tv_sec  = 0;
				wait.tv_nsec = 100 * 1000 * 1000;
				
				::nanosleep(&wait, NULL);
			}
		}
	
	}
	while(1);
	
	for(int i=0;i<vstr.size();i++)
	{
		std::cout << "shoutdown: " << vstr[i] << std::endl;
		pLXC->ShoutdownGuestContainer(vstr[i]);
	}
	
	
	delete pLXC;
	
	return 0;
}
