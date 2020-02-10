#include "ContainerElement.hpp"
#include "ContainerCommands.hpp"
#include <string>
#include <iostream>
#include <fstream>

#include <stdlib.h>

#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>


const char LXC_PATH[] = "/var/lib/lxc/";
//const char LXC_PATH[] = "/cross/container-dev/lxc/";
//-----------------------------------------------------------------------------
int SyncExecCommand(std::string command);
bool GetDevNum(std::string node, dev_t &dev);
bool CreateCANDevName(std::string &host, std::string &guest);
//-----------------------------------------------------------------------------
CContainerElement::CContainerElement() : m_Valid(false), m_autoboot(false)
{
	
}
//-----------------------------------------------------------------------------
CContainerElement::~CContainerElement()
{
	
}
//-----------------------------------------------------------------------------
bool CContainerElement::SetJsonString(std::string &jsonstring)
{
	Json::Reader reader;
	if (reader.parse(jsonstring, this->m_JsonValue) == true);
	{
		if ( this->m_JsonValue.isMember("name") == true )
		{
			if ( this->m_JsonValue["name"].asString().length() > 0)
			{
				this->m_Valid = true;
				this->m_GuestName = this->m_JsonValue["name"].asString();
			}
			
			// any
			if (this->m_JsonValue.isMember("autoboot") == true )
			{
				this->m_autoboot = this->m_JsonValue["autoboot"].asBool();
			}
			else
			{
				this->m_autoboot = false;
			}
		}
	}
	
	return this->m_Valid;
}
//-----------------------------------------------------------------------------
bool CContainerElement::GetName(std::string &name)
{
	if ( m_Valid == false ) return false;
	
	name = this->m_JsonValue["name"].asString();
	
	return true;
}
//-----------------------------------------------------------------------------
bool CContainerElement::GetJsonValue(Json::Value &jsonvalue)
{
	if ( this->m_Valid = true )
	{
		jsonvalue = this->m_JsonValue;
	}
	
	return this->m_Valid;
}
//-----------------------------------------------------------------------------
bool CContainerElement::SetupContainer()
{
	this->GeneratConfig( std::string(LXC_PATH) );
	
	
	return true;
}
//-----------------------------------------------------------------------------
bool CContainerElement::StartContainer()
{
	std::string command;
	
	this->RunPreStartProcess();
	
	command = std::string("lxc-start -n ") + m_GuestName;
	SyncExecCommand(command);
	
	this->RunPostStartProcess();

}
//-----------------------------------------------------------------------------
bool CContainerElement::StopContainer()
{
	std::string command;
	
	this->RunPreStopProcess();
	
	command = std::string("lxc-stop -n ") + m_GuestName;
	SyncExecCommand(command);
	
	this->RunPostStopProcess();
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool CContainerElement::GeneratConfig(std::string basepath)
{
	bool result = false;
	
	std::string outfile;
		
	outfile = basepath + this->m_GuestName;
	
	::mkdir(outfile.c_str(), (S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH));
	
	outfile = outfile + std::string("/config");
	std::ofstream cnfout(outfile, std::ios::trunc);
	
	if (cnfout.is_open())
	{
		this->HeaderOut(cnfout);
		this->BasicConfigOut(cnfout, this->m_JsonValue);
		this->ResourceConfigOut(cnfout, this->m_JsonValue);;
		this->MountConfigOut(cnfout, this->m_JsonValue);
		this->NetworkConfigOut(cnfout, this->m_JsonValue);
		this->ICCConfigOut(cnfout, this->m_JsonValue);
		
		result = true;
	}
	
	return result;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CContainerElement::HeaderOut(std::ofstream &ofs)
{
	ofs << std::string("# LXC config generated by AGL Container Manager") << std::endl;
}
//-----------------------------------------------------------------------------
void CContainerElement::BasicConfigOut(std::ofstream &ofs, Json::Value &json)
{
	std::string containername = json["name"].asString();	//must value
	
	ofs << std::endl;
	ofs << "# basic settings" << std::endl;
	ofs << "lxc.uts.name = \"" << containername  << "\"" << std::endl;
	
	if (json["setting"].isObject() == true )
	{
		//have a child value
		Json::Value setting = json["setting"];
		
		if ( setting.isMember("rootfs") == true )
		{
			Json::Value rootfs = setting["rootfs"];
			if (rootfs["path"].asString().length() > 0)
			{
				ofs << "lxc.rootfs.path = dir:" << rootfs["path"].asString() << std::endl;
			}
		}
		
		if ( setting.isMember("lifecycle") == true)
		{
			Json::Value lifecycle = setting["lifecycle"];
			
			if ( lifecycle.isMember("halt") == true )
			{
				if (lifecycle["halt"].asString().length() > 0)
				{
					ofs << "lxc.signal.halt = " << lifecycle["halt"].asString() << std::endl;
				}
			}
			
			if ( lifecycle.isMember("reboot") == true )
			{
				if (lifecycle["reboot"].asString().length() > 0)
				{
					ofs << "lxc.signal.reboot = " << lifecycle["reboot"].asString() << std::endl;
				}
			}
		}
		
		if ( setting.isMember("cap") == true)
		{
			Json::Value cap = setting["cap"];
			
			if ( cap.isMember("drop") == true )
			{
				if (cap["drop"].asString().length() > 0)
				{
					ofs << "lxc.cap.drop = " << cap["drop"].asString() << std::endl;
				}
			}
			
			if ( cap.isMember("keep") == true )
			{
				if (cap["keep"].asString().length() > 0)
				{
					ofs << "lxc.cap.keep = " << cap["keep"].asString() << std::endl;
				}
			}
		}
		
		if (setting["environment"].isArray() == true )
		{
			Json::Value environments = setting["environment"];
			Json::Value::ArrayIndex count, max;
			max = environments.size();
			
			for(count = 0; count < max; count++)
			{
				std::string environ = environments[count].asString();
				
				if (environ.length() > 0)
				{
					ofs << "lxc.environment = " << environ << std::endl;
				}
				else { }
			}
		}
	}
	
	ofs << "lxc.tty.max = 1" << std::endl;
	ofs << "lxc.pty.max = 1" << std::endl;
}
//-----------------------------------------------------------------------------
void CContainerElement::ResourceConfigOut(std::ofstream &ofs, Json::Value &json)
{
	ofs << std::endl;
	ofs << "# resource settings" << std::endl;
	
	if (json["resource"].isArray() == true )
	{
		//have a child value
		Json::Value resources = json["resource"];
		
		Json::Value::ArrayIndex count, max;
		max = resources.size();
		
		for(count = 0; count < max; count++)
		{
			Json::Value res = resources[count];
			
			// must need parameta
			if ( res.isMember("type") == true )
			{
				std::string restype = res["type"].asString();
			
				if ( restype == std::string("cgroup"))
				{
					// must need parameta
					if ( res.isMember("object") == true && res.isMember("value") == true )
					{
						ofs << "lxc.cgroup." << res["object"].asString() << " = " 
							<< res["value"].asString() << std::endl;
						ofs << std::endl;
					}
					else { }
				}
				else
				{
					//TODO
				}
			}
			else { }
		}
	}
}
//-----------------------------------------------------------------------------
void CContainerElement::MountConfigOut(std::ofstream &ofs, Json::Value &json)
{
	ofs << std::endl;
	ofs << "# mount settings" << std::endl;
	
	if (json["mount"].isArray() == true )
	{
		//have a child value
		Json::Value mounts = json["mount"];
		
		Json::Value::ArrayIndex count, max;
		max = mounts.size();
		
		for(count = 0; count < max; count++)
		{
			Json::Value mnt = mounts[count];
			
			// must need parameta
			if ( mnt.isMember("type") == true )
			{
				std::string mounttype = mnt["type"].asString();
			
				if ( mounttype == std::string("auto"))
				{
					if ( mnt.isMember("param") == true )
					{
						ofs << "# standard kernel file systems" << std::endl;
						ofs << "lxc.mount.auto = " << mnt["param"].asString() << std::endl;
						ofs << std::endl;
					}
					else { }
				}
				else
				{
					if ( mnt.isMember("type") && mnt.isMember("from") && mnt.isMember("to") 
						&& mnt.isMember("fstype") && mnt.isMember("option") )
					{
						ofs << "# " << mnt["type"].asString() << " mount" << std::endl;
						
						ofs << "lxc.mount.entry = " << mnt["from"].asString() << " " << mnt["to"].asString() << " "
							<< mnt["fstype"].asString() << " " << mnt["option"].asString() << std::endl;
						
						if (mnt.isMember("devnode") == true)
						{
							dev_t dev;
							
							if (::GetDevNum(mnt["devnode"].asString(), dev) == true)
							{
								if (major(dev) > 0)
								{
									ofs << "lxc.cgroup.devices.allow = c " << std::to_string(major(dev)) << ":* rwm" << std::endl;
								}
							}
						}
						ofs << std::endl;
					}
				}
			}
			else { }
		}
	}
}
//-----------------------------------------------------------------------------
void CContainerElement::NetworkConfigOut(std::ofstream &ofs, Json::Value &json)
{
	ofs << std::endl;
	ofs << "# network settings" << std::endl;
	
	if (json["network"].isArray() == true )
	{
		//have a child value
		Json::Value network = json["network"];
		
		Json::Value::ArrayIndex count, num, max;
		max = network.size();
		num = 0;
		
		for(count = 0; count < max; count++)
		{
			Json::Value net = network[count];
			
			// must need parameta
			if ( net.isMember("type") == true )
			{
				std::string nettype = net["type"].asString();
			
				if ( nettype == std::string("veth"))
				{
					// must need
					if ( net.isMember("link") == true )
					{
						if (net["link"].asString().length() > 0)
						{
							ofs << "# veth device" << std::endl;
							ofs << "lxc.net." << std::to_string(num) << ".type = veth" << std::endl;
							ofs << "lxc.net." << std::to_string(num) << ".link = " << net["link"].asString() << std::endl;
							
							// any
							if ( net.isMember("flags") == true )
							{
								if (net["flags"].asString().length() > 0)
								{
									ofs << "lxc.net." << std::to_string(num) << ".flags = " << net["flags"].asString() << std::endl;
								}
							}
							
							if ( net.isMember("hwaddr") == true )
							{
								if (net["hwaddr"].asString().length() > 0)
								{
									ofs << "lxc.net." << std::to_string(num) << ".hwaddr = " << net["hwaddr"].asString() << std::endl;
								}
							}
							
							if ( net.isMember("veth") == true )
							{
								Json::Value veth = net["veth"];
								
								if ( veth.isMember("mode") == true )
								{
									if (veth["mode"].asString().length() > 0)
									{
										ofs << "lxc.net." << std::to_string(num) << ".veth.mode = " << veth["mode"].asString() << std::endl;
									}
								}
							}
							
							if ( net.isMember("ipv4") == true )
							{
								Json::Value ipv4 = net["ipv4"];
								
								if ( ipv4.isMember("address") == true )
								{
									if (ipv4["address"].asString().length() > 0)
									{
										ofs << "lxc.net." << std::to_string(num) << ".ipv4.address = " << ipv4["address"].asString() << std::endl;
									}
								}
								if ( ipv4.isMember("gateway") == true )
								{
									if (ipv4["gateway"].asString().length() > 0)
									{
										ofs << "lxc.net." << std::to_string(num) << ".ipv4.gateway = " << ipv4["gateway"].asString() << std::endl;
									}
								}
							}
							
							if ( net.isMember("ipv6") == true )
							{
								Json::Value ipv6 = net["ipv6"];
								
								if ( ipv6.isMember("address") == true )
								{
									if (ipv6["address"].asString().length() > 0)
									{
										ofs << "lxc.net." << std::to_string(num) << ".ipv6.address = " << ipv6["address"].asString() << std::endl;
									}
								}
								if ( ipv6.isMember("gateway") == true )
								{
									if (ipv6["gateway"].asString().length() > 0)
									{
										ofs << "lxc.net." << std::to_string(num) << ".ipv6.gateway = " << ipv6["gateway"].asString() << std::endl;
									}
								}
							}
							ofs << std::endl;
							num = num + 1;
						}
						else { }
					}
					else { }
				}
				else if ( nettype == std::string("can"))
				{
					if ( net.isMember("can") == true )
					{
						std::string canhost;
						Json::Value can = net["can"];
						
						//must
						if ( can.isMember("hostdev") == true && can.isMember("rules") == true )
						{
							canhost = std::string("vxcan-") + can["hostdev"].asString();
							
							ofs << "# can device" << std::endl;
							ofs << "lxc.net." << std::to_string(num) << ".type = phys" << std::endl;
							ofs << "lxc.net." << std::to_string(num) << ".link = vxcan0" << std::endl;
									
							// any
							if ( net.isMember("flags") == true )
							{
								if (net["flags"].asString().length() > 0)
								{
									ofs << "lxc.net." << std::to_string(num) << ".flags = " << net["flags"].asString() << std::endl;
								}
							}
							
							CCANCommand *cc = new (std::nothrow) CCANCommand();
							cc->SetVXCANDeviceNames(canhost, std::string("vxcan0"));
							
							Json::Value canrules = can["rules"];
							Json::Value::ArrayIndex rulenum,rulemax;
							
							if ( canrules.isMember("receive") == true )
							{
								Json::Value recvrules = canrules["receive"];
								rulemax = recvrules.size();
								
								for(rulenum=0; rulenum < rulemax; rulenum++)
								{
									std::string rulestr = recvrules[rulenum].asString();
									cc->SetGatewayRuleReceive(rulestr);
								}
							}
							
							if ( canrules.isMember("send") == true )
							{
								Json::Value sendrules = canrules["send"];
								rulemax = sendrules.size();
								
								for(rulenum=0; rulenum < rulemax; rulenum++)
								{
									std::string rulestr = sendrules[rulenum].asString();
									cc->SetGatewayRuleSend(rulestr);
								}
							}
							
							int vsize = m_Commands.size();
							m_Commands.resize(vsize+1);
							m_Commands[vsize] = dynamic_cast< CContainerCommand* > (cc);
						}
					}
				}
			}
			else { }
		}
	}
}
//-----------------------------------------------------------------------------
void CContainerElement::ICCConfigOut(std::ofstream &ofs, Json::Value &json)
{
	
	
	
	
}
//-----------------------------------------------------------------------------
bool CContainerElement::RunPreStartProcess()
{
	int vsize = m_Commands.size();
	
	for (int i=0; i < vsize; i++)
	{
		CContainerCommand *pcc = NULL;
		
		pcc = m_Commands[i];
		if (pcc != NULL)
		{
			pcc->ExecPreStartCommand();
		}
	}
	
	return true;
}
//-----------------------------------------------------------------------------
bool CContainerElement::RunPostStartProcess()
{
	int vsize = m_Commands.size();
	
	for (int i=0; i < vsize; i++)
	{
		CContainerCommand *pcc = NULL;
		
		pcc = m_Commands[i];
		if (pcc != NULL)
		{
			pcc->ExecPostStartCommand();
		}
	}
	
	return true;
}
//-----------------------------------------------------------------------------
bool CContainerElement::RunPreStopProcess()
{
	int vsize = m_Commands.size();
	
	for (int i=0; i < vsize; i++)
	{
		CContainerCommand *pcc = NULL;
		
		pcc = m_Commands[i];
		if (pcc != NULL)
		{
			pcc->ExecPreStopCommand();
		}
	}
	
	return true;
}
//-----------------------------------------------------------------------------
bool CContainerElement::RunPostStopProcess()
{
	int vsize = m_Commands.size();
	
	for (int i=0; i < vsize; i++)
	{
		CContainerCommand *pcc = NULL;
		
		pcc = m_Commands[i];
		if (pcc != NULL)
		{
			pcc->ExecPostStopCommand();
		}
	}
	
	return true;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
bool GetDevNum(std::string node, dev_t &dev)
{
	struct stat sb = {0};
	
	if (::stat(node.c_str(), &sb) < 0)
	{
		return false;
	}
	
	dev = sb.st_rdev;
	
	return true;
}
//-----------------------------------------------------------------------------
bool CreateCANDevName(std::string &host, std::string &guest)
{
	//とりあえず
	
	host = std::string("vxcan-host0");
	guest = std::string("vxcan-guest0");
	
	return 0;
}

//-----------------------------------------------------------------------------
void CContainerElement::DebugPrintJson()
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

