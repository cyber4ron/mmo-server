#pragma once

#include "server.h"
#include "script.h"
#include "entity_interface.h"
#include "geom.h"
#include "inference_engine.h"
#include "task_queue.h"
#include "worker.h"

using namespace luabind;

class script;
class entity_interface;

class script_manager : public boost::enable_shared_from_this<script_manager>//single
{
public:
	typedef boost::function<void()> task_type;
	typedef task_queue<task_type> queue_type;

	//worker
	queue_type m_taskQueue;
	worker<queue_type> m_worker;

	//lua_State *master_state;
	//boost::mutex master_state_mutex;

	//scripts容器
	std::vector<boost::shared_ptr<script>> scripts;
	boost::mutex scripts_mutex;

	//通过script名（文件::函数）找到script
	std::map<std::string, boost::weak_ptr<script>> script_map;//"xxx.lua::xxx" -> script
	boost::mutex script_map_mutex;

	//通过entity_id找到script
	std::multimap<std::string, boost::weak_ptr<script>> entity_id_map;
	boost::mutex entity_id_map_mutex;

	//通过function_name找到script
	std::map<std::string, boost::weak_ptr<script>> function_map;
	boost::mutex function_map_mutex;

	//依赖对象
	boost::shared_ptr<server> server_ptr;
	boost::shared_ptr<boost::asio::io_service> io_s_ptr;

	script_manager(boost::shared_ptr<server> svr_p);

	//attach脚本
	void attach_script(std::string file_name, std::string entry, std::vector<std::string> entity_ids);
	void attach_scripts();

	//外部调scr_mgr接口，直接调就行，内部做异步处理
	void resume_script_by_name(std::string script_name);
	void resume_script_by_name_async(std::string script_name);
	void resume_script_by_name_async(std::string script_name, std::string branch);
	void yield_script_by_name(std::string script_name);
	void end_script_by_name(std::string script_name);

	std::string get_func_name_by_script(std::string scr_name);
	void set_func_real_person(std::string ent_id);
	void reset_func_real_person(std::string ent_id);//ie. set NPC
	void set_func_branch(std::string func_name, std::string branch);

	//run worker
	void run();
	void async_call(task_type task);//ie. push进queue
};