#include "stdafx.h"

#include "simulation.h"

void simu_fire::init(boost::shared_ptr<timer_scheduler> t, boost::shared_ptr<entity> ent, int interval, double v_max_cur, double v_min_cur, double v_min_end, double v_max_end, double sp_max, double sp_min, std::string func_type, internal_msg_type::imt imt_simu, int delay_t)
{
	simulator::init(t, ent, interval, imt_simu, delay_t);

	vel_max_end = v_max_end;
	vel_min_end = v_min_end;

	vel_max_cur = vel_max_start = v_max_cur;
	vel_min_cur = vel_min_start= v_min_cur;

	speed_max = sp_max;
	speed_min = sp_min;

	function_type = func_type;
}
void simu_fire::start()
{
	last_time = boost::posix_time::microsec_clock::local_time();
	timer_scheduler_ptr->add_timer(ent_id, time_interval + overhead, boost::bind(&simu_fire::update, shared_from_this()), tid);
}

void simu_fire::func_change_vel()
{
	boost::posix_time::time_duration time_elapse = boost::posix_time::microsec_clock::local_time() - last_time;
	last_time = boost::posix_time::microsec_clock::local_time();

	double ms = time_elapse.total_milliseconds();

	vel_max_cur += speed_max * ms / 1000;
	vel_min_cur += speed_min * ms / 1000;

	if((vel_max_end - vel_max_start) * (vel_max_end - vel_max_cur) <= 0) vel_max_cur = vel_max_end;
	if((vel_min_end - vel_min_start) * (vel_min_end - vel_min_cur) <= 0) vel_min_cur = vel_min_end;
}

bool simu_fire::is_end_change_vel()
{
	return vel_max_cur == vel_max_end && vel_min_cur == vel_min_end;
}

void simu_fire::update()
{
	if(function_type == "change_vel") func_change_vel();

	internal_msg msg = pack_msg();
	entity_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, entity_ptr->ent_mgr, msg.ent_id, msg));

	if(function_type == "change_vel" && !is_end_change_vel()) timer_scheduler_ptr->add_timer(ent_id, time_interval, boost::bind(&simu_fire::update, shared_from_this()), tid);
}

internal_msg simu_fire::pack_msg()
{
	internal_msg msg;

	if(function_type == "change_vel")
	{
		msg.p_start = vel_max_cur;
		msg.p_end = vel_min_cur;
		msg.msg_type = internal_msg_type::req_set_par_vel;
		msg.ent_id = ent_id;

		if(function_type == "change_vel")
		{
			if(!is_end_change_vel())msg.delay_time = -1;
			else msg.delay_time = 0;
		}
	}

	return msg;
}