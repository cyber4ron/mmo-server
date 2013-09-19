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

	//scripts����
	std::vector<boost::shared_ptr<script>> scripts;
	boost::mutex scripts_mutex;

	//ͨ��script�����ļ�::�������ҵ�script
	std::map<std::string, boost::weak_ptr<script>> script_map;//"xxx.lua::xxx" -> script
	boost::mutex script_map_mutex;

	//ͨ��entity_id�ҵ�script
	std::multimap<std::string, boost::weak_ptr<script>> entity_id_map;
	boost::mutex entity_id_map_mutex;

	//ͨ��function_name�ҵ�script
	std::map<std::string, boost::weak_ptr<script>> function_map;
	boost::mutex function_map_mutex;

	//��������
	boost::shared_ptr<server> server_ptr;
	boost::shared_ptr<boost::asio::io_service> io_s_ptr;

	script_manager(boost::shared_ptr<server> svr_p);

	//attach�ű�
	void attach_script(std::string file_name, std::string entry, std::vector<std::string> entity_ids);
	void attach_scripts();

	//�ⲿ��scr_mgr�ӿڣ�ֱ�ӵ����У��ڲ����첽����
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
	void async_call(task_type task);//ie. push��queue
};