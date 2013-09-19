#include "stdafx.h"

#include "inference_engine.h"

using namespace std;

inference_engine::inference_engine( boost::shared_ptr<script_manager> sm_p ) : m_taskQueue(), m_worker(m_taskQueue)
{
	scp_mgr_ptr = sm_p;
}

void inference_engine::build(const configuration &config)
{
	//states
	for(vector<string>::const_iterator iter = config.states.cbegin(); iter != config.states.cend(); iter++)
	{
		int pos = iter->find_first_of(',');
		int pos1 = iter->find_first_of('!');

		string staName = iter->substr(0,pos);
		string scpName = iter->substr(pos+1,pos1-pos-1) + ".lua::" + iter->substr(pos+1,pos1-pos-1);
		string brName = iter->substr(pos1+1);
		boost::shared_ptr<state> sta = boost::make_shared<state>(staName, scp_mgr_ptr, scpName, brName);
		states.push_back(sta);
		state_id_map[staName] = sta;
	}

	//state transition
	int stateSize = config.transitions.size();
	for (vector< pair<int, int> >::const_iterator iter = config.transitions.cbegin(); iter != config.transitions.cend(); iter++)
	{
		states[iter->first]->forwards.push_back(states[iter->second]);
		states[iter->second]->backwards.push_back(states[iter->first]);
	}

	//expression
	for(vector<string>::const_iterator iter = config.expressions.cbegin(); iter != config.expressions.cend(); iter++)
	{
		int pos = iter->find_first_of(',');
		int pos1 = iter->find_first_of('!');

		string exprName = iter->substr(0,pos);
		string typeName = iter->substr(pos+1,pos1-pos-1);
		string factName = iter->substr(pos1+1);
		logic_operation::lo logic = (typeName=="no_op"?logic_operation::lo::no_op:(typeName=="not"?logic_operation::lo::not:(typeName=="and"?logic_operation::lo::and:logic_operation::lo::or)));
		boost::shared_ptr<expression> expr = boost::make_shared<expression>(exprName, logic, factName);
		expr_map[exprName] = expr;
		exprs.push_back(expr);
	}

	//expression transition
	for (vector< struct tranNode >::const_iterator iter = config.extransitions.cbegin(); iter != config.extransitions.cend(); iter++)
	{
		if (iter->type == "parent-child")
		{
			exprs[iter->backward]->parent_exprs.push_back(exprs[iter->forward]);
			exprs[iter->forward]->sub_exprs.push_back(exprs[iter->backward]);
		}
		else if (iter->type == "bro-bro")
		{
			exprs[iter->backward]->sibling_exprs.push_back(exprs[iter->forward]);
			exprs[iter->forward]->sibling_exprs.push_back(exprs[iter->backward]);
		}
	}

	for (vector< struct tranNode >::const_iterator iter = config.exstates.cbegin(); iter != config.exstates.cend(); iter++)
	{
		if (iter->type == "ex-state")
		{
			exprs[iter->backward]->forwards.push_back(states[iter->forward]);
			states[iter->forward]->back_exprs.push_back(exprs[iter->backward]);
		}
		else if (iter->type == "state-ex")
		{
			std::cout << "no conversion by now" << std::endl;
		}
	}

}

void inference_engine::start()
{
	cout<<"esrtet"<<endl;
	for(int i=0;i<states.size();i++)
	{
		if(!states[i]->has_backwards())
			states[i]->do_work();
	}

	//push进第一个分支("_0")
	boost::mutex::scoped_lock lock(cur_branch_seq_lock);
	if(cmpl_branch_seq.size()>0) cur_branch_seq.push_back(cmpl_branch_seq[0]);
}

void inference_engine::state_work_done( std::string state_id )
{
	boost::shared_ptr<state> st = state_id_map[state_id].lock();
	if(st) 
	{
		async_call(boost::bind(&state::work_done, st));//try advance, 暂lock确保不重复resume
		st->end_state();//yield script
	}
}

bool inference_engine::prefix( std::string a, std::string b )
{
	//b="", return true;
	if(a.length() < b.length())return false;
	for(int i=0;i<b.length();i++)
	{
		if(a[i]!=b[i])return false;
	}
	return true;
}

std::vector<std::string> inference_engine::get_branches()
{
	boost::mutex::scoped_lock lock(cur_branch_seq_lock);
	string cur_branch = cur_branch_seq[cur_branch_seq.size() - 1];

	std::vector<std::string> res;

	for(int i=0;i<cmpl_branch_seq.size();i++)
	{
		if(cmpl_branch_seq[i].length() == cur_branch.length() + 2 
			&& prefix(cmpl_branch_seq[i], cur_branch))
			res.push_back(cmpl_branch_seq[i]);
	}

	return res;
}

void inference_engine::set_branch(std::string branch_id) /*set(intervene) next branch*/
{
	boost::mutex::scoped_lock lock(cur_branch_seq_lock);
	string cur_branch = cur_branch_seq[cur_branch_seq.size() - 1];

	if(branch_id.length() == cur_branch.length() + 2 && prefix(branch_id, cur_branch))
	{
		//push branch_id
		cur_branch_seq.push_back(branch_id);
		//set branch
		boost::shared_ptr<expression> p_bs = expr_map[branch_id].lock();
		if(p_bs) p_bs->set_value();
	}
}

void inference_engine::add_fact( std::string expr_id, std::string brh_fact )
{
	boost::shared_ptr<expression> exp = expr_map[expr_id].lock();
	if(exp) exp->set_fact(brh_fact);
}

void inference_engine::input_event(std::string evt)
{

}

void inference_engine::run()
{
	m_worker.start();
}

void inference_engine::async_call( task_type task )
{
	m_taskQueue.push(task);
}