#include <stdio.h>

#include "ContainerConfig.hpp"
#include "lxc-runtime.hpp"

#include <iostream>

int main (int argc, char *argv[]) 
{
	std::vector< std::string > vstr;
	
	CLXCRuntime *pLXC = new CLXCRuntime();
	
	pLXC->GetGuestList(vstr);
	
	for(int i=0;i<vstr.size();i++)
	{
		std::cout << vstr[i] << std::endl;
		pLXC->ExecGuestContainer(vstr[i]);
	}
	
	
	delete pLXC;
	
	return 0;
}
