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

// ÿһ��opid��Ӧһ��
class criteria_op
{
public:
	std::string op_id; // ��id
  eval_type::et evl_type;//������������Ҫ˳��

	operation std_op;// single, ��ʱstd_op.id = op_id
  std::vector<operation> std_ops;//�����ж�˳�����vector�ŵ�����id��ͬ��subid��ͬ��
  double full_score;

  operation user_op;
  std::vector<operation> user_ops;//�����ж�˳��

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

  std::vector<criteria_op> std_ops; // ��id��opid
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