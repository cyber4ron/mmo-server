#pragma once

#include "geom.h"
#include "internal_message.h"

class operation;

namespace msg_type
{
  enum mt{
    echo_server,
    echo_client,
    upload_role,

    //role
    rep_login,
    resp_login,
    intl_client_disconnect,//internal
    req_update_roles,
    resp_update_roles,

    req_select_role,
    resp_select_role,
    resp_add_role,
    req_cancel_role,
    resp_cancel_role,
    resp_delete_role,
    req_complete_roles,
    resp_complete_roles,

    resp_set_camera,
    req_observer,
    resp_observer,

    req_user_move,
    resp_user_move,
    req_positioning,
    req_user_rotate,
    resp_user_rotate,
    req_user_act,
    resp_user_act,

    req_bear,
    req_drop,
    resp_disable_normal_move,
    resp_enable_normal_move,

    req_role_move,
    resp_role_move,
    req_role_rotate,
    resp_role_rotate,
    req_role_act,
    resp_role_act,

    req_set_attach_sub_ent,
    resp_set_attach_sub_ent,
    req_set_detach_sub_ent,
    resp_set_detach_sub_ent,

    //role deprecated
    resp_set_state,
    req_roles,
    resp_roles,
    req_role_position,
    resp_role_position,
    resp_roles_move,
    resp_set_actor_action,

    //process control
    req_training_start,
    req_training_suspend,
    req_training_resume,
    req_training_end,

    //device
    req_set_valve_run,
    resp_set_valve_run,
    resp_set_button_run,
    resp_set_entity_move,
    resp_set_entity_position,
    resp_set_static_entity_move,

    //particle
    resp_add_particle,
    resp_set_par,
    resp_set_par_run,
    resp_set_par_vel,
    resp_set_par_dir,

    //text
    resp_add_textblock,
    resp_showtext,//append
    resp_changetext,
    resp_deletetext,

    //dialog
    resp_push_questions,

    //operation
    req_operation_submission,

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
    resp_set_camera_goal,

    req_submit_operation
  };
};


namespace operation_type
{
  enum ot
  {
    valve,
    button,
    wear_mask,
    wear_hat,
    carry_fire_ext,
    phone_call,
    water_branch_spray,
    water_bag_connect,
    fire_hydrant_open,
    flag_place,
    car_guide,
    banner_hang
  };
}

struct internal_msg;

struct message
{
  std::string endpoint;
  msg_type::mt type;
  std::string id;

  std::vector<vector3> vecs;
  std::vector<std::string> strings;
  std::vector<double> p_dbls;
  std::vector<int> p_ints;

  std::vector<std::vector<int>> p_vec_ints;
  std::vector<std::vector<std::string>> p_vec_strings;
  std::vector<std::vector<vector3>> p_vec_vec3s;

  int p_int;//deprecated
  double p_dbl;//deprecated
  double p_start,p_end;//deprecated

  std::vector<operation> qsts;
  std::vector<operation> ops;

  template <typename Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & endpoint;
    ar & type;
    ar & id;
    ar & vecs;
    ar & strings;
    ar & p_dbls;
    ar & p_ints;
    ar & p_int;
    ar & p_dbl;
    ar & p_start;
    ar & p_end;
    ar & p_vec_ints;
    ar & p_vec_strings;
    ar & p_vec_vec3s;
    ar & qsts;
    ar & ops;
  }
};