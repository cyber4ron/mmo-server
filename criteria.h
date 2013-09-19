#pragma once

#include "operation.h"

namespace eval_type
{
  enum et
  {
    single,
    composite
  };
}

// 每一个opid对应一个
class criteria_op
{
public:
	std::string op_id; // 主id
  eval_type::et evl_type;//单个，还是需要顺序

	operation std_op;// single, 此时std_op.id = op_id
  std::vector<operation> std_ops;//用于判断顺序，这个vector放的是主id相同，subid不同的
  double full_score;

  operation user_op;
  std::vector<operation> user_ops;//用于判断顺序

  void set_op();//set user operation
  void set_ops();//set user operations

	virtual void check(double &result, std::string &eval)
	{

    if(evl_type == eval_type::single)
    {
      /*if(user_op == std_op)
        result = full_score;
      else result = 0;*/
    }
    else if(evl_type == eval_type::composite)
    {
      for(int i=0; i<std_ops.size(); i++)
      {
        //if(std_ops[i] != user_ops[i]) {result = 0; return;}
      }

      result = full_score;
    }
	}
};

class criteria_op_with_wind : public criteria_op
{

};

class criteria_qst
{
public:

};


class person_evals
{
public:
	std::string personid;

  std::vector<criteria_op> std_ops; // 主id，opid
  std::hash_map<std::string, criteria_op*> opid_map;

  std::vector<criteria_qst> std_qsts;
  std::hash_map<std::string, criteria_qst*> qstid_map;

	double total_score;
  std::string eval;
	//...

	void check()
	{
		//for each ops
		//for each qsts
	}
};