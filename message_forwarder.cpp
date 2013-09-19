#include "stdafx.h"

#include "message_forwarder.h"

using namespace std;

message_forwarder::message_forwarder( boost::shared_ptr<server> svr_ptr ) : server_ptr(svr_ptr), io_s_ptr(server_ptr->io_service_ptr)
{

}

void message_forwarder::forward_message(message msg)
{
	//switch(), construct internal message, push into ent_mgr queue
	internal_msg int_msg;

	switch(msg.type)
	{
	case msg_type::echo_client:
		server_ptr->io_service_ptr->post(boost::bind(&server::unicast, server_ptr, msg.endpoint, msg));
		break;

	case msg_type::upload_role:
		int_msg.ent_id = msg.id;
		int_msg.endpoint = msg.endpoint;
		server_ptr->handle_upload_role(msg.endpoint, int_msg);
		break;

	case msg_type::rep_login:
		//auth
		break;

	case msg_type::req_update_roles:
		int_msg.endpoint = msg.endpoint;
		int_msg.msg_type = internal_msg_type::req_update_roles;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::get_all_taken_roles, server_ptr->ent_mgr, msg.endpoint));
		break;

	case msg_type::req_roles:
		int_msg.endpoint = msg.endpoint;
		int_msg.msg_type = internal_msg_type::req_roles_available;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::get_available_roles, server_ptr->ent_mgr, msg));
		break;

	case msg_type::req_select_role:
		int_msg.endpoint = msg.endpoint;
		int_msg.msg_type = internal_msg_type::req_client_select_role;
		int_msg.ent_id = msg.id;
		server_ptr->handle_select_role(int_msg.endpoint, int_msg);
		break;

	case msg_type::req_cancel_role:
		break;
		
	case msg_type::req_role_position:
		int_msg.endpoint = msg.endpoint;
		int_msg.msg_type = internal_msg_type::req_role_position;
		int_msg.ent_id = msg.id;
		int_msg.vecs.push_back(msg.vecs[0]);
		int_msg.vecs.push_back(msg.vecs[1]);
		int_msg.vecs.push_back(msg.vecs[2]);
		int_msg.broadcast_option = 0;//broadcast
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;

	case msg_type::req_positioning:
		int_msg.msg_type = internal_msg_type::req_positioning;
		int_msg.p_vec_vec3s = msg.p_vec_vec3s;
		int_msg.ent_id = msg.id;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;

	case msg_type::req_training_start:
		server_ptr->pcs_ctl->start();
		break;

	case msg_type::intl_client_disconnect:
		//entity, script etc. reset;
		break;

	case msg_type::req_user_move:
		int_msg.msg_type = internal_msg_type::req_user_move;
		int_msg.ent_id = msg.id;
		int_msg.vecs = msg.vecs;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;

	case msg_type::req_user_rotate:
		int_msg.msg_type = internal_msg_type::req_user_rotate;
		int_msg.ent_id = msg.id;
		int_msg.vecs = msg.vecs;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;

	case msg_type::req_user_act:
		int_msg.msg_type = internal_msg_type::req_user_act;
		int_msg.ent_id = msg.id;
		int_msg.p_int = msg.p_int;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;

	case msg_type::req_bear:

		//!!!!!!!!!!!!!!
		//int_msg = msg;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.strings[0], int_msg));
		break;

	case msg_type::req_drop:

		//!!!!!!!!!!!!!!
		//int_msg = msg;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.strings[0], int_msg));
		break;

	case msg_type::req_set_attach_sub_ent:
		int_msg.msg_type = internal_msg_type::req_set_attach_sub_ent;
		int_msg.ent_id = msg.id;
		int_msg.strings = msg.strings;
		int_msg.delay_time = -1;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;

	case msg_type::req_set_detach_sub_ent:
		int_msg.msg_type = internal_msg_type::req_set_attach_sub_ent;
		int_msg.ent_id = msg.id;
		int_msg.strings = msg.strings;
		int_msg.delay_time = -1;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;
	
	case msg_type::req_set_valve_run:
		int_msg.msg_type = internal_msg_type::req_set_valve_run;
		int_msg.ent_id = msg.id;//valve id
		int_msg.p_dbl = msg.p_dbl;//angle
		int_msg.p_dbls = msg.p_dbls;//angle
		int_msg.p_ints = msg.p_ints;//user_id, for evaluation, etc.
		int_msg.p_int = msg.p_int;//direction
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;

	case msg_type::submit_operation:
		int_msg.msg_type = internal_msg_type::submit_operation;
		int_msg.ent_id = msg.id;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;

	case msg_type::submit_positioning:
		int_msg.msg_type = internal_msg_type::submit_positioning;
		int_msg.ent_id = msg.id;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;

	case msg_type::submit_action:

		break;

	case msg_type::submit_question_answer:
		int_msg.msg_type = internal_msg_type::submit_question_answer;
		int_msg.ent_id = msg.id;
		server_ptr->ent_mgr->async_call(boost::bind(&entity_manager::handle_internal_msg_check, server_ptr->ent_mgr, int_msg.ent_id, int_msg));
		break;

  case msg_type::req_operation_submission:
    server_ptr->eval_mgr->push_user_operaiton(msg.ops);
    break;

	default:
		break;
	}
}

void message_forwarder::forward_internal_message(internal_msg int_msg)
{
	//switch(), construct internal message, push into ent_mgr queue
	message msg, msg2, msg3;

	//!!!!!!!!!!!!!!
	//msg = int_msg;
	vector<message> msgs;

	if(int_msg.msg_type != internal_msg_type::resp_complete_roles)
		assert(int_msg.ent_id != "");

	switch(int_msg.msg_type)
	{
	case internal_msg_type::resp_update_roles:
		msg.type = msg_type::resp_update_roles;
		msg.endpoint = int_msg.endpoint;
		msg.strings = int_msg.strings;
		msg.p_vec_vec3s = int_msg.p_vec_vec3s;
		server_ptr->unicast(msg.endpoint, msg);
		break;

	case internal_msg_type::resp_client_cancel_role:
		msg.type = msg_type::resp_cancel_role;
		msg.p_int = int_msg.p_int;//success or fail
		msg.id = int_msg.ent_id;
		msg.endpoint = int_msg.endpoint;
		server_ptr->unicast(msg.endpoint, msg);
		break;

	case internal_msg_type::resp_client_delete_role:
		msg.type = msg_type::resp_delete_role;
		msg.id = int_msg.ent_id;
		msg.endpoint = int_msg.endpoint;
		server_ptr->broadcast_except(msg, msg.endpoint);
		break;

	case internal_msg_type::resp_role_move:
		assert(int_msg.ent_id != "");
		msg.type = msg_type::resp_role_move;
		msg.id = int_msg.ent_id;
		msg.p_dbl = int_msg.p_dbl;
		msg.vecs = int_msg.vecs;
		assert(msg.vecs.size() != 0);
		server_ptr->broadcast(msg);
		break;

	case internal_msg_type::resp_user_move:
		msg.type = msg_type::resp_user_move;
		msg.id = int_msg.ent_id;
		msg.vecs = int_msg.vecs;
		server_ptr->broadcast(msg);
		break;

	case internal_msg_type::resp_user_rotate:
		msg.type = msg_type::resp_user_rotate;
		msg.id = int_msg.ent_id;
		msg.vecs = int_msg.vecs;
		server_ptr->broadcast(msg);
		break;

	case internal_msg_type::resp_user_act:
		msg.type = msg_type::resp_user_act;
		msg.id = int_msg.ent_id;
		msg.p_int = int_msg.p_int;
		server_ptr->broadcast(msg);
		break;

	case internal_msg_type::resp_role_position:
		msg.endpoint = int_msg.endpoint;
		msg.type = msg_type::resp_role_position;
		msg.id = int_msg.ent_id;
		msg.vecs.push_back(int_msg.vecs[0]);
		msg.vecs.push_back(int_msg.vecs[1]);
		msg.vecs.push_back(int_msg.vecs[2]);                                                                                              
		server_ptr->broadcast(msg);
		break;

	case internal_msg_type::resp_roles_available:
		msg.endpoint = int_msg.endpoint;
		msg.type = msg_type::resp_roles;
		msg.strings = int_msg.strings;
		server_ptr->unicast(msg.endpoint, msg);
		break;

	case internal_msg_type::resp_client_select_role:
		//confirm
		msg.type = msg_type::resp_select_role;
		msg.endpoint = int_msg.endpoint;
		msg.id = int_msg.ent_id;
		msg.p_int = int_msg.p_int;//confirm or reject
		msg.vecs = int_msg.vecs;

		//set camera
		msg3.type = msg_type::resp_set_camera;
		msg3.endpoint = int_msg.endpoint;
		msg3.id = int_msg.ent_id;

		msgs.clear();
		msgs.push_back(msg);
		msgs.push_back(msg3);

		server_ptr->unicast_msgs(msg.endpoint, msgs);
		break;

	case internal_msg_type::resp_add_role:

		//!!!!!!!!!!!!!!
		//msg = int_msg;
		msg.type = msg_type::resp_add_role;
		msg.id = int_msg.ent_id;
		msg.vecs = int_msg.vecs;
		msg.endpoint = int_msg.endpoint;
		server_ptr->broadcast_except(msg, msg.endpoint);
		break;

	case internal_msg_type::resp_complete_roles:
		msg.type = msg_type::resp_complete_roles;
		msg.p_vec_strings = int_msg.p_vec_strings;
		msg.p_vec_vec3s = int_msg.p_vec_vec3s;
		server_ptr->broadcast(msg);
		break;

	case  internal_msg_type::resp_set_state:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_state;
		msg.vecs.push_back(int_msg.vecs[0]);
		msg.vecs.push_back(int_msg.vecs[1]);
		msg.vecs.push_back(int_msg.vecs[2]);
		msg.strings.push_back(int_msg.strings[0]);
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_set_button_run:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_button_run;
		msg.p_dbl = int_msg.p_dbl;
		msg.p_int = int_msg.p_int;
		server_ptr->broadcast(msg);
		break;
	case internal_msg_type::resp_set_valve_run:
		msg.id = int_msg.ent_id;//valve id
		msg.type = msg_type::resp_set_valve_run;
		msg.p_dbl = int_msg.p_dbl;//angle
		msg.p_dbls = int_msg.p_dbls;//angle
		msg.p_int = int_msg.p_int;//direction
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_set_attach_sub_ent:
		assert(int_msg.ent_id != "");
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_attach_sub_ent;
		msg.strings.push_back(int_msg.strings[0]);
		msg.strings.push_back(int_msg.strings[1]);
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_set_detach_sub_ent:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_detach_sub_ent;
		msg.strings.push_back(int_msg.strings[0]);
		msg.strings.push_back(int_msg.strings[1]);
		server_ptr->broadcast(msg);
		break;
	case internal_msg_type::resp_set_entity_move:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_entity_move;
		msg.vecs.push_back(int_msg.vecs[0]);
		msg.p_dbl = int_msg.p_dbl;
		msg.p_int = int_msg.p_int;
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_add_particle:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_add_particle;
		msg.vecs.push_back(int_msg.vecs[0]);
		msg.strings.push_back(int_msg.strings[0]);
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_set_par_run:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_par_run;
		msg.p_start = int_msg.p_start;
		msg.p_end = int_msg.p_end;
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_set_par_vel:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_par_vel;
		msg.p_start = int_msg.p_start;
		msg.p_end = int_msg.p_end;
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_set_par_dir:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_par_dir;
		msg.vecs = int_msg.vecs;
		server_ptr->broadcast(msg);
		break;

	case internal_msg_type::resp_add_textblock:
		//!!!!!!!!!!!!!!
		//msg = int_msg;
		msg.type = msg_type::resp_add_textblock;
		server_ptr->broadcast(msg);
		break;

	case  internal_msg_type::resp_showtext:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_showtext;
		msg.p_start = int_msg.p_start;
		msg.p_end = int_msg.p_end;
		msg.p_int = int_msg.p_int;
		msg.vecs.push_back(int_msg.vecs[0]);
		msg.strings.push_back(int_msg.strings[0]);
		msg.strings.push_back(int_msg.strings[1]);
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_changetext:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_changetext;
		msg.strings.push_back(int_msg.strings[0]);
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_deletetext:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_deletetext;
		server_ptr->broadcast(msg);
		break;
	case internal_msg_type::resp_set_actor_action:
		assert(int_msg.ent_id != "");
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_actor_action;
		msg.strings.push_back(int_msg.strings[0]);
		msg.p_dbl = int_msg.p_dbl;
		server_ptr->broadcast(msg);
		break;
	case internal_msg_type::resp_set_entity_position:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_entity_position;
		msg.vecs.push_back(int_msg.vecs[0]);
		server_ptr->broadcast(msg);
		break;

		//added by Wu,Huachi
	case  internal_msg_type::resp_set_static_entity_move:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_static_entity_move;
		msg.vecs = int_msg.vecs;
		server_ptr->broadcast(msg);
		break;

	case internal_msg_type::resp_push_questions:
		msg.type = msg_type::resp_push_questions;
		msg.id = int_msg.ent_id;
		msg.endpoint = int_msg.endpoint;
		msg.strings = int_msg.strings;
		server_ptr->unicast(msg.endpoint, msg);
		break;

	case  internal_msg_type::resp_set_par:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_par;
		msg.p_int = int_msg.p_int;
		server_ptr->broadcast(msg);
		break;

		//added by Wu,Huachi
	case internal_msg_type::resp_set_water_face:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_water_face;
		msg.p_dbl = int_msg.p_dbl;
		msg.p_int = int_msg.p_int;
		msg.vecs = int_msg.vecs;
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_set_knob_turn:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_set_knob_turn;
		msg.p_dbl = int_msg.p_dbl;
		server_ptr->broadcast(msg);
		break;
	case internal_msg_type::resp_start_alarm_flicker:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_start_alarm_flicker;
		msg.p_int = int_msg.p_int;
		server_ptr->broadcast(msg);
		break;
	case  internal_msg_type::resp_end_alarm_flicker:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_end_alarm_flicker;
		server_ptr->broadcast(msg);
		break;
	case internal_msg_type::resp_add_music:
		msg.id = int_msg.ent_id;
		msg.type = msg_type::resp_add_music;
		msg.strings = int_msg.strings;
		msg.p_dbl = int_msg.p_dbl;
		msg.p_int = int_msg.p_int;
		server_ptr->broadcast(msg);
		break;

	case internal_msg_type::resp_set_camera_goal:
		msg.type = msg_type::resp_set_camera_goal;
		msg.id = int_msg.ent_id;
		msg.vecs = int_msg.vecs;
		msg .p_dbl = int_msg.p_dbl;
		msg.p_int = int_msg.p_int;
		server_ptr->broadcast(msg);
		break;

	case internal_msg_type::resp_disable_normal_move:
		msg.type = msg_type::resp_disable_normal_move;
		//!!!!!!!!!!
		//msg = int_msg;
		server_ptr->unicast(msg.endpoint, msg);
		break;

	case internal_msg_type::resp_enable_normal_move:
		msg.type = msg_type::resp_enable_normal_move;
		//!!!!!!!!!!
		//msg = int_msg;
		server_ptr->unicast(msg.endpoint, msg);
		break;

	default:
		break;
	} 

	if(msg.type != msg_type::resp_complete_roles)
		assert(msg.id != "");
}