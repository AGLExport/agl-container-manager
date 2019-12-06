#ifndef	CONTAINER_CONFIG_H
#define	CONTAINER_CONFIG_H
//-----------------------------------------------------------------------------
#include "ContainerBase.hpp"
#include "ContainerElement.hpp"

#include <vector>
#include <string>

//-----------------------------------------------------------------------------
class CContainerConfig : public CContainerBase
{
private:
protected:
	std::vector< CContainerElement* > m_ContainerElement;
	
	
public:
	// set method
	
	
	// get method
	bool GetGuestList(std::vector< std::string > &guestlist);
	bool GetElementByName(std::string guestname, CContainerElement **ppelement);
	
	CContainerConfig();
	CContainerConfig(const CContainerConfig&) = delete;
	CContainerConfig &operator = (const CContainerConfig&) = delete;
	
	virtual ~CContainerConfig();
};
//-----------------------------------------------------------------------------
#endif	//#ifndef CONTAINER_CONFIG_H
