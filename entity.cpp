#include "stdafx.h"

#include "entity.h"

using namespace std;

entity::entity( boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> ts_ptr, std::string ent_type, std::string eid )
{
	ent_mgr = e_mgr;
	entity_id = eid;
	entity_type = ent_type;
	timer_scheduler_ptr = ts_ptr;
}

entity::entity( std::string ent_type, std::string eid )
{
	entity_id = eid;
	entity_type = ent_type;
}

void entity::setEntManager(boost::shared_ptr<entity_manager> ent_mgr)
{
	this->ent_mgr = ent_mgr;
}

void entity::setTimer(boost::shared_ptr<timer_scheduler> t_ptr)
{
	this->timer_scheduler_ptr = t_ptr;
}

void entity::setElemManager(boost::shared_ptr<eval_manager> ele_mgr)
{
	this->ele_mgr = ele_mgr;
}

std::string entity::get_entity_type()
{
	return entity_type;
}
std::string entity::get_entity_id()
{
	return entity_id;
}

void entity::register_entity( std::string script )
{
	boost::mutex::scoped_lock lock(scripts_to_notify_mutex);
	scripts_to_notify.insert(script);
}

void entity::unregister_entity(std::string script)
{
	boost::mutex::scoped_lock lock(scripts_to_notify_mutex);
	scripts_to_notify.erase(script);
}

void entity::notify_script()
{
	boost::mutex::scoped_lock lock(scripts_to_notify_mutex);

	for(set<string>::iterator it=scripts_to_notify.begin();it!=scripts_to_notify.end();it++)
	{
		ent_mgr->svr_ptr->scp_mgr->resume_script_by_name_async(*it);
	}
}

entity::~entity()
{

}

void entity::notify_script_by_name( std::string script_name )
{
	ent_mgr->svr_ptr->scp_mgr->resume_script_by_name_async(script_name);
}