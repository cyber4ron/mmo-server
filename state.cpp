#include "stdafx.h"

#include "state.h"

using namespace std;

state::state( std::string sid, boost::shared_ptr<script_manager> scp_mgr_p, std::string scp_name, std::string bl )
{
	state_id = sid;
	scp_mgr_ptr = scp_mgr_p;
	script_name = scp_name;
	done = false;
	branch_label = bl;
	int i = scp_name.find("::");
	function_name = scp_name.substr(i+2, scp_name.length() - i - 2);
}

state::state()
{
	done = false;
}

bool state::has_backwards()
{
	return !(backwards.empty() && back_exprs.empty());
}

bool state::is_ready()
{
	for(int i=0;i<backwards.size();i++)
	{
		if(!backwards[i]->done) return false;
	}

	for(int i=0;i<back_exprs.size();i++)
	{
		if(boost::indeterminate(back_exprs[i]->value) || !back_exprs[i]->value) return false;
	}

	return true;
}

void state::do_work()
{
	cout<<"in do work state id = "<<state_id<<endl;
	if(!done)
	{
		if(branch_label != "") scp_mgr_ptr->resume_script_by_name_async(script_name, branch_label);
		else scp_mgr_ptr->resume_script_by_name_async(script_name);
	}
}

void state::work_done()
{
	done = true;

	//尝试推进
	for(int i=0;i<forwards.size();i++)
	{
		if(forwards[i]->is_ready())
			forwards[i]->do_work();
	}

	//try推进expr
	for(int i=0;i<forw_exprs.size();i++)
		forw_exprs[i]->calc();
}

void state::end_state()
{
	scp_mgr_ptr->yield_script_by_name(script_name);
}