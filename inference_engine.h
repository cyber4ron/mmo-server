#pragma once

#include "state.h"
#include "script.h"
#include "script_manager.h"
#include "configuration.h"
#include "task_queue.h"
#include "worker.h"

class state;
class script_manager;
class branch_state;
class expression;

class inference_engine : public boost::enable_shared_from_this<inference_engine>
{
public:

	typedef boost::function<void()> task_type;
	typedef task_queue<task_type> queue_type;

	//worker
	queue_type m_taskQueue;
	worker<queue_type> m_worker;

	//for states
	std::vector<boost::shared_ptr<state>> states;//script states
	std::map<std::string, boost::weak_ptr<state>> state_id_map;
	boost::shared_ptr<script_manager> scp_mgr_ptr;

	//for branches
	std::vector<std::string> cmpl_branch_seq;//预案完整分支，树形结构保存在数组里
	std::vector<std::string> cur_branch_seq;//当前的分支序列
	boost::mutex cur_branch_seq_lock;

	//分支状态，需要初始设置
	std::vector<boost::shared_ptr<expression>> exprs;
	std::map<std::string, boost::weak_ptr<expression>> expr_map;//branch_id -> expr

	//evt->branch_id，通过输入事件确定分支
	std::map<std::string, std::string> evt_branch_map;

	//constr
	inference_engine(boost::shared_ptr<script_manager> sm_p);
	~inference_engine(){}

	//for states
	//通过配置文件构建状态关系，状态关系设置要避免出现resume正在执行的script!
	void build(const configuration &config);

	//开始推理状态
	void start();

	//lua脚本接口，执行完一个状态后调用
	void state_work_done(std::string state_id);

	//for branches
	bool prefix(std::string a, std::string b);//auxiliary func, b is prefix of a

	//得到当前分支子分支，即得到可干预分支
	std::vector<std::string> get_branches();

	//设置下一分支
	void set_branch(std::string branch_id);

	//添加分支选择判断事实，用于与或等规则推理
	void add_fact(std::string expr_id, std::string brh_fact);

	//向推理机输入事件，推出下一分支
	void input_event(std::string evt);

	//run worker
	void run();
	void async_call(task_type task);//ie. push进queue
};