#pragma once

#include "entity.h"
#include "server.h"
#include "connection_server.h"
#include "message.h"
#include "internal_message.h"
#include "eval_manager.h"
#include "worker.h"
#include "task_queue.h"
#include "entity_factory.h"

#include "configuration.h"

class server;
class entity;
class role;
struct internal_msg;
class server_conn;
class eval_manager;
class entity_factory;

class entity_manager : public boost::enable_shared_from_this<entity_manager>//single
{
public:
	typedef boost::function<void()> task_type;
	typedef task_queue<task_type> queue_type;

	boost::shared_ptr<server> svr_ptr;
	boost::shared_ptr<boost::asio::io_service> io_s_ptr;//svr_ptr->io_service_ptr
	boost::shared_ptr<eval_manager> eval_mgr;//svr_ptr->io_service_ptr->ele_mgr

	//task_queue, worker
	queue_type m_taskQueue;
	worker<queue_type> m_worker;

	//实体容器
	std::vector<boost::shared_ptr<entity>> entities;
	std::map<std::string, boost::weak_ptr<entity>> entity_id_map;
	
	std::vector<boost::shared_ptr<role>> roles;//role entities, for optimazation
	std::map<std::string, boost::weak_ptr<role>> role_id_map;

	entity_manager(boost::shared_ptr<server> s_ptr);
	entity_manager(boost::shared_ptr<server> s_ptr, boost::shared_ptr<eval_manager> el_mgr);

	//创建实体，并赋初始值
	void create_entities(const configuration &config);
	void takeoverEntity(boost::shared_ptr<entity> item);
	void takeoverDialogEntity(boost::shared_ptr<entity> item);

	//worker run on task_queue
	void run();
	void async_call(task_type task);

	void handle_internal_msg_check(std::string ent_id, internal_msg in_msg);//带检查，检查动作是否可执行
	void handle_internal_msg(internal_msg in_msg);

	//用于resume lua 函数
	void register_entity(std::string entity_id, std::string script_name);
	void unregister_entity(std::string entity_id, std::string script_name);

	//client获取角色列表
	void get_available_roles(message msg);

	//选择和取消角色
	bool is_role_available(std::string ent_id);
	inline bool is_role_available(boost::shared_ptr<role> rl);
	void set_role_client_port(std::string entity_id, std::string endpoint);
	void reset_role_client_port( std::string entity_id);
	bool client_select_role(std::string endpoint, internal_msg msg);
	void client_cancel_role(std::string entity_id, std::string endpoint);

	//客户端update当前role entities
	void get_all_taken_roles(std::string endpoint);

	void complete_by_npcs();//演练脚本开始前执行

	//脚本函数得到id，先找真人，再找NPC
	std::string req_role_ent(std::string role_type);
	//直接通过id，并返回是否是真人
	std::string req_role_ent_by_id(std::string ent_id);

	//get状态
	vector3 get_role_position(std::string id);
	double get_valve_state(std::string id);
	bool check_actor_equipment(std::string id, std::string equipment_name);

	//collabrate
	void disable_normal_move(std::string rid);
	void enable_normal_move(std::string rid);

	void set_collab_move(std::string rid, std::vector<std::string> binds);
	void set_normal_move(std::string rid);

	void set_actor_postion_relative(std::string rid, vector3 vec);
};