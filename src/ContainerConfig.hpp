#ifndef	CONTAINER_CONFIG_H
#define	CONTAINER_CONFIG_H
//-----------------------------------------------------------------------------
#include "ContainerBase.hpp"
#include "ConfigElement.hpp"

#include <vector>
#include <string>

//-----------------------------------------------------------------------------
class CContainerConfig : public CContainerBase
{
private:
protected:
	std::vector< CConfigElement* > m_ConfElement;
	
	
	
	
	
	
	
public:
	// set method
	
	
	// get method
	bool GetGuestList(std::vector< std::string > &guestlist);
	bool GetElementByName(std::string guestname, CConfigElement **ppelement);
	
	CContainerConfig();
	CContainerConfig(const CContainerConfig&) = delete;
	CContainerConfig &operator = (const CContainerConfig&) = delete;
	
	virtual ~CContainerConfig();
};
//-----------------------------------------------------------------------------
#endif	//#ifndef CONTAINER_CONFIG_H
