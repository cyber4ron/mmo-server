#include "stdafx.h"

#include "entity.h"

using namespace std;

commonEntity::commonEntity( boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid ):
device(e_mgr, timer_scheduler_ptr, e_type, d_name, eid)
{
	s_move_ptr = boost::make_shared<simu_move>();
}

commonEntity::commonEntity(entity_facade* facade):
device(facade) 
{
	s_move_ptr = boost::make_shared<simu_move>();
	position = facade->getPosition();
}

void commonEntity::handle_msg( internal_msg in_msg )
{
	switch(in_msg.msg_type)
	{
	case internal_msg_type::req_set_entity_move:
		in_msg.msg_type = internal_msg_type::resp_set_entity_move;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		s_move_ptr->init(timer_scheduler_ptr, shared_from_this(), position, in_msg.vecs[0], in_msg.p_dbl, 200, internal_msg_type::req_set_entity_position, in_msg.delay_time);
		s_move_ptr->start();
		break;
	case internal_msg_type::req_set_entity_position:
		position = in_msg.vecs[0];
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	case  internal_msg_type::req_set_static_entity_move:
		in_msg.msg_type = internal_msg_type::resp_set_static_entity_move;
		position = in_msg.vecs[0];
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	case internal_msg_type::req_start_alarm_flicker:
		in_msg.msg_type = internal_msg_type::resp_start_alarm_flicker;
		//state start
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	case internal_msg_type::req_end_alarm_flicker:
		in_msg.msg_type = internal_msg_type::resp_end_alarm_flicker;
		//state end
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	default:break;
	}

	if(in_msg.delay_time == -1) return;
	else if(in_msg.delay_time == 0)	notify_script();
	else timer_scheduler_ptr->add_timer(entity_id, in_msg.delay_time, boost::bind(&entity::notify_script, shared_from_this()), task_id);
}
