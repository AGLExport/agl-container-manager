#include "ContainerConfig.hpp"
#include "ContainerElement.hpp"

#include <string>
#include <iostream>
#include <fstream>

#include <stdlib.h>

#include <sys/types.h>
#include <dirent.h>


const char DEFAULT_CONF_PATH[] = u8"/lxc/conf/";

//-----------------------------------------------------------------------------
CContainerConfig::CContainerConfig()
{
	std::string confpath;
	char *str = getenv("CONTAINER_MANAGER_CONF");
	
	if (str != NULL)
	{
		confpath = std::string(str);
	}
	
	if (confpath.empty()) confpath = std::string(DEFAULT_CONF_PATH);
	
	
	DIR *dir = NULL;
	dir = opendir(confpath.c_str());
	
	if (dir != NULL)
	{
		struct dirent *dent = NULL;
		
		do
		{
			dent = readdir(dir); 
			if (dent != NULL)
			{
				std::string filename = std::string(dent->d_name);
				
				if ( filename.find(std::string(".json")) != std::string::npos)
				{
					std::ifstream jsonfile(confpath + filename);
					
					if (jsonfile.is_open())
					{
						std::string tmpstring;
						std::string jsonstring;
						size_t size;
						
						do
						{
							std::getline(jsonfile,tmpstring);
							jsonstring = jsonstring + tmpstring;
						}
						while(jsonfile.eof() == false);
						
						CContainerElement *pelement = new(std::nothrow) CContainerElement();
						
						if ( pelement != NULL )
						{
							if (pelement->SetJsonString(jsonstring) == true)
							{
								pelement->SetupContainer();
								
								size = this->m_ContainerElement.size();
								this->m_ContainerElement.resize(size + 1);
								
								this->m_ContainerElement[size] = pelement;
								
							}
							else
							{
								delete pelement;
								pelement = NULL;
							}
						}
					}
				}
			}
		}
		while(dent != NULL);
	
		closedir(dir);
	}
}
//-----------------------------------------------------------------------------
CContainerConfig::~CContainerConfig()
{
	size_t num = this->m_ContainerElement.size();
	
	for (int i =0;i < num;i++)
	{
		delete this->m_ContainerElement[i];
	}
	
	this->m_ContainerElement.resize(0);
}
//-----------------------------------------------------------------------------
bool CContainerConfig::GetGuestList(std::vector< std::string > &guestlist)
{
	size_t size = this->m_ContainerElement.size();

	guestlist.resize(size);
	
	for(int i=0;i < size;i++)
	{
		this->m_ContainerElement[i]->GetName(guestlist[i]);
	}
	
	return true;
}
//-----------------------------------------------------------------------------
bool CContainerConfig::GetElementByName(std::string guestname, CContainerElement **ppelement)
{
	bool findelem = false;
	size_t size = this->m_ContainerElement.size();
	std::string name;

	for(int i=0;i < size;i++)
	{
		
		if (this->m_ContainerElement[i]->GetName(name) == true)
		{
			if (name == guestname)
			{
				(*ppelement) = m_ContainerElement[i];
				findelem = true;
			}
		}
	}
	
	return findelem;
}



//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


