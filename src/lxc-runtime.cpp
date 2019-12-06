#include "lxc-runtime.hpp"


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//-----------------------------------------------------------------------------
// public
//-----------------------------------------------------------------------------
CLXCRuntime::CLXCRuntime()
{
	
	
}
//-----------------------------------------------------------------------------
CLXCRuntime::~CLXCRuntime()
{
	
	
}
//-----------------------------------------------------------------------------
//bool CLXCRuntime::SetConfig(CContainerConfig *pconfig)
//{
//	this->m_pContainerConfig = pconfig;
//}
//-----------------------------------------------------------------------------
bool CLXCRuntime::GetGuestList(std::vector< std::string > &guestlist)
{
	return this->m_ContainerConfig.GetGuestList(guestlist);
}
//-----------------------------------------------------------------------------
bool CLXCRuntime::ExecGuestContainer(std::string guest)
{
	bool result = false;
	CContainerElement *pelem = NULL;
	
	if (this->m_ContainerConfig.GetElementByName(guest, &pelem) == true)
	{
		result = pelem->ExecContainer();
	}
	
	return result;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// protected
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

