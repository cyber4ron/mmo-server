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
	std::vector<std::string> cmpl_branch_seq;//Ԥ��������֧�����νṹ������������
	std::vector<std::string> cur_branch_seq;//��ǰ�ķ�֧����
	boost::mutex cur_branch_seq_lock;

	//��֧״̬����Ҫ��ʼ����
	std::vector<boost::shared_ptr<expression>> exprs;
	std::map<std::string, boost::weak_ptr<expression>> expr_map;//branch_id -> expr

	//evt->branch_id��ͨ�������¼�ȷ����֧
	std::map<std::string, std::string> evt_branch_map;

	//constr
	inference_engine(boost::shared_ptr<script_manager> sm_p);
	~inference_engine(){}

	//for states
	//ͨ�������ļ�����״̬��ϵ��״̬��ϵ����Ҫ�������resume����ִ�е�script!
	void build(const configuration &config);

	//��ʼ����״̬
	void start();

	//lua�ű��ӿڣ�ִ����һ��״̬�����
	void state_work_done(std::string state_id);

	//for branches
	bool prefix(std::string a, std::string b);//auxiliary func, b is prefix of a

	//�õ���ǰ��֧�ӷ�֧�����õ��ɸ�Ԥ��֧
	std::vector<std::string> get_branches();

	//������һ��֧
	void set_branch(std::string branch_id);

	//��ӷ�֧ѡ���ж���ʵ���������ȹ�������
	void add_fact(std::string expr_id, std::string brh_fact);

	//������������¼����Ƴ���һ��֧
	void input_event(std::string evt);

	//run worker
	void run();
	void async_call(task_type task);//ie. push��queue
};