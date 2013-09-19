
#include "stdafx.h"
#include "entity.h"

musicEntity::musicEntity( entity_facade* facade ) : 
entity(facade->getType(), facade->getId())
{
	music_name = facade->getName();
}

void musicEntity::handle_msg( internal_msg in_msg )
{
	switch(in_msg.msg_type)
	{
	case internal_msg_type::req_add_music:
		in_msg.msg_type = internal_msg_type::resp_add_music;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	case internal_msg_type::req_set_camera_goal:
		in_msg.msg_type = internal_msg_type::resp_set_camera_goal;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	default:break;
	}

	if(in_msg.delay_time == -1) return;
	else if(in_msg.delay_time == 0)	notify_script_by_name(in_msg.strings[0]);//strings[0]
	else timer_scheduler_ptr->add_timer(entity_id, in_msg.delay_time, boost::bind(&entity::notify_script_by_name, shared_from_this(), in_msg.strings[0]), task_id);
}