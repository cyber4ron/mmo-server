#pragma once

#include "entity_manager.h"
#include "entity_facade.h"
#include "internal_message.h"
#include "timer_scheduler.h"
#include "geom.h"
#include "simulation.h"
#include "eval_manager.h"

class entity_manager;
class timer_scheduler;
class simu_move;
class simu_fire;
class simu_delay;
class eval_manager;
class entity_facade;

class entity : public boost::enable_shared_from_this<entity>
{
public:
	boost::recursive_mutex entity_mutex;
	boost::shared_ptr<entity_manager> ent_mgr;
	std::string entity_type;
	std::string entity_id;
	boost::shared_ptr<timer_scheduler> timer_scheduler_ptr;
	std::string task_id;
	boost::shared_ptr<eval_manager> ele_mgr;//set

	entity(boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> ts_ptr, std::string e_type, std::string eid);
	
	entity(std::string e_type, std::string eid);
	entity(entity_facade* facade);
	virtual ~entity();

	std::set<std::string> scripts_to_notify;
	boost::mutex scripts_to_notify_mutex;

	std::string get_entity_type();
	std::string get_entity_id();

	void setEntManager(boost::shared_ptr<entity_manager> ent_mgr);
	void setTimer(boost::shared_ptr<timer_scheduler> t_ptr);
	void setElemManager(boost::shared_ptr<eval_manager> ele_mgr);

	void register_entity(std::string script);
	void unregister_entity(std::string script);
	void notify_script();
	void notify_script_by_name(std::string script_name);

	virtual void handle_msg(internal_msg msg_) = 0;
};

class role : public entity
{
public:
	std::string client_port;//=="" is available;
	std::string role_type;
	std::string role_name;
	bool is_idle;
	vector3 position, rotation, scale;
	std::string action;
	double speed;
	double velocity;
	std::vector<std::string> attachments;

	boost::shared_ptr<simu_move> s_move_ptr;

	role(boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string r_name, std::string eid);
	role(std::string e_type, std::string r_name, std::string eid);
	role(entity_facade* facade);
	virtual ~role();
	
	void set_client_port(std::string client_port);
	void reset_client_port();

	void send_instruction(std::string instr);
	std::string get_role_name();

	bool has_attachment(std::string item);
	void add_attachment(std::string item);
	void remove_attachment(std::string item);
};

class role_operator : public role
{
public:
	role_operator(boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string r_name, std::string eid);
	role_operator(std::string e_type, std::string r_name, std::string eid);
	role_operator(entity_facade* facade);
	
	void handle_msg(internal_msg msg_);

	//APIs(getter/setter, commands)

	std::vector<std::string> bearer_ids;
	bool normal_move_disabled;//script or user control

	bool collab_move;
	std::vector<std::string> bind_list;//ids
};

 class device : public entity
 {
 public:
	 std::string device_name;
	 device( boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid );
	 device( std::string e_type, std::string d_name, std::string eid );
	 device(entity_facade* facade);
	 virtual ~device();
 };

 class valve : public device
 {
 public:
	 double state;//需初始设定
	 double unit_angle;
	 valve(boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid);
	 valve(std::string e_type, std::string d_name, std::string eid);	
	 valve(entity_facade* facade);
	 
	 void handle_msg(internal_msg in_msg);
 };

 class button : public device
 {
 public:
	 button(boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid);
	 button(std::string e_type, std::string d_name, std::string eid);
	 button(entity_facade* facade);
	 
	 void handle_msg(internal_msg in_msg);
 };

 class knob : public device
 {
 public:
	 knob(entity_facade* facade);

	 void handle_msg(internal_msg in_msg);
 };

 class commonEntity: public device
 {
 public:
	 vector3 position;
	 boost::shared_ptr<simu_move> s_move_ptr;

	 commonEntity(boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid);
	 commonEntity(std::string e_type, std::string d_name, std::string eid);
	 commonEntity(entity_facade* facade);

	 
	 void handle_msg(internal_msg in_msg);
 };

 class disaster : public entity
 {
 public:
	 std::string diaster_name;
	 vector3 position;
	 double v_max, v_min;
	 double width, height;
	 int particle_quota;
	 double live_time;

	 disaster( boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid );
	 disaster(std::string e_type, std::string d_name, std::string eid ); 
	 disaster(entity_facade* facade);
	 virtual ~disaster();
 };

 class dist_fire : public disaster
 {
 public:
	 boost::shared_ptr<simu_fire> ss_fire_ptr;

	 dist_fire(boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid);
	 dist_fire(std::string e_type, std::string d_name, std::string eid);	 
	 dist_fire(entity_facade* facade);
	 
	 void handle_msg(internal_msg in_msg);
 };

 class water : public disaster
 {
 public:
	 water(entity_facade* facade);

	 void handle_msg(internal_msg in_msg);
 };

 //single
 class textEntity : public entity
 {
 public:
	 std::string text_name;
	 textEntity( boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string d_name, std::string eid );
	 textEntity(std::string e_type, std::string d_name, std::string eid );	 
	 textEntity(entity_facade* facade);	 
	 
	 void handle_msg(internal_msg msg_);
 };

 //single
 class musicEntity : public entity
 {
 public:
	 std::string music_name;
	 musicEntity(entity_facade* facade);

	 void handle_msg(internal_msg in_msg);
 };