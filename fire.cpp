#include "stdafx.h"

#include "entity.h"

dist_fire::dist_fire( boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid ):
disaster(e_mgr, timer_scheduler_ptr, e_type, d_name, eid)
{
	v_max = 500;
	v_min = 100;
	width = height = 250;
	particle_quota  = 50;
	live_time = 1;
	ss_fire_ptr = boost::make_shared<simu_fire>();
}

dist_fire::dist_fire( entity_facade* facade ) : disaster(facade)
{

}

void dist_fire::handle_msg( internal_msg in_msg )
{
	switch(in_msg.msg_type)
	{
	case internal_msg_type::req_add_particle:
		in_msg.msg_type = internal_msg_type::resp_add_particle;
		diaster_name = in_msg.strings[0];
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	case internal_msg_type::req_set_par_run:
		in_msg.msg_type = internal_msg_type::resp_set_par_run;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	case internal_msg_type::req_change_par_vel:
		ss_fire_ptr->init(timer_scheduler_ptr, shared_from_this(), 200, v_max, v_min, in_msg.p_start, in_msg.p_end, in_msg.p_dbls[0], in_msg.p_dbls[1], "change_vel", internal_msg_type::req_set_par_vel, in_msg.delay_time);
		ss_fire_ptr->start();
		break;
	case internal_msg_type::req_set_par_vel:
		v_max = in_msg.p_start;
		v_min = in_msg.p_end;
		in_msg.msg_type = internal_msg_type::resp_set_par_vel;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
	case internal_msg_type::req_set_par:
		in_msg.msg_type = internal_msg_type::resp_set_par;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	case internal_msg_type::req_set_par_dir:
		in_msg.msg_type = internal_msg_type::resp_set_par_dir;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	default:break;
	}

	if(in_msg.delay_time == -1) return;
	else if(in_msg.delay_time == 0)	notify_script();
	else timer_scheduler_ptr->add_timer(entity_id, in_msg.delay_time, boost::bind(&entity::notify_script, shared_from_this()), task_id);
}


/////water


water::water( entity_facade* facade ):disaster(facade)
{

}


void water::handle_msg( internal_msg in_msg )
{
	switch(in_msg.msg_type)
	{
	case internal_msg_type::req_set_water_face:
		in_msg.msg_type = internal_msg_type::resp_set_water_face;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	default:break;
	}

	if(in_msg.delay_time == -1) return;
	else if(in_msg.delay_time == 0)	notify_script();
	else timer_scheduler_ptr->add_timer(entity_id, in_msg.delay_time, boost::bind(&entity::notify_script, shared_from_this()), task_id);
}