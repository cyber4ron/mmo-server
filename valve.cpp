#include "stdafx.h"

#include "entity.h"


valve::valve( boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid) : 
device(e_mgr, timer_scheduler_ptr, e_type, d_name, eid)
{
	state = false;
}

valve::valve(entity_facade* facade) : 
device(facade->getType(), facade->getName(), facade->getId())
{
	unit_angle = atoi(facade->getUnitAngle().c_str());
	state = atoi(facade->getValveState().c_str());
}


void valve::handle_msg( internal_msg in_msg )
{
	switch(in_msg.msg_type)
	{
	case internal_msg_type::req_set_valve_run://client operation

		//check if operation valid
		if(state >= 1 && in_msg.p_int == -1 || state <= 0 && in_msg.p_int == 1) {in_msg.delay_time = -1; break;}
		else state -= in_msg.p_dbl * in_msg.p_int / unit_angle;//in_msg.p_int = -1代表打开

		in_msg.delay_time = 0;//notify subscribing scripts

		//broadcast operation
		in_msg.msg_type = internal_msg_type::resp_set_valve_run;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));

		//simulate the operation, or async simulate, 没有连续的改变valve状态
		boost::this_thread::sleep(boost::posix_time::millisec(in_msg.p_dbl / in_msg.p_dbls[0] * 1000));
		break;

	default:break;
	}

	if(in_msg.delay_time == -1) return;
	else if(in_msg.delay_time == 0)	notify_script();
	else timer_scheduler_ptr->add_timer(entity_id, in_msg.delay_time, boost::bind(&entity::notify_script, shared_from_this()), task_id);
}


///knob


knob::knob( entity_facade* facade ): 
device(facade->getType(), facade->getName(), facade->getId())
{

}


void knob::handle_msg( internal_msg in_msg )
{
	switch(in_msg.msg_type)
	{
	case internal_msg_type::req_set_knob_turn:
		in_msg.msg_type = internal_msg_type::resp_set_knob_turn;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
	default:break;
	}

	if(in_msg.delay_time == -1) return;
	else if(in_msg.delay_time == 0)	notify_script();
	else timer_scheduler_ptr->add_timer(entity_id, in_msg.delay_time, boost::bind(&entity::notify_script, shared_from_this()), task_id);
}
