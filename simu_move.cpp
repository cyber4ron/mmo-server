#include "stdafx.h"

#include "simulation.h"

using namespace std;

void simu_move::init(boost::shared_ptr<timer_scheduler> t, boost::shared_ptr<entity> ent, vector3 v_st, vector3 v_end, double sp, int interval, internal_msg_type::imt imt_simu, int delay_t)
{
	simulator::init(t, ent, interval, imt_simu, delay_t);
	vec_cur = v_st;
	vec_end = v_end;
	speed = sp;
}

void simu_move::start()
{
	last_time = boost::posix_time::microsec_clock::local_time();
	if(!is_end()) timer_scheduler_ptr->add_timer(ent_id, time_interval + overhead, boost::bind(&simu_move::update, shared_from_this()), tid);
	else
	{
		internal_msg msg = pack_msg();
		entity_ptr->ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg_check, entity_ptr->ent_mgr, msg.ent_id, msg));
	}
}

void simu_move::run_function()
{
	boost::posix_time::time_duration time_elapse = boost::posix_time::microsec_clock::local_time() - last_time;
	last_time = boost::posix_time::microsec_clock::local_time();

	double ms = time_elapse.total_milliseconds();

	vector3 vec_dir = vec_end - vec_cur;
	vec_dir = vec_dir.normalise();
	vector3 runSpeed = vec_dir * speed * (ms / 1000.0);

	vector3 vec_left = vec_end - vec_cur - runSpeed;
	vec_left = vec_left * vec_dir;
	if (vec_left.x <=0 && vec_left.y <=0 &&vec_left.z <=0)
	{
		vec_cur = vec_end;
	}
	else
	{
		vec_cur = vec_cur + runSpeed;
	}
}

void simu_move::update()
{
	run_function();

	internal_msg msg = pack_msg();
	entity_ptr->ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg_check, entity_ptr->ent_mgr, msg.ent_id, msg));

	if(!is_end()) timer_scheduler_ptr->add_timer(role_id, time_interval, boost::bind(&simu_move::update, shared_from_this()), tid);
}

internal_msg simu_move::pack_msg()
{
	internal_msg in_msg;
	in_msg.msg_type = imt_simulating;
	in_msg.ent_id = entity_ptr->entity_id;
	in_msg.vecs.push_back(vec_cur);
	in_msg.vecs.push_back(*(new vector3(0, 0, 0)));
	in_msg.vecs.push_back(*(new vector3(0.3, 0.3, 0.3)));

	if(!is_end()) in_msg.delay_time = -1;
	else in_msg.delay_time = 0;

	in_msg.broadcast_option = -1;//dont braodcast

	return in_msg;
}

bool simu_move::is_end()
{
	return vec_cur == vec_end;
}

void simu_move::cancel()
{

}