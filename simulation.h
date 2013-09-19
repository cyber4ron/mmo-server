#pragma once

#include "timer.h"
#include "timer_scheduler.h"
#include "geom.h"
#include "internal_message.h"
#include "entity.h"
#include "entity_manager.h"

class timer_scheduler;
class entity;

class simulator
{
public:
	std::string ent_id;

	boost::shared_ptr<timer_scheduler> timer_scheduler_ptr;
	boost::shared_ptr<entity> entity_ptr;

	internal_msg_type::imt imt_simulating;
	int delay_time;

	std::string tid;
	int time_interval;//millisec
	boost::posix_time::ptime last_time;//time_elapse
	int overhead;//network delay, etc., millisec

	void init(boost::shared_ptr<timer_scheduler> t, boost::shared_ptr<entity> ent, int interval, internal_msg_type::imt imt_simu, int delay_t);

	virtual void start() = 0;

	virtual void update() = 0;

	virtual internal_msg pack_msg() = 0;
};

class simu_move : public simulator,  public boost::enable_shared_from_this<simu_move>
{
public:
	std::string role_id;

	vector3 vec_start, vec_end, vec_cur;
	double speed;

	void init(boost::shared_ptr<timer_scheduler> t, boost::shared_ptr<entity> ent, vector3 v_st, vector3 v_end, double sp, int interval, internal_msg_type::imt imt_simu, int delay_t);

	void start();

	void run_function();

	void update();

	internal_msg pack_msg();

	bool equal_dbl(double a, double b);

	bool is_end();

	void cancel();
};

class simu_fire : public simulator, public boost::enable_shared_from_this<simu_fire>
{
public:
	double vel_max_cur, vel_min_cur, vel_max_end, vel_min_end, vel_max_start, vel_min_start;
	double speed_max, speed_min;
	std::string function_type;

	void init(boost::shared_ptr<timer_scheduler> t, boost::shared_ptr<entity> ent, int interval, double v_max_cur, double v_min_cur, double v_min_end, double v_max_end, double sp_max, double sp_min, std::string func_type, internal_msg_type::imt imt_simu, int delay_t);

	void start();

	void func_change_vel();

	bool is_end_change_vel();

	void update();

	internal_msg pack_msg();
};