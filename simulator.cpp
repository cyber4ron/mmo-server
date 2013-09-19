#include "stdafx.h"

#include "simulation.h"

using namespace std;


void simulator::init( boost::shared_ptr<timer_scheduler> t, boost::shared_ptr<entity> ent, int interval, internal_msg_type::imt imt_simu, int delay_t )
{
	timer_scheduler_ptr = t;
	entity_ptr = ent;
	ent_id = entity_ptr->entity_id;
	imt_simulating = imt_simu;
	time_interval = interval;
	delay_time = delay_t;
	overhead = 50;
}