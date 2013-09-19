#include "stdafx.h"

#include "entity.h"

device::device( boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> ts_ptr, std::string e_type, std::string d_name, std::string eid ) : 
entity(e_mgr, ts_ptr, e_type, eid)
{
	device_name = d_name;
}

device::device(std::string e_type, std::string d_name, std::string eid) :
entity(e_type, eid)
{
	device_name = d_name;
}

device::~device(){}

device::device(entity_facade* facade) :
entity(facade->getType(), facade->getId())
{
	device_name = facade->getName();
}