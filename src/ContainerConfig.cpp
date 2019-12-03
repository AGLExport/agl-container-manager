#include "ContainerConfig.hpp"
#include "ConfigElement.hpp"

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
			
			CConfigElement *pelement = new CConfigElement();
			
			if (pelement->SetJsonString(jsonstring) == true)
			{
				size = this->m_ConfElement.size();
				this->m_ConfElement.resize(size + 1);
				
				this->m_ConfElement[size] = pelement;
				
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
		this->m_ConfElement[i]->DebugPrintJson();
	}
}
//-----------------------------------------------------------------------------
CContainerConfig::~CContainerConfig()
{
	size_t num = this->m_ConfElement.size();
	
	for (int i =0;i < num;i++)
	{
		delete this->m_ConfElement[i];
	}
	
	this->m_ConfElement.resize(0);
}
//-----------------------------------------------------------------------------
bool CContainerConfig::GetGuestList(std::vector< std::string > &guestlist)
{
	size_t size = this->m_ConfElement.size();

	guestlist.resize(size);
	
	for(int i=0;i < size;i++)
	{
		this->m_ConfElement[i]->GetName(guestlist[i]);
	}
}
//-----------------------------------------------------------------------------
bool CContainerConfig::GetElementByName(std::string guestname, CConfigElement **ppelement)
{
	bool findelem = false;
	size_t size = this->m_ConfElement.size();
	std::string name;

	for(int i=0;i < size;i++)
	{
		
		if (this->m_ConfElement[i]->GetName(name) == true)
		{
			if (name == guestname)
			{
				(*ppelement) = m_ConfElement[i];
				findelem = true;
			}
		}
	}
	
	return findelem;
}



//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


