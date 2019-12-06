﻿#include "ContainerConfig.hpp"
#include "ContainerElement.hpp"

#include <string>
#include <iostream>
#include <fstream>

#include <stdlib.h>


const char DEFAULT_CONF_PATH[] = u8"/lxc/conf";
const char *file_name_debug[] = {"/cluster.json","/ivi.json"};

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
	
	//本当はディレクトリサーチしないといけないけど、とりあえず決め打ち
	
	for (int i =0;i < 2;i++)
	{
		std::ifstream jsonfile(confpath + std::string(file_name_debug[i]));

		if (jsonfile.is_open())
		{
			char buf[256];
			std::string jsonstring;
			size_t size;
			
			do
			{
				jsonfile.getline(buf,256);
				jsonstring = jsonstring + std::string(buf);
			}
			while(jsonfile.eof() == false);
			
			CContainerElement *pelement = new CContainerElement();
			
			if (pelement->SetJsonString(jsonstring) == true)
			{
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

	for (int i =0;i < 2;i++)
	{
		this->m_ContainerElement[i]->DebugPrintJson();
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


