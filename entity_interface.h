#pragma once

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

#include "geom.h"
#include "entity_manager.h"

class entity_manager;
class entity;

class entity_interface : public boost::enable_shared_from_this<entity_interface>//single
{
public:

	boost::shared_ptr<entity_manager> ent_mgr_ptr;

	entity_interface(boost::shared_ptr<entity_manager> em_ptr);

	std::string get_role_ent_id(std::string role_type);

	void register_entity(std::string entity_id, std::string script_name);
	void unregister_entity(std::string entity_id, std::string script_name);

	void send_instruction_by_id(std::string id, std::string instr);
	void add_actor(std::string id, vector3 position, vector3 rotation, vector3 scale, std::string action);
	void complete_by_npcs();
	void set_actor_move(std::string id, vector3 v_end, double speed);
	vector3 get_actor_position(std::string id);
	bool check_actor_equipment(std::string id, std::string equipment_name);

	//edited by Wu,Huachi
	//设置人物动作
	void set_actor_action(std::string id, std::string act,double vel, int delay); 
	void attach_sub_entity(std::string id,std::string bone,std::string meshname, int ms=0);
	void detach_sub_entity(std::string id,std::string bone,std::string meshname);

	//button
	void set_button_run(std::string idx1,std::string idx2,double dis,int dir);

	//valve
	void set_valve_run(std::string idx1,std::string idx2,double angle,double speed,int dir);
	double get_valve_state(std::string id);
	//common entity
	void set_common_entity_run(std::string idx1,std::string idx2,vector3 goal,int ifTran,double speed);
	void set_common_entity_pos(std::string idx1,std::string idx2,vector3 goal);

	//particle
	void add_particle(std::string idx1,std::string idx2,std::string pname,vector3 pos);
	void set_particle_running(std::string idx1,std::string idx2,double v1,double v2);
	void set_particle_run(std::string idx1,std::string idx2,bool flag);
	void set_particle_velocity(std::string idx1,std::string idx2,double v1,double v2,double speed1,double speed2);
	void set_particle_dir(std::string idx1, std::string idx2, vector3 vec);

	//text
	void add_textblock_onscreen(std::string id, std::string name, int x, int y, int size, vector3 color, double a);
	void show_text_onscreen(std::string textname, std::string role_id, std::string text,double posx,double posy,int fsize,vector3 color, int delay);
	void change_text_onscreen(std::string textname,std::string text, int delay);
	void delete_text_onscreen(std::string textname, int delay = 0);

	//question, instruction
	void push_questions(std::string role_id, std::string info, std::string qids, std::string first_id);//qids format "1,2,3,7,8"

	//added by Wu,Huachi at 2013/4/25
	void set_entity_position(std::string idx1,std::string idx2,vector3 vec,int delayTime);

	void set_water_face_move(std::string idx1,std::string idx2,vector3 goal,double speed,int ifTransient,int delayTime);

	void set_knob_turn(std::string idx1,std::string idx2,double angle,int delayTime);

	void start_alarm_flicker(std::string idx1,std::string idx2, int frameCount,int delayTime);

	void end_alarm_flicker(std::string idx1,std::string idx2,int delayTime);

	//added by Wu at 2013/5/5
	void add_one_music(std::string idx,std::string MusicPath,double Volume,int IfLoop, std::string script_to_notify,std::string clientid, int delayTime);

	void move_camera_to_goal(std::string id,vector3 vec,double speed,int ifTransient);
};										
