#pragma once

#include "geom.h"
#include "message.h"

namespace internal_msg_type{
	enum imt{
	req_role_move,
	resp_role_move,
	req_role_position,
	resp_role_position,
	req_user_move,
	resp_user_move,
	req_positioning,
	req_user_rotate,
	resp_user_rotate,
	req_user_act,
	resp_user_act,

	req_roles_available,
	resp_roles_available,

	req_complete_roles,
	resp_complete_roles,

	req_client_select_role,
	resp_client_select_role,
	resp_client_cancel_role,
	resp_client_delete_role,

	resp_add_role,
	req_update_roles,
	resp_update_roles,

	req_set_state,
	resp_set_state,
	req_set_actor_action,

	req_bear,
	req_drop,
	req_collab_move,
	resp_disable_normal_move,
	resp_enable_normal_move,
	resp_set_collab_move,
	resp_reset_collab_move,

	req_get_actor_id,

	req_set_button_run,
	resp_set_button_run,

	req_set_valve_run,
	resp_set_valve_run,

	req_set_attach_sub_ent,
	resp_set_attach_sub_ent,

	req_set_detach_sub_ent,
	resp_set_detach_sub_ent,

	req_set_entity_move,
	req_set_entity_position,
	req_set_static_entity_move,
	resp_set_entity_move,
	resp_set_entity_position,
	resp_set_static_entity_move,

	req_add_particle,
	resp_add_particle,

	req_set_par_run,
	resp_set_par_run,

	req_set_par_vel,
	resp_set_par_vel,

	req_set_par_dir,
	resp_set_par_dir,

	req_change_par_vel,

	req_add_textblock,
	resp_add_textblock,
	req_showtext,
	resp_showtext,
	req_changetext,
	resp_changetext,
	req_deletetext,
	resp_deletetext,
	resp_set_actor_action,
	req_set_par,
	resp_set_par,

	req_push_questions,
	resp_push_questions,

	//req_add_question,
	//rep_add_operation,

	//req_positioning,
	//resp_positioning，

	req_operation_submit,

	//water
	req_set_water_face,
	resp_set_water_face,

	req_set_knob_turn,
	resp_set_knob_turn,

	req_start_alarm_flicker,
	resp_start_alarm_flicker,

	req_end_alarm_flicker,
	resp_end_alarm_flicker,

	req_add_music,
	resp_add_music,

	submit_positioning,
	submit_action,//无关其他物品，idle, walking, run, holdphone, 引导，。。
	submit_operation,//设备、物件相关，valve, button, knob, 警戒线，大旗，袖标，打电话，拿喇叭，帽子，面具。。。
	submit_question_answer,//回答选择题。返回题目id，用户id，选择结果，时间等
	req_set_camera_goal,
	resp_set_camera_goal
	};
};

struct message;
struct internal_msg
{
	std::string endpoint;

	internal_msg_type::imt msg_type;

	std::string ent_type;
	std::string ent_id;

	int delay_time;//-1, 0, positive
	int broadcast_option;//-1, 0

	std::vector<std::string> strings;
	std::vector<vector3> vecs;
	std::vector<double> p_dbls;
	std::vector<int> p_ints;
	std::vector<bool> p_bls;//尽量别用
	std::vector<std::vector<int>> p_vec_ints;
	std::vector<std::vector<std::string>> p_vec_strings;
	std::vector<std::vector<vector3>> p_vec_vec3s;

	int p_int;//deprecated
	double p_dbl;//deprecated
	double p_start,p_end;//deprecated

	internal_msg()
	{
		delay_time = 0;
		broadcast_option = 0;
	}

	//internal_msg(message msg);
};
