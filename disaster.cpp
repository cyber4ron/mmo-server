#include "stdafx.h"

#include "entity.h"

disaster::disaster( boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid ):
entity(e_mgr, timer_scheduler_ptr, e_type, eid)
{
	diaster_name = d_name;
}

disaster::~disaster(){}

disaster::disaster(entity_facade* facade):
entity(facade->getType(), facade->getId())
{
	diaster_name = facade->getName();
}