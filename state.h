#pragma once

#include "script.h"
#include "script_manager.h"

class script_manager;
class inference_engine;
class expression;

//script state, 包含一段lua脚本的state
class state : public boost::enable_shared_from_this<state>
{
public:
	std::vector<boost::shared_ptr<state>> backwards;
	std::vector<boost::shared_ptr<state>> forwards;

	std::vector<boost::shared_ptr<expression>> back_exprs;
	std::vector<boost::shared_ptr<expression>> forw_exprs;

	boost::shared_ptr<script_manager> scp_mgr_ptr;
	std::string script_name;
	std::string function_name;
	std::string branch_label;
	std::string ent_id;

	std::string state_id;

	//状态是否已完成
	bool done;

	state();
	state(std::string sid, boost::shared_ptr<script_manager> scp_mgr_p, std::string scp_name, std::string bl);

	bool has_backwards();

	//前件是否完成，是否具备执行条件，ie.this状态是否可以执行
	bool is_ready();

	//state里为执行脚本
	void do_work();

	//尝试推后件
	void work_done();

	//状态执行完
	void end_state();
};

namespace logic_operation
{
	enum lo
	{
		no_op,
		not,
		and,  
		or
	};
}

class expression
{
public:
	std::string expr_id;
	boost::tribool value;
	logic_operation::lo logic_op;
	std::string fact, pattern;
	std::vector<boost::shared_ptr<expression>> sub_exprs;//用于得到结果
	std::vector<boost::shared_ptr<expression>> parent_exprs;//用于向前推表达式结果
	std::vector<boost::shared_ptr<expression>> sibling_exprs;//conflict resoluttion, forcibly set false
	std::vector<boost::shared_ptr<state>> forwards;

	expression()
	{
		value = boost::indeterminate;
		fact = "";
	}

	expression(std::string id, logic_operation::lo lo, std::string p)
	{
		expr_id = id;
		logic_op = lo;
		pattern = p;
		value = boost::indeterminate;
		fact = "";
	}

	//fact + state.done 得到结果
	void set_fact(std::string f);
	void set_value();
	void calc();
private:
	void try_forward();
};
