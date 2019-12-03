#include "ConfigElement.hpp"
#include <string>
#include <iostream>
#include <fstream>

#include <stdlib.h>

//-----------------------------------------------------------------------------
CConfigElement::CConfigElement() : m_Valid(false)
{
	
}
//-----------------------------------------------------------------------------
CConfigElement::~CConfigElement()
{
	
}
//-----------------------------------------------------------------------------
bool CConfigElement::SetJsonString(std::string &jsonstring)
{
	Json::Reader reader;
	if (reader.parse(jsonstring, this->m_JsonValue) == true);
	{
		this->m_Valid = true;
	}
	
	return this->m_Valid;
}
//-----------------------------------------------------------------------------
bool CConfigElement::GetName(std::string &name)
{
	if (m_Valid == false) return false;
	
	name = this->m_JsonValue["name"].asString();
	
	return true;
}
//-----------------------------------------------------------------------------
bool CConfigElement::GetJsonValue(Json::Value &jsonvalue)
{
	if (this->m_Valid = true)
	{
		jsonvalue = this->m_JsonValue;
	}
	
	return this->m_Valid;
}
//-----------------------------------------------------------------------------
void CConfigElement::DebugPrintJson()
{
	if (this->m_Valid == true)
	{
		Json::FastWriter writer;
		const std::string json_file = writer.write(this->m_JsonValue);
		
		std::cout << json_file << std::endl;
	}
	else
	{
		std::cout << std::string("invalid") << std::endl;
	}
}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------

