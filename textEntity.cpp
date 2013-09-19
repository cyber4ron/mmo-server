#include "stdafx.h"

#include "entity.h"

textEntity::textEntity( boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid ):
entity(e_mgr, timer_scheduler_ptr, e_type, eid)
{
	text_name = d_name;
}

textEntity::textEntity(entity_facade* facade):
entity(facade->getType(), facade->getId())
{
	text_name = facade->getName();
}

void textEntity::handle_msg( internal_msg in_msg )
{
	switch(in_msg.msg_type)
	{
	case internal_msg_type::req_add_textblock:
		in_msg.msg_type = internal_msg_type::resp_add_textblock;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	case internal_msg_type::req_showtext:
		in_msg.msg_type = internal_msg_type::resp_showtext;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	case internal_msg_type::req_changetext:
		in_msg.msg_type = internal_msg_type::resp_changetext;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	case  internal_msg_type::req_deletetext:
		in_msg.msg_type = internal_msg_type::resp_deletetext;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	default:break;
	}

	if(in_msg.delay_time == -1) return;
	else if(in_msg.delay_time == 0)	notify_script();
	else timer_scheduler_ptr->add_timer(entity_id, in_msg.delay_time, boost::bind(&entity::notify_script, shared_from_this()), task_id);
}