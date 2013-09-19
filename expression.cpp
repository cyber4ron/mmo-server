#include "stdafx.h"

#include "state.h"

using namespace std;

void expression::set_fact(std::string f)
{
	//应该只set一遍
	if(fact != "") return;

	fact = f;
	calc();
}

void expression::set_value()
{
	if(value != boost::indeterminate) {value = true; try_forward();}
}

void expression::calc()
{
	//应该只计算一遍，保证一致性
	if(!boost::indeterminate(value)) return;

	//计算
	if(logic_op == logic_operation::no_op) value = fact == pattern;
	else if(logic_op == logic_operation::not) value = fact == pattern;
	else if(logic_op == logic_operation::and)
	{
		boost::tribool temp(true);
		for(int i=0;i<sub_exprs.size();i++)
		{
			temp = sub_exprs[i]->value && temp;
		}
		if(!boost::indeterminate(temp)) value = temp;
	}
	else if(logic_op == logic_operation::or)
	{
		boost::tribool temp(false);
		for(int i=0;i<sub_exprs.size();i++)
		{
			temp = sub_exprs[i]->value || temp;
		}
		if(!boost::indeterminate(temp)) value = temp;
	}
	else ;//exception

	if(boost::indeterminate(value)) return;

	//在确定value之后，设置sibling为false以保证一致性
	//有问题，racing condition!!!, 可能导致都为false
	if(value == true)
	for(int i=0;i<sibling_exprs.size();i++)
		sibling_exprs[i]->value = false;

	if (!boost::indeterminate(value)) try_forward();
}

void expression::try_forward()
{
	//由indeterminate变为determinate时，尝试推进
	if(!boost::indeterminate(value))
	{
		for(int i=0;i<parent_exprs.size();i++)
			parent_exprs[i]->calc();

		for(int i=0;i<forwards.size();i++)
			if(forwards[i]->is_ready()) 
				forwards[i]->do_work();
	}
}

