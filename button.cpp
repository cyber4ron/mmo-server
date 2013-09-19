#include "stdafx.h"

#include "entity.h"

button::button(boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid) : 
device(e_mgr, timer_scheduler_ptr, e_type, d_name, eid)
{

}

button::button(entity_facade* facade) : 
device( facade->getType(), facade->getName(), facade->getId())
{}

void button::handle_msg( internal_msg in_msg)
{
	switch(in_msg.msg_type)
	{
	case internal_msg_type::req_set_button_run:
		in_msg.msg_type = internal_msg_type::resp_set_button_run;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	default:break;
	}

	if(in_msg.delay_time == -1) return;
	else if(in_msg.delay_time == 0)	notify_script();
	else timer_scheduler_ptr->add_timer(entity_id, in_msg.delay_time, boost::bind(&entity::notify_script, shared_from_this()), task_id);
}
