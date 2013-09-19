#pragma once

#include "internal_message.h"
#include "question.h"
#include "operation.h"
#include "criteria.h"

class question;
class question_ans;
class operation;

class eval_manager
{
public:
	std::vector<question> questions;
	std::map<std::string, question*> qid_map;

	std::vector<question_ans> user_answers;
	boost::mutex user_answers_mutex;
	std::map<std::string, question_ans*> ans_map;
	boost::mutex ans_map_mutex;

	std::multiset<operation> user_operations;
	boost::mutex user_operations_mutex;
  std::multimap<std::string, operation*> opid_map;//indexed by id
	boost::mutex opid_map_mutex;

	std::vector<person_evals> evals;
  std::map<std::string, person_evals*> personid_map;

	//集成王瑜代码
	void load_quesitons();

	void push_user_ans(question_ans ans);

  void push_user_operaiton(operation op);
  void delete_user_operation_by_id(std::string opid);//用于有些错误操作不可接受，如操作对象错误、操作类型错误等，需要用户重新提交
  void push_user_operaiton(std::vector<operation> ops);

	void get_all_evals();

  bool translateStr2Op(const std::vector<std::string> &segs, std::vector<operation> &ops);

  void constr_evals(const std::vector<operation> &ops);
};