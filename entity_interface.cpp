#include "stdafx.h"

#include "entity_interface.h"

using namespace std;

entity_interface::entity_interface( boost::shared_ptr<entity_manager> em_ptr )
{
	ent_mgr_ptr = em_ptr;
}

void entity_interface::register_entity( std::string entity_id, std::string script_name )
{
	ent_mgr_ptr->register_entity(entity_id, script_name);
}

void entity_interface::unregister_entity( std::string entity_id, std::string script_name )
{
	ent_mgr_ptr->unregister_entity(entity_id, script_name);
}

void entity_interface::set_actor_move(std::string id, vector3 v_end, double speed)
{
	assert(id != "");
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_role_move;
	in_msg.ent_id = id;
	in_msg.delay_time = -1;

	in_msg.vecs.push_back(v_end);
	in_msg.p_dbl = speed;

	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

vector3 entity_interface::get_actor_position(std::string id)
{
	return ent_mgr_ptr->get_role_position(id);
}

void entity_interface::add_actor( std::string id, vector3 position, vector3 rotation, vector3 scale, std::string action )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_state;
	in_msg.ent_id = id;

	in_msg.vecs.push_back(position);
	in_msg.vecs.push_back(rotation);
	in_msg.vecs.push_back(scale);

	in_msg.strings.push_back(action);

	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::complete_by_npcs()
{
	ent_mgr_ptr->complete_by_npcs();

	boost::this_thread::sleep(boost::posix_time::millisec(2000));
}

void entity_interface::set_actor_action( std::string id, std::string act,double vel, int delay)
{
	assert(id != "");
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_actor_action;
	in_msg.ent_id = id;
	in_msg.strings.push_back(act);
	in_msg.p_dbl = vel;
	in_msg.delay_time = delay;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::set_button_run( std::string idx1,std::string idx2,double dis,int dir )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_button_run;
	std::string id = idx1 + ":" + idx2;
	in_msg.ent_id = id;

	in_msg.p_dbl = dis;
	in_msg.p_int = dir;

	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));

}

void entity_interface::set_valve_run( std::string idx1,std::string idx2,double angle,double speed,int dir )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_valve_run;
	std::string id = idx1 + ":" + idx2;
	in_msg.ent_id = id;

	in_msg.p_dbl = angle;
	in_msg.p_dbls.push_back(speed);
	in_msg.p_int = dir;

	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

double entity_interface::get_valve_state( std::string id )
{
	return ent_mgr_ptr->get_valve_state(id);
}

void entity_interface::attach_sub_entity( std::string id,std::string bone,std::string meshname, int ms )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_attach_sub_ent;
	in_msg.ent_id = id;

	in_msg.strings.push_back(bone);
	in_msg.strings.push_back(meshname);

	in_msg.delay_time = 0;

	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));

}

void entity_interface::detach_sub_entity( std::string id,std::string bone,std::string meshname )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_detach_sub_ent;
	in_msg.ent_id = id;

	in_msg.strings.push_back(bone);
	in_msg.strings.push_back(meshname);
	in_msg.delay_time = 0;


	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));

}

void entity_interface::set_common_entity_run( std::string idx1,std::string idx2,vector3 goal,int ifTran,double speed )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_entity_move;
	string id = idx1 + ":" + idx2;
	in_msg.ent_id = id;
	in_msg.vecs.push_back(goal);
	in_msg.p_int = ifTran;
	in_msg.p_dbl = speed;
	in_msg.delay_time = -1;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::add_particle( std::string idx1,std::string idx2,std::string pname,vector3 pos )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_add_particle;
	string id = idx1 + ":" + idx2;
	in_msg.ent_id = id;
	in_msg.strings.push_back(pname);
	in_msg.vecs.push_back(pos);
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::set_particle_running( std::string idx1,std::string idx2,double v1,double v2 )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_par_run;
	string id = idx1 + ":" + idx2;
	in_msg.ent_id = id;
	in_msg.p_start = v1;
	in_msg.p_end = v2;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));

}

void entity_interface::set_particle_velocity( std::string idx1,std::string idx2,double v1,double v2,double speed1,double speed2 )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_par_vel;
	string id = idx1 + ":" + idx2;
	in_msg.ent_id = id;
	in_msg.p_start = v1;
	in_msg.p_end = v2;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::set_particle_dir( std::string idx1, std::string idx2, vector3 vec )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_par_dir;
	string id = idx1 + ":" + idx2;
	in_msg.ent_id = id;
	in_msg.vecs.push_back(vec);
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::add_textblock_onscreen( std::string id, std::string name, int x, int y, int size, vector3 color, double a )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_add_textblock;
	in_msg.ent_id = id;
	in_msg.strings.push_back(name);
	in_msg.p_start = x;
	in_msg.p_end = y;
	in_msg.p_int = size;
	in_msg.vecs.push_back(color);
	in_msg.p_dbl = a;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::show_text_onscreen( std::string textname, std::string role_id, std::string text,double posx,double posy,int fsize,vector3 color, int delay )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_showtext;
	in_msg.ent_id = textname;
	in_msg.strings.push_back(text);
	in_msg.p_start = posx;
	in_msg.p_end = posy;
	in_msg.vecs.push_back(color);
	in_msg.p_int = fsize;
	in_msg.delay_time = delay;
	in_msg.strings.push_back(role_id);
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::change_text_onscreen( std::string textname,std::string text, int delay )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_changetext;
	in_msg.ent_id = textname;
	in_msg.strings.push_back(text);
	in_msg.delay_time = delay;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::delete_text_onscreen( std::string textname, int delay )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_deletetext;
	in_msg.ent_id = textname;
	in_msg.delay_time = delay;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::set_common_entity_pos( std::string idx1,std::string idx2,vector3 goal )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_entity_position;
	std::string id = idx1 + ":" + idx2;
	in_msg.ent_id = id;
	in_msg.vecs.push_back(goal);
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}
           
//question, instruction
void entity_interface::push_questions( std::string role_id, std::string info, std::string qids, std::string first_id )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_push_questions;
	in_msg.ent_id = role_id;
	in_msg.strings.push_back(info);
	in_msg.strings.push_back(qids);
	in_msg.strings.push_back(first_id);
	in_msg.delay_time = -1;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::set_particle_run( std::string idx1,std::string idx2,bool flag )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_par;
	std::string id = idx1+":"+idx2;
	in_msg.ent_id = id;
	if(flag==false)
		in_msg.p_int = 0;
	else in_msg.p_int = 1;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::set_entity_position( std::string idx1,std::string idx2,vector3 vec ,int delayTime)
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_static_entity_move;
	std::string id = idx1+":"+idx2;
	in_msg.ent_id = id;
	in_msg.delay_time = delayTime;
	in_msg.vecs.push_back(vec);
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));

}

void entity_interface::set_water_face_move( std::string idx1,std::string idx2,vector3 goal,double speed,int ifTransient ,int delayTime)
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_water_face;
	std::string id = idx1+":"+idx2;
	in_msg.ent_id = id;
	in_msg.vecs.push_back(goal);
	in_msg.p_dbl = speed;
	in_msg.p_int = ifTransient;
	in_msg.delay_time = delayTime;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::set_knob_turn( std::string idx1,std::string idx2,double angle ,int delayTime)
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_knob_turn;
	std::string id = idx1+":"+idx2;
	in_msg.ent_id = id;
	in_msg.p_dbl = angle;
	in_msg.delay_time = delayTime;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::start_alarm_flicker( std::string idx1,std::string idx2, int frameCount ,int delayTime)
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_start_alarm_flicker;
	std::string id = idx1+":"+idx2;
	in_msg.ent_id = id;
	in_msg.p_int = frameCount;
	in_msg.delay_time = delayTime;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::end_alarm_flicker( std::string idx1,std::string idx2 ,int delayTime)
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_end_alarm_flicker;
	std::string id = idx1+":"+idx2;
	in_msg.ent_id = id;
	in_msg.delay_time = delayTime;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

void entity_interface::add_one_music( std::string idx,std::string MusicPath,double Volume,int IfLoop, std::string script_to_notify,std::string clientid, int delayTime)
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_add_music;
	std::string id = idx;
	in_msg.ent_id = id;
	in_msg.strings.push_back(script_to_notify);
	in_msg.strings.push_back(MusicPath);
	in_msg.strings.push_back(clientid);
	in_msg.p_dbl = Volume;
	in_msg.p_int = IfLoop;
	in_msg.delay_time = delayTime;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));
}

bool entity_interface::check_actor_equipment( std::string id, std::string equipment_name )
{
	return ent_mgr_ptr->check_actor_equipment(id, equipment_name);
}

void entity_interface::move_camera_to_goal( std::string id, vector3 vec,double speed,int ifTransient )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::req_set_camera_goal;
	in_msg.ent_id = id;
	in_msg.vecs.push_back(vec);
	in_msg.p_dbl = speed;
	in_msg.p_int = ifTransient;
	ent_mgr_ptr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, ent_mgr_ptr, in_msg.ent_id, in_msg));

}

